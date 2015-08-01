#pragma once
#include <dctl/piece.hpp>       // pawn, king
#include <cstddef>              // size_t

namespace dctl {
namespace detail {

template<class Board>
class PrecedenceQuality
{
        std::size_t num_captured_[2];
public:
        PrecedenceQuality() = default;

        template<class Tracker>
        explicit constexpr PrecedenceQuality(Tracker const& t) noexcept
        :
                num_captured_{t.captured(Piece::pawn).count(), t.captured(Piece::king).count()}
        {}

        constexpr auto num_captured(Piece p) const noexcept
        {
                return num_captured_[xstd::to_underlying_type(p)];
        }
};

}       // namespace detail
}       // namespace dctl
