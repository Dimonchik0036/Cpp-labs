#ifndef CSV_TUPLE_H
#define CSV_TUPLE_H

#include <tuple>
#include <iosfwd>


template< typename Ch, typename Tr, size_t n, typename... T >
typename std::enable_if<(n >= sizeof...(T))>::type
print_tuple(std::basic_ostream<Ch, Tr>&, const std::tuple<T...>&) {}

template< typename Ch, typename Tr, size_t n, typename... T >
typename std::enable_if<(n < sizeof...(T))>::type
print_tuple(std::basic_ostream<Ch, Tr>& os, const std::tuple<T...>& t);

template< typename Ch, typename Tr, typename... T >
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<T...>& t);


#include "tuple.tpp"
#endif //CSV_TUPLE_H
