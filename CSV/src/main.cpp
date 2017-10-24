#include "tuple.h"
#include "csv_parser.h"
#include <string>

int main() {
    try {
        CsvParser<int, int, std::string, double > f("input.txt");
        for (auto item : f) {
            std::cout << item << std::endl;
        }
    } catch (std::exception const& exception) {
        std::cerr << exception.what() << std::endl;
    }

    return 0;
}