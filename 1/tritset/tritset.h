#pragma once

#include <unordered_map>
#include "trit.h"

typedef uint32_t uint;

class TritSet {
public:
	class Proxy {
	public:
		TritSet& operator=(Trit);
		Trit operator[](size_t) const;
		operator Trit() const;
	private:
		size_t _pos;
		TritSet& _set;
		Proxy(TritSet&, size_t);
		Proxy(const Proxy&);
		Proxy& operator=(Proxy const&) = delete;

		friend class TritSet;
	};

	TritSet(size_t = 0, Trit = Unknown);
	TritSet(TritSet const&);
	TritSet& operator=(TritSet const&);
	~TritSet();
	Trit operator[](size_t) const;
	Proxy operator[](size_t);
	size_t capacity() const;
	void shrink();
	void trim(size_t);
	size_t length() const;
	size_t cardinality(Trit) const;
	std::unordered_map<Trit, size_t, std::hash<size_t> > cardinality() const;
private:
	void _correction(Trit& trit);
	void _resize(size_t);
	void _set_trit(size_t, Trit);
	Trit _get_trit(size_t) const;
	long long int _search_last_index() const;
	size_t _maxLength;
	size_t _allocLength;
	size_t _dataLength;
	size_t _logicalLength;
	uint *_data;
};

TritSet operator&(TritSet const& a, TritSet const& b);
TritSet operator|(TritSet const& a, TritSet const& b);
TritSet operator~(TritSet const& a);

bool operator==(TritSet const& a, TritSet const& b);
bool operator!=(TritSet const& a, TritSet const& b);