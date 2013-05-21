#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/contains.hpp>       // contains
#include <boost/mpl/end.hpp>            // end
#include <boost/mpl/find_if.hpp>        // find_if
#include <boost/mpl/logical.hpp>        // not_

namespace dctl {
namespace mpl {

// predicate extension of boost::mpl::contains
template<class Sequence, class Pred>
struct contains_if
:
        boost::mpl::not_<
                std::is_same< typename
                        boost::mpl::find_if< Sequence, Pred >::type, typename
                        boost::mpl::end< Sequence >::type
                >
        >
{};

// compile-time equivalent of std::any_of
template<class Sequence, class Pred>
struct any_of
:
        contains_if<Sequence, Pred>
{};

// compile-time equivalent of std::all_of
template<class Sequence, class Pred>
struct all_of
:
        boost::mpl::not_< contains_if< Sequence, boost::mpl::not_< Pred > > >
{};

// compile-time equivalent of std::none_of
template<class Sequence, class Pred>
struct none_of
:
        boost::mpl::not_< contains_if< Sequence, Pred > >
{};

// equivalent to boost::mpl::contains
// dctl::mpl::detect / boost::mpl::count / boost::mpl::find / boost::mpl::copy
template<class Sequence, class T>
struct detect
:
        boost::mpl::contains< Sequence, T >
{};

// equivalent to dctl::mpl::contains_if
// dctl::mpl::detect_if / boost::mpl::count_if / boost::mpl::find_if / boost::mpl::copy_if
template<class Sequence, class Pred>
struct detect_if
:
        contains_if< Sequence, Pred >
{};

}       // namespace mpl
}       // namespace dctl
