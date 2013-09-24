#pragma once

namespace dctl {
namespace bit {
namespace detail {

template<class T, class WordT, int Nw>
struct base_set;

template<class T, class WordT>
struct base_set<T, WordT, 1>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
