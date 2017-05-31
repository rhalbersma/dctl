#include <dctl/core/board.hpp>                       // micro, mini, checkers, Roman, spantsiretti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, Compact_10_12, Compact_12_10, rectangular<12, 10>, canadian, srilankan, dumm
#include <dctl/core/board_traits.hpp>                // add_orthogonal_captures, remove_orthogonal_captures
#include <dctl/util/type_traits.hpp>         // set_t
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestBoard)

using SmallBoardSequence = boost::mpl::vector
<
        micro,
        mini,
        checkers,
        italian,
        spantsiretti,
        international,
        frisian,
        ktar<10, 11>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(SetTypeMaxSizeIs64, T, SmallBoardSequence)
{
        static_assert(set_t<remove_orthogonal_captures_t<T>>::max_size() <= 64);
        static_assert(set_t<   add_orthogonal_captures_t<T>>::max_size() <= 64);
}

using IntermediateBoardSequence = boost::mpl::vector
<
               ktar<10, 12>,
        rectangular<12, 10>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(SetTypeMaxSizeIs64Or128, T, IntermediateBoardSequence)
{
        static_assert(set_t<remove_orthogonal_captures_t<T>>::max_size() <=  64);
        static_assert(set_t<   add_orthogonal_captures_t<T>>::max_size() <= 128);
}

using LargeBoardSequence = boost::mpl::vector
<
        canadian,
        dumm,
        srilankan
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(SetTypeMaxSizeIs128, T, LargeBoardSequence)
{
        static_assert(set_t<remove_orthogonal_captures_t<T>>::max_size() <= 128);
        static_assert(set_t<   add_orthogonal_captures_t<T>>::max_size() <= 128);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl