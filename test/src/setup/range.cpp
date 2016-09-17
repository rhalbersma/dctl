#include <dctl/board.hpp>
#include <boost/range/irange.hpp>
#include <cstddef>
#include <iostream>

using namespace dctl;
using square = std::size_t;
using B = board::Checkers;

template<class CharT, class Traits, std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, board::Rectangular<Width, Height, Inverted, OrthogonalCaptures> const& b)
{
        using Coord = board::detail::coordinates<board::detail::upper_left>;
        using namespace xstd::support_literals;

        for (auto y : boost::irange(0_zu, b.height)) {
                for (auto x : boost::irange(0_zu, b.width)) {
                        auto const coord = Coord{x, y};
                        if (b.is_square(coord)) {
                                ostr << std::setw(2) << b.to_square(coord);
                        } else {
                                ostr << std::string(2, ' ');
                        }
                }
                ostr << '\n';
        }
        return ostr;
}

int main()
{
        std::cout << B{};
}
