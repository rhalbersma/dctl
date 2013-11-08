#pragma once
#include <stdexcept>                    // invalid_argument
#include <tuple>                        // tie
#include <dctl/angle.hpp>               // Angle

namespace dctl {
namespace grid {

class DimensionsObject
{
public:
        // structors

        constexpr DimensionsObject(int h, int w, bool p) noexcept
        :
                height_{h},
                width_{w},
                parity_{p}
        {}

        // queries

        constexpr auto height() const noexcept { return height_; }
        constexpr auto width () const noexcept { return width_ ; }
        constexpr auto parity() const noexcept { return parity_; }

        // predicates

        friend /* constexpr */ auto operator==(DimensionsObject const& lhs, DimensionsObject const& rhs) noexcept
        {
                return (
                       std::tie(lhs.height_, lhs.width_, lhs.parity_) ==
                       std::tie(rhs.height_, rhs.width_, rhs.parity_)
                );
        }

        friend /* constexpr */ auto operator!=(DimensionsObject const& lhs, DimensionsObject const& rhs) noexcept
        {
                return !(lhs == rhs);
        }

private:
        // representation

        int height_;
        int width_;
        bool parity_;
};

inline
constexpr auto rotate(DimensionsObject const& dim, Angle const& theta)
{
        switch (theta) {
        case   0: return dim;
        case  90: return DimensionsObject{ dim.width() , dim.height(), static_cast<bool>((dim.height() % 2) ^ !dim.parity()) };
        case 180: return DimensionsObject{ dim.height(), dim.width() , static_cast<bool>((dim.height() % 2) ^ (dim.width() % 2) ^ (!!dim.parity())) };
        case 270: return DimensionsObject{ dim.width() , dim.height(), static_cast<bool>((dim.width()  % 2) ^ !dim.parity()) };
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

        template<int Direction>
        struct Rotate
        {
                static constexpr auto theta = Angle{Direction};
                static constexpr auto rotated = rotate(DimensionsObject{height, width, parity}, theta);
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
