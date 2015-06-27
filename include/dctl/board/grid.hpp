#pragma once
#include <dctl/angle.hpp>               // Angle
#include <dctl/board/dimensions.hpp>    // Dimensions
#include <xstd/cstddef.hpp>             // _z
#include <cassert>                      // assert
#include <cstddef>                      // size_t

namespace dctl {
namespace board {

class InnerGrid
{
        Dimensions dim;
public:
        explicit constexpr InnerGrid(Dimensions const& d) noexcept : dim{d} {}

        constexpr auto width() const noexcept { return dim.width(); }
        constexpr auto height() const noexcept { return dim.height(); }
        constexpr auto inverted() const noexcept { return dim.inverted(); }
        constexpr auto edge() const noexcept { return 0; }

        constexpr bool ll_parity() const noexcept { return board::ll_parity(dim); }
        constexpr bool ul_parity() const noexcept { return board::ul_parity(dim); }

        constexpr auto modulo() const noexcept { return width(); }

        constexpr auto edge_le() const noexcept { return 0;                                              }
        constexpr auto edge_re() const noexcept { return edge_le() + ((width() +  ul_parity()) / 2 - 1); }
        constexpr auto edge_lo() const noexcept { return edge_re() + 1;                                  }
        constexpr auto edge_ro() const noexcept { return edge_lo() + ((width() + !ul_parity()) / 2 - 1); }

        constexpr auto size() const noexcept { return (width() * height()) / 2 + (width() * height() * ul_parity()) % 2; }

        friend constexpr auto rotate(InnerGrid const& g, Angle const& a)
        {
                return InnerGrid{rotate(g.dim, a)};
        }
};

class OuterGrid
{
        InnerGrid inner_;
        std::size_t edge_;
public:
        constexpr OuterGrid(InnerGrid const& i, std::size_t e) noexcept : inner_{i}, edge_{e} {}

        constexpr auto width() const noexcept { return inner_.width(); }
        constexpr auto height() const noexcept { return inner_.height(); }
        constexpr auto inverted() const noexcept { return inner_.inverted(); }
        constexpr auto edge() const noexcept { return edge_; }

        constexpr auto ll_parity() const noexcept { return inner_.ll_parity(); }
        constexpr auto ul_parity() const noexcept { return inner_.ul_parity(); }
private:
        constexpr auto left_down () const noexcept { return (width() + edge()) / 2; }
        constexpr auto right_down() const noexcept { return left_down() + 1;        }
public:
        constexpr auto modulo() const noexcept { return left_down() + right_down(); }

        constexpr auto edge_le() const noexcept { return inner_.edge_le(); }
        constexpr auto edge_re() const noexcept { return inner_.edge_re(); }
        constexpr auto edge_lo() const noexcept { return ul_parity() ? right_down() : left_down(); }
        constexpr auto edge_ro() const noexcept { return edge_lo() + (inner_.edge_ro() - inner_.edge_lo()); }

        constexpr auto size() const noexcept { return modulo() * ((height() - 1) / 2) + ((height() % 2) ? edge_re() : edge_ro()) + 1; }

        friend constexpr auto rotate(OuterGrid const& g, Angle const& a)
        {
                return OuterGrid{rotate(g.inner_, a), g.edge_};
        }
};

}       // namespace board
}       // namespace dctl
