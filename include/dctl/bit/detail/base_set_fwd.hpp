#pragma once

namespace dctl {
namespace bit {
namespace detail {

template<class WordT, int Nw>
struct base_set;

template<class WordT>
struct base_set<WordT, 1>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
