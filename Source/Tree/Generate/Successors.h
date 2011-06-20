#pragma once
#include <cstddef>
#include "TemplateMethod.h"
#include "../Node/Pieces.h"
#include "../Node/Side.h"
#include "../Node/Stack.h"

namespace tree {

namespace node { template<typename> class Position; }

namespace generate {
        
template<typename Rules, typename Board>
class Successors
{
public:
        // interface
        static void generate(const node::Position<Board>&, node::Stack*);
        static void generate_captures(const node::Position<Board>&, node::Stack*);
        static void generate_reverse(const node::Position<Board>&, node::Stack*);
        static void generate_promotions(const node::Position<Board>&, node::Stack*);

        static size_t count(const node::Position<Board>&);
        static size_t count_captures(const node::Position<Board>&);
        static size_t count_reverse(const node::Position<Board>&);
        static size_t count_promotions(const node::Position<Board>&);

        template<bool> static size_t count_mobility(const node::Position<Board>&);

        static bool detect(const node::Position<Board>&);
        static bool detect_captures(const node::Position<Board>&);
        static bool detect_reverse(const node::Position<Board>&);
        static bool detect_promotions(const node::Position<Board>&);

private:
        // implementation
        static const TemplateMethodInterface<Rules, Board>* select(const node::Position<Board>&);
        template<bool> static const TemplateMethodInterface<Rules, Board>* select(const node::Position<Board>&);

        static int state(const node::Position<Board>&);
        template<bool> static int state(const node::Position<Board>&);
        static int state(bool, BitBoard, BitBoard);

        static bool invariant(const node::Position<Board>&, size_t);
        static bool reverse_invariant(const node::Position<Board>&, size_t);
        static bool promotions_invariant(const node::Position<Board>&, size_t);

        // representation
        static const TemplateMethodInterface<Rules, Board>* const FACTORY[];

        static const TemplateMethod<node::Side::BLACK, node::Pieces::NONE, Rules, Board> BLACK_NONE;
        static const TemplateMethod<node::Side::BLACK, node::Pieces::PAWN, Rules, Board> BLACK_PAWN;
        static const TemplateMethod<node::Side::BLACK, node::Pieces::KING, Rules, Board> BLACK_KING;
        static const TemplateMethod<node::Side::BLACK, node::Pieces::BOTH, Rules, Board> BLACK_BOTH;
        static const TemplateMethod<node::Side::WHITE, node::Pieces::NONE, Rules, Board> WHITE_NONE;
        static const TemplateMethod<node::Side::WHITE, node::Pieces::PAWN, Rules, Board> WHITE_PAWN;
        static const TemplateMethod<node::Side::WHITE, node::Pieces::KING, Rules, Board> WHITE_KING;
        static const TemplateMethod<node::Side::WHITE, node::Pieces::BOTH, Rules, Board> WHITE_BOTH;
};

}       // namespace generate
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Successors.hpp"
