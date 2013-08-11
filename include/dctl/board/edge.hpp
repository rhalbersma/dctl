#pragma once

namespace dctl {
namespace board {

template<int NumColumns, int Orientation = 0>
struct Edge
{
        static_assert(NumColumns >= 0, "The number of edge columns shall be non-negative.");
        static_assert(Orientation % 90 == 0, "The edge orientation shall be a multiple of 90 degrees.");

        static constexpr auto num_columns = NumColumns;
        static constexpr auto orientation = Orientation;
};

using ColumnLessEdge = Edge<0>;
using SingleColumnEdge = Edge<1>;
using DoubleColumnEdge = Edge<2>;

}       // namespace board
}       // namespace dctl
