#pragma once
#include <dctl/angle.hpp>                       // Angle, _deg, rotate, is_diagonal, is_up, is_down, is_left, is_right
#include <dctl/board/mask/squares.hpp>          // Squares
#include <dctl/utility/make_array.hpp>          // make_array
#include <dctl/ray/fill.hpp>                    // fill
#include <dctl/ray/iterator.hpp>                // Iterator
#include <array>                                // array
#include <cstddef>                              // size_t

namespace dctl {
namespace board {

template<class Board>
class KingTargets
{
private:
        template<int Direction>
        static constexpr auto init(std::size_t sq) noexcept
        {
                return ray::fill(ray::make_iterator<Board, Direction>(sq), Squares<Board>::mask());
        }

        static constexpr auto theta = 45_deg; //Board::is_orthogonal_captures ? 45_deg : 90_deg;
        static constexpr auto beta  =  0_deg; //Board::is_orthogonal_captures ?  0_deg : 45_deg;
        static constexpr auto N     = Board::set_type::size();
        using Set = typename Board::set_type;

        using table_type = std::array<Set, N>;

        static table_type const table[];

public:
        static constexpr auto mask(Angle const& alpha, std::size_t sq) noexcept
        {
                auto const segment = (alpha - beta) / theta;
                return table[segment][sq];
        }
};

template<class Board>
constexpr Angle
KingTargets<Board>::theta;

template<class Board>
constexpr Angle
KingTargets<Board>::beta;

template<class Board>
typename KingTargets<Board>::table_type const
KingTargets<Board>::table[] =
{
        make_array<N>(init<  0_deg>),
        make_array<N>(init< 45_deg>),
        make_array<N>(init< 90_deg>),
        make_array<N>(init<135_deg>),
        make_array<N>(init<180_deg>),
        make_array<N>(init<225_deg>),
        make_array<N>(init<270_deg>),
        make_array<N>(init<315_deg>)
};

}       // namespace board
}       // namespace dctl
