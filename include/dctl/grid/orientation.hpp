#pragma once
#include <dctl/grid/dimensions.hpp>             // Rotate
#include <dctl/grid/grid.hpp>                   // Grid
#include <dctl/utility/mpl_min_element.hpp>     // instead of boost::mpl::min_element
#include <boost/mpl/deref.hpp>                  // deref
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/less_equal.hpp>             // less
#include <boost/mpl/placeholders.hpp>           // _1
#include <boost/mpl/transform_view.hpp>         // transform_view
#include <boost/mpl/vector_c.hpp>               // vector_c

namespace dctl {
namespace grid {
namespace detail {

template<class Dimensions, class EdgeColumns, class Orientation>
struct make
:
        GridClass<Rotate<Dimensions, Orientation>, EdgeColumns::value>
{};

template<class Grid>
struct size
:
        boost::mpl::int_<Grid::size>
{};

}       // namespace detail

template
<
        class Dimensions,
        int EdgeColumns,
        class Orientations = boost::mpl::vector_c<int, 0, 90, 180, 270>
>
struct SizeMinimizingOrientation
:
        boost::mpl::deref< typename
                dctl::mpl::min_element< typename
                        boost::mpl::transform_view<
                                Orientations, detail::size<detail::make<Dimensions, boost::mpl::int_<EdgeColumns>, boost::mpl::_1>>
                        >::type
                >::type::base
        >::type
{};

template<class Dimensions, int EdgeColumns, int Orientation>
struct Make
:
        // delegate to wrapper that takes type template parameters
        // in order to hide the Boost.MPL implementation from clients
        detail::make<Dimensions, boost::mpl::int_<EdgeColumns>, boost::mpl::int_<Orientation>>
{};

}       // namespace grid
}       // namespace dctl
