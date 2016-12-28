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

// https://en.wikipedia.org/wiki/Lexicographical_order#Finite_subsets

template<class IntSet, class UnaryFunction>
auto reverse_colex_rank_combination(IntSet const& is, UnaryFunction fun)
{
        auto index = 0LL;
        auto i = 0;
        is.reverse_for_each([&](auto const sq){
                index += choose(fun(sq), ++i);
        });
        return index;
}

template<class IntSet, class UnaryFunction>
auto colex_rank_combination(IntSet const& is, UnaryFunction fun)
{
        auto index = 0LL;
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

// https://en.wikipedia.org/wiki/Positional_notation

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
        int64_t wk_range, bk_range, wp_range, bp_range;
        int64_t wk_value, bk_value, wp_value, bp_value, m_size;
public:
        using position_type = Position;
        using index_type = int64_t;

        subdatabase(int const nbp, int const nwp, int const nbk, int const nwk)
        :
                bp_count{nbp},
                wp_count{nwp},
                bk_count{nbk},
                wk_count{nwk},
                wk_range{choose(wk_squares, wk_count)},
                bk_range{choose(bk_squares, bk_count)},
                wp_range{choose(wp_squares, wp_count)},
                bp_range{choose(bp_squares, bp_count)},
                wk_value{1LL},
                bk_value{wk_range * wk_value},
                wp_value{bk_range * bk_value},
                bp_value{wp_range * wp_value},
                  m_size{bp_range * bp_value}
        {}

        auto size() const noexcept
        {
                return m_size;
        }

        auto rank_position(position_type const& p) const
        {
                auto const bp_index = reverse_colex_rank_combination(p.pieces(black_c, pawns_c), bp_ext);
                auto const wp_index =         colex_rank_combination(p.pieces(white_c, pawns_c), wp_ext);
                auto const bk_index = reverse_colex_rank_combination(p.pieces(black_c, kings_c), bk_ext);
                auto const wk_index =         colex_rank_combination(p.pieces(white_c, kings_c), wk_ext);

                assert(0 <= bp_index); assert(bp_index < bp_range);
                assert(0 <= wp_index); assert(wp_index < wp_range);
                assert(0 <= bk_index); assert(bk_index < bk_range);
                assert(0 <= wk_index); assert(wk_index < wk_range);

                auto index = bp_index * bp_value + wp_index * wp_value + bk_index * bk_value + wk_index;
                assert(0 <= index); assert(index < size());
                return index;
        }

        auto unrank_position(index_type index) const
                -> std::experimental::optional<position_type>
        {
                assert(0 <= index); assert(index < size());

                auto const bp_index = index / bp_value; index %= bp_value;
                auto const wp_index = index / wp_value; index %= wp_value;
                auto const bk_index = index / bk_value; index %= bk_value;
                auto const wk_index = index; assert(wk_value == 1);

                assert(0 <= bp_index); assert(bp_index < bp_range);
                assert(0 <= wp_index); assert(wp_index < wp_range);
                assert(0 <= bk_index); assert(bk_index < bk_range);
                assert(0 <= wk_index); assert(wk_index < wk_range);

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
