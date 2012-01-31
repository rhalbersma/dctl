#pragma once
#include <boost/mpl/list.hpp>           // list
#include <boost/mpl/quote.hpp>          // quote1, quote2
#include "Group.hpp"
#include "Angle.hpp"
#include "Degrees.hpp"

namespace dctl {
namespace group {

typedef Group<
        boost::mpl::list<
                angle<degrees::D000> 
        >,
        boost::mpl::quote2< rotate >,
        angle<degrees::D000>,
        boost::mpl::quote1< inverse >
> C1;

typedef Group<
        boost::mpl::list<
                angle<degrees::D000>, 
                angle<degrees::D180>
        >,
        boost::mpl::quote2< rotate >,
        angle<degrees::D000>,
        boost::mpl::quote1< inverse >
> C2;

typedef Group<
        boost::mpl::list<
                angle<degrees::D000>, 
                angle<degrees::D090>, 
                angle<degrees::D180>, 
                angle<degrees::D270>
        >,
        boost::mpl::quote2< rotate >,
        angle<degrees::D000>,
        boost::mpl::quote1< inverse >
> C4;

typedef Group<
        boost::mpl::list<
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

}       // namespace group
}       // namespace dctl
