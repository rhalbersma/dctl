#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/apply.hpp>          // apply
#include <boost/mpl/comparison.hpp>     // less, less_equal
#include <boost/mpl/logical.hpp>        // and_

namespace dctl {
namespace mpl {

// F(F(X)) = X
template<class F, class X>
struct is_idempotent
:
        std::is_same< typename
                boost::mpl::apply<
                        F, typename
                        boost::mpl::apply< F, X >::type
                >::type,
                X
        >
{};

// Value lies within the range [First, Last), i.e. First <= Value < Last
template<class Value, class First, class Last>
struct is_within_range
:
        boost::mpl::and_<
                boost::mpl::less_equal< First, Value>,
                boost::mpl::less< Value, Last >
        >
{};

}       // namespace mpl
}       // namespace dctl
