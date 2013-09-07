#pragma once

namespace dctl {
namespace grid {
namespace detail {

class Dimensions
{
public:
        constexpr Dimensions(int h, int w, bool p) noexcept
        :
                height_(h),
                width_(w),
                parity_(p)
        {}

        constexpr auto height() const noexcept
        {
                return height_;
        }

        constexpr auto width() const noexcept
        {
                return width_;
        }

        constexpr auto parity() const noexcept
        {
                return parity_;
        }

private:
        int height_;
        int width_;
        bool parity_;
};

}       // namespace detail

template<int Height, int Width, bool Parity = false>
struct Dimensions
{
        static_assert(Height > 0, "The height shall be positive.");
        static_assert(Width > 0, "The width shall be positive.");

        static constexpr auto height = Height;
        static constexpr auto width = Width;
        static constexpr auto parity = Parity;

        static constexpr auto Object() noexcept
        {
                return detail::Dimensions{height, width, parity};
        }
};

}       // namespace grid
}       // namespace dctl
