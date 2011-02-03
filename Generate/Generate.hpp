#include "GenerateColor.h"
#include "../Position/Position.h"

template<typename Rules, typename Board>
void Generate<Rules, Board>::generate(const Position<Board>& p, MoveList& moves)
{
        if (p.to_move())
                GenerateColor<Side::WHITE, Rules, Board>::generate(p, moves);
        else
                GenerateColor<Side::BLACK, Rules, Board>::generate(p, moves);

        assert(invariant(p, moves.size()));
}

template<typename Rules, typename Board>
void Generate<Rules, Board>::generate_captures(const Position<Board>& p, MoveList& moves)
{
        if (p.to_move())
                GenerateColor<Side::WHITE, Rules, Board>::generate_captures(p, moves);
        else
                GenerateColor<Side::BLACK, Rules, Board>::generate_captures(p, moves);
}

template<typename Rules, typename Board>
void Generate<Rules, Board>::generate_promotions(const Position<Board>& p, MoveList& moves)
{
        if (p.to_move())
                GenerateColor<Side::WHITE, Rules, Board>::generate_promotions(p, moves);
        else
                GenerateColor<Side::BLACK, Rules, Board>::generate_promotions(p, moves);
}

template<typename Rules, typename Board>
size_t Generate<Rules, Board>::count(const Position<Board>& p)
{
        if (p.to_move())
                return GenerateColor<Side::WHITE, Rules, Board>::count(p);
        else
                return GenerateColor<Side::BLACK, Rules, Board>::count(p);
}

template<typename Rules, typename Board>
bool Generate<Rules, Board>::detect(const Position<Board>& p)
{
        if (p.to_move())
                return GenerateColor<Side::WHITE, Rules, Board>::detect(p);
        else
                return GenerateColor<Side::BLACK, Rules, Board>::detect(p);
}

template<typename Rules, typename Board>
bool Generate<Rules, Board>::detect_captures(const Position<Board>& p)
{
        if (p.to_move())
                return GenerateColor<Side::WHITE, Rules, Board>::detect_captures(p);
        else
                return GenerateColor<Side::BLACK, Rules, Board>::detect_captures(p);
}

template<typename Rules, typename Board>
bool Generate<Rules, Board>::detect_promotions(const Position<Board>& p)
{
        if (p.to_move())
                return GenerateColor<Side::WHITE, Rules, Board>::detect_promotions(p);
        else
                return GenerateColor<Side::BLACK, Rules, Board>::detect_promotions(p);
}

template<typename Rules, typename Board>
bool Generate<Rules, Board>::invariant(const Position<Board>& p, size_t num_moves)
{
        if (p.to_move())
                return GenerateColor<Side::WHITE, Rules, Board>::invariant(p, num_moves);
        else
                return GenerateColor<Side::BLACK, Rules, Board>::invariant(p, num_moves);
}
