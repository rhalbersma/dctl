#include <dctl/board/types.hpp>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestBoard)

template<class>
struct remove_orthogonal_capture;

template<int Width, int Height, bool Inverted, bool OrthogonalCapture>
struct remove_orthogonal_capture<Board<Width, Height, Inverted, OrthogonalCapture>>
{
        using type = Board<Width, Height, Inverted, false>;
};

template<class T>
using remove_orthogonal_capture_t = typename remove_orthogonal_capture<T>::type;

template<class>
struct add_orthogonal_capture;

template<int Width, int Height, bool Inverted, bool OrthogonalCapture>
struct add_orthogonal_capture<Board<Width, Height, Inverted, OrthogonalCapture>>
{
        using type = Board<Width, Height, Inverted, true>;
};

template<class T>
using add_orthogonal_capture_t = typename add_orthogonal_capture<T>::type;

using SmallBoardSequence = boost::mpl::vector
<
        Micro,
        Mini,
        Checkers,
        International,
        Frisian,
        Roman,
        Spantsireti,
        Ktar<10, 11>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(SetTypeMaxSizeIs64, T, SmallBoardSequence)
{
        static_assert(remove_orthogonal_capture_t<T>::set_type::max_size() == 64, "");
        static_assert(   add_orthogonal_capture_t<T>::set_type::max_size() == 64, "");
}

using IntermediateBoardSequence = boost::mpl::vector
<
        Ktar <10, 12>,
        Board<12, 10>
>;

BOOST_AUTO_TEST_CASE(RemoveAddOrthogonalCapture)
{
        static_assert(std::is_same<remove_orthogonal_capture_t<Ktar <10, 12>>, Compact1012>::value, "");
        static_assert(std::is_same<   add_orthogonal_capture_t<Compact1012>, Ktar <10, 12>>::value, "");

        static_assert(std::is_same<remove_orthogonal_capture_t<Board<12, 10>>, Compact1210>::value, "");
        static_assert(std::is_same<   add_orthogonal_capture_t<Compact1210>, Board<12, 10>>::value, "");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(SetTypeMaxSizeIs64Or128, T, IntermediateBoardSequence)
{
        static_assert(remove_orthogonal_capture_t<T>::set_type::max_size() ==  64, "");
        static_assert(   add_orthogonal_capture_t<T>::set_type::max_size() == 128, "");
}

using LargeBoardSequence = boost::mpl::vector
<
        Canadian,
        Dumm,
        SriLankan
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(SetTypeMaxSizeIs128, T, LargeBoardSequence)
{
        static_assert(remove_orthogonal_capture_t<T>::set_type::max_size() == 128, "");
        static_assert(   add_orthogonal_capture_t<T>::set_type::max_size() == 128, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
