#pragma once

#include <vector>
#include <iostream>

enum Trit {
	False = 1,
	Unknown = 0,
	True = 2,
};

Trit operator!(Trit const& a);
Trit operator&(Trit const& a, Trit const& b);
Trit operator|(Trit const& a, Trit const& b);

class TritSet {
public:
	class Proxy {
	public:
		Proxy& operator=(Trit);
		operator Trit() const;
	private:
		size_t _pos;
		TritSet& _set;
		Proxy(TritSet&, size_t);
		Proxy(const Proxy&);
		Proxy& operator=(Proxy const&);

		friend class TritSet;
	};

	TritSet(size_t);
	Trit operator[](size_t) const;
	Proxy operator[](size_t);
	size_t capacity() const;
	void shrink();
	void trim(size_t);
	size_t length();
	size_t cardinality(Trit); //TODO: add
private:
	void _set_trit(size_t, Trit const&);
	Trit _get_trit(size_t) const;
	size_t _search_last_index() const;
	size_t _maxLength;
	size_t _allocLength;
	size_t _logicalLength;
	std::vector<uint8_t> _data;
};
