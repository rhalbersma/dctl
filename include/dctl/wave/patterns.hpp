#pragma once
#include <dctl/wave/algorithm.hpp>      // Fill
#include <dctl/wave/iterator.hpp>
#include <dctl/type_traits.hpp>
#include <iterator>                     // next, prev
#include <type_traits>                  // false_type, true_type

namespace dctl {

// primary template
template<class Board, int Direction, class Range>
struct Sink;

// specialization for short ranged kings
template<class Board, int Direction>
struct Sink<Board, Direction, std::false_type>
{
        using Set = set_type_t<Board>;
        auto operator()(Set const& from, Set const& dest) const
        {
                return Set(*std::next(wave::make_iterator<Board, Direction>(from))) & dest;
        }
};

// specialization for long ranged kings
template<class Board, int Direction>
struct Sink<Board, Direction, std::true_type>
{
        using Set = set_type_t<Board>;
        auto operator()(Set const& from, Set const& dest) const
        {
                return from ^ Fill<Board, Direction>{}(from, dest);
        }
};

// primary template
template<class Board, int Direction, class Range>
struct Sandwich;

// specialization for short ranged kings
template<class Board, int Direction>
struct Sandwich<Board, Direction, std::false_type>
{
        using Set = set_type_t<Board>;
        auto operator()(Set const& from, Set const& through, Set const& dest) const
        {
                return
                        Set(*std::next(wave::make_iterator<Board, Direction>(from))) &
                        through &
                        Set(*std::prev(wave::make_iterator<Board, Direction>(dest)))
                ;
        }
};

// specialization for long ranged kings
template<class Board, int Direction>
struct Sandwich<Board, Direction, std::true_type>
{
        using Set = set_type_t<Board>;
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
