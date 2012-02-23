#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/end.hpp>            // end
#include <boost/mpl/find_if.hpp>        // find_if
#include <boost/mpl/logical.hpp>        // not_ 

namespace dctl {
namespace mpl {

template<typename Sequence, typename Pred>
struct all_of
:
        std::is_same< typename 
                boost::mpl::find_if<
                        Sequence,
                        boost::mpl::not_<Pred>
                >::type, typename 
                boost::mpl::end<Sequence>::type
        >
{};

template<typename Sequence, typename Pred>
struct none_of
:
        all_of< Sequence, boost::mpl::not_< Pred > >
{};

template<typename Sequence, typename Pred>
struct any_of
:
        boost::mpl::not_< none_of< Sequence, Pred > >
{};

}       // namespace mpl
}       // namespace dctl
