#pragma once
#include <tuple>        // forward_as_tuple

namespace dctl {
namespace frisian {
namespace precedence {

struct equal_to
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return
                        std::forward_as_tuple(lhs.num_pieces(), lhs.num_kings(), lhs.is_with_king()) ==
                        std::forward_as_tuple(rhs.num_pieces(), rhs.num_kings(), rhs.is_with_king())
                ;
        }
};

struct less
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                auto const delta_pieces = lhs.num_pieces() - rhs.num_pieces();
                auto const delta_kings = lhs.num_kings() - rhs.num_kings();
                auto const delta_pawns = delta_pieces - delta_kings;

                // Art. 11
                if (delta_kings * delta_pawns < 0)
                        // delta_kings and delta_pawns are both non-zero and have opposite sign
                        // [2 n - 1] pawns < [n] kings < [2 n] pawns
                        return delta_pawns + 2 * delta_kings - (delta_kings > 0) <  0;
                else
                        // delta_kings or delta_pawns is zero or they have equal sign
                        return
                                std::forward_as_tuple(lhs.num_pieces(), lhs.is_with_king()) <
                                std::forward_as_tuple(rhs.num_pieces(), rhs.is_with_king())
                        ;
        }
};

}       // namespace precedence
}       // namespace frisian
}       // namespace dctl
