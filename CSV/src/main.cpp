#include "tuple.h"


int main() {
    auto t = std::make_tuple(1, "foo", std::make_tuple(511, 24179, "fdfdf"));
    std::cout << t;
    return 0;
}