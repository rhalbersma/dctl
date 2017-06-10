#include <core/board/sequence.hpp>                   // micro, mini, checkers, Roman, spantsiretti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, Compact_10_12, Compact_12_10, rectangular<12, 10>, canadian, srilankan, dumm
#include <dctl/core/board/jump_group.hpp>       // jump_group
#include <dctl/util/type_traits.hpp>         // set_t
#include <boost/algorithm/cxx11/all_of.hpp>     // all_of
#include <boost/range/numeric.hpp>              // accumulate
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <vector>

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(TestBoard)

BOOST_AUTO_TEST_CASE_TEMPLATE(jump_groupsEquivalencePartitionSquares, T, board::BoardSequence)
{
        auto const jump_groups = std::vector<int>{ 0, 1, 2, 3 };

        BOOST_CHECK(
                boost::algorithm::all_of(jump_groups, [=](auto i){
                        return boost::algorithm::all_of(jump_groups, [=](auto j){
                                return i == j ? true : disjoint(jump_group<T>{}(i), jump_group<T>{}(j));
                        });
                })
        );

        BOOST_CHECK(
                boost::accumulate(jump_groups, set_t<T>{}, [](auto result, auto i){
                        return result ^ jump_group<T>{}(i);
                }) == squares_v<T>
        );
}

BOOST_AUTO_TEST_SUITE_END()
