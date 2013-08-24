#pragma once
#include <boost/mpl/quote.hpp>          // quote1, quote2
#include <boost/mpl/vector.hpp>         // vector
#include <dctl/angle/degrees.hpp>       // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/angle/transform.hpp>     // inverse, rotate (partial specialization definitions)
#include <dctl/group/primitives.hpp>    // tuple

namespace dctl {
namespace group {

using C1 = tuple<
        boost::mpl::vector<
                angle::Degrees<angle::D000>
        >,
        boost::mpl::quote2< mpl::lazy::rotate >,
        angle::Degrees<angle::D000>,
        boost::mpl::quote1< mpl::lazy::inverse >
>;

using C2 = tuple<
        boost::mpl::vector<
                angle::Degrees<angle::D000>, angle::Degrees<angle::D180>
        >,
        boost::mpl::quote2< mpl::lazy::rotate >,
        angle::Degrees<angle::D000>,
        boost::mpl::quote1< mpl::lazy::inverse >
>;

using C4 = tuple<
        boost::mpl::vector<
                angle::Degrees<angle::D000>, angle::Degrees<angle::D090>,
                angle::Degrees<angle::D180>, angle::Degrees<angle::D270>
        >,
        boost::mpl::quote2< mpl::lazy::rotate >,
        angle::Degrees<angle::D000>,
        boost::mpl::quote1< mpl::lazy::inverse >
>;

using C8 = tuple<
        boost::mpl::vector<
                angle::Degrees<angle::D000>, angle::Degrees<angle::D045>,
                angle::Degrees<angle::D090>, angle::Degrees<angle::D135>,
                angle::Degrees<angle::D180>, angle::Degrees<angle::D225>,
                angle::Degrees<angle::D270>, angle::Degrees<angle::D315>
        >,
        boost::mpl::quote2< mpl::lazy::rotate >,
        angle::Degrees<angle::D000>,
        boost::mpl::quote1< mpl::lazy::inverse >
>;

}       // namespace group
}       // namespace dctl
