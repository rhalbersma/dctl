#include "Predicates.h"
#include "../../Geometry/Board.h"
#include "../../Utilities/Bit.h"
#include <iomanip>
#include <sstream>

namespace Tree {
namespace Move {
namespace String {

template<typename Rules, typename Token>
struct write<Rules, NUM_tag, Token>
{
        template<typename Board>
        std::string operator()(const Node::Position<Board>& p, const Node::Pieces& m)
        {
                std::stringstream sstr;
                sstr << std::setw(2) << std::right << Board::TABLE_BIT2SQUARE[Bit::scan_forward(Move::from_sq(p, m))] + 1;
                sstr << (Move::is_capture(p, m)? Token::CAPTURE : Token::MOVE);
                sstr << std::setw(2) << std::left  << Board::TABLE_BIT2SQUARE[Bit::scan_forward(Move::dest_sq(p, m))] + 1;
                return sstr.str();
        }
};

template<typename Rules, typename Token>
struct write<Rules, ALG_tag, Token>
{
        template<typename Board>
        std::string operator()(const Node::Position<Board>& p, const Node::Pieces& m)
        {
                std::stringstream sstr;
                sstr << std::setw(2) << std::right << Board::TABLE_BIT2SQUARE[Bit::scan_forward(Move::from_sq(p, m))] + 1;
                sstr << (Move::is_capture(p, m)? Token::CAPTURE : Token::MOVE);
                sstr << std::setw(2) << std::left  << Board::TABLE_BIT2SQUARE[Bit::scan_forward(Move::dest_sq(p, m))] + 1;
                return sstr.str();
        }
};

}       // namespace String
}       // namespace Move
}       // namespace Tree
