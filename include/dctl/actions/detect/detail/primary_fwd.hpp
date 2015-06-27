#pragma once
#include <dctl/player.hpp>       // Player
#include <dctl/piece.hpp>       // Piece

namespace dctl {
namespace actions {
namespace detail {

template
<
        Player /* ToMove     */,
        Piece /* With       */,
        class /* Select     */,
        bool  /* Reverse  */,
        class /* State   */
>
class Detect;

}       // namespace detail
}       // namespace actions
}       // namespace dctl
