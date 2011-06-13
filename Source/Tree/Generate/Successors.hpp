#include "../Node/Position.h"

namespace tree {
namespace generate {

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate(const node::Position<Board>& p, move::Stack* move_stack)
{
        select(p)->generate(p, move_stack);
        assert(invariant(p, move_stack->size()));
}

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate_captures(const node::Position<Board>& p, move::Stack* move_stack)
{
        select(p)->generate_captures(p, move_stack);
}

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate_promotions(const node::Position<Board>& p, move::Stack* move_stack)
{
        select(p)->generate_promotions(p, move_stack);
}

template<typename Rules, typename Board>
size_t Successors<Rules, Board>::count(const node::Position<Board>& p)
{
        return select(p)->count(p);
}

template<typename Rules, typename Board>
size_t Successors<Rules, Board>::count_captures(const node::Position<Board>& p)
{
        return select(p)->count_captures(p);
}

template<typename Rules, typename Board>
size_t Successors<Rules, Board>::count_promotions(const node::Position<Board>& p)
{
        return select(p)->count_promotions(p);
}

template<typename Rules, typename Board> template<bool Color>
size_t Successors<Rules, Board>::count_mobility(const node::Position<Board>& p)
{
        return select<Color>(p)->count_mobility(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::detect(const node::Position<Board>& p)
{
        return select(p)->detect(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::detect_captures(const node::Position<Board>& p)
{
        return select(p)->detect_captures(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::detect_promotions(const node::Position<Board>& p)
{
        return select(p)->detect_promotions(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::invariant(const node::Position<Board>& p, size_t num_moves)
{
        return count(p) == num_moves && detect(p) == (num_moves > 0);
}

template<typename Rules, typename Board>
const TemplateMethodInterface<Rules, Board>* Successors<Rules, Board>::select(const node::Position<Board>& p)
{
        return FACTORY[state(p)];
};

template<typename Rules, typename Board> template<bool Color>
const TemplateMethodInterface<Rules, Board>* Successors<Rules, Board>::select(const node::Position<Board>& p)
{
        return FACTORY[state<Color>(p)];
};

template<typename Rules, typename Board>
int Successors<Rules, Board>::state(const node::Position<Board>& p)
{
        return state(p.to_move(), p.kings(p.to_move()), p.men(p.to_move()));
}

template<typename Rules, typename Board> template<bool Color>
int Successors<Rules, Board>::state(const node::Position<Board>& p)
{
        return state(Color, p.kings(Color), p.men(Color));
}

template<typename Rules, typename Board>
int Successors<Rules, Board>::state(bool color, BitBoard kings, BitBoard men)
{
        return (color << 2) + ((kings != 0) << 1) + (men != 0);
}

template<typename Rules, typename Board> 
const TemplateMethodInterface<Rules, Board>* const Successors<Rules, Board>::FACTORY[] = {
        &BLACK_NONE, &BLACK_PAWN, &BLACK_KING, &BLACK_BOTH, 
        &WHITE_NONE, &WHITE_PAWN, &WHITE_KING, &WHITE_BOTH
};

template<typename Rules, typename Board> 
const TemplateMethod<node::Side::BLACK, node::Pieces::NONE, Rules, Board> Successors<Rules, Board>::BLACK_NONE;

template<typename Rules, typename Board> 
const TemplateMethod<node::Side::BLACK, node::Pieces::PAWN, Rules, Board> Successors<Rules, Board>::BLACK_PAWN;

template<typename Rules, typename Board> 
const TemplateMethod<node::Side::BLACK, node::Pieces::KING, Rules, Board> Successors<Rules, Board>::BLACK_KING;

template<typename Rules, typename Board> 
const TemplateMethod<node::Side::BLACK, node::Pieces::BOTH, Rules, Board> Successors<Rules, Board>::BLACK_BOTH;

template<typename Rules, typename Board> 
const TemplateMethod<node::Side::WHITE, node::Pieces::NONE, Rules, Board> Successors<Rules, Board>::WHITE_NONE;

template<typename Rules, typename Board> 
const TemplateMethod<node::Side::WHITE, node::Pieces::PAWN, Rules, Board> Successors<Rules, Board>::WHITE_PAWN;

template<typename Rules, typename Board> 
const TemplateMethod<node::Side::WHITE, node::Pieces::KING, Rules, Board> Successors<Rules, Board>::WHITE_KING;

template<typename Rules, typename Board> 
const TemplateMethod<node::Side::WHITE, node::Pieces::BOTH, Rules, Board> Successors<Rules, Board>::WHITE_BOTH;

}       // namespace generate
}       // namespace tree
