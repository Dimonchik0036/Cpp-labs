#include "tritset.h"

TritSet operator&(TritSet const& a, TritSet const& b) {
	size_t len = a.capacity() > b.capacity() ? a.capacity() : b.capacity();
	TritSet tritSet(len);
	for (size_t i = 0; i < len; ++i) {
		tritSet[i] = a[i] & b[i];
	}

	return tritSet;
}

TritSet operator|(TritSet const& a, TritSet const& b) {
	size_t len = a.capacity() > b.capacity() ? a.capacity() : b.capacity();
	TritSet tritSet(len);
	for (size_t i = 0; i < len; ++i) {
		tritSet[i] = a[i] | b[i];
	}

	return tritSet;
}

TritSet operator!(TritSet const& a) {
	size_t len = a.capacity();
	TritSet tritSet(len);
	for (size_t i = 0; i < len; ++i) {
		tritSet[i] = (Trit)!a[i];
	}

	return tritSet;
}

bool operator==(TritSet const& a, TritSet const& b) {
	if (a.length() != b.length()) {
		return false;
	}

	for (size_t i = 0; i < a.length(); ++i) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}

bool operator!=(TritSet const& a, TritSet const& b) {
	return !(a == b);
}

TritSet::TritSet(size_t length) : _logicalLength(0), _maxLength(length) {
	_allocLength = _dataLength = length / (4 * sizeof(uint8_t)) + 1;
	_data = new uint8_t[_allocLength];
	for (uint8_t *i = _data; i != (_data + _allocLength + 1); ++i) {
		*i = 0;
	}
}

size_t TritSet::capacity() const {
	return this->_maxLength;
}

void TritSet::_set_trit(size_t index, Trit value) {
	switch (value) {
		case True:
			value = True;
			break;
		case False:
			value = False;
			break;
		default:
			value = Unknown;
			break;
	}

	size_t dataIndex = index / (4 * sizeof(uint8_t));
	if (_dataLength <= dataIndex) {
		if (value == Unknown) {
			return;
		}

		_resize(dataIndex + 1);
		_maxLength = (dataIndex + 1) * 4 * sizeof(uint8_t);
	}

	size_t dataPos = index % (4 * sizeof(uint8_t));
	_data[dataIndex] = ~((uint8_t)(3 << dataPos * 2)) & _data[dataIndex] | ((uint8_t)value << dataPos * 2);

	if (index + 1 == _logicalLength && value == Unknown) {
		_logicalLength = _search_last_index();
	} else {
		if (value != Unknown && index + 1 > _logicalLength) {
			_logicalLength = index + 1;
		}
	}
}

Trit TritSet::_get_trit(size_t index) const {
	size_t dataIndex = index / (4 * sizeof(uint8_t));
	if (_dataLength <= dataIndex) {
		return Unknown;
	}

	size_t dataPos = index % (4 * sizeof(uint8_t));
	return Trit((_data[dataIndex] >> dataPos * 2) & 0x3);
}

Trit TritSet::operator[](size_t index) const {
	return _get_trit(index);
}

void TritSet::shrink() {
	if (!_allocLength) {
		return;
	}

	size_t lastIndex = _search_last_index();
	if (lastIndex < (_allocLength - 1) * 4 * sizeof(uint8_t)) {
		return;
	}

	_resize(lastIndex / (4 * sizeof(uint8_t)) + 1);
	_maxLength = lastIndex + 1;
}

size_t TritSet::length() const {
	return _logicalLength;
}

void TritSet::trim(size_t lastIndex) {
	size_t dataIndex = lastIndex / (4 * sizeof(uint8_t));
	_resize(dataIndex + 1);
	_maxLength = lastIndex;
	_allocLength = dataIndex + 1;
	_data[dataIndex] &= ~((uint8_t)0) >> (6 * sizeof(uint8_t) - 2 * (lastIndex % (4 * sizeof(uint8_t))));
	_logicalLength = _search_last_index();
}

size_t TritSet::_search_last_index() const {
	size_t lastIndex = 0;
	for (size_t i = lastIndex; i < _maxLength; ++i) {
		if (_get_trit(i) != Unknown) {
			lastIndex = i;
		}
	}
	return lastIndex;
}

TritSet::~TritSet() {
	delete[]_data;
}

void TritSet::_resize(size_t newSize) {
	uint8_t *tmp = new uint8_t[newSize];
	for (uint8_t *i = tmp; i != (tmp + newSize + 1); ++i) {
		*i = 0;
	}

	memmove(tmp, _data, (newSize >= _dataLength ? _dataLength : newSize) * sizeof(uint8_t));
	_dataLength = newSize;
	delete[]_data;
	_data = tmp;
}

size_t TritSet::cardinality(Trit value) const {
	size_t count = 0;
	for (size_t i = 0; i < length(); ++i) {
		if (_get_trit(i) == value) {
			++count;
		}
	}

	return count;
}

std::unordered_map<Trit, int, std::hash<int> > TritSet::cardinality() {
	std::unordered_map<Trit, int, std::hash<int>> unorderedMap;
	int trueCount = 0;
	int falseCount = 0;
	int unknownCount = 0;

	for (size_t i = 0; i < length(); ++i) {
		switch (_get_trit(i)) {
			case True:
				++trueCount;
				break;
			case False:
				++falseCount;
				break;
			default:
				++unknownCount;
				break;
		}
	}

	unorderedMap[True] = trueCount;
	unorderedMap[False] = falseCount;
	unorderedMap[Unknown] = unknownCount;
	return unorderedMap;
}

TritSet& TritSet::operator=(TritSet const& set) {
	this->~TritSet();
	this->_allocLength = set._allocLength;
	this->_logicalLength = set._logicalLength;
	this->_maxLength = set._maxLength;
	this->_dataLength = set._dataLength;
	this->_data = new uint8_t[set._dataLength];
	memcpy(this->_data, set._data, set._dataLength * sizeof(uint8_t));

	return *this;
}

TritSet::TritSet(TritSet const& set) {
	this->_allocLength = set._allocLength;
	this->_logicalLength = set._logicalLength;
	this->_maxLength = set._maxLength;
	this->_dataLength = set._dataLength;
	this->_data = new uint8_t[set._dataLength];
	memcpy(this->_data, set._data, set._dataLength * sizeof(uint8_t));
}

TritSet::Proxy::operator Trit() const {
	return _set._get_trit(_pos);
}

TritSet::Proxy TritSet::operator[](size_t index) {
	return Proxy(*this, index);
}

TritSet::Proxy& TritSet::Proxy::operator=(Trit t) {
	_set._set_trit(_pos, t);
	return *this;
}

TritSet::Proxy::Proxy(TritSet& set, size_t pos) : _set(set), _pos(pos) {}
