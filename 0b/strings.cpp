#include <fstream>
#include <iostream>
#include "strings.h"

namespace Strings {
	void sort_strings(std::list<std::string> &strings) {
		strings.sort();
	}

	int read_file(char const *input, std::list<std::string> &strings) {
		std::ifstream is(input);
		if (!is.is_open()) {
			std::cerr << std::string(input) << " not found." << std::endl;
			return EXIT_FAILURE;
		}

		_read_stream(is, strings);

		return EXIT_SUCCESS;
	}

	int write_file(char const *output, std::list<std::string> const &strings) {
		std::ofstream os(output);
		if (!os.is_open()) {
			std::cerr << "Could not open " << std::string(output) << "." << std::endl;
			return EXIT_FAILURE;
		}

		for (auto const &s : strings) {
			os << s;
		}

		return EXIT_SUCCESS;
	}

	void _read_stream(std::istream &is, std::list<std::string> &strings) {
		std::string str;
		while (std::getline(is, str)) {
			if (!is.eof()) {
				str += '\n';
			}

			strings.push_back(str);
		}
	}

}
