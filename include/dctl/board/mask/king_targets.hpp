#pragma once
#include <dctl/angle.hpp>                       // Angle, _deg, rotate, is_diagonal, is_up, is_down, is_left, is_right
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
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
        // simulate a constexpr lambda (not allowed in C++14)
        template<int Direction>
        struct lambda
        {
                int const sq_;

                constexpr auto operator()() noexcept
                {
                        auto const it = ray::make_iterator<Board, Direction>(sq_);
                        constexpr auto squares = board::Squares<Board>::mask();
                        return ray::fill(it, squares);
                }
        };

        template<int Direction>
        static constexpr auto init(int sq) noexcept
        {
                return lambda<Direction>{sq}();
        }

        static constexpr auto theta = 45_deg; //Board::is_orthogonal_captures ? 45_deg : 90_deg;
        static constexpr auto beta  =  0_deg; //Board::is_orthogonal_captures ?  0_deg : 45_deg;
        static constexpr auto N     = Board::set_type::N;
        using Set = typename Board::set_type;

        using table_type = std::array<Set, N>;

        static table_type const table[];

public:
        static constexpr auto mask(Angle const& alpha, int n) noexcept
        {
                auto const segment = (alpha - beta) / theta;
                return table[segment][static_cast<std::size_t>(n)];
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
        make_array<N>(init<  0>),
        make_array<N>(init< 45>),
        make_array<N>(init< 90>),
        make_array<N>(init<135>),
        make_array<N>(init<180>),
        make_array<N>(init<225>),
        make_array<N>(init<270>),
        make_array<N>(init<315>)
};

}       // namespace board
}       // namespace dctl
