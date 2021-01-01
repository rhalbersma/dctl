#pragma once

//          Copyright Rein Halbersma 2010-2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <xstd/type_traits.hpp> // is_integral_constant_v
#include <xstd/utility.hpp>     // to_underlying
#include <type_traits>          // integral_constant

namespace dctl::core {

enum struct piece
:
        unsigned char
{
        pawn,
        king,
        size
};

template<piece N>
using piece_ = std::integral_constant<piece, N>;

using pawn_ = piece_<piece::pawn>;
using king_ = piece_<piece::king>;

template<class T>
inline constexpr auto is_piece = std::is_same_v<T, piece> || xstd::is_integral_constant_v<T, piece>;

template<piece N>
inline constexpr auto piece_c = piece_<N>();

inline constexpr auto pawn_c = piece_c<piece::pawn>;
inline constexpr auto king_c = piece_c<piece::king>;

struct board_ {};
struct empty_ {};
struct occup_ {};

inline constexpr auto board_c = board_();
inline constexpr auto empty_c = empty_();
inline constexpr auto occup_c = occup_();

}       // namespace dctl::core
