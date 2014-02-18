#pragma once
#include <algorithm>                            // accumulate
#include <cassert>                              // assert
#include <boost/math/special_functions/binomial.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <dctl/bit/detail/intrinsic.hpp>        // count

namespace dctl {
namespace egdb {

inline
int binomial(int n, int k)
{
        return (n < k) ? 0 : static_cast<int>(boost::math::binomial_coefficient<double>(static_cast<unsigned>(n), static_cast<unsigned>(k)));
}

template<class SinglePassRange>
auto colex_subset_rank(SinglePassRange const& rng)
{
        return boost::accumulate(
                rng, 0, [i = 1](auto index, auto sq) mutable {
                return index + binomial(sq, i++);
        });
}

/*
template<class Set>
auto remove_interference(Set const& s, Set const& interference)
{
        return s | boost::adaptors::transformed([](auto const& sq){
                return bit::popcount(interference.before(sq));
        });
}

template<class Set, class Index>
auto placement_from_index(Index index, Set const& interference)
{
        return colex_subset_rank(remove_interference(set, pattern));
}

template<class Position>
auto index_from_position(Position const& p)
{
        constexpr auto board_size = Position::board_type::size();
        auto const num_bk = p.kings(Side::black).count();
        auto const num_wk = p.kings(Side::white).count();
        auto const bk_range = binomial(board_size, num_bk);
        auto const wk_range = binomial(board_size - num_bk, num_wk);

        auto constexpr ghosts = ~board::Squares<typename Position::board_type>::mask();
        auto const bk_index = index_from_placement(p.kings(Side::black), ghosts);
        auto const wk_index = index_from_placement(p.kings(Side::white), ghosts ^ p.kings(Side::black));
        assert(0 <= bk_index && bk_index < bk_range);
        assert(0 <= wk_index && wk_index < wk_range);

        return bk_index + wk_index * bk_range;
}

template<class Position, class Index>
auto position_from_index(Index index)
{
        constexpr auto board_size = Position::board_type::size();
        auto const bk_range = binomial(board_size, Index::num_bk);
        auto const wk_range = binomial(board_size - Index::num_bk, Index::num_wk);

        auto const bk_index = index % bk_range;
        auto const wk_index = index / bk_range;

}
*/
}       // namespace egdb
}       // namespace dctl
