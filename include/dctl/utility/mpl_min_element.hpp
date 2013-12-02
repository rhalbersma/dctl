#pragma once
#include <boost/mpl/bind.hpp>           // bind
#include <boost/mpl/lambda.hpp>         // lambda
#include <boost/mpl/less.hpp>           // less
#include <boost/mpl/max_element.hpp>    // max_element
#include <boost/mpl/placeholders.hpp>   // _1, _2

namespace dctl {
namespace mpl {

template
<
        class Sequence,
        class Predicate = boost::mpl::less<boost::mpl::_1, boost::mpl::_2>
>
struct min_element
:
        boost::mpl::max_element<
                  Sequence,
                  boost::mpl::bind< typename
                          boost::mpl::lambda<Predicate>::type,
                          boost::mpl::_2, boost::mpl::_1
                  >
        >
{};

}       // namespace mpl
}       // namespace dctl
