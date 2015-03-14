#pragma once
#include <dctl/color.hpp>               // Color
#include <dctl/evaluate/feature.hpp>    // Feature

namespace dctl {
namespace evaluate {

template<Color ToMove>
class Delta
{
private:
        using Active  = Feature< ToMove>;
        using Passive = Feature<!ToMove>;

public:
        template<class Position>
        static auto value(Position const& p)
        {
                return Active::value(p) - Passive::value(p);
        }

private:
        template<class Position>
        static auto material(Position const& p)
        {
                return Active::material(p) - Passive::material(p);
        }

        template<class Position>
        static auto tempo(Position const& p)
        {
                return Active::tempo(p) - Passive::tempo(p);
        }

        template<class Position>
        static auto center(Position const& p)
        {
                return Active::center(p) - Passive::center(p);
        }

        template<class Position>
        static auto balance(Position const& p)
        {
                return Active::balance(p) - Passive::balance(p);
        }

        template<class Position>
        static auto mobility(Position const& p)
        {
                return Active::mobility(p) - Passive::mobility(p);
        }
};

}       // namespace evaluate
}       // namespace dctl