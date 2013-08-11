#pragma once

namespace dctl {
namespace board {

template<int H, int W, bool P = false>
struct Dimensions
{
        static_assert(H > 0, "A board cannot have non-positive height.");
        static_assert(W > 0, "A board cannot have non-positive width.");

        static constexpr auto height = H;
        static constexpr auto width = W;
        static constexpr auto parity = P;

        using type = Dimensions<H, W, P>;
};

}       // namespace board
}       // namespace dctl
