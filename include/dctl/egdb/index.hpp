#pragma once
#include <dctl/egdb/binomial.hpp>               // Binomial
#include <dctl/egdb/detail/rank_insert.hpp>     // RankInsert
#include <dctl/egdb/detail/rank_remove.hpp>     // RankRemove
#include <dctl/egdb/detail/unrank.hpp>          // UnRank
#include <boost/range/adaptor/reversed.hpp>     // reversed
#include <boost/range/adaptor/transformed.hpp>  // transformed
#include <boost/range/irange.hpp>               // irange
#include <boost/range/numeric.hpp>              // accumulate
#include <cassert>                              // assert
#include <utility>                              // pair

namespace dctl {
namespace egdb {

// input: an index in the range [0, choose(N, K))
// output: a strictly decreasing range { sq_K, ..., sq_1 }
// with all elements in the range [0, N)
template<class Index, class Generator = Binomial<>>
auto colex_combination_unrank(std::pair<int /* N */, int /* K */> const& combination, Index index, Generator const& = Generator{})
{
        assert(0 <= index && index < Generator::coefficient(combination.first, combination.second));
        return boost::irange(1, combination.second + 1)
                | boost::adaptors::reversed
                | boost::adaptors::transformed(detail::UnRank<Generator>{index, combination.first})
        ;
}

// input: a strictly increasing pattern range { pat_1, ..., pat_P }
// output: applied to a strictly increasing source range { src_1, ..., src_K },
// a strictly increasing destination range { dst_1, ..., dst_K },
// where dst_i = src_i + delta_i, with delta_i = #(pat_i < dst_i)
template<class Range>
auto rank_inserted(Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return boost::adaptors::transformed(detail::RankInsert<Range>{pat});
}

// input: a strictly increasing pattern range { pat_1, ..., pat_P }
// output: applied to a non-overlapping strictly increasing source range { src_1, ..., src_K },
// a strictly increasing destination range { dst_1, ..., dst_K },
// where dst_i = src_i - delta_i, with delta_i = #(pat_i < src_i)
template<class Range>
auto rank_removed(Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return boost::adaptors::transformed(detail::RankRemove<Range>{pat});
}

// input: a strictly increasing range { sq_1, ..., sq_K }
// with all elements in the range [0, N)
// output: an index in the range [0, choose(N, K))
template<class Range, class Generator = Binomial<>>
auto colex_combination_rank(Range const& src, Generator const& = Generator{})
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        using Index = typename Generator::value_type;
        return boost::accumulate(
                src, Index{0}, [i = 1](auto index, auto sq) mutable {
                return index + Generator::coefficient(sq, i++);
        });
}

template<class Position, class Generator = Binomial<>>
auto index_from_position(Position const& p, Generator const& = Generator{})
{
        return p;
/*
        bk_index = colex_combination_rank(bk | mixed_out(gh), bin);
        wk_index = colex_combination_rank(wk | mixed_out(gh | bk), bin);
*/
}

}       // namespace egdb
}       // namespace dctl
