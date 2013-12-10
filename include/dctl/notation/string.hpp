#pragma once
#include <iomanip>                      // left, right, setw
#include <sstream>                      // stringstream
#include <string>                       // string
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
        template<class Move>
        std::string operator()(Move const& m) const
        {
                using Board = typename Move::board_type;

                std::stringstream sstr;
                sstr << std::setfill('0') << std::setw(2) << Board::square_from_bit(m.from()) + 1;
                sstr << (m.captured_pieces().empty() ? static_cast<char>(Separator::move) : static_cast<char>(Separator::jump));
                sstr << std::setfill('0') << std::setw(2) << Board::square_from_bit(m.dest()) + 1;
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

template<class Move>
std::string write(Move const& m)
{
        using Rules = typename Move::rules_type;

        return detail::write<Rules>{}(m);
}

}       // namespace notation
}       // namespace dctl
