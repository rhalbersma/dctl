#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/eval/feature.hpp>        // Feature
#include <dctl/core/state/color.hpp>    // not_

namespace dctl::eval {

template<class Color>
class Delta
{
private:
        using Active  = Feature<               Color >;
        using Passive = Feature<core::not_<Color>>;

public:
        template<class State>
        static auto value(State const& s)
        {
                return Active::value(s) - Passive::value(s);
        }

private:
        template<class State>
        static auto material(State const& s)
        {
                return Active::material(s) - Passive::material(s);
        }

        template<class State>
        static auto tempo(State const& s)
        {
                return Active::tempo(s) - Passive::tempo(s);
        }

        template<class State>
        static auto center(State const& s)
        {
                return Active::center(s) - Passive::center(s);
        }

        template<class State>
        static auto balance(State const& s)
        {
                return Active::balance(s) - Passive::balance(s);
        }

        template<class State>
        static auto mobility(State const& s)
        {
                return Active::mobility(s) - Passive::mobility(s);
        }
};

}       // namespace dctl::eval
