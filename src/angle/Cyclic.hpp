#pragma once
#include <boost/mpl/quote.hpp>          // quote1, quote2
#include <boost/mpl/vector.hpp>         // vector
#include "Degrees.hpp"
#include "../group/primitives.hpp"
#include "../mpl/transform.hpp"

namespace dctl {
namespace angle {

typedef group::tuple<
        boost::mpl::vector<
                D000
        >,
        boost::mpl::quote2< mpl::rotate >,
        D000,
        boost::mpl::quote1< mpl::inverse >
> C1;

typedef group::tuple<
        boost::mpl::vector<
                D000, D180
        >,
        boost::mpl::quote2< mpl::rotate >,
        D000,
        boost::mpl::quote1< mpl::inverse >
> C2;

typedef group::tuple<
        boost::mpl::vector<
                D000, D090, D180, D270
        >,
        boost::mpl::quote2< mpl::rotate >,
        D000,
        boost::mpl::quote1< mpl::inverse >
> C4;

typedef group::tuple<
        boost::mpl::vector<
                D000, D045, D090, D135,
                D180, D225, D270, D315
        >,
        boost::mpl::quote2< mpl::rotate >,
        D000,
        boost::mpl::quote1< mpl::inverse >
> C8;

}       // namespace angle
}       // namespace dctl
