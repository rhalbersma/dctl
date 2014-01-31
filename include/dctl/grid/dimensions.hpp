#pragma once
#include <stdexcept>                    // invalid_argument
#include <tuple>                        // forward_as_tuple
#include <dctl/angle.hpp>               // Angle

namespace dctl {
namespace grid {

class DimensionsObject
{
public:
        // structors

        constexpr DimensionsObject(int w, int h, bool p) noexcept
        :
                width_{w},
                height_{h},
                parity_{p}
        {}

        // queries

        constexpr auto width () const noexcept { return width_ ; }
        constexpr auto height() const noexcept { return height_; }
        constexpr auto parity() const noexcept { return parity_; }

        // predicates

        friend /* constexpr */ auto
        operator==(DimensionsObject const& lhs, DimensionsObject const& rhs) noexcept
        {
                return
                       std::forward_as_tuple(lhs.width_, lhs.height_, lhs.parity_) ==
                       std::forward_as_tuple(rhs.width_, rhs.height_, rhs.parity_)
                ;
        }

        friend /* constexpr */ auto
        operator!=(DimensionsObject const& lhs, DimensionsObject const& rhs) noexcept
        {
                return !(lhs == rhs);
        }

private:
        // representation

        int width_;
        int height_;
        bool parity_;
};

inline
constexpr auto rotate(DimensionsObject const& dim, Angle const& theta)
{
        switch (theta) {
        case   0: return dim;
        case  90: return DimensionsObject{ dim.height(), dim.width() , static_cast<bool>((dim.height() % 2) ^ !dim.parity()) };
        case 180: return DimensionsObject{ dim.width() , dim.height(), static_cast<bool>((dim.width()  % 2) ^ (dim.height() % 2) ^ (!!dim.parity())) };
        case 270: return DimensionsObject{ dim.height(), dim.width() , static_cast<bool>((dim.width()  % 2) ^ !dim.parity()) };
        default: return throw std::invalid_argument("Dimensions rotation angles shall be a multiple of 90 degrees."), dim;
        }
}

template<int Width, int Height, bool Parity = false>
class Dimensions
{
public:
        static_assert(0 < Width, "Dimensions width shall be positive.");
        static_assert(0 < Height, "Dimensions height shall be positive.");

        static constexpr auto width = Width;
        static constexpr auto height = Height;
        static constexpr auto parity = Parity;

        template<class Direction>
        struct DoRotate
        {
                static constexpr auto theta = Angle{Direction::value};
                static constexpr auto rotated = rotate(DimensionsObject{width, height, parity}, theta);
                using type = Dimensions<rotated.width(), rotated.height(), rotated.parity()>;
        };
};

template<int Width, int Height, bool Parity>
constexpr int Dimensions<Width, Height, Parity>::height;

template<int Width, int Height, bool Parity>
constexpr int Dimensions<Width, Height, Parity>::width;

template<int Width, int Height, bool Parity>
constexpr bool Dimensions<Width, Height, Parity>::parity;

// NOTE: a template alias does not work if Theta is a Boost.MPL placeholder expression
template<class T, class Theta>
struct Rotate
:
        T::template DoRotate<Theta>::type
{};

}       // namespace grid
}       // namespace dctl
