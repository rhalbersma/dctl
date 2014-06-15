#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/successor/count/jump.hpp>                // Count (jump specialization)
#include <dctl/successor/count/all_push.hpp>            // Count (piece push specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/successor/select/push.hpp>               // push

namespace dctl {
namespace successor {

// partial specialization for legal successors
template<bool Color, class Pieces>
struct Count<Color, Pieces, select::legal>
{
        template<class Position>
        int operator()(Position const& p)
        {
                using Jump = Count<Color, Pieces, select::jump>;
                using Push = Count<Color, Pieces, select::push>;

                auto num_moves = Jump{}(p);
                if (!num_moves)
                        num_moves += Push{}(p);
                return num_moves;
        }
};

}       // namespace successor
}       // namespace dctl
