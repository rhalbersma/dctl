#pragma once
#include <dctl/angle.hpp>       // Angle
#include <stdexcept>            // invalid_argument
#include <tuple>                // forward_as_tuple

namespace dctl {
namespace grid {

struct Dimensions
{
        int width, height;
        bool inverted;
};

constexpr auto ll_parity(Dimensions const& dim) noexcept
{
        return !dim.inverted;
}

constexpr auto ul_parity(Dimensions const& dim) noexcept
{
        return static_cast<bool>(dim.inverted ^ (dim.height % 2));
}

inline
constexpr auto
operator==(Dimensions const& lhs, Dimensions const& rhs) noexcept
{
        return
               std::forward_as_tuple(lhs.width, lhs.height, lhs.inverted) ==
               std::forward_as_tuple(rhs.width, rhs.height, rhs.inverted)
        ;
}

inline
constexpr auto
operator!=(Dimensions const& lhs, Dimensions const& rhs) noexcept
{
        return !(lhs == rhs);
}

inline
constexpr auto rotate(Dimensions const& dim, Angle const& theta)
{
        switch (theta) {
        case   0: return dim;
        case  90: return Dimensions{ dim.height, dim.width , static_cast<bool>((dim.width % 2) ^                    !dim.inverted) };
        case 180: return Dimensions{ dim.width , dim.height, static_cast<bool>((dim.width % 2) ^ (dim.height % 2) ^  dim.inverted) };
        case 270: return Dimensions{ dim.height, dim.width , static_cast<bool>(                  (dim.height % 2) ^ !dim.inverted) };
        default: return throw std::invalid_argument("Dimensions rotation angles shall be a multiple of 90 degrees."), dim;
        }
}

template<int Width, int Height, bool Inverted = false>
class DimensionsClass
{
public:
        static_assert(0 < Width, "Dimensions width shall be positive.");
        static_assert(0 < Height, "Dimensions height shall be positive.");

        static constexpr auto width = Width;
        static constexpr auto height = Height;
        static constexpr auto inverted = Inverted;

        static constexpr auto ul_parity = static_cast<bool>(Inverted ^ (Height % 2));

        template<int Direction>
        struct DoRotate
        {
                static constexpr auto theta = Angle{Direction};
                static constexpr auto rotated = rotate(Dimensions{width, height, inverted}, theta);
                using type = DimensionsClass<rotated.width, rotated.height, rotated.inverted>;
        };
};

template<int Width, int Height, bool Inverted>
constexpr int DimensionsClass<Width, Height, Inverted>::width;

template<int Width, int Height, bool Inverted>
constexpr int DimensionsClass<Width, Height, Inverted>::height;

template<int Width, int Height, bool Inverted>
constexpr bool DimensionsClass<Width, Height, Inverted>::inverted;

template<int Width, int Height, bool Inverted>
constexpr bool DimensionsClass<Width, Height, Inverted>::ul_parity;

// NOTE: a template alias does not work if Theta is a Boost.MPL placeholder expression
template<class T, int Theta>
struct Rotate
:
        T::template DoRotate<Theta>::type
{};

}       // namespace grid
}       // namespace dctl
