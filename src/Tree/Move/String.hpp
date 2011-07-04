#include <iomanip>
#include <sstream>
#include "../Node/Predicates.h"
#include "../../board/Board.h"
#include "../../Utilities/Bit.h"

namespace tree {
namespace move {
namespace string {

template<typename Rules, typename Token>
struct write<Rules, NUM_tag, Token>
{
        template<typename Board>
        std::string operator()(const node::Position<Board>& p, const node::Pieces& m)
        {
                std::stringstream sstr;
                sstr << std::setw(2) << std::right << Board::TABLE_BIT2SQUARE[bit::scan_forward(node::from_sq(p, m))] + 1;
                sstr << (node::is_capture(p, m)? Token::CAPTURE : Token::MOVE);
                sstr << std::setw(2) << std::left  << Board::TABLE_BIT2SQUARE[bit::scan_forward(node::dest_sq(p, m))] + 1;
                return sstr.str();
        }
};

template<typename Rules, typename Token>
struct write<Rules, ALG_tag, Token>
{
        template<typename Board>
        std::string operator()(const node::Position<Board>& p, const node::Pieces& m)
        {
                std::stringstream sstr;
                sstr << std::setw(2) << std::right << Board::TABLE_BIT2SQUARE[bit::scan_forward(node::from_sq(p, m))] + 1;
                sstr << (node::is_capture(p, m)? Token::CAPTURE : Token::MOVE);
                sstr << std::setw(2) << std::left  << Board::TABLE_BIT2SQUARE[bit::scan_forward(node::dest_sq(p, m))] + 1;
                return sstr.str();
        }
};

}       // namespace string
}       // namespace move
}       // namespace tree
