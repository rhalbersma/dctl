#pragma once
#include <dctl/piece.hpp>       // pawn, king
#include <xstd/type_traits.hpp> // to_underlying_type
#include <cstddef>              // size_t

namespace dctl {
namespace detail {

template<class Board>
class PrecedenceQuality
{
        std::size_t num_captured_[2] {};
public:
        PrecedenceQuality() = default;

        constexpr auto num_captured(Piece p) const noexcept
        {
                return num_captured_[xstd::to_underlying_type(p)];
        }

        constexpr auto num_captured_kings() const noexcept
        {
                return num_captured(Piece::king);
        }
};

}       // namespace detail
}       // namespace dctl
