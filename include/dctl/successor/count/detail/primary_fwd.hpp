#pragma once
#include <dctl/color.hpp>       // Color
#include <dctl/piece.hpp>       // Piece

namespace dctl {
namespace successor {
namespace detail {

template
<
        Color /* ToMove     */,
        Piece /* With       */,
        class /* Select     */,
        bool  /* Reverse  */,
        class /* Position   */
>
class Count;

}       // namespace detail
}       // namespace successor
}       // namespace dctl
