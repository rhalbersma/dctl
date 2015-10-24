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

        template<class Builder>
        explicit constexpr PrecedenceQuality(Builder const& b) noexcept
        :
                num_captured_{b.captured(Piece::pawn).count(), b.captured(Piece::king).count()}
        {}

        constexpr auto num_captured(Piece p) const noexcept
        {
                return num_captured_[xstd::to_underlying_type(p)];
        }
};

}       // namespace detail
}       // namespace dctl
