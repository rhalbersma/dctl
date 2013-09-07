#pragma once
#include <boost/preprocessor/repetition.hpp>    // BOOST_PP_ENUM
#include <dctl/angle/degrees.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/grid/coordinates/transform.hpp>
#include <dctl/grid/dimensions/transform.hpp>
#include <dctl/grid/edge.hpp>
#include <dctl/grid/grid.hpp>
#include <dctl/grid/predicates.hpp>
#include <dctl/node/side.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace board {

template<class Dimensions, class Edge = grid::DoubleColumnEdge>
struct Board
:
        public Dimensions, public Edge
{
        static constexpr auto dim = grid::detail::rotate(Dimensions::Object(), Edge::orientation);
        using ReOrientedDimensions = grid::Dimensions< dim.height(), dim.width(), dim.parity() >;

public:
        // internal and external grids
        using InternalGrid = grid::Grid<ReOrientedDimensions, Edge>;
        using ExternalGrid = grid::Grid<Dimensions, grid::ZeroColumnEdge>;

        using bit_type = BitBoard;

        static constexpr auto shift_size(int direction)
        {
                return InternalGrid::shift_size(direction);
        }

        static constexpr auto begin() noexcept
        {
                return 0;
        }

        static constexpr auto end() noexcept
        {
                return ExternalGrid::size;
        }

        static constexpr auto is_valid(int square) noexcept
        {
                return begin() <= square && square < end();
        }

private:
        template<class DestGrid, class FromSquare>
        static constexpr auto transform(FromSquare const& from_sq, int theta)
        {
                return grid::coordtosq<DestGrid>(grid::rotate(grid::sqtocoord(from_sq), theta));
        }

        static constexpr auto init_square2bit(int n) noexcept
        {
                return transform<InternalGrid>(grid::Square<ExternalGrid>{n}, Edge::orientation).value();
        }

        static constexpr auto init_bit2square(int n) noexcept
        {
                return transform<ExternalGrid>(grid::Square<InternalGrid>{n}, angle::inverse(Edge::orientation)).value();
        }

#define DCTL_PP_SQUARE2BIT(z, i, data) init_square2bit(i)

        static constexpr int table_square2bit[] =
        {
                BOOST_PP_ENUM(64, DCTL_PP_SQUARE2BIT, ~)
        };

#undef DCTL_PP_SQUARE2BIT

#define DCTL_PP_BIT2SQUARE(z, i, data) init_bit2square(i)

        static constexpr int table_bit2square[] =
        {
                BOOST_PP_ENUM(64, DCTL_PP_BIT2SQUARE, ~)
        };

#undef DCTL_PP_BIT2SQUARE

public:
        static constexpr auto square2bit(int number) noexcept
        {
                return table_square2bit[number];
        }

        static constexpr auto bit2square(int number) noexcept
        {
                return table_bit2square[number];
        }

private:
        template<class Predicate /* NOTE: no lambda in constexpr */>
        static constexpr auto copy_if(Predicate pred) noexcept
        {
                auto result = BitBoard{0};
                for (auto sq = 0; sq < ExternalGrid::size; ++sq)
                        if (pred(sq))
                                result ^= BitBoard{1} << square2bit(sq);
                return result;
        }

public:
        static constexpr BitBoard squares = copy_if(grid::is_square<ExternalGrid>{});

#define DCTL_PP_INITIAL_MASK(z, i, data) copy_if(grid::is_initial<ExternalGrid, i, data>{})

        static constexpr BitBoard initial_mask[][5] =
        {
                { BOOST_PP_ENUM(5, DCTL_PP_INITIAL_MASK, Side::black) },
                { BOOST_PP_ENUM(5, DCTL_PP_INITIAL_MASK, Side::white) }
        };

#undef DCTL_PP_INITIAL_MASK

#define DCTL_PP_ROW_MASK(z, i, data) copy_if(grid::is_row<ExternalGrid, i, data>{})

        static constexpr BitBoard promotion_mask[][2] =
        {
                { BOOST_PP_ENUM(2, DCTL_PP_ROW_MASK, Side::white) },
                { BOOST_PP_ENUM(2, DCTL_PP_ROW_MASK, Side::black) }
        };

        static constexpr BitBoard row_mask[][12] =
        {
                { BOOST_PP_ENUM(12, DCTL_PP_ROW_MASK, Side::black) },
                { BOOST_PP_ENUM(12, DCTL_PP_ROW_MASK, Side::white) }
        };

#undef DCTL_PP_ROW_MASK

#define DCTL_PP_COL_MASK(z, i, data) copy_if(grid::is_col<ExternalGrid, i, data>{})

        static constexpr BitBoard col_mask[][12] =
        {
                { BOOST_PP_ENUM(12, DCTL_PP_COL_MASK, Side::black) },
                { BOOST_PP_ENUM(12, DCTL_PP_COL_MASK, Side::white) }
        };

#undef DCTL_PP_COL_MASK

private:

#define DCTL_PP_JUMP_START(z, i, data) copy_if(grid::is_jump_start<ExternalGrid, angle::rotate(i * 45, Edge::orientation)>{})

        static constexpr BitBoard table_jump_start[] =
        {
                BOOST_PP_ENUM(8, DCTL_PP_JUMP_START, ~)
        };

#undef DCTL_PP_JUMP_START

public:
        static constexpr auto jump_start(int direction) noexcept
        {
                return table_jump_start[make_angle(direction) / 45];
        }
};

template<class Dimensions, class Edge>
constexpr int Board<Dimensions, Edge>::table_square2bit[];

template<class Dimensions, class Edge>
constexpr int Board<Dimensions, Edge>::table_bit2square[];

template<class Dimensions, class Edge>
constexpr BitBoard Board<Dimensions, Edge>::squares;

template<class Dimensions, class Edge>
constexpr BitBoard Board<Dimensions, Edge>::initial_mask[][5];

template<class Dimensions, class Edge>
constexpr BitBoard Board<Dimensions, Edge>::promotion_mask[][2];

template<class Dimensions, class Edge>
constexpr BitBoard Board<Dimensions, Edge>::row_mask[][12];

template<class Dimensions, class Edge>
constexpr BitBoard Board<Dimensions, Edge>::col_mask[][12];

template<class Dimensions, class Edge>
constexpr BitBoard Board<Dimensions, Edge>::table_jump_start[];

}       // namespace board
}       // namespace dctl
