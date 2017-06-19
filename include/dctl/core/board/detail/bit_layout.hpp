#pragma once
#include <dctl/core/board/angle.hpp>    // angle
#include <cassert>                      // assert
#include <tuple>                        // make_tuple

namespace dctl::core {
namespace detail {

struct dimensions
{
        int const width;
        int const height;
        bool const is_inverted;
};

constexpr auto operator==(dimensions const lhs, dimensions const rhs) noexcept
{
        constexpr auto as_tuple = [](auto const d) {
                return std::make_tuple(d.width, d.height, d.is_inverted);
        };
        return as_tuple(lhs) == as_tuple(rhs);
}

constexpr auto operator!=(dimensions const lhs, dimensions const rhs) noexcept
{
        return !(lhs == rhs);
}

constexpr auto width_parity(dimensions const dim) noexcept
{
        return static_cast<bool>(dim.width % 2);
}

constexpr auto height_parity(dimensions const dim) noexcept
{
        return static_cast<bool>(dim.height % 2);
}

constexpr auto lower_left_is_square(dimensions const dim) noexcept
{
        return !dim.is_inverted;
}

constexpr auto upper_left_is_square(dimensions const dim) noexcept
{
        return height_parity(dim) ^ !lower_left_is_square(dim);
}

constexpr auto upper_right_is_square(dimensions const dim) noexcept
{
        return width_parity(dim) ^ height_parity(dim) ^ lower_left_is_square(dim);
}

constexpr auto lower_right_is_square(dimensions const dim) noexcept
{
        return width_parity(dim) ^ !lower_left_is_square(dim);
}

constexpr auto rotate(dimensions const dim, angle const a) noexcept
        -> dimensions
{
        switch (a.value()) {
        case   0 : return dim;
        case  90 : return { dim.height, dim.width , !upper_left_is_square(dim) };
        case 180 : return { dim.width , dim.height, !upper_right_is_square(dim) };
        case 270 : return { dim.height, dim.width , !lower_right_is_square(dim) };
        default  : assert(false); return dim;
        }
}

class InnerGrid
{
        dimensions dim;
public:
        explicit constexpr InnerGrid(dimensions const& d) noexcept : dim{d} {}

        constexpr auto width()       const noexcept { return dim.width; }
        constexpr auto height()      const noexcept { return dim.height; }
        constexpr auto is_inverted() const noexcept { return dim.is_inverted; }

        constexpr auto edge()        const noexcept { return 0; }

        constexpr bool lower_left_is_square() const noexcept { return detail::lower_left_is_square(dim); }
        constexpr bool upper_left_is_square() const noexcept { return detail::upper_left_is_square(dim); }

        constexpr auto modulo() const noexcept { return width(); }

        constexpr auto edge_le() const noexcept { return 0;                                            }
        constexpr auto edge_re() const noexcept { return edge_le() + ((width() +  upper_left_is_square()) / 2 - 1); }
        constexpr auto edge_lo() const noexcept { return edge_re() + 1;                                             }
        constexpr auto edge_ro() const noexcept { return edge_lo() + ((width() + !upper_left_is_square()) / 2 - 1); }

        constexpr auto size() const noexcept { return (width() * height()) / 2 + (width() * height() * upper_left_is_square()) % 2; }

        friend constexpr auto rotate(InnerGrid const g, angle const a)
        {
                return InnerGrid{rotate(g.dim, a)};
        }
};

class bit_layout
{
        InnerGrid const inner_;
        int const edge_;
public:
        constexpr bit_layout(InnerGrid const i, int const e) noexcept : inner_{i}, edge_{e} {}

        constexpr auto width()       const noexcept { return inner_.width(); }
        constexpr auto height()      const noexcept { return inner_.height(); }
        constexpr auto is_inverted() const noexcept { return inner_.is_inverted(); }
        constexpr auto edge() const noexcept { return edge_; }

        constexpr auto lower_left_is_square() const noexcept { return inner_.lower_left_is_square(); }
        constexpr auto upper_left_is_square() const noexcept { return inner_.upper_left_is_square(); }
private:
        constexpr auto left_down () const noexcept { return (width() + edge()) / 2; }
        constexpr auto right_down() const noexcept { return left_down() + 1;        }
public:
        constexpr auto modulo() const noexcept { return left_down() + right_down(); }

        constexpr auto edge_le() const noexcept { return inner_.edge_le(); }
        constexpr auto edge_re() const noexcept { return inner_.edge_re(); }
        constexpr auto edge_lo() const noexcept { return upper_left_is_square() ? right_down() : left_down(); }
        constexpr auto edge_ro() const noexcept { return edge_lo() + (inner_.edge_ro() - inner_.edge_lo());   }

        constexpr auto size() const noexcept { return modulo() * ((height() - 1) / 2) + ((height() % 2) ? edge_re() : edge_ro()) + 1; }

        friend constexpr auto rotate(bit_layout const g, angle const a)
        {
                return bit_layout{rotate(g.inner_, a), g.edge_};
        }
};

}       // namespace detail
}       // namespace dctl::core
