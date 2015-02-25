#pragma once
#include <xstd/bitset.hpp>      // bitset
#include <cassert>              // assert

namespace dctl {

template<class T>
constexpr auto align_on(T offset, T align) noexcept
{
        assert(align && !(align & (align - 1)));
        return (offset + align - 1) & ~(align - 1);
}

template<class Board>
using set_type = xstd::bitset<align_on(Board::bits(), 64)>;

}       // dctl
