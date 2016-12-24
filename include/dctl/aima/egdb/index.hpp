#pragma once
#include <dctl/aima/egdb/binomial.hpp>          // choose
#include <dctl/board_traits.hpp>                // promotion_v, squares_v
#include <dctl/color_piece.hpp>                 // black_c, white_c, pawns_c, kings_c
#include <dctl/state/position/legal.hpp>        // is_onboard_pieces, is_promoted_pawns, is_overlapping_pieces
#include <dctl/utility/type_traits.hpp>         // board_t, set_t
#include <experimental/optional>                // nullopt, optional
#include <cassert>                              // assert
#include <cstdint>                              // int64_t

namespace dctl {
namespace egdb {

template<class IntSet, class UnaryFunction>
auto reverse_colex_rank_combination(IntSet const& is, UnaryFunction fun)
{
        auto index = int64_t{0};
        auto i = 0;
        is.reverse_for_each([&](auto const sq){
                index += choose(fun(sq), ++i);
        });
        return index;
}

template<class IntSet, class UnaryFunction>
auto colex_rank_combination(IntSet const& is, UnaryFunction fun)
{
        auto index = int64_t{0};
        auto i = 0;
        is.for_each([&](auto const sq){
                index += choose(fun(sq), ++i);
        });
        return index;
}

template<class IntSet, class UnaryFunction>
auto colex_unrank_combination(int64_t index, int const N, int const K, UnaryFunction fun)
{
        IntSet is{};
        for (auto sq = N, i = K; i > 0; --sq, --i) {
                while (choose(sq, i) > index) {
                        --sq;
                }
                is.insert(fun(sq));
                index -= choose(sq, i);
        }
        return is;
}

template<class Position>
class subdatabase
{
        using board_type = board_t<Position>;
        using   set_type =   set_t<Position>;

        static constexpr auto bp_squares = (squares_v<board_type> ^ promotion_v<board_type, black_>).size();
        static constexpr auto wp_squares = (squares_v<board_type> ^ promotion_v<board_type, white_>).size();
        static constexpr auto bk_squares =  squares_v<board_type>.size();
        static constexpr auto wk_squares =  squares_v<board_type>.size();
            int bp_count, wp_count, bk_count, wk_count;
        int64_t wk_power, bk_power, wp_power, bp_power, m_size;
public:
        using position_type = Position;
        using index_type = int64_t;

        subdatabase(int const nbp, int const nwp, int const nbk, int const nwk)
        :
                bp_count{nbp},
                wp_count{nwp},
                bk_count{nbk},
                wk_count{nwk},
                wk_power{1LL},
                bk_power{wk_power * choose(wk_squares, wk_count)},
                wp_power{bk_power * choose(bk_squares, bk_count)},
                bp_power{wp_power * choose(wp_squares, wp_count)},
                  m_size{bp_power * choose(bp_squares, bp_count)}
        {}

        auto size() const noexcept
        {
                return m_size;
        }

        auto rank_position(position_type const& p) const
                -> index_type
        {
                auto const bp_index = reverse_colex_rank_combination(p.pieces(black_c, pawns_c), bp_ext);
                auto const wp_index =         colex_rank_combination(p.pieces(white_c, pawns_c), wp_ext);
                auto const bk_index = reverse_colex_rank_combination(p.pieces(black_c, kings_c), bk_ext);
                auto const wk_index =         colex_rank_combination(p.pieces(white_c, kings_c), wk_ext);

                auto index = bp_index * bp_power + wp_index * wp_power + bk_index * bk_power + wk_index;
                assert(0 <= index); assert(index < size());
                return index;
        }

        auto unrank_position(index_type index) const
                -> std::experimental::optional<position_type>
        {
                assert(0 <= n); assert(n < size());

                auto const bp_index = index / bp_power; index %= bp_power;
                auto const wp_index = index / wp_power; index %= wp_power;
                auto const bk_index = index / bk_power; index %= bk_power;
                auto const wk_index = index;

                auto const bp = colex_unrank_combination<set_type>(bp_index, bp_squares, bp_count, bp_dep);
                auto const wp = colex_unrank_combination<set_type>(wp_index, wp_squares, wp_count, wp_dep);
                auto const bk = colex_unrank_combination<set_type>(bk_index, bk_squares, bk_count, bk_dep);
                auto const wk = colex_unrank_combination<set_type>(wk_index, wk_squares, wk_count, wk_dep);

                if (is_overlapping_pieces(bp, wp, bk, wk)) {
                        return std::experimental::nullopt;
                }

                assert( is_onboard_pieces<board_type>(bp | wp | bk | wk));
                assert(!is_promoted_pawns<board_type>(bp, wp));
                return position_type(bp, wp, bk, wk);
        }

private:
        static auto bp_ext(int n) { return -board_type::square_from_bit(n) + bp_squares - 1;          }
        static auto wp_ext(int n) { return +board_type::square_from_bit(n) - wk_squares + wp_squares; }
        static auto bk_ext(int n) { return -board_type::square_from_bit(n) + bk_squares - 1;          }
        static auto wk_ext(int n) { return +board_type::square_from_bit(n);                           }

        static auto bp_dep(int n) { return board_type::bit_from_square(-n + bp_squares - 1         ); }
        static auto wp_dep(int n) { return board_type::bit_from_square(+n + wk_squares - wp_squares); }
        static auto bk_dep(int n) { return board_type::bit_from_square(-n + bk_squares - 1         ); }
        static auto wk_dep(int n) { return board_type::bit_from_square(+n                          ); }
};

}       // namespace egdb
}       // namespace dctl
