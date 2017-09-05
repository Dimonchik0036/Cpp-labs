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
	std::cout << set.length() << std::endl;
	std::cout << set[0] << std::endl;
	set[0] = True;
	std::cout << "len " << set.length() << std::endl;
	std::cout << set[0] << std::endl;
	std::cout << std::endl;
	std::cout << set.capacity() << std::endl;
	set[5000] = True;
	std::cout << set.capacity() << std::endl;
	std::cout << "len " << set.length() << std::endl;
	set[5000] = Unknown;
	std::cout << set.capacity() << std::endl;
	set.shrink();
	std::cout << "len " << set.length() << std::endl;
	std::cout << set.capacity() << std::endl;
	return 0;
}

