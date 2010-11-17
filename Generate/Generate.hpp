#include "ManMoves.h"
#include "ManCaptures.h"
#include "KingCaptures.h"
#include "KingMoves.h"
#include "ManKingCaptures.h"
#include "ManKingMoves.h"
#include "Propagate.h"
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
        switch(p.composition(Color)) {
        case Pieces::NONE:
		return;
        case Pieces::ONLY_MEN:
                return generate<Color>(p, moves, Int2Type<Pieces::ONLY_MEN>());
        case Pieces::ONLY_KINGS:
                return generate<Color>(p, moves, Int2Type<Pieces::ONLY_KINGS>());
        case Pieces::MEN_AND_KINGS:
                return generate<Color>(p, moves, Int2Type<Pieces::MEN_AND_KINGS>());
        default:
                assert(false);
                return;
        }
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Generate::generate(const Position<Board>& p, Propagate<Rules, Board>& moves, Int2Type<Pieces::ONLY_MEN>)
{
        ManCaptures::generate<Color>(p, moves);
        if (!moves.size())
                ManMoves::generate<Color>(p, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Generate::generate(const Position<Board>& p, Propagate<Rules, Board>& moves, Int2Type<Pieces::ONLY_KINGS>)
{
        KingCaptures::generate<Color>(p, moves);
        if (!moves.size())
                KingMoves::generate<Color>(p, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Generate::generate(const Position<Board>& p, Propagate<Rules, Board>& moves, Int2Type<Pieces::MEN_AND_KINGS>)
{
        ManKingCaptures::generate<Color>(p, moves);
        if (!moves.size())
                ManKingMoves::generate<Color>(p, moves);
}

template<typename Rules, typename Board>
void Generate::generate_captures(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        if (p.to_move())
                generate_captures<Side::WHITE>(p, moves);
        else
                generate_captures<Side::BLACK>(p, moves);
}

template<bool Color, typename Rules, typename Board>
void Generate::generate_captures(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        switch(p.composition(Color)) {
        case Pieces::NONE:
		return;
        case Pieces::ONLY_MEN:
                return ManCaptures::generate<Color>(p, moves)
        case Pieces::ONLY_KINGS:
                return KingCaptures::generate<Color>(p, moves);
        case Pieces::MEN_AND_KINGS:
                return ManKingCaptures::generate<Color>(p, moves);
        default:
                assert(false);
                return;
        }
}

template<typename Rules, typename Board>
void Generate::generate_promotions(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        if (p.to_move())
                generate_promotions<Side::WHITE>(p, moves);
        else
                generate_promotions<Side::BLACK>(p, moves);
}

template<bool Color, typename Rules, typename Board>
void Generate::generate_promotions(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        if (p.men(Color))
                return ManMoves::generate_promotions<Color>(p, moves);
        else
                return;
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
        switch(p.composition(Color)) {
        case Pieces::NONE:
		return 0;
        case Pieces::ONLY_MEN:
                return count<Color, Rules>(p, Int2Type<Pieces::ONLY_MEN>());
        case Pieces::ONLY_KINGS:
                return count<Color, Rules>(p, Int2Type<Pieces::ONLY_KINGS>());
        case Pieces::MEN_AND_KINGS:
                return count<Color, Rules>(p, Int2Type<Pieces::MEN_AND_KINGS>());
        default:
                assert(false);
                return 0;
        }
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Generate::count(const Position<Board>& p, Int2Type<Pieces::ONLY_MEN>)
{
        size_t num_moves = ManCaptures::count<Color, Rules>(p);
        if (!num_moves)
                num_moves += ManMoves::count<Color>(p);
        return num_moves;
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Generate::count(const Position<Board>& p, Int2Type<Pieces::ONLY_KINGS>)
{
        size_t num_moves = KingCaptures::count<Color, Rules>(p);
        if (!num_moves)
                num_moves += KingMoves::count<Color, Rules>(p);
        return num_moves;
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Generate::count(const Position<Board>& p, Int2Type<Pieces::MEN_AND_KINGS>)
{
        size_t num_moves = ManKingCaptures::count<Color, Rules>(p);
        if (!num_moves)
                num_moves += ManKingMoves::count<Color, Rules>(p);
        return num_moves;
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Generate::count_moves(const Position<Board>& p)
{
        switch(p.composition(Color)) {
        case Pieces::NONE:
		return 0;
        case Pieces::ONLY_MEN:
                return ManMoves::count<Color>(p);
        case Pieces::ONLY_KINGS:
                return KingMoves::count<Color, Rules>(p);
        case Pieces::MEN_AND_KINGS:
                return ManKingMoves::count<Color, Rules>(p);
        default:
                assert(false);
                return 0;
        }
}

template<typename Rules, typename Board>
bool Generate::detect(const Position<Board>& p)
{
        if (p.to_move())
                return detect<Side::WHITE, Rules>(p);
        else
                return detect<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board>
bool Generate::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        switch(p.composition(Color)) {
        case Pieces::NONE:
		return false;
        case Pieces::ONLY_MEN:
                return detect<Color, Rules>(p, Int2Type<Pieces::ONLY_MEN>());
        case Pieces::ONLY_KINGS:
                return detect<Color, Rules>(p, Int2Type<Pieces::ONLY_KINGS>());
        case Pieces::MEN_AND_KINGS:
                return detect<Color, Rules>(p, Int2Type<Pieces::MEN_AND_KINGS>());
        default:
                assert(false);
                return false;
        }
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Generate::detect(const Position<Board>& p, Int2Type<Pieces::ONLY_MEN>)
{
        return ManMoves::detect<Color>(p) || ManCaptures::detect<Color, Rules>(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Generate::detect(const Position<Board>& p, Int2Type<Pieces::ONLY_KINGS>)
{
        return KingMoves::detect<Color, Rules>(p) || KingCaptures::detect<Color, Rules>(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Generate::detect(const Position<Board>& p, Int2Type<Pieces::MEN_AND_KINGS>)
{
        return ManKingMoves::detect<Color, Rules>(p) || ManKingCaptures::detect<Color, Rules>(p);
}

template<typename Rules, typename Board>
bool Generate::detect_captures(const Position<Board>& p)
{
        if (p.to_move())
                return detect_captures<Side::WHITE, Rules>(p);
        else
                return detect_captures<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board>
bool Generate::detect_captures(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        switch(p.composition(Color)) {
        case Pieces::NONE:
		return false;
        case Pieces::ONLY_MEN:
                return ManCaptures::detect<Color, Rules>(p);
        case Pieces::ONLY_KINGS:
                return KingCaptures::detect<Color, Rules>(p);
        case Pieces::MEN_AND_KINGS:
                return ManKingCaptures::detect<Color, Rules>(p);
        default:
                assert(false);
                return false;
        }
}

template<typename Rules, typename Board>
bool Generate::detect_promotions(const Position<Board>& p)
{
        if (p.to_move())
                return detect_promotions<Side::WHITE, Rules>(p);
        else
                return detect_promotions<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board>
bool Generate::detect_promotions(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        if (p.men(Color))
                return ManMoves::detect_promotions<Color>(p);
        else
                return false;
}

template<typename Rules, typename Board>
bool Generate::invariant(const Position<Board>& p, size_t num_moves)
{
        if (p.to_move())
                return invariant<Side::WHITE, Rules>(p, num_moves);
        else
                return invariant<Side::BLACK, Rules>(p, num_moves);
}

template<bool Color, typename Rules, typename Board>
bool Generate::invariant(const Position<Board>& p, size_t num_moves)
{
        assert(p.to_move() == Color);
        return count<Color, Rules>(p) == num_moves && detect<Color, Rules>(p) == (num_moves > 0);
}
