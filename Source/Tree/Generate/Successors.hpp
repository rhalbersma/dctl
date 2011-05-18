#include "../Node/Position.h"

namespace Tree {
namespace Generate {

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate(const Node::Position<Board>& p, Move::Stack& move_stack)
{
        select(p)->generate(p, move_stack);
        assert(invariant(p, move_stack.size()));
}

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate_captures(const Node::Position<Board>& p, Move::Stack& move_stack)
{
        select(p)->generate_captures(p, move_stack);
}

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate_promotions(const Node::Position<Board>& p, Move::Stack& move_stack)
{
        select(p)->generate_promotions(p, move_stack);
}

template<typename Rules, typename Board>
size_t Successors<Rules, Board>::count(const Node::Position<Board>& p)
{
        return select(p)->count(p);
}

template<typename Rules, typename Board>
size_t Successors<Rules, Board>::count_captures(const Node::Position<Board>& p)
{
        return select(p)->count_captures(p);
}

template<typename Rules, typename Board>
size_t Successors<Rules, Board>::count_promotions(const Node::Position<Board>& p)
{
        return select(p)->count_promotions(p);
}

template<typename Rules, typename Board> template<bool Color>
size_t Successors<Rules, Board>::count_mobility(const Node::Position<Board>& p)
{
        return select<Color>(p)->count_mobility(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::detect(const Node::Position<Board>& p)
{
        return select(p)->detect(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::detect_captures(const Node::Position<Board>& p)
{
        return select(p)->detect_captures(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::detect_promotions(const Node::Position<Board>& p)
{
        return select(p)->detect_promotions(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::invariant(const Node::Position<Board>& p, size_t num_moves)
{
        return count(p) == num_moves && detect(p) == (num_moves > 0);
}

template<typename Rules, typename Board>
const AbstractState<Rules, Board>* Successors<Rules, Board>::select(const Node::Position<Board>& p)
{
        return STATE[state(p)];
};

template<typename Rules, typename Board> template<bool Color>
const AbstractState<Rules, Board>* Successors<Rules, Board>::select(const Node::Position<Board>& p)
{
        return STATE[state<Color>(p)];
};

template<typename Rules, typename Board>
int Successors<Rules, Board>::state(const Node::Position<Board>& p)
{
        return state(p.to_move(), p.kings(p.to_move()), p.men(p.to_move()));
}

template<typename Rules, typename Board> template<bool Color>
int Successors<Rules, Board>::state(const Node::Position<Board>& p)
{
        return state(Color, p.kings(Color), p.men(Color));
}

template<typename Rules, typename Board>
int Successors<Rules, Board>::state(bool color, BitBoard kings, BitBoard men)
{
        return (color << 2) + ((kings != 0) << 1) + (men != 0);
}

template<typename Rules, typename Board> 
const AbstractState<Rules, Board>* const Successors<Rules, Board>::STATE[] = {
        &BLACK_NONE, &BLACK_PAWN, &BLACK_KING, &BLACK_BOTH,
        &WHITE_NONE, &WHITE_PAWN, &WHITE_KING, &WHITE_BOTH
};

template<typename Rules, typename Board> 
const State<Node::Side::BLACK, Node::Pieces::NONE, Rules, Board> Successors<Rules, Board>::BLACK_NONE;

template<typename Rules, typename Board> 
const State<Node::Side::BLACK, Node::Pieces::PAWN, Rules, Board> Successors<Rules, Board>::BLACK_PAWN;

template<typename Rules, typename Board> 
const State<Node::Side::BLACK, Node::Pieces::KING, Rules, Board> Successors<Rules, Board>::BLACK_KING;

template<typename Rules, typename Board> 
const State<Node::Side::BLACK, Node::Pieces::BOTH, Rules, Board> Successors<Rules, Board>::BLACK_BOTH;

template<typename Rules, typename Board> 
const State<Node::Side::WHITE, Node::Pieces::NONE, Rules, Board> Successors<Rules, Board>::WHITE_NONE;

template<typename Rules, typename Board> 
const State<Node::Side::WHITE, Node::Pieces::PAWN, Rules, Board> Successors<Rules, Board>::WHITE_PAWN;

template<typename Rules, typename Board> 
const State<Node::Side::WHITE, Node::Pieces::KING, Rules, Board> Successors<Rules, Board>::WHITE_KING;

template<typename Rules, typename Board> 
const State<Node::Side::WHITE, Node::Pieces::BOTH, Rules, Board> Successors<Rules, Board>::WHITE_BOTH;

}       // namespace Generate
}       // namespace Tree
