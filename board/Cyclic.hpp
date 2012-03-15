#pragma once
#include <boost/mpl/quote.hpp>          // quote1, quote2
#include <boost/mpl/vector.hpp>         // vector
#include "Angle.hpp"
#include "Degrees.hpp"
#include "Transform.hpp"
#include "../group/primitives.hpp"

namespace dctl {

typedef group::tuple<
        boost::mpl::vector<
                angle<degrees::D000>
        >,
        boost::mpl::quote2< rotate >,
        angle<degrees::D000>,
        boost::mpl::quote1< inverse >
> C1;

typedef group::tuple<
        boost::mpl::vector<
                angle<degrees::D000>,
                angle<degrees::D180>
        >,
        boost::mpl::quote2< rotate >,
        angle<degrees::D000>,
        boost::mpl::quote1< inverse >
> C2;

typedef group::tuple<
        boost::mpl::vector<
                angle<degrees::D000>,
                angle<degrees::D090>,
                angle<degrees::D180>,
                angle<degrees::D270>
        >,
        boost::mpl::quote2< rotate >,
        angle<degrees::D000>,
        boost::mpl::quote1< inverse >
> C4;

typedef group::tuple<
        boost::mpl::vector<
                angle<degrees::D000>,
                angle<degrees::D045>,
                angle<degrees::D090>,
                angle<degrees::D135>,
                angle<degrees::D180>,
                angle<degrees::D225>,
                angle<degrees::D270>,
                angle<degrees::D315>
        >,
        boost::mpl::quote2< rotate >,
        angle<degrees::D000>,
        boost::mpl::quote1< inverse >
> C8;

}       // namespace dctl
