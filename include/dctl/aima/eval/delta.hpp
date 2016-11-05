#pragma once
#include <dctl/aima/eval/feature.hpp>   // Feature
#include <dctl/color_piece.hpp>               // flip

namespace dctl {
namespace evaluate {

template<class color>
class Delta
{
private:
        using Active  = Feature<         color >;
        using Passive = Feature<opposite<color>>;

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

}       // namespace evaluate
}       // namespace dctl
