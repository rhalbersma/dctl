#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/at.hpp>             // at
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/vector.hpp>         // vector

namespace dctl {
namespace group {

template
<
        class Set,
        class Plus,
        class Identity,
        class Minus
>
struct tuple
:
        boost::mpl::vector<Set, Plus, Identity, Minus>
{};

template<class G>
struct set
:
        boost::mpl::at< G, boost::mpl::int_<0> >
{};

template<class G>
struct plus
:
        boost::mpl::at< G, boost::mpl::int_<1> >
{};

template<class G>
struct identity
:
        boost::mpl::at< G, boost::mpl::int_<2> >
{};

template<class G>
struct minus
:
        boost::mpl::at< G, boost::mpl::int_<3> >
{};

}       // namespace group
}       // namespace dctl
