#pragma once
#include <dctl/evaluate/Feature.hpp>

namespace dctl {
namespace evaluate {

template
<
        bool Color
>
class Delta
{
private:
        // typedefs
        typedef Feature< Color> Active;
        typedef Feature<!Color> Passive;

public:
        template<typename Position>
        static int value(Position const& p)
        {
                return (Active::value(p) - Passive::value(p));
        }

private:
        template<typename Position>
        static int material(Position const& p)
        {
                return (Active::material(p) - Passive::material(p));
        }

        template<typename Position>
        static int tempo(Position const& p)
        {
                return (Active::tempo(p) - Passive::tempo(p));
        }

        template<typename Position>
        static int center(Position const& p)
        {
                return (Active::center(p) - Passive::center(p));
        }

        template<typename Position>
        static int balance(Position const& p)
        {
                return (Active::balance(p) - Passive::balance(p));
        }

        template<typename Position>
        static int mobility(Position const& p)
        {
                return (Active::mobility(p) - Passive::mobility(p));
        }
};

}       // namespace evaluate
}       // namespace dctl
