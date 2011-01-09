#include "GenerateStrategy.h"
#include "../Position/Position.h"
#include "../Utilities/InlineOptions.h"

template<typename Rules, typename Board>
void Generate::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        if (p.to_move())
                generate<Side::WHITE>(p, moves);
        else
                generate<Side::BLACK>(p, moves);

        assert(Generate::invariant<Rules>(p, moves.size()));
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Generate::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);        
        GenerateStrategy<Color, Rules, Board>::select_strategy(p.composition(Color))->generate(p, moves);
}

template<typename Rules, typename Board>
void Generate::generate_captures(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        if (p.to_move())
                generate_captures<Side::WHITE>(p, moves);
        else
                generate_captures<Side::BLACK>(p, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Generate::generate_captures(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        GenerateStrategy<Color, Rules, Board>::select_strategy(p.composition(Color))->generate_captures(p, moves);
}

template<typename Rules, typename Board>
void Generate::generate_promotions(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        if (p.to_move())
                generate_promotions<Side::WHITE>(p, moves);
        else
                generate_promotions<Side::BLACK>(p, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Generate::generate_promotions(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        GenerateStrategy<Color, Rules, Board>::select_strategy(p.composition(Color))->generate_promotions(p, moves);
}

template<typename Rules, typename Board>
size_t Generate::count(const Position<Board>& p)
{
        if (p.to_move())
                return count<Side::WHITE, Rules>(p);
        else
                return count<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Generate::count(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return GenerateStrategy<Color, Rules, Board>::select_strategy(p.composition(Color))->count(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Generate::count_moves(const Position<Board>& p)
{
        // NO color assertion here because we want to count available moves for both sides
        return GenerateStrategy<Color, Rules, Board>::select_strategy(p.composition(Color))->count_moves(p);
}

template<typename Rules, typename Board>
bool Generate::detect(const Position<Board>& p)
{
        if (p.to_move())
                return detect<Side::WHITE, Rules>(p);
        else
                return detect<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Generate::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return GenerateStrategy<Color, Rules, Board>::select_strategy(p.composition(Color))->detect(p);
}

template<typename Rules, typename Board>
bool Generate::detect_captures(const Position<Board>& p)
{
        if (p.to_move())
                return detect_captures<Side::WHITE, Rules>(p);
        else
                return detect_captures<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Generate::detect_captures(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return GenerateStrategy<Color, Rules, Board>::select_strategy(p.composition(Color))->detect_captures(p);
}

template<typename Rules, typename Board>
bool Generate::detect_promotions(const Position<Board>& p)
{
        if (p.to_move())
                return detect_promotions<Side::WHITE, Rules>(p);
        else
                return detect_promotions<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Generate::detect_promotions(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        GenerateStrategy<Color, Rules, Board>::select_strategy(p.composition(Color))->detect_promotions(p);
}

template<typename Rules, typename Board>
bool Generate::invariant(const Position<Board>& p, size_t num_moves)
{
        if (p.to_move())
                return invariant<Side::WHITE, Rules>(p, num_moves);
        else
                return invariant<Side::BLACK, Rules>(p, num_moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Generate::invariant(const Position<Board>& p, size_t num_moves)
{
        assert(p.to_move() == Color);
        return count<Color, Rules>(p) == num_moves && detect<Color, Rules>(p) == (num_moves > 0);
}
