#pragma once
#include <iomanip>                      // std::left, std::right, std::setw
#include <sstream>                      // std::stringsream
#include <string>                       // std::string
#include "Notation.hpp"
#include "../bit/Bit.hpp"
#include "../node/Predicates.hpp"

namespace dctl {

template<typename> class Position;
class Move;
        
namespace notation {

template
<
        typename Rules,
        typename Format = typename Format<Rules>::type,
        typename Separator = typename Separator<Rules>::type
>
struct read
{
	template<typename Board> 
        const Move operator()(const Position<Board>&, const std::string&) const;
};

template
<
        typename Rules, 
        typename Format = typename Format<Rules>::type,
        typename Separator = typename Separator<Rules>::type
>
struct write
{
	template<typename Board> 
        const std::string operator()(const Position<Board>&, const Move&) const;
};

// partial specialization for numeric notation
template<typename Rules, typename Separator>
struct write<Rules, numeric, Separator>
{
        template<typename Board>
        const std::string operator()(const Position<Board>& p, const Move& m) const
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

// include template definitions inside header
#include "String.hpp"
