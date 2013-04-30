#pragma once

namespace dctl {
namespace bit {

template<typename T> T zero();
template<typename T> T universe();

template<typename T> T singlet(int);
template<typename T> T reverse_singlet(int);

namespace first {

template<typename T> T equal(T);
template<typename T> T not_equal_to(T);
template<typename T> T greater(T);
template<typename T> T less(T);
template<typename T> T greater_equal(T);
template<typename T> T less_equal(T);

template<typename T> int find(T);

template<typename T> void clear(T&);

}       // namespace first

namespace range {

template<typename T> T equal(int);
template<typename T> T not_equal_to(int);
template<typename T> T greater(int);
template<typename T> T less(int);
template<typename T> T greater_equal(int);
template<typename T> T less_equal(int);

}       // namespace range

template<typename T> std::size_t count(T);
template<typename T> int index(T);

template<typename T> bool is_zero(T);
template<typename T> bool is_single(T);
template<typename T> bool is_double(T);
template<typename T> bool is_multiple(T);

template<typename T> bool is_subset_of(T, T);
template<typename T> bool is_exclusive(T, T);

template<typename Iterator, typename Board>
bool is_element(Iterator, Board);

}       // namespace bit
}       // namespace dctl
