#include <tuple>
#include <iostream>

template< typename Ch, typename Tr, size_t n, typename... T >
typename std::enable_if<(n < sizeof...(T))>::type
print_tuple(std::basic_ostream<Ch, Tr>& os, const std::tuple<T...>& t) {
    if (n != 0) {
        os << ", ";
    }

    os << std::get<n>(t);
    print_tuple<Ch, Tr, n + 1>(os, t);
}

template< typename Ch, typename Tr, typename... T >
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<T...>& t) {
    os << "[ ";
    print_tuple<Ch, Tr, 0>(os, t);
    return os << " ]";
}