#include <dctl/board/types.hpp>
#include <range/v3/all.hpp>
#include <boost/format.hpp>
#include <cstddef>
#include <iostream>

using namespace dctl;
using namespace ranges;
using square = int;
using B = board::checkers;

inline
auto row_number(square sq)
{
        return board::detail::to_ulo(sq, B::inner_grid).y;
}

inline
auto by_row()
{
        return view::group_by([](auto a, auto b) {
                return row_number(a) == row_number(b);
        });
}

inline
auto format_square(square sq)
{
        return boost::str(boost::format("%|3|") % B::bit_from_square(sq));
}

inline
auto format_rows()
{
        return view::transform([](auto row){
                return boost::str(boost::format("%1%%2%%|31t|")
                        % std::string(3, ' ')
                        % (row | view::transform(format_square) | action::join)
                );
        });
}

inline
auto byte_number(square sq)
{
        return sq / 8;
}

inline
auto by_byte()
{
        return view::group_by([](auto a, auto b) {
                return byte_number(a) == byte_number(b);
        });
}

inline
auto format_bit(square b)
{
        return boost::str(boost::format("%|3|") % b);
}

inline
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
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, board::rectangular<Width, Height, Inverted, OrthogonalCaptures> const& b)
{
        using Coord = board::detail::coordinates<board::detail::upper_left>;
        using namespace xstd::support_literals;

        RANGES_FOR(auto y, view::iota(0_zu, b.height)) {
                RANGES_FOR(auto x, view::iota(0_zu, b.width)) {
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
