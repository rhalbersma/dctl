#include "../Position/Position.h"

template<typename Rules, typename Board>
void Generate<Rules, Board>::generate(const Position<Board>& p, Move::Stack& move_stack)
{
        select(p)->generate(p, move_stack);
        assert(invariant(p, move_stack.size()));
}

template<typename Rules, typename Board>
void Generate<Rules, Board>::generate_captures(const Position<Board>& p, Move::Stack& move_stack)
{
        select(p)->generate_captures(p, move_stack);
}

template<typename Rules, typename Board>
void Generate<Rules, Board>::generate_promotions(const Position<Board>& p, Move::Stack& move_stack)
{
        select(p)->generate_promotions(p, move_stack);
}

template<typename Rules, typename Board>
size_t Generate<Rules, Board>::count(const Position<Board>& p)
{
        return select(p)->count(p);
}

template<typename Rules, typename Board>
size_t Generate<Rules, Board>::count_captures(const Position<Board>& p)
{
        return select(p)->count_captures(p);
}

template<typename Rules, typename Board>
size_t Generate<Rules, Board>::count_promotions(const Position<Board>& p)
{
        return select(p)->count_promotions(p);
}

template<typename Rules, typename Board> template<bool Color>
size_t Generate<Rules, Board>::count_mobility(const Position<Board>& p)
{
        return select<Color>(p)->count_mobility(p);
}

template<typename Rules, typename Board>
bool Generate<Rules, Board>::detect(const Position<Board>& p)
{
        return select(p)->detect(p);
}

template<typename Rules, typename Board>
bool Generate<Rules, Board>::detect_captures(const Position<Board>& p)
{
        return select(p)->detect_captures(p);
}

template<typename Rules, typename Board>
bool Generate<Rules, Board>::detect_promotions(const Position<Board>& p)
{
        return select(p)->detect_promotions(p);
}

template<typename Rules, typename Board>
bool Generate<Rules, Board>::invariant(const Position<Board>& p, size_t num_moves)
{
        return count(p) == num_moves && detect(p) == (num_moves > 0);
}

template<typename Rules, typename Board>
const AbstractState<Rules, Board>* Generate<Rules, Board>::select(const Position<Board>& p)
{
        return STATE[state(p)];
};

template<typename Rules, typename Board> template<bool Color>
const AbstractState<Rules, Board>* Generate<Rules, Board>::select(const Position<Board>& p)
{
        return STATE[state<Color>(p)];
};

template<typename Rules, typename Board>
int Generate<Rules, Board>::state(const Position<Board>& p)
{
        return state(p.to_move(), p.kings(p.to_move()), p.men(p.to_move()));
}

template<typename Rules, typename Board> template<bool Color>
int Generate<Rules, Board>::state(const Position<Board>& p)
{
        return state(Color, p.kings(Color), p.men(Color));
}

template<typename Rules, typename Board>
int Generate<Rules, Board>::state(bool color, BitBoard kings, BitBoard men)
{
        return (color << 2) + ((kings != 0) << 1) + (men != 0);
}

template<typename Rules, typename Board> 
const AbstractState<Rules, Board>* const Generate<Rules, Board>::STATE[] = {
        &BLACK_NONE, &BLACK_PAWN, &BLACK_KING, &BLACK_BOTH,
        &WHITE_NONE, &WHITE_PAWN, &WHITE_KING, &WHITE_BOTH
};

template<typename Rules, typename Board> 
const State<Side::BLACK, Pieces::NONE, Rules, Board> Generate<Rules, Board>::BLACK_NONE;

template<typename Rules, typename Board> 
const State<Side::BLACK, Pieces::PAWN, Rules, Board> Generate<Rules, Board>::BLACK_PAWN;

template<typename Rules, typename Board> 
const State<Side::BLACK, Pieces::KING, Rules, Board> Generate<Rules, Board>::BLACK_KING;

template<typename Rules, typename Board> 
const State<Side::BLACK, Pieces::BOTH, Rules, Board> Generate<Rules, Board>::BLACK_BOTH;

template<typename Rules, typename Board> 
const State<Side::WHITE, Pieces::NONE, Rules, Board> Generate<Rules, Board>::WHITE_NONE;

template<typename Rules, typename Board> 
const State<Side::WHITE, Pieces::PAWN, Rules, Board> Generate<Rules, Board>::WHITE_PAWN;

template<typename Rules, typename Board> 
const State<Side::WHITE, Pieces::KING, Rules, Board> Generate<Rules, Board>::WHITE_KING;

template<typename Rules, typename Board> 
const State<Side::WHITE, Pieces::BOTH, Rules, Board> Generate<Rules, Board>::WHITE_BOTH;
