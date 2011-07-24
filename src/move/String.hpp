#include <iomanip>
#include <sstream>
#include "../node/Predicates.h"
#include "../board/Board.h"
#include "../utils/Bit.h"

namespace dctl {
namespace notation {
namespace string {

template<typename Rules, typename Separator>
struct write<Rules, protocol::notation::numeric, Separator>
{
        template<typename Board>
        std::string operator()(const Position<Board>& p, const Move& m)
        {
                std::stringstream sstr;
                sstr << std::setw(2) << std::right << Board::bit2square(bit::find_first(from_sq(p, m))) + 1;
                sstr << (is_capture(p, m)? Separator::jump : Separator::move);
                sstr << std::setw(2) << std::left  << Board::bit2square(bit::find_first(dest_sq(p, m))) + 1;
                return sstr.str();
        }
};

template<typename Rules, typename Separator>
struct write<Rules, protocol::notation::algebraic, Separator>
{
        template<typename Board>
        std::string operator()(const Position<Board>& p, const Move& m)
        {
                std::stringstream sstr;
                sstr << std::setw(2) << std::right << Board::bit2square(bit::find_first(from_sq(p, m))) + 1;
                sstr << (is_capture(p, m)? Separator::jump : Separator::move);
                sstr << std::setw(2) << std::left  << Board::bit2square(bit::find_first(dest_sq(p, m))) + 1;
                return sstr.str();
        }
};

}       // namespace string
}       // namespace notation
}       // namespace dctl
