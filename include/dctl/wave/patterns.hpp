#pragma once
#include <dctl/wave/algorithm.hpp>      // Fill
#include <dctl/wave/iterator.hpp>       // make_iterator
#include <iterator>                     // next, prev
#include <type_traits>                  // false_type, true_type

namespace dctl {

template<class Board, int Direction, class IsLongRangedKing>
struct Sink;

// short ranged kings
template<class Board, int Direction>
struct Sink<Board, Direction, std::false_type>
{
        template<class Set>
        auto operator()(Set const& from, Set const& dest) const
        {
                return Set(*std::next(wave::make_iterator<Board, Direction>(from))) & dest;
        }
};

// long ranged kings
template<class Board, int Direction>
struct Sink<Board, Direction, std::true_type>
{
        template<class Set>
        auto operator()(Set const& from, Set const& dest) const
        {
                return from ^ Fill<Board, Direction>{}(from, dest);
        }
};

template<class Board, int Direction, class IsLongRangedKing>
struct Sandwich;

// short ranged kings
template<class Board, int Direction>
struct Sandwich<Board, Direction, std::false_type>
{
        template<class Set>
        auto operator()(Set const& from, Set const& through, Set const& dest) const
        {
                return
                        Set(*std::next(wave::make_iterator<Board, Direction>(from))) &
                        through &
                        Set(*std::prev(wave::make_iterator<Board, Direction>(dest)))
                ;
        }
};

// long ranged kings
template<class Board, int Direction>
struct Sandwich<Board, Direction, std::true_type>
{
        template<class Set>
        auto operator()(Set const& from, Set const& through, Set const& dest) const
        {
                return
                        Set(*std::next(wave::make_iterator<Board, Direction>(Fill<Board, Direction>{}(from, dest)))) &
                        through &
                        Set(*std::prev(wave::make_iterator<Board, Direction>(dest)))
                ;
        }
};

}       // namespace dctl
