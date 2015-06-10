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
        return board::to_ulo(sq, B::inner_grid).y;
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

int main()
{
    RANGES_FOR(auto r, B::squares() | by_row() | format_rows())
            std::cout << r << '\n';
}
