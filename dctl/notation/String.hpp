#pragma once
#include <iomanip>                      // left, right, setw
#include <sstream>                      // stringsream
#include <string>                       // string
#include <dctl/bit/bit.hpp>
#include <dctl/node/predicates.hpp>
#include <dctl/notation/notation.hpp>

namespace dctl {
namespace notation {
namespace detail {

// primary template
template
<
        typename Rules,
        typename Format = typename notation::Format<Rules>::type,
        typename Separator = notation::Separator<Rules>
>
struct write;

// partial specialization for numeric notation
template<typename Rules, typename Separator>
struct write<Rules, numeric, Separator>
{
        template<typename Position, typename Move>
        std::string operator()(Position const& p, Move const& m) const
        {
                typedef typename Position::board_type Board;

                std::stringstream sstr;
                sstr << std::setw(2) << std::right << Board::bit2square(bit::first::find(from_sq(p, m))) + 1;
                sstr << (is_capture(p, m)? Separator::jump : Separator::move);
                sstr << std::setw(2) << std::left  << Board::bit2square(bit::first::find(dest_sq(p, m))) + 1;
                return (sstr.str());
        }
};

template
<
        typename Rules,
        typename Format = typename notation::Format<Rules>::type,
        typename Separator = notation::Separator<Rules>
>
struct read;

// TODO: partial specialization for algebraic notation

}       // namespace detail

template<typename Position, typename Move>
std::string write(Position const& p, Move const& m)
{
        typedef typename Position::rules_type Rules;

        return (detail::write<Rules>()(p, m));
}

}       // namespace notation
}       // namespace dctl
