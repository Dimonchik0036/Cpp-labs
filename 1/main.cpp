#include <iostream>
#include "tritset.h"

int main(int argc, char *argv[]) {
	Trit a = False;
	Trit b = Unknown;
	Trit c = True;
	std::cout << !a << !b << !c << std::endl;
	std::cout << std::endl;
	std::cout << (a & a) << (a & b) << (a & c) << std::endl;
	std::cout << (b & a) << (b & b) << (b & c) << std::endl;
	std::cout << (c & a) << (c & b) << (c & c) << std::endl;
	std::cout << std::endl;
	std::cout << (a | a) << (a | b) << (a | c) << std::endl;
	std::cout << (b | a) << (b | b) << (b | c) << std::endl;
	std::cout << (c | a) << (c | b) << (c | c) << std::endl;
	TritSet set(1000);
	std::cout << std::endl;
	set.trim(10);
	std::cout << std::endl;
	std::cout << set.capacity() << std::endl;
	std::cout << set.length() << std::endl;
	std::cout << set[0] << std::endl;
	set[9999999] = True;
	set[50] = False;
	set[0] = False;
	set[0] = True;
	std::cout << "True " << set.cardinality(True) << std::endl;
	std::cout << "False " << set.cardinality(False) << std::endl;
	std::cout << "Unknown " << set.cardinality(Unknown) << std::endl;
	TritSet set2 = !set;
	std::cout << "True " << set2.cardinality(True) << std::endl;
	std::cout << "False " << set2.cardinality(False) << std::endl;
	std::cout << "Unknown " << set2.cardinality(Unknown) << std::endl;
	TritSet set3 = set & set2;
	std::cout << "True " << set3.cardinality(True) << std::endl;
	std::cout << "False " << set3.cardinality(False) << std::endl;
	std::cout << "Unknown " << set3.cardinality(Unknown) << std::endl;
	TritSet set4 = set | set2;
	std::cout << "True " << set4.cardinality(True) << std::endl;
	std::cout << "False " << set4.cardinality(False) << std::endl;
	std::cout << "Unknown " << set4.cardinality(Unknown) << std::endl;
	std::cout << "len " << set.length() << std::endl;
	std::cout << set[0] << std::endl;
	std::cout << std::endl;
	std::cout << set.capacity() << std::endl;
	set[5000] = True;
	std::cout << set.capacity() << std::endl;
	set.trim(100);
	std::cout << "len " << set.length() << std::endl;
	set[5000] = Unknown;
	std::cout << set.capacity() << std::endl;
	set.shrink();
	std::cout << "len " << set.length() << std::endl;
	std::cout << set.capacity() << std::endl;
	return 0;
}

