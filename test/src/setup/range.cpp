#include <dctl/board/types.hpp>
#include <range/v3/all.hpp>
#include <boost/format.hpp>
#include <cstddef>
#include <iostream>

using namespace dctl;
using namespace ranges;
using square = std::size_t;
using B = board::International;

auto row_number(square sq)
{
        return board::to_ulo(sq, B::inner_grid).y();
}

auto by_row()
{
        return view::group_by([](auto a, auto b) {
                return row_number(a) == row_number(b);
        });
}

auto format_square(square sq)
{
        return boost::str(boost::format("%|3|") % B::bit_from_square(sq));
}

auto format_rows()
{
        return view::transform([](auto row){
                return boost::str(boost::format("%1%%2%%|31t|")
                        % std::string(3, ' ')
                        % (row | view::transform(format_square) | action::join)
                );
        });
}

auto byte_number(square sq)
{
        return sq / 8;
}

auto by_byte()
{
        return view::group_by([](auto a, auto b) {
                return byte_number(a) == byte_number(b);
        });
}

auto format_bit(square b)
{
        return boost::str(boost::format("%|3|") % b);
}

auto format_bytes()
{
        return view::transform([](auto byte){
                return boost::str(boost::format("%1%%2%%|25t|")
                        % std::string(3, ' ')
                        % (byte | view::transform(format_bit) | action::join)
                );
        });
}

template<class CharT, class Traits, std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, board::Board<Width, Height, Inverted, OrthogonalCaptures> const& b)
{
        using Coord = board::Coordinates<board::origin::UpperLeft>;

        RANGES_FOR(auto y, view::iota(0, b.height())) {
                RANGES_FOR(auto x, view::iota(0, b.width())) {
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
        using namespace xstd::literals;
        std::cout << B{};
}
