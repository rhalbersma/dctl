#pragma once
#include <stdexcept>                    // logic_error
#include <dctl/angle/angle.hpp>         // make_angle
#include <dctl/angle/degrees.hpp>       // D000, L090, R090, D180

namespace dctl {
namespace grid {

template<int Height, int Width, bool Parity = false>
class Dimensions
{
public:
        static_assert(Height > 0, "Dimensions height shall be positive.");
        static_assert(Width > 0, "Dimensions width shall be positive.");

        static constexpr auto height = Height;
        static constexpr auto width = Width;
        static constexpr auto parity = Parity;

private:
        class Object
        {
        public:
                constexpr Object(int h, int w, bool p) noexcept
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

        template<class T>
        static constexpr auto rotate(Object const& dim, T const& theta)
        {
                switch(dctl::make_angle(theta)) {
                case angle::D000: return dim;
                case angle::L090: return Object{ dim.width() , dim.height(), static_cast<bool>((dim.height() % 2) ^ !dim.parity()) };
                case angle::R090: return Object{ dim.width() , dim.height(), static_cast<bool>((dim.width()  % 2) ^ !dim.parity()) };
                case angle::D180: return Object{ dim.height(), dim.width() , static_cast<bool>((dim.height() % 2) ^ (dim.width() % 2) ^ (!!dim.parity())) };
                default: return throw std::logic_error("Dimensions rotation angles shall be a multiple of 90 degrees."), dim;
                }
        }

public:
        template<int Theta>
        struct Rotate
        {
                static constexpr auto r = rotate(Object{height, width, parity}, Theta);
                using type = Dimensions<r.height(), r.width(), r.parity()>;
        };
};

template<class T, int Theta>
using Rotate_t = typename T::template Rotate<Theta>::type;

}       // namespace grid
}       // namespace dctl
