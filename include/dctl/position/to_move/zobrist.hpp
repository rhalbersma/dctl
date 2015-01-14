#pragma once
#include <dctl/color.hpp>               // Color
#include <dctl/random/uniform.hpp>      // cat_zero_runif
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <array>                        // array
#include <cstddef>                      // size_t
#include <cstdint>                      // uint64_t

namespace dctl {
namespace zobrist {

template<class = void>
class ToMove
{
        static std::array<uint64_t, 2> const to_move_;
public:
        static auto to_move(Color c) noexcept
        {
                return to_move_[xstd::to_underlying_type(c)];
        }
};

template<class _>
std::array<uint64_t, 2> const
ToMove<_>::to_move_ = random::cat_zero_runif<1>();

}       // namespace zobrist
}       // namespace dctl
