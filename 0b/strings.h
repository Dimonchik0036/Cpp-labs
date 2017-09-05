#pragma once

#include <string>
#include <list>

namespace Strings {
	int read_file(char const * input, std::list<std::string>& strings);
	int write_file(char const * output, std::list<std::string> const& strings);
	void sort_strings(std::list<std::string> &strings);
	void _read_stream(std::istream &is, std::list<std::string> &strings);
}
