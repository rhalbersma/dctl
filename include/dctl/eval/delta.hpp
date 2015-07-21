#pragma once
#include <dctl/color.hpp>               // Player
#include <dctl/eval/feature.hpp>    // Feature

namespace dctl {
namespace evaluate {

template<Color ToMove>
class Delta
{
private:
        using Active  = Feature< ToMove>;
        using Passive = Feature<!ToMove>;

public:
        template<class State>
        static auto value(State const& p)
        {
                return Active::value(p) - Passive::value(p);
        }

private:
        template<class State>
        static auto material(State const& p)
        {
                return Active::material(p) - Passive::material(p);
        }

        template<class State>
        static auto tempo(State const& p)
        {
                return Active::tempo(p) - Passive::tempo(p);
        }

        template<class State>
        static auto center(State const& p)
        {
                return Active::center(p) - Passive::center(p);
        }

        template<class State>
        static auto balance(State const& p)
        {
                return Active::balance(p) - Passive::balance(p);
        }

        template<class State>
        static auto mobility(State const& p)
        {
                return Active::mobility(p) - Passive::mobility(p);
        }
};

}       // namespace evaluate
}       // namespace dctl
