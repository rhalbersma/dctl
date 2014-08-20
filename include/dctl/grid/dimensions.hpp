#pragma once
#include <dctl/angle.hpp>               // Angle
#include <stdexcept>                    // invalid_argument
#include <tuple>                        // forward_as_tuple

namespace dctl {
namespace grid {

class DimensionsObject
{
public:
        // constructors

        constexpr DimensionsObject(int w, int h, bool i) noexcept
        :
                width_{w},
                height_{h},
                inverted_{i}
        {}

        // observers

        constexpr auto width ()   const noexcept { return width_   ; }
        constexpr auto height()   const noexcept { return height_  ; }
        constexpr auto inverted() const noexcept { return inverted_; }

        constexpr auto ll_parity() const noexcept
        {
                return !inverted_;
        }
        constexpr auto ul_parity() const noexcept
        {
                return static_cast<bool>(inverted_ ^ (height_ % 2));
        }

        // predicates

        friend /* constexpr */ auto
        operator==(DimensionsObject const& lhs, DimensionsObject const& rhs) noexcept
        {
                return
                       std::forward_as_tuple(lhs.width_, lhs.height_, lhs.inverted_) ==
                       std::forward_as_tuple(rhs.width_, rhs.height_, rhs.inverted_)
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
        bool inverted_;
};

inline
constexpr auto rotate(DimensionsObject const& dim, Angle const& theta)
{
        switch (theta) {
        case   0: return dim;
        case  90: return DimensionsObject{ dim.height(), dim.width() , static_cast<bool>((dim.width()  % 2) ^ !dim.inverted()) };
        case 180: return DimensionsObject{ dim.width() , dim.height(), static_cast<bool>((dim.width()  % 2) ^ (dim.height() % 2) ^ (!!dim.inverted())) };
        case 270: return DimensionsObject{ dim.height(), dim.width() , static_cast<bool>((dim.height() % 2) ^ !dim.inverted()) };
        default: return throw std::invalid_argument("Dimensions rotation angles shall be a multiple of 90 degrees."), dim;
        }
}

template<int Width, int Height, bool Inverted = false>
class Dimensions
{
public:
        static_assert(0 < Width, "Dimensions width shall be positive.");
        static_assert(0 < Height, "Dimensions height shall be positive.");

        static constexpr auto width = Width;
        static constexpr auto height = Height;
        static constexpr auto inverted = Inverted;

        static constexpr auto ll_parity = !Inverted;
        static constexpr auto ul_parity = static_cast<bool>(Inverted ^ (Height % 2));

        template<class Direction>
        struct DoRotate
        {
                static constexpr auto theta = Angle{Direction::value};
                static constexpr auto rotated = rotate(DimensionsObject{width, height, inverted}, theta);
                using type = Dimensions<rotated.width(), rotated.height(), rotated.inverted()>;
        };
};

template<int Width, int Height, bool Inverted>
constexpr int Dimensions<Width, Height, Inverted>::width;

template<int Width, int Height, bool Inverted>
constexpr int Dimensions<Width, Height, Inverted>::height;

template<int Width, int Height, bool Inverted>
constexpr bool Dimensions<Width, Height, Inverted>::inverted;

template<int Width, int Height, bool Inverted>
constexpr bool Dimensions<Width, Height, Inverted>::ll_parity;

template<int Width, int Height, bool Inverted>
constexpr bool Dimensions<Width, Height, Inverted>::ul_parity;

// NOTE: a template alias does not work if Theta is a Boost.MPL placeholder expression
template<class T, class Theta>
struct Rotate
:
        T::template DoRotate<Theta>::type
{};

}       // namespace grid
}       // namespace dctl
