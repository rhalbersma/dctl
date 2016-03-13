#pragma once
#include <dctl/piece.hpp>       // pawn, king
#include <xstd/type_traits.hpp> // to_underlying_type
#include <cstddef>              // size_t

namespace dctl {
namespace detail {

template<class Board>
class PrecedenceQuality
{
        std::size_t num_captured_[2];
public:
        PrecedenceQuality() = default;

        template<class State>
        explicit constexpr PrecedenceQuality(State const&) noexcept
        :
                num_captured_{0, 0}
        {}

        template<class... State>
        constexpr auto num_captured(Piece p, State const&...) const noexcept
        {
                return num_captured_[xstd::to_underlying_type(p)];
        }

        template<class... State>
        constexpr auto num_captured_kings(State const&...) const noexcept
        {
                return num_captured(Piece::king);
        }
};

}       // namespace detail
}       // namespace dctl
