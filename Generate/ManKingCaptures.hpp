#include "KingCaptures.h"
#include "ManCaptures.h"
#include "Propagate.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<typename Rules, typename Board> FORCE_INLINE
void ManKingCaptures::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        if (p.to_move())
                generate<Side::WHITE>(p, moves);
        else
                generate<Side::BLACK>(p, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void ManKingCaptures::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        generate_precede<Color>(p, moves);
}

// tag dispatching based on absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void ManKingCaptures::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        generate_precede<Color>(p, moves, Int2Type<is_AbsoluteKingPrecedence<Rules>::VALUE>());
}

// partial specialization for no absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void ManKingCaptures::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& moves, Int2Type<false>)
{
        KingCaptures::generate<Color>(p, moves);
        ManCaptures::generate<Color>(p, moves);
}

// partial specialization for absolute king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void ManKingCaptures::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& moves, Int2Type<true>)
{
        KingCaptures::generate<Color>(p, moves);
        if (!moves.size())
                ManCaptures::generate<Color>(p, moves);
}

template<typename Rules, typename Board> FORCE_INLINE
size_t ManKingCaptures::count(const Position<Board>& p)
{
        if (p.to_move())
                return count<Side::WHITE, Rules>(p);
        else
                return count<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t ManKingCaptures::count(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        Propagate<Rules, Board> captures(p);
        generate<Color>(p, captures);
        return captures.size();
}

template<typename Rules, typename Board> FORCE_INLINE
bool ManKingCaptures::detect(const Position<Board>& p)
{
        if (p.to_move())
                return detect<Side::WHITE, Rules>(p);
        else
                return detect<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool ManKingCaptures::detect(const Position<Board>& p)
{
        return ManCaptures::detect<Color, Rules>(p) || KingCaptures::detect<Color, Rules>(p);
}
