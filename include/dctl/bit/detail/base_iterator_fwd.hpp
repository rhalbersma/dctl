#pragma once

namespace dctl {
namespace bit {
namespace detail {

template<class WordT, int Nw>
struct base_iterator;

template<class WordT>
struct base_iterator<WordT, 1>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
