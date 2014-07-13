#pragma once
#include <iterator>                     // next, prev
#include <dctl/rules/types.hpp>         // range::distance_1, range::distance_N
#include <dctl/wave/algorithm.hpp>      // Fill
#include <dctl/wave/iterator.hpp>

namespace dctl {

// primary template
template<class Board, int Direction, class Range>
struct Sink;

template<class Board, int Direction>
struct Sink<Board, Direction, rules::range::distance_1>
{
        using Set = typename Board::set_type;
        auto operator()(Set const& from, Set const& dest) const
        {
                return *std::next(wave::make_iterator<Board, Direction>(from)) & dest;
        }
};

template<class Board, int Direction>
struct Sink<Board, Direction, rules::range::distance_N>
{
        using Set = typename Board::set_type;
        auto operator()(Set const& from, Set const& dest) const
        {
                return from ^ Fill<Board, Direction>{}(from, dest);
        }
};

// primary template
template<class Board, int Direction, class Range>
struct Sandwich;

template<class Board, int Direction>
struct Sandwich<Board, Direction, rules::range::distance_1>
{
        using Set = typename Board::set_type;
        auto operator()(Set const& from, Set const& through, Set const& dest) const
        {
                return
                        *std::next(wave::make_iterator<Board, Direction>(from)) &
                        through &
                        *std::prev(wave::make_iterator<Board, Direction>(dest))
                ;
        }
};

template<class Board, int Direction>
struct Sandwich<Board, Direction, rules::range::distance_N>
{
        using Set = typename Board::set_type;
        auto operator()(Set const& from, Set const& through, Set const& dest) const
        {
                return
                        *std::next(wave::make_iterator<Board, Direction>(Fill<Board, Direction>{}(from, dest))) &
                        through &
                        *std::prev(wave::make_iterator<Board, Direction>(dest))
                ;
        }
};

}       // namespace dctl
