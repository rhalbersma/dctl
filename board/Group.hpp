#pragma once
#include <boost/mpl/list.hpp>           // list
#include "Angle.hpp"
#include "Degrees.hpp"

namespace dctl {
namespace group {

typedef boost::mpl::list<
        angle<degrees::D000> 
> C1;

typedef boost::mpl::list<
        angle<degrees::D000>, 
        angle<degrees::D180>
> C2;

typedef boost::mpl::list<
        angle<degrees::D000>, 
        angle<degrees::D090>, 
        angle<degrees::D180>, 
        angle<degrees::D270>
> C4;

typedef boost::mpl::list<
        angle<degrees::D000>, 
        angle<degrees::D045>, 
        angle<degrees::D090>, 
        angle<degrees::D135>, 
        angle<degrees::D180>, 
        angle<degrees::D225>, 
        angle<degrees::D270>, 
        angle<degrees::D315>
> C8;

}       // namespace group
}       // namespace dctl
