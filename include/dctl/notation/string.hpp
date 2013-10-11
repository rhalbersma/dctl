#pragma once
#include <iomanip>                      // left, right, setw
#include <sstream>                      // stringstream
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
        class Rules,
        class Format = typename notation::Format<Rules>::type,
        class Separator = notation::Separator<Rules>
>
struct write;

// partial specialization for numeric notation
template<class Rules, class Separator>
struct write<Rules, numeric, Separator>
{
        template<class Position, class Move>
        std::string operator()(Position const& p, Move const& m) const
        {
                using Board = typename Position::board_type;

                auto f = *bit::bit_set<int, uint64_t, 1>(from_sq(p, m)).begin();
                auto d = *bit::bit_set<int, uint64_t, 1>(dest_sq(p, m)).begin();

                std::stringstream sstr;
                sstr << std::setw(2) << std::right << Board::bit2square(f) + 1;
                sstr << (is_capture(p, m) ? static_cast<char>(Separator::jump) : static_cast<char>(Separator::move));
                sstr << std::setw(2) << std::left  << Board::bit2square(d) + 1;
                return sstr.str();
        }
};

template
<
        class Rules,
        class Format = typename notation::Format<Rules>::type,
        class Separator = notation::Separator<Rules>
>
struct read;

// TODO: partial specialization for algebraic notation

}       // namespace detail

template<class Position, class Move>
std::string write(Position const& p, Move const& m)
{
        using Rules = typename Position::rules_type;

        return detail::write<Rules>()(p, m);
}

}       // namespace notation
}       // namespace dctl
