#pragma once
#include <dctl/aima/egdb/binomial.hpp>
#include <dctl/aima/egdb/colex.hpp>
#include <dctl/aima/egdb/obstr.hpp>
#include <dctl/board_traits.hpp>
#include <dctl/utility/type_traits.hpp>
#include <boost/math/special_functions/binomial.hpp>
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <dctl/color_piece.hpp>
#include <dctl/state.hpp>

namespace dctl {
namespace egdb {

inline
auto choose(int n, int k)
{
        //if (n < k) {
        //        return int64_t{0};
        //}
        //return static_cast<int64_t>(boost::math::binomial_coefficient<double>(static_cast<unsigned>(n), static_cast<unsigned>(k)));
        return static_cast<int64_t>(BinomialTable<60,8>::coefficient(n, k));
}

template<class IntSet>
auto comb_rank(IntSet const& is)
{
        assert(std::is_sorted(is.begin(), is.end()));
        auto index = int64_t{0};
        auto i = 0;
        is.for_each([&](auto sq){
                index += choose(sq, ++i);
        });
        return index;
}

template<class IntSet>
auto comb_unrank(int64_t index, int N, int K)
{
        IntSet is{};
        for (auto sq = N, i = K; i > 0; --sq, --i) {
                while (choose(sq, i) > index)
                        --sq;
                is.insert(sq);
                index -= choose(sq, i);
        }
        assert(is.size() == K);
        assert(std::is_sorted(is.begin(), is.end()));
        return is;
}

template<class Board>
struct dbtuple
{
        static constexpr auto nsq = Board::bits();
        int nbp, nwp, nbk, nwk;

        auto range() const
        {
                return
                        bp_range() *
                        wp_range() *
                        bk_range() *
                        wk_range()
                ;
        }

        auto bp_range() const { return choose(nsq, nbp); }
        auto wp_range() const { return choose(nsq, nwp); }
        auto bk_range() const { return choose(nsq, nbk); }
        auto wk_range() const { return choose(nsq, nwk); }
};

template<class Position>
auto make_dbtuple(Position const& p)
{
        return dbtuple<board_t<Position>>
        {
                p.num_pieces(black_c, pawns_c),
                p.num_pieces(white_c, pawns_c),
                p.num_pieces(black_c, kings_c),
                p.num_pieces(white_c, kings_c),
        };
}

template<class Position>
auto position2index(Position const& p)
{
        auto const bp_idx = comb_rank(p.pieces(black_c, pawns_c));
        auto const wp_idx = comb_rank(p.pieces(white_c, pawns_c));
        auto const bk_idx = comb_rank(p.pieces(black_c, kings_c));
        auto const wk_idx = comb_rank(p.pieces(white_c, kings_c));

        auto const t = make_dbtuple(p);
        auto const index = ((bp_idx * t.wp_range() + wp_idx) * t.bk_range() + bk_idx) * t.wk_range() + wk_idx;
        assert(0 <= index); assert(index < t.range());
        return index;
}

template<class Position>
auto index2position(int64_t index, dbtuple<board_t<Position>> const& t)
{
        assert(0 <= index); assert(index < t.range());

        auto const wp_range = t.wp_range();
        auto const bk_range = t.bk_range();
        auto const wk_range = t.wk_range();

        auto mult = wp_range * bk_range * wk_range;
        auto const bp_idx = index / mult; index %= mult; mult /= wp_range;
        auto const wp_idx = index / mult; index %= mult; mult /= bk_range;
        auto const bk_idx = index / mult; index %= mult; assert(mult / wk_range == 1);
        auto const wk_idx = index;

        using set_type = set_t<Position>;
        auto const bp = comb_unrank<set_type>(bp_idx, t.nsq, t.nbp);
        auto const wp = comb_unrank<set_type>(wp_idx, t.nsq, t.nwp);
        auto const bk = comb_unrank<set_type>(bk_idx, t.nsq, t.nbk);
        auto const wk = comb_unrank<set_type>(wk_idx, t.nsq, t.nwk);

        return make_position<Position>(bp, wp, bk, wk);
}

}       // namespace egdb
}       // namespace dctl
