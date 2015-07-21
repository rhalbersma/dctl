#pragma once
#include <dctl/color.hpp>       // Player
#include <dctl/piece.hpp>       // Piece

namespace dctl {
namespace actions {
namespace detail {

template
<
        Color /* ToMove     */,
        Piece /* With       */,
        class /* Select     */,
        bool  /* Reverse  */,
        class /* State   */
>
class Detect;

}       // namespace detail
}       // namespace actions
}       // namespace dctl
