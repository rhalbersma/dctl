#pragma once
#include <cstddef>
#include "State.h"
#include "../Node/Pieces.h"
#include "../Node/Side.h"

namespace Tree {

namespace Node { template<typename> class Position; }
namespace Move { class Stack; }

namespace Generate {
        
template<typename Rules, typename Board>
class Successors
{
public:
        // interface
        static void generate(const Node::Position<Board>&, Move::Stack&);
        static void generate_captures(const Node::Position<Board>&, Move::Stack&);
        static void generate_promotions(const Node::Position<Board>&, Move::Stack&);

        static size_t count(const Node::Position<Board>&);
        static size_t count_captures(const Node::Position<Board>&);
        static size_t count_promotions(const Node::Position<Board>&);
        template<bool> static size_t count_mobility(const Node::Position<Board>&);

        static bool detect(const Node::Position<Board>&);
        static bool detect_captures(const Node::Position<Board>&);
        static bool detect_promotions(const Node::Position<Board>&);

private:
        // implementation
        static const AbstractState<Rules, Board>* select(const Node::Position<Board>&);
        template<bool> static const AbstractState<Rules, Board>* select(const Node::Position<Board>&);

        static int state(const Node::Position<Board>&);
        template<bool> static int state(const Node::Position<Board>&);
        static int state(bool, BitBoard, BitBoard);

        static bool invariant(const Node::Position<Board>&, size_t);

        // representation
        static const AbstractState<Rules, Board>* const STATE[];

        static const State<Node::Side::BLACK, Node::Pieces::NONE, Rules, Board> BLACK_NONE;
        static const State<Node::Side::BLACK, Node::Pieces::PAWN, Rules, Board> BLACK_PAWN;
        static const State<Node::Side::BLACK, Node::Pieces::KING, Rules, Board> BLACK_KING;
        static const State<Node::Side::BLACK, Node::Pieces::BOTH, Rules, Board> BLACK_BOTH;
        static const State<Node::Side::WHITE, Node::Pieces::NONE, Rules, Board> WHITE_NONE;
        static const State<Node::Side::WHITE, Node::Pieces::PAWN, Rules, Board> WHITE_PAWN;
        static const State<Node::Side::WHITE, Node::Pieces::KING, Rules, Board> WHITE_KING;
        static const State<Node::Side::WHITE, Node::Pieces::BOTH, Rules, Board> WHITE_BOTH;
};

}       // namespace Generate
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Successors.hpp"
