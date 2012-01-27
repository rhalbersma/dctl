#pragma once
#include <boost/mpl/at.hpp>             // at
#include <boost/mpl/list.hpp>           // list
#include <boost/mpl/quote.hpp>          // quote2
#include <boost/mpl/vector.hpp>         // vector
#include "Angle.hpp"
#include "Degrees.hpp"
#include "Transform.hpp"

namespace dctl {
namespace group {

template<
        typename Set,
        typename Plus,
        typename Identity
>
struct Group
:
        boost::mpl::vector<Set, Plus, Identity>
{};

template<typename G>
struct set
:
        boost::mpl::at< G, boost::mpl::int_<0> >
{};

template<typename G>
struct plus
:
        boost::mpl::at< G, boost::mpl::int_<1> >
{};

template<typename G>
struct identity
:
        boost::mpl::at< G, boost::mpl::int_<2> >
{};

typedef Group<
        boost::mpl::list<
                angle<degrees::D000> 
        >,
        boost::mpl::quote2< rotate >,
        angle<degrees::D000>
> C1;

typedef Group<
        boost::mpl::list<
                angle<degrees::D000>, 
                angle<degrees::D180>
        > ,
        boost::mpl::quote2< rotate >,
        angle<degrees::D000>
> C2;

typedef Group<
        boost::mpl::list<
                angle<degrees::D000>, 
                angle<degrees::D090>, 
                angle<degrees::D180>, 
                angle<degrees::D270>
        > ,
        boost::mpl::quote2< rotate >,
        angle<degrees::D000>
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
        > ,
        boost::mpl::quote2< rotate >,
        angle<degrees::D000>
> C8;

}       // namespace group
}       // namespace dctl
