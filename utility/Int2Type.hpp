#pragma once
#include <boost/config.hpp>

/*

        Trick by Andrei Alexandrescu, Dr. Dobb's, October 01, 2001
        http://www.drdobbs.com/cpp/184403750?_requestid=260625

        Int2Type gives a distinct type for each constant integral value passed.
        This is because different template instantiations are distinct types,
        so Int2Type<0> is different from Int2Type<1> and so on.

        This "tag dispatching" simulates partial specialization of function templates.

*/

namespace dctl {

template<int V>
struct Int2Type
{
        BOOST_STATIC_CONSTANT(auto, value = V);
};

}       // namespace dctl
