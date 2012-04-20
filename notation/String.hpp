#pragma once
#include <iomanip>                      // left, right, setw
#include <sstream>                      // stringsream
#include <string>                       // string
#include "Notation.hpp"
#include "../bit/Bit.hpp"
#include "../node/Move_fwd.hpp"
#include "../node/Predicates.hpp"

namespace dctl {
namespace notation {

template
<
        typename Rules,
        typename Format = typename Format<Rules>::type,
        typename Separator = typename Separator<Rules>::type
>
struct read;

template
<
        typename Rules,
        typename Format = typename Format<Rules>::type,
        typename Separator = typename Separator<Rules>::type
>
struct write;

// partial specialization for numeric notation
template<typename Rules, typename Separator>
struct write<Rules, numeric, Separator>
{
        template<typename Rules, typename Board, template<typename, typename> class Position>
        const std::string operator()(const Position<Rules, Board>& p, const Move& m) const
        {
                std::stringstream sstr;
                sstr << std::setw(2) << std::right << Board::bit2square(bit::find_first(from_sq(p, m))) + 1;
                sstr << (is_capture(p, m)? Separator::jump : Separator::move);
                sstr << std::setw(2) << std::left  << Board::bit2square(bit::find_first(dest_sq(p, m))) + 1;
                return sstr.str();
        }
};

// TODO: partial specialization for algebraic notation

}       // namespace notation
}       // namespace dctl
