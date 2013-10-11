#pragma once
#include <stdexcept>                    // invalid_argument
#include <tuple>                        // tie
#include <dctl/angle/angle.hpp>         // make_angle
#include <dctl/angle/degrees.hpp>       // D000, L090, R090, D180

namespace dctl {
namespace grid {

class DimensionsObject
{
public:
        constexpr DimensionsObject(int h, int w, bool p) noexcept
        : height_(h), width_(w), parity_(p) {}

        constexpr auto height() const noexcept { return height_; }
        constexpr auto width () const noexcept { return width_ ; }
        constexpr auto parity() const noexcept { return parity_; }

        friend /* constexpr */ auto operator==(DimensionsObject const& L, DimensionsObject const& R) noexcept
        { return std::tie(L.height_, L.width_, L.parity_) == std::tie(R.height_, R.width_, R.parity_); }

        friend /* constexpr */ auto operator!=(DimensionsObject const& L, DimensionsObject const& R) noexcept
        { return !(L == R); }

private:
        int height_;
        int width_;
        bool parity_;
};

template<class T>
constexpr auto rotate(DimensionsObject const& dim, T const& theta)
{
        switch (dctl::make_angle(theta)) {
        case angle::D000: return dim;
        case angle::L090: return DimensionsObject{ dim.width() , dim.height(), static_cast<bool>((dim.height() % 2) ^ !dim.parity()) };
        case angle::R090: return DimensionsObject{ dim.width() , dim.height(), static_cast<bool>((dim.width()  % 2) ^ !dim.parity()) };
        case angle::D180: return DimensionsObject{ dim.height(), dim.width() , static_cast<bool>((dim.height() % 2) ^ (dim.width() % 2) ^ (!!dim.parity())) };
        default: return throw std::invalid_argument("Dimensions rotation angles shall be a multiple of 90 degrees."), dim;
        }
}

template<int Height, int Width, bool Parity = false>
class Dimensions
{
public:
        static_assert(0 < Height, "Dimensions height shall be positive.");
        static_assert(0 < Width, "Dimensions width shall be positive.");

        static constexpr auto height = Height;
        static constexpr auto width = Width;
        static constexpr auto parity = Parity;

        template<int Theta>
        struct Rotate
        {
                static constexpr auto rotated = grid::rotate(DimensionsObject{height, width, parity}, Theta);
                using type = Dimensions<rotated.height(), rotated.width(), rotated.parity()>;
        };
};

template<int Height, int Width, bool Parity>
constexpr int Dimensions<Height, Width, Parity>::height;

template<int Height, int Width, bool Parity>
constexpr int Dimensions<Height, Width, Parity>::width;

template<int Height, int Width, bool Parity>
constexpr bool Dimensions<Height, Width, Parity>::parity;

template<class T, int Theta>
using Rotate = typename T::template Rotate<Theta>::type;

}       // namespace grid
}       // namespace dctl
