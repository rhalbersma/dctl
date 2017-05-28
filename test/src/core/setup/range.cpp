#include <dctl/core/board.hpp>
#include <boost/range/irange.hpp>
#include <iostream>

using namespace dctl;
using square = int;
using B = board::checkers;

template<class CharT, class Traits, int Width, int Height, bool Inverted, bool OrthogonalCaptures>
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, board::rectangular<Width, Height, Inverted, OrthogonalCaptures> const& b)
{
        using Coord = board::detail::coordinates<board::detail::upper_left>;
        for (auto y : boost::irange(0, b.height)) {
                for (auto x : boost::irange(0, b.width)) {
                        if (auto const coord = Coord{x, y}; b.is_square(coord)) {
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
