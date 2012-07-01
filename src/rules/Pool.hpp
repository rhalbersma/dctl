#pragma once
#include "Rules.hpp"
#include "Enum.hpp"
#include "../capture/Value.hpp"

namespace dctl {
namespace rules {

// http://americanpoolcheckers.us/americanpoolcheckers/index.php?option=com_content&view=article&id=48:the-apca-playing-rules&catid=37:documents&Itemid=56
struct Pool
: 
        Rules<
                Pool,
                king_range<range::distance_N>,
                pawn_jump_Compasss<Compasss::diag>,
                jump_precedence<precedence::none>
        > 
{};

}       // namespace rules
}       // namespace dctl
