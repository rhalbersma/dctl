#include <dctl/board/types.hpp>
#include <range/v3/all.hpp>
#include <boost/format.hpp>
#include <cstddef>
#include <iostream>

using namespace dctl;
using namespace ranges;
using square = std::size_t;

template<class Board, class Square>
auto row_number(Square sq)
{
        return board::to_ulo(sq, Board::outer_grid).y;
}

template<class Board>
auto by_row()
{
        return view::group_by([](auto a, auto b) {
                return row_number<Board>(a) == row_number<Board>(b);
        });
}

auto format_square(square sq)
{
        return boost::str(boost::format("%|3|") % sq);
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

int main()
{
    using B = board::International;

    RANGES_FOR(auto r, B::squares() | by_row<B>() | format_rows())
            std::cout << r << '\n';
}
