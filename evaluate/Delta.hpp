#pragma once
#include "Feature.hpp"

namespace dctl {

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
        static int evaluate(const Position& p)
        {
                return Active::evaluate(p) - Passive::evaluate(p);
        }

private:
        template<typename Position>
        static int material(const Position& p)
        {
                return Active::material(p) - Passive::material(p);
        }

        template<typename Position>
        static int tempo(const Position& p)
        {
                return Active::tempo(p) - Passive::tempo(p);
        }

        template<typename Position>
        static int center(const Position& p)
        {
                return Active::center(p) - Passive::center(p);
        }

        template<typename Position>
        static int balance(const Position& p)
        {
                return Active::balance(p) - Passive::balance(p);
        }

        template<typename Position>
        static int mobility(const Position& p)
        {
                return Active::mobility(p) - Passive::mobility(p);
        }
};

}       // namespace dctl
