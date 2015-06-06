#pragma once
#include <xstd/bitset.hpp>      // bitset
#include <cassert>              // assert
#include <cstddef>              // size_t

namespace dctl {

constexpr auto align_on(std::size_t offset, std::size_t align) noexcept
{
        assert(align && !(align & (align - 1)));
        return (offset + align - 1) & ~(align - 1);
}

template<class Board>
using set_type = xstd::bitset<align_on(Board::bits(), 64)>;

}       // dctl
