#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/at.hpp>             // at
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/vector.hpp>         // vector

namespace dctl {
namespace group {

template
<
        typename Set,
        typename Plus,
        typename Identity,
        typename Minus
>
struct tuple
:
        boost::mpl::vector<Set, Plus, Identity, Minus>
{};

template<typename G>
struct set
:
        boost::mpl::at< G, boost::mpl::int_<0> >
{};

template<typename G>
struct plus
:
        boost::mpl::at< G, boost::mpl::int_<1> >
{};

template<typename G>
struct identity
:
        boost::mpl::at< G, boost::mpl::int_<2> >
{};

template<typename G>
struct minus
:
        boost::mpl::at< G, boost::mpl::int_<3> >
{};

}       // namespace group
}       // namespace dctl
