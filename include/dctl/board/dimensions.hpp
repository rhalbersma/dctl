#pragma once

namespace dctl {
namespace board {

template<int Height, int Width, bool Parity = false>
struct Dimensions
{
        static_assert(Height > 0, "The height needs to be positive.");
        static_assert(Width > 0, "The width needs to be positive.");

        static constexpr auto height = Height;
        static constexpr auto width = Width;
        static constexpr auto parity = Parity;

        using type = Dimensions<Height, Width, Parity>;
};

}       // namespace board
}       // namespace dctl
