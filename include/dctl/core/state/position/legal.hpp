#pragma once
#include <dctl/core/board/traits.hpp>        // promotion_v, squares_v
#include <dctl/util/type_traits.hpp> // board_t

namespace dctl::core {

template<class Board, class Set>
constexpr auto is_onboard_pieces(Set const occupied) noexcept
{
        return is_subset_of(occupied, squares_v<Board>);
}

template<class Board, class Set>
constexpr auto is_promoted_pawns(Set const black_pawns, Set const white_pawns) noexcept
{
        return
                intersects(black_pawns, promotion_v<Board, black_>) ||
                intersects(white_pawns, promotion_v<Board, white_>)
        ;
}

template<class Set>
constexpr auto is_overlapping_pieces(Set const black_pawns, Set const white_pawns, Set const black_kings, Set const white_kings) noexcept
{
        return
                intersects(black_pawns | black_kings, white_pawns | white_kings) ||
                intersects(black_pawns | white_pawns, black_kings | white_kings)
        ;
}

template<class Set>
constexpr auto is_overlapping_pieces(Set const black_pawns, Set const white_pawns) noexcept
{
        return intersects(black_pawns, white_pawns);
}

template<class Board, class Set>
constexpr auto is_legal(Set const black_pawns, Set const white_pawns, Set const black_kings, Set const white_kings) noexcept
{
        return
                is_onboard_pieces<Board>(black_pawns | white_pawns | black_kings | white_kings) &&
                !is_promoted_pawns<Board>(black_pawns, white_pawns) &&
                !is_overlapping_pieces(black_pawns, white_pawns,  black_kings, white_kings)
        ;
}

template<class Board, class Set>
constexpr auto is_legal(Set const black_pawns, Set const white_pawns) noexcept
{
        return
                is_onboard_pieces<Board>(black_pawns | white_pawns) &&
                !is_promoted_pawns<Board>(black_pawns, white_pawns) &&
                !is_overlapping_pieces(black_pawns, white_pawns)
        ;
}

}       // namespace dctl::core
