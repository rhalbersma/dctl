#include <dctl/board/types.hpp>                 // micro, mini, checkers, roman, spantsireti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, compact1012, compact120, rectangular<12, 10>, canadian, srilankan, dumm
#include <dctl/utility/type_traits.hpp>         // set_t
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                          // is_same

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestBoard)

template<class>
struct remove_orthogonal_capture;

template<int Width, int Height, bool Inverted, bool OrthogonalCapture>
struct remove_orthogonal_capture<rectangular<Width, Height, Inverted, OrthogonalCapture>>
{
        using type = rectangular<Width, Height, Inverted, false>;
};

template<class T>
using remove_orthogonal_capture_t = typename remove_orthogonal_capture<T>::type;

template<class>
struct add_orthogonal_capture;

template<int Width, int Height, bool Inverted, bool OrthogonalCapture>
struct add_orthogonal_capture<rectangular<Width, Height, Inverted, OrthogonalCapture>>
{
        using type = rectangular<Width, Height, Inverted, true>;
};

template<class T>
using add_orthogonal_capture_t = typename add_orthogonal_capture<T>::type;

using SmallBoardSequence = boost::mpl::vector
<
        micro,
        mini,
        checkers,
        roman,
        spantsireti,
        international,
        frisian,
        ktar<10, 11>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(SetTypeMaxSizeIs64, T, SmallBoardSequence)
{
        static_assert(set_t<remove_orthogonal_capture_t<T>>::size() <= 64);
        static_assert(set_t<   add_orthogonal_capture_t<T>>::size() <= 64);
}

using IntermediateBoardSequence = boost::mpl::vector
<
        ktar <10, 12>,
        rectangular<12, 10>
>;

BOOST_AUTO_TEST_CASE(RemoveAddOrthogonalCapture)
{
        static_assert(std::is_same_v<remove_orthogonal_capture_t<ktar <10, 12>>, compact1012>);
        static_assert(std::is_same_v<   add_orthogonal_capture_t<compact1012>, ktar <10, 12>>);

        static_assert(std::is_same_v<remove_orthogonal_capture_t<rectangular<12, 10>>, compact1210>);
        static_assert(std::is_same_v<   add_orthogonal_capture_t<compact1210>, rectangular<12, 10>>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetTypeMaxSizeIs64Or128, T, IntermediateBoardSequence)
{
        static_assert(set_t<remove_orthogonal_capture_t<T>>::size() <=  64);
        static_assert(set_t<   add_orthogonal_capture_t<T>>::size() <= 128);
}

using LargeBoardSequence = boost::mpl::vector
<
        canadian,
        dumm,
        srilankan
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(SetTypeMaxSizeIs128, T, LargeBoardSequence)
{
        static_assert(set_t<remove_orthogonal_capture_t<T>>::size() <= 128);
        static_assert(set_t<   add_orthogonal_capture_t<T>>::size() <= 128);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
