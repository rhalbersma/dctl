#include <cassert>
#include "Generator.h"
#include "../node/Position.h"

namespace dctl {

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate(const Position<Board>& p, Stack& moves)
{
        select(p)->generate(p, moves);
        assert(invariant(p, moves.size()));
}

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate_non_captures(const Position<Board>& p, Stack& moves)
{
        select(p)->generate_non_captures(p, moves);
}

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate_captures(const Position<Board>& p, Stack& moves)
{
        select(p)->generate_captures(p, moves);
}

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate_reverse(const Position<Board>& p, Stack& moves)
{
        select(p)->generate_reverse(p, moves);
        assert(reverse_invariant(p, moves.size()));
}

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate_promotions(const Position<Board>& p, Stack& moves)
{
        select(p)->generate_promotions(p, moves);
        assert(promotions_invariant(p, moves.size()));
}

template<typename Rules, typename Board>
int Successors<Rules, Board>::count(const Position<Board>& p)
{
        return select(p)->count(p);
}

template<typename Rules, typename Board>
int Successors<Rules, Board>::count_non_captures(const Position<Board>& p)
{
        return select(p)->count_non_captures(p);
}

template<typename Rules, typename Board>
int Successors<Rules, Board>::count_captures(const Position<Board>& p)
{
        return select(p)->count_captures(p);
}

template<typename Rules, typename Board>
int Successors<Rules, Board>::count_reverse(const Position<Board>& p)
{
        return select(p)->count_reverse(p);
}

template<typename Rules, typename Board>
int Successors<Rules, Board>::count_promotions(const Position<Board>& p)
{
        return select(p)->count_promotions(p);
}

template<typename Rules, typename Board> template<bool Color>
int Successors<Rules, Board>::count_mobility(const Position<Board>& p)
{
        return select<Color>(p)->count_non_captures(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::detect(const Position<Board>& p)
{
        return select(p)->detect(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::detect_non_captures(const Position<Board>& p)
{
        return select(p)->detect_captures(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::detect_captures(const Position<Board>& p)
{
        return select(p)->detect_captures(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::detect_reverse(const Position<Board>& p)
{
        return select(p)->detect_reverse(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::detect_promotions(const Position<Board>& p)
{
        return select(p)->detect_promotions(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::invariant(const Position<Board>& p, int num_moves)
{
        return (
                (count(p) == num_moves) && 
                (detect(p) == (num_moves > 0))
        );
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::reverse_invariant(const Position<Board>& p, int num_reverse)
{
        return (
                (count_reverse(p) == num_reverse) && 
                (detect_reverse(p) == (num_reverse > 0))
        );
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::promotions_invariant(const Position<Board>& p, int num_promotions)
{
        return (
                (count_promotions(p) == num_promotions) && 
                (detect_promotions(p) == (num_promotions > 0))
        );
}

template<typename Rules, typename Board>
int Successors<Rules, Board>::state(const Position<Board>& p)
{
        return state(p.active_color(), active_kings(p), active_men(p));
}

template<typename Rules, typename Board> template<bool Color>
int Successors<Rules, Board>::state(const Position<Board>& p)
{
        return state(Color, p.kings(Color), p.men(Color));
}

template<typename Rules, typename Board>
int Successors<Rules, Board>::state(bool color, BitBoard kings, BitBoard men)
{
        return (
                ((color != 0) << 2) +
                ((kings != 0) << 1) +
                ((  men != 0) << 0)
        );
}

template<typename Rules, typename Board>
typename const Successors<Rules, Board>::Generator Successors<Rules, Board>::select(const Position<Board>& p)
{
        return instance()[state(p)];
}

template<typename Rules, typename Board> template<bool Color>
typename const Successors<Rules, Board>::Generator Successors<Rules, Board>::select(const Position<Board>& p)
{
        return instance()[state<Color>(p)];
}

template<typename Rules, typename Board>
typename Successors<Rules, Board>::GeneratorArray& Successors<Rules, Board>::instance()
{
        static const successors::Generator<Side::BLACK, Material::NONE, Rules, Board> BLACK_NONE_;
        static const successors::Generator<Side::BLACK, Material::PAWN, Rules, Board> BLACK_PAWN_;
        static const successors::Generator<Side::BLACK, Material::KING, Rules, Board> BLACK_KING_;
        static const successors::Generator<Side::BLACK, Material::BOTH, Rules, Board> BLACK_BOTH_;
        static const successors::Generator<Side::WHITE, Material::NONE, Rules, Board> WHITE_NONE_;
        static const successors::Generator<Side::WHITE, Material::PAWN, Rules, Board> WHITE_PAWN_;
        static const successors::Generator<Side::WHITE, Material::KING, Rules, Board> WHITE_KING_;
        static const successors::Generator<Side::WHITE, Material::BOTH, Rules, Board> WHITE_BOTH_;

        // Meyers Singleton, see Modern C++ Design p.117
        static const GeneratorArray singleton_ = {
                &BLACK_NONE_, &BLACK_PAWN_, &BLACK_KING_, &BLACK_BOTH_, 
                &WHITE_NONE_, &WHITE_PAWN_, &WHITE_KING_, &WHITE_BOTH_
        };
        return singleton_;
}

}       // namespace dctl
