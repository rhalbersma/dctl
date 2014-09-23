#pragma once
#include <dctl/evaluate/feature.hpp>

namespace dctl {
namespace evaluate {

template<bool Color>
class Delta
{
private:
        using Active = Feature<Color>;
        using Passive = Feature<!Color>;

public:
        template<class Position>
        static int value(Position const& p)
        {
                return Active::value(p) - Passive::value(p);
        }

private:
        template<class Position>
        static int material(Position const& p)
        {
                return Active::material(p) - Passive::material(p);
        }

        template<class Position>
        static int tempo(Position const& p)
        {
                return Active::tempo(p) - Passive::tempo(p);
        }

        template<class Position>
        static int center(Position const& p)
        {
                return Active::center(p) - Passive::center(p);
        }

        template<class Position>
        static int balance(Position const& p)
        {
                return Active::balance(p) - Passive::balance(p);
        }

        template<class Position>
        static int mobility(Position const& p)
        {
                return static_cast<int>(Active::mobility(p) - Passive::mobility(p));
        }
};

}       // namespace evaluate
}       // namespace dctl
