#pragma once

namespace dctl {
namespace grid {

template<int Height, int Width, bool Parity = false>
struct Dimensions
{
        static_assert(Height > 0, "The height shall be positive.");
        static_assert(Width > 0, "The width shall be positive.");

        static constexpr auto height = Height;
        static constexpr auto width = Width;
        static constexpr auto parity = Parity;

        using type = Dimensions<Height, Width, Parity>;
};

}       // namespace grid
}       // namespace dctl
