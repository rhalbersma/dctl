#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core.hpp>                // is_off_board_pieces, is_promoted_pawns, is_overlapping_pieces
#include <dctl/egdb/binomial.hpp>       // choose
#include <dctl/util/type_traits.hpp>    // board_t, set_t
#include <cassert>                      // assert
#include <cstdint>                      // int64_t
#include <numeric>                      // accumulate
#include <optional>                     // nullopt, optional
#include <sstream>                      // stringstream
#include <iostream>

namespace dctl::egdb {

// https://en.wikipedia.org/wiki/Combinatorial_number_system
// https://en.wikipedia.org/wiki/Lexicographical_order#Colexicographic_order
// https://en.wikipedia.org/wiki/Lexicographical_order#Finite_subsets

template<class IntSet, class UnaryFunction>
auto reverse_colex_rank_combination(IntSet const& is, UnaryFunction fun)
{
        return std::accumulate(is.rbegin(), is.rend(), 0LL, [&, i = 1](auto const result, auto const sq) mutable {
                return result + choose(fun(sq), i++);
        });
}

template<class IntSet, class UnaryFunction>
auto colex_rank_combination(IntSet const& is, UnaryFunction fun)
{
        return std::accumulate(is.begin(), is.end(), 0LL, [&, i = 1](auto const result, auto const sq) mutable {
                return result + choose(fun(sq), i++);
        });
}

template<class IntSet, class UnaryFunction>
auto colex_unrank_combination(int64_t index, int const N, int const K, UnaryFunction fun)
{
        assert(0 <= K); assert(K <= N);
        assert(0 <= index); assert(index < choose(N, K));
        IntSet is;
        for (auto sq = N - 1, i = K; i > 0; index -= choose(sq--, i--)) {
                while (choose(sq, i) > index) {
                        --sq;
                }
                is.add(fun(sq));
        }
        assert(static_cast<int>(is.size()) == K);
        return is;
}

// https://en.wikipedia.org/wiki/Mixed_radix

// TODO: leading rank, leading rank configuration, subleading rank
template<class Position>
class subdivision
{
public:
        using board_type = core::board_t<Position>;
        using  mask_type = core:: mask_t<Position>;
        using   set_type = core::  set_t<Position>;

private:
        static constexpr auto CLANG_WORKAROUND_bp_squares = mask_type::squares - mask_type::promotion(core::black_c);
        static constexpr auto CLANG_WORKAROUND_wp_squares = mask_type::squares - mask_type::promotion(core::white_c);

public:
        static constexpr auto bp_squares = static_cast<int>(CLANG_WORKAROUND_bp_squares.size());
        static constexpr auto wp_squares = static_cast<int>(CLANG_WORKAROUND_wp_squares.size());
        static constexpr auto bk_squares = static_cast<int>(mask_type::squares.size());
        static constexpr auto wk_squares = static_cast<int>(mask_type::squares.size());

        int n_count, b_count, w_count;
        int bp_count, wp_count, bk_count, wk_count;
        int64_t wk_radix, bk_radix, wp_radix, bp_radix;
        int64_t wk_value, bk_value, wp_value, bp_value;
        int64_t m_size, m_legal;
public:
        using position_type = Position;
        using index_type = int64_t;

        subdivision(int const b, int const w, int const bp, int const wp)
        :
                n_count{b + w},
                b_count{b},
                w_count{w},
                bp_count{bp},
                wp_count{wp},
                bk_count{b - bp},
                wk_count{w - wp},
                wk_radix{choose(wk_squares, wk_count)},
                bk_radix{choose(bk_squares, bk_count)},
                wp_radix{choose(wp_squares, wp_count)},
                bp_radix{choose(bp_squares, bp_count)},
                wk_value{1LL},
                bk_value{wk_radix * wk_value},
                wp_value{bk_radix * bk_value},
                bp_value{wp_radix * wp_value},
                m_size{bp_radix * bp_value},
                m_legal{count_legal()}
        {}

        auto size() const noexcept
        {
                return m_size;
        }

        auto legal() const noexcept
        {
                return m_legal;
        }

        auto to_string() const
        {
                std::stringstream s;
                s << "db" << n_count << "-" << b_count << w_count << "-" << bp_count << wp_count;
                return s.str();
        }

        auto index(position_type const& p) const
        {
                auto const bp = p.pieces(core::black_c, core::pawn_c);
                auto const wp = p.pieces(core::white_c, core::pawn_c);
                auto const bk = p.pieces(core::black_c, core::king_c);
                auto const wk = p.pieces(core::white_c, core::king_c);

                auto const bp_digit = reverse_colex_rank_combination(bp, bp_ext);
                auto const wp_digit =         colex_rank_combination(wp, wp_ext);
                auto const bk_digit = reverse_colex_rank_combination(bk, bk_ext);
                auto const wk_digit =         colex_rank_combination(wk, wk_ext);

                assert(0 <= bp_digit); assert(bp_digit < bp_radix);
                assert(0 <= wp_digit); assert(wp_digit < wp_radix);
                assert(0 <= bk_digit); assert(bk_digit < bk_radix);
                assert(0 <= wk_digit); assert(wk_digit < wk_radix);

                auto const n =
                        bp_digit * bp_value +
                        wp_digit * wp_value +
                        bk_digit * bk_value +
                        wk_digit * wk_value
                ;
                assert(wk_value == 1);
                assert(0 <= n); assert(n < size());
                return n;
        }

        auto position(index_type n) const
                -> std::optional<position_type>
        {
                assert(0 <= n); assert(n < size());

                auto const bp_digit = n / bp_value; n %= bp_value;
                auto const wp_digit = n / wp_value; n %= wp_value;
                auto const bk_digit = n / bk_value; n %= bk_value;
                auto const wk_digit = n / wk_value; assert(wk_value == 1);

                assert(0 <= bp_digit); assert(bp_digit < bp_radix);
                assert(0 <= wp_digit); assert(wp_digit < wp_radix);
                assert(0 <= bk_digit); assert(bk_digit < bk_radix);
                assert(0 <= wk_digit); assert(wk_digit < wk_radix);

                auto const bp = colex_unrank_combination<set_type>(bp_digit, bp_squares, bp_count, bp_dep);
                auto const wp = colex_unrank_combination<set_type>(wp_digit, wp_squares, wp_count, wp_dep);
                auto const bk = colex_unrank_combination<set_type>(bk_digit, bk_squares, bk_count, bk_dep);
                auto const wk = colex_unrank_combination<set_type>(wk_digit, wk_squares, wk_count, wk_dep);

                if (core::is_overlapping_pieces(bp, wp, bk, wk)) {
                        return std::nullopt;
                }

                assert(!core::is_off_board_pieces<board_type>(bp | wp | bk | wk));
                assert(!core::is_promoted_pawns<board_type>(bp, wp));
                return position_type(bp, wp, bk, wk);
        }

private:
        auto count_legal() const
        {
                auto const b0_squares = static_cast<int>(mask_type::promotion(core::white_c).size());
                auto const w0_squares = static_cast<int>(mask_type::promotion(core::black_c).size());
                auto const center_squares = board_type::size() - b0_squares - w0_squares;
                auto n = 0LL;
                for (auto b0_count  = 0; b0_count <= std::min(b0_squares, bp_count); ++b0_count) {
                        for (auto w0_count = 0; w0_count <= std::min(w0_squares, wp_count); ++w0_count) {
                                n +=
                                        choose(b0_squares, b0_count) *
                                        choose(w0_squares, w0_count) *
                                        choose(center_squares                        , bp_count - b0_count) *
                                        choose(center_squares - (bp_count - b0_count), wp_count - w0_count) *
                                        choose(bk_squares - bp_count - wp_count           , bk_count) *
                                        choose(wk_squares - bp_count - wp_count - bk_count, wk_count)
                                ;
                        }
                }
                return n;
        }

        static auto bp_ext(int n) { return -board_type::numeric0(n) + bp_squares - 1;          }
        static auto wp_ext(int n) { return +board_type::numeric0(n) - wk_squares + wp_squares; }
        static auto bk_ext(int n) { return -board_type::numeric0(n) + bk_squares - 1;          }
        static auto wk_ext(int n) { return +board_type::numeric0(n);                           }

        static auto bp_dep(int n) { return board_type::embedding0(-n + bp_squares - 1         ); }
        static auto wp_dep(int n) { return board_type::embedding0(+n + wk_squares - wp_squares); }
        static auto bk_dep(int n) { return board_type::embedding0(-n + bk_squares - 1         ); }
        static auto wk_dep(int n) { return board_type::embedding0(+n                          ); }
};

}       // namespace dctl::egdb
