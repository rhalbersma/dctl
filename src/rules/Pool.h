#pragma once
#include "Rules.h"

namespace dctl {
namespace rules {

// Pool checkers is the default rule set because this requires the least amount of redefinitions for other variants
// http://americanpoolcheckers.us/americanpoolcheckers/index.php?option=com_content&view=article&id=48:the-apca-playing-rules&catid=37:documents&Itemid=56
struct Pool {};

}       // namespace rules
}       // namespace dctl
