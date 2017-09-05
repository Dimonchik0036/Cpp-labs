#include "strings.h"
#include <fstream>
#include <iostream>


int main(int argc, char *argv[]) {
	if (argc < 3) {
		std::cout << "There are not enough arguments" << std::endl;
		return EXIT_SUCCESS;
	}

	std::list<std::string> inputData;
	if (Strings::read_file(argv[1], inputData)) {
		return EXIT_FAILURE;
	}

	Strings::sort_strings(inputData);
	if (Strings::write_file(argv[2], inputData)) {
		return EXIT_FAILURE;
	}
}

