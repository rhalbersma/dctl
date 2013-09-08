#pragma once
#include <boost/preprocessor/repetition.hpp>    // BOOST_PP_ENUM
#include <dctl/angle/degrees.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/grid/coordinates/transform.hpp>
#include <dctl/grid/dimensions.hpp>             // Rotate_t
#include <dctl/grid/grid.hpp>                   // Grid
#include <dctl/grid/shift_size.hpp>             // shift_size
#include <dctl/grid/predicates.hpp>
#include <dctl/node/side.hpp>
#include <dctl/utility/int.hpp>
#include <dctl/utility/range.hpp>               // is_element

namespace dctl {
namespace board {

template<class Dimensions, int EdgeColumns = 2, int Orientation = angle::D000>
struct Board
:
        public Dimensions
{
public:
        static constexpr auto edge_columns = EdgeColumns;
        static constexpr auto orientation = Orientation;

        // internal and external grids
        using InternalGrid = grid::Grid<grid::Rotate<Dimensions, orientation>, edge_columns>;
        using ExternalGrid = grid::Grid<Dimensions, 0>;

        using bit_type = BitBoard;

        static constexpr auto shift_size(int direction)
        {
                return grid::shift_size<InternalGrid>(direction);
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
                return util::is_element(square, {begin(), end()});
        }

private:
        template<class DestGrid, class FromSquare>
        static constexpr auto transform(FromSquare const& from_sq, int theta)
        {
                return grid::coordtosq<DestGrid>(grid::rotate(grid::sqtocoord(from_sq), theta));
        }

        static constexpr auto init_square2bit(int n) noexcept
        {
                return transform<InternalGrid>(grid::Square<ExternalGrid>{n}, orientation).value();
        }

        static constexpr auto init_bit2square(int n) noexcept
        {
                return transform<ExternalGrid>(grid::Square<InternalGrid>{n}, angle::inverse(orientation)).value();
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

#define DCTL_PP_JUMP_START(z, i, data) copy_if(grid::is_jump_start<ExternalGrid, angle::rotate(i * 45, orientation)>{})

        static constexpr BitBoard table_jump_start[] =
        {
                BOOST_PP_ENUM(8, DCTL_PP_JUMP_START, ~)
        };

#undef DCTL_PP_JUMP_START

public:
        static constexpr auto jump_start(int direction) noexcept
        {
                return table_jump_start[dctl::make_angle(direction) / 45];
        }
};

template<class Dimensions, int EdgeColumns, int Orientation>
constexpr int Board<Dimensions, EdgeColumns, Orientation>::table_square2bit[];

template<class Dimensions, int EdgeColumns, int Orientation>
constexpr int Board<Dimensions, EdgeColumns, Orientation>::table_bit2square[];

template<class Dimensions, int EdgeColumns, int Orientation>
constexpr BitBoard Board<Dimensions, EdgeColumns, Orientation>::squares;

template<class Dimensions, int EdgeColumns, int Orientation>
constexpr BitBoard Board<Dimensions, EdgeColumns, Orientation>::initial_mask[][5];

template<class Dimensions, int EdgeColumns, int Orientation>
constexpr BitBoard Board<Dimensions, EdgeColumns, Orientation>::promotion_mask[][2];

template<class Dimensions, int EdgeColumns, int Orientation>
constexpr BitBoard Board<Dimensions, EdgeColumns, Orientation>::row_mask[][12];

template<class Dimensions, int EdgeColumns, int Orientation>
constexpr BitBoard Board<Dimensions, EdgeColumns, Orientation>::col_mask[][12];

template<class Dimensions, int EdgeColumns, int Orientation>
constexpr BitBoard Board<Dimensions, EdgeColumns, Orientation>::table_jump_start[];

}       // namespace board
}       // namespace dctl
