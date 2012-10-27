#pragma once
#include <boost/mpl/quote.hpp>          // quote1, quote2
#include <boost/mpl/vector.hpp>         // vector
#include <dctl/angle/Degrees.hpp>       // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/angle/transform.hpp>     // inverse, rotate (partial specialization definitions)
#include <dctl/group/primitives.hpp>    // tuple

namespace dctl {
namespace group {

typedef tuple<
        boost::mpl::vector<
                angle::D000
        >,
        boost::mpl::quote2< mpl::rotate >,
        angle::D000,
        boost::mpl::quote1< mpl::inverse >
> C1;

typedef tuple<
        boost::mpl::vector<
                angle::D000, angle::D180
        >,
        boost::mpl::quote2< mpl::rotate >,
        angle::D000,
        boost::mpl::quote1< mpl::inverse >
> C2;

typedef tuple<
        boost::mpl::vector<
                angle::D000, angle::D090, angle::D180, angle::D270
        >,
        boost::mpl::quote2< mpl::rotate >,
        angle::D000,
        boost::mpl::quote1< mpl::inverse >
> C4;

typedef tuple<
        boost::mpl::vector<
                angle::D000, angle::D045, angle::D090, angle::D135,
                angle::D180, angle::D225, angle::D270, angle::D315
        >,
        boost::mpl::quote2< mpl::rotate >,
        angle::D000,
        boost::mpl::quote1< mpl::inverse >
> C8;

}       // namespace group
}       // namespace dctl
