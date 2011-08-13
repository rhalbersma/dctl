#include <iomanip>
#include <sstream>
#include "../node/Predicates.h"
#include "../bit/Bit.h"

namespace dctl {
namespace notation {

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
