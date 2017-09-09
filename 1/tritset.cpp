#include "tritset.h"

TritSet operator&(TritSet const& a, TritSet const& b) {
	size_t len = a.capacity() > b.capacity() ? a.capacity() : b.capacity();
	TritSet set(len);
	for (size_t i = 0; i < len; ++i) {
		set[i] = a[i] & b[i];
	}

	return set;
}

TritSet operator|(TritSet const& a, TritSet const& b) {
	size_t len = a.capacity() > b.capacity() ? a.capacity() : b.capacity();
	TritSet set(len);
	for (size_t i = 0; i < len; ++i) {
		set[i] = a[i] | b[i];
	}

	return set;
}

TritSet operator~(TritSet const& a) {
	size_t len = a.capacity();
	TritSet set(len);
	for (size_t i = 0; i < len; ++i) {
		set[i] = (Trit)!a[i];
	}

	return set;
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

TritSet::TritSet(size_t length, Trit value) : _logicalLength(value == Unknown ? 0 : length), _maxLength(length) {
	_correction(value);
	_allocLength = _dataLength = length / (4 * sizeof(uint)) + 1;
	_data = new uint[_allocLength];

	uint mask = 0;
	for (size_t j = 0; j < 4 * sizeof(uint); ++j) {
		mask |= value << (j * 2);
	}

	for (uint *i = _data; i != (_data + _allocLength + 1); ++i) {
		*i = mask;
	}
}

size_t TritSet::capacity() const {
	return this->_maxLength;
}

void TritSet::_set_trit(size_t index, Trit value) {
	_correction(value);
	size_t dataIndex = index / (4 * sizeof(uint));
	if (_maxLength <= index) {
		if (value == Unknown) {
			return;
		}

		_resize(dataIndex + 1);
		_maxLength = (index + 1);
	}

	size_t dataPos = index % (4 * sizeof(uint));
	_data[dataIndex] = ~((uint)(3 << dataPos * 2)) & _data[dataIndex] | ((uint)value << dataPos * 2);

	if (index + 1 == _logicalLength && value == Unknown) {
		_logicalLength = _search_last_index() + 1;
	} else {
		if (value != Unknown && index + 1 > _logicalLength) {
			_logicalLength = index + 1;
		}
	}
}

Trit TritSet::_get_trit(size_t index) const {
	if (_maxLength <= index) {
		return Unknown;
	}

	return Trit((_data[index / (4 * sizeof(uint))] >> index % (4 * sizeof(uint)) * 2) & 0x3);
}

Trit TritSet::operator[](size_t index) const {
	return _get_trit(index);
}

void TritSet::shrink() {
	if (!_allocLength) {
		return;
	}

	size_t lastIndex = _search_last_index() + 1;
	if (lastIndex < (_allocLength - 1) * 4 * sizeof(uint)) {
		return;
	}

	_resize(lastIndex / (4 * sizeof(uint)) + 1);
	_maxLength = (lastIndex + 1);
}

size_t TritSet::length() const {
	return _logicalLength;
}

void TritSet::trim(size_t lastIndex) {
	size_t dataIndex = (lastIndex) / (4 * sizeof(uint));
	_resize(dataIndex + 1);
	_maxLength = lastIndex;
	_allocLength = dataIndex + 1;
	_data[dataIndex] &= ~((uint)0) >> (6 * sizeof(uint) - 2 * ((lastIndex) % (4 * sizeof(uint)) - 1));
	_logicalLength = _search_last_index() + 1;
}

long long int TritSet::_search_last_index() const {
	long long int lastIndex = -1;
	for (size_t i = 0; i < _maxLength; ++i) {
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
	uint *tmp = new uint[newSize];
	for (uint *i = tmp; i != (tmp + newSize + 1); ++i) {
		*i = 0;
	}

	memmove(tmp, _data, (newSize > _dataLength ? _dataLength : newSize) * sizeof(uint));
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

std::unordered_map<Trit, size_t, std::hash<size_t> > TritSet::cardinality() const {
	std::unordered_map<Trit, size_t, std::hash<size_t> > unorderedMap;
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
	this->_data = new uint[set._dataLength];
	memcpy(this->_data, set._data, set._dataLength * sizeof(uint));

	return *this;
}

TritSet::TritSet(TritSet const& set) {
	this->_allocLength = set._allocLength;
	this->_logicalLength = set._logicalLength;
	this->_maxLength = set._maxLength;
	this->_dataLength = set._dataLength;
	this->_data = new uint[set._dataLength];
	memcpy(this->_data, set._data, set._dataLength * sizeof(uint));
}

void TritSet::_correction(Trit& trit) {
	switch (trit) {
		case True:
			trit = True;
			break;
		case False:
			trit = False;
			break;
		default:
			trit = Unknown;
			break;
	}
}

TritSet::Proxy::operator Trit() const {
	return _set._get_trit(_pos);
}

TritSet::Proxy TritSet::operator[](size_t index) {
	return Proxy(*this, index);
}

TritSet& TritSet::Proxy::operator=(Trit t) {
	_set._set_trit(_pos, t);
	return this->_set;
}

TritSet::Proxy::Proxy(TritSet& set, size_t pos) : _set(set), _pos(pos) {}

Trit TritSet::Proxy::operator[](size_t index) const {
	return this->_set._get_trit(index);
}
