#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>  // integral_constant

namespace dctl::core {

enum struct piece
:
        unsigned char
{
        pawn,
        king,
        size
};

template<piece P>
using piece_ = std::integral_constant<piece, P>;

using pawn_ = piece_<piece::pawn>;
using king_ = piece_<piece::king>;

template<piece P>
inline constexpr auto piece_c = piece_<P>();

inline constexpr auto pawn_c = piece_c<piece::pawn>;
inline constexpr auto king_c = piece_c<piece::king>;

struct board_ {};
struct empty_ {};
struct occup_ {};

inline constexpr auto board_c = board_();
inline constexpr auto empty_c = empty_();
inline constexpr auto occup_c = occup_();

}       // namespace dctl::core
