#pragma once
#include <limits>
#include <cstdint>
#include <type_traits>
#include <dctl/angle.hpp>                       // Angle, _deg
#include <dctl/grid/coordinates.hpp>
#include <dctl/grid/dimensions.hpp>             // Rotate_t
#include <dctl/grid/grid.hpp>                   // Grid
#include <dctl/grid/shift_size.hpp>             // shift_size
#include <dctl/grid/predicates.hpp>
#include <dctl/node/side.hpp>
#include <dctl/bit/bit_set.hpp>
#include <dctl/utility/make_array.hpp>
#include <tuple>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/min_element.hpp>
#include <boost/mpl/at.hpp>
//#include <boost/mpl/at_c.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/less_equal.hpp>

namespace dctl {
namespace board {

template<class Dimensions, bool IsOrthogonalCaptures = true>
struct Board
:
        public Dimensions
{
public:
        using is_orthogonal_captures = std::integral_constant<bool, IsOrthogonalCaptures>;

        static constexpr auto edge_columns = IsOrthogonalCaptures ? 2 : 1;

        using Orientations = boost::mpl::vector_c<int, 0, 90, 180, 270>;
        using Grids = boost::mpl::vector<
                grid::Grid< grid::Rotate<Dimensions,   0 >, edge_columns >,
                grid::Grid< grid::Rotate<Dimensions,  90 >, edge_columns >,
                grid::Grid< grid::Rotate<Dimensions, 180 >, edge_columns >,
                grid::Grid< grid::Rotate<Dimensions, 270 >, edge_columns >
        >;
        using Sizes = boost::mpl::vector_c<int,
                grid::Grid< grid::Rotate<Dimensions,   0 >, edge_columns >::size,
                grid::Grid< grid::Rotate<Dimensions,  90 >, edge_columns >::size,
                grid::Grid< grid::Rotate<Dimensions, 180 >, edge_columns >::size,
                grid::Grid< grid::Rotate<Dimensions, 270 >, edge_columns >::size
        >;
        // internal and external grids
        using Index = boost::mpl::distance
        <
                typename boost::mpl::begin<Sizes>::type,
                typename boost::mpl::min_element<Sizes, boost::mpl::less_equal<boost::mpl::_1, boost::mpl::_2> >::type
        >;

        static constexpr auto orientation = Angle{boost::mpl::at<Orientations, Index >::type::value};
        using InternalGrid = typename boost::mpl::at<Grids, Index>::type;
        using ExternalGrid = grid::Grid<Dimensions, 0>;

        using Block = uint64_t;
        static constexpr auto Nb = 2;//InternalGrid::size / B + 1;
        static constexpr auto N = Nb * std::numeric_limits<Block>::digits;
        using bit_type = bit::bit_set<int, Block, Nb>;

        static constexpr auto shift_size(Angle const& direction)
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
                return begin() <= square && square < end();
        }

private:
        template<class DestGrid, class FromSquare>
        static constexpr auto transform(FromSquare const& from_sq, Angle const& theta)
        {
                return grid::sq_from_coord<DestGrid>(rotate(grid::coord_from_sq(from_sq), theta));
        }

        static constexpr int init_bit_from_square(int n) noexcept
        {
                return transform<InternalGrid>(grid::Square<ExternalGrid>{n}, orientation).value();
        }

        static constexpr int init_square_from_bit(int n) noexcept
        {
                return transform<ExternalGrid>(grid::Square<InternalGrid>{n}, inverse(orientation)).value();
        }

        static constexpr std::array<int, N> table_bit_from_square = make_array<N>(init_bit_from_square);
        static constexpr std::array<int, N> table_square_from_bit = make_array<N>(init_square_from_bit);

public:
        static constexpr auto bit_from_square(int n) noexcept
        {
                return table_bit_from_square[static_cast<std::size_t>(n)];
        }

        static constexpr auto square_from_bit(int n) noexcept
        {
                return table_square_from_bit[static_cast<std::size_t>(n)];
        }

        /* NOTE: for C++11/14, constexpr predicate != lambda expression */
        template<class Predicate>
        static constexpr auto copy_if(Predicate pred) noexcept
        {
                bit_type result{};
                for (auto sq = 0; sq < ExternalGrid::size; ++sq)
                        if (pred(grid::Square<ExternalGrid>{sq}))
                                result.set(bit_from_square(sq));
                return result;
        }
};

template<class D, bool O>
constexpr std::array<int, Board<D, O>::N> Board<D, O>::table_bit_from_square;

template<class D, bool O>
constexpr std::array<int, Board<D, O>::N> Board<D, O>::table_square_from_bit;

}       // namespace board
}       // namespace dctl
