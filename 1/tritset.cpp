#include "tritset.h"

Trit operator!(Trit const& a) {
	if (a == Unknown) {
		return Unknown;
	}

	if (a == True) {
		return False;
	}

	return True;
}

Trit operator&(Trit const& a, Trit const& b) {
	if (a == False || b == False) {
		return False;
	}

	if (a == Unknown || b == Unknown) {
		return Unknown;
	}

	return True;
}

Trit operator|(Trit const& a, Trit const& b) {
	if (a == True || b == True) {
		return True;
	}

	if (a == Unknown || b == Unknown) {
		return Unknown;
	}

	return False;
}

TritSet::TritSet(size_t count) : _logicalLength(0) {
	_allocLength = _maxLength = count / (4 * sizeof(uint8_t)) + 1;
	_data = std::vector<uint8_t>(_allocLength, 0);
}

size_t TritSet::capacity() const {
	return this->_maxLength;
}

void TritSet::_set_trit(size_t tripIndex, Trit const& trit) {
	size_t dataIndex = tripIndex / (4 * sizeof(uint8_t));
	if (_maxLength <= dataIndex) {
		if (trit == Unknown) {
			return;
		}
		_data.resize(dataIndex + 1, Unknown);
		_maxLength = dataIndex + 1;
	}

	size_t dataPos = tripIndex % (4 * sizeof(uint8_t));
	uint8_t& item = _data[dataIndex];
	item = ~((uint8_t)(3 << dataPos * 2)) & item | ((uint8_t)trit << dataPos * 2);

	if (tripIndex + 1 == _logicalLength && trit == Unknown) {
		_logicalLength = _search_last_index() + 1;
	} else {
		if (tripIndex + 1 > _logicalLength) {
			_logicalLength = tripIndex + 1;
		}
	}
}

Trit TritSet::_get_trit(size_t tripIndex) const {
	size_t dataIndex = tripIndex / (4 * sizeof(uint8_t));
	if (_maxLength <= dataIndex) {
		return Unknown;
	}

	size_t dataPos = tripIndex % (4 * sizeof(uint8_t));
	return Trit((_data[dataIndex] >> dataPos * 2) & 0x3);
}

Trit TritSet::operator[](size_t index) const {
	return _get_trit(index);
}

TritSet::Proxy TritSet::operator[](size_t index) {
	return Proxy(*this, index);
}

void TritSet::shrink() {
	if (!_allocLength) {
		return;
	}

	size_t lastIndex = _search_last_index();
	if (lastIndex < (_allocLength - 1) * 4 * sizeof(uint8_t)) {
		return;
	}

	_maxLength = lastIndex / (4 * sizeof(uint8_t)) + 1;
	_data.resize(_maxLength);
}

size_t TritSet::length() {
	return _logicalLength;
}

void TritSet::trim(size_t lastIndex) {
	size_t dataIndex = lastIndex / (4 * sizeof(uint8_t));
	_data.resize(dataIndex + 1);
	_maxLength = dataIndex + 1;
	_allocLength = dataIndex + 1;
	//TODO: add func
}

size_t TritSet::_search_last_index() const {
	size_t lastIndex = 0;
	for (size_t i = lastIndex; i < (_maxLength - 1) * 4 * sizeof(uint8_t); ++i) {
		if (_get_trit(i) != Unknown) {
			lastIndex = i;
		}
	}
	return lastIndex;
}

TritSet::Proxy::operator Trit() const {
	return _set._get_trit(_pos);
}

TritSet::Proxy& TritSet::Proxy::operator=(Trit trit) {
	_set._set_trit(_pos, trit);
	return *this;
}

TritSet::Proxy::Proxy(TritSet& set, size_t pos) : _set(set), _pos(pos) {}
