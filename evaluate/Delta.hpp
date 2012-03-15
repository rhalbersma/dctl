#pragma once
#include "Feature.hpp"
#include "../node/Position_fwd.hpp"

namespace dctl {

template
<
        bool Color,
        typename Rules,
        typename Board
>
class Delta
{
private:
        // typedefs
        typedef Feature< Color, Rules, Board> Active;
        typedef Feature<!Color, Rules, Board> Passive;

public:
        static int evaluate(const Position<Board>& p)
        {
                return Active::evaluate(p) - Passive::evaluate(p);
        }

private:
        static int material(const Position<Board>& p)
        {
                return Active::material(p) - Passive::material(p);
        }

        static int tempo(const Position<Board>& p)
        {
                return Active::tempo(p) - Passive::tempo(p);
        }

        static int center(const Position<Board>& p)
        {
                return Active::center(p) - Passive::center(p);
        }

        static int balance(const Position<Board>& p)
        {
                return Active::balance(p) - Passive::balance(p);
        }

        static int mobility(const Position<Board>& p)
        {
                return Active::mobility(p) - Passive::mobility(p);
        }
};

}       // namespace dctl
