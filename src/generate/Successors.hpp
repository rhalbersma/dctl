#include "../node/Position.h"

namespace dctl {
namespace generate {

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate(const Position<Board>& p, Stack& move_stack)
{
        select(p)->generate(p, move_stack);
        assert(invariant(p, move_stack.size()));
}

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate_captures(const Position<Board>& p, Stack& move_stack)
{
        select(p)->generate_captures(p, move_stack);
}

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate_reverse(const Position<Board>& p, Stack& move_stack)
{
        select(p)->generate_reverse(p, move_stack);
        assert(reverse_invariant(p, move_stack.size()));
}

template<typename Rules, typename Board>
void Successors<Rules, Board>::generate_promotions(const Position<Board>& p, Stack& move_stack)
{
        select(p)->generate_promotions(p, move_stack);
        assert(promotions_invariant(p, move_stack.size()));
}

template<typename Rules, typename Board>
int Successors<Rules, Board>::count(const Position<Board>& p)
{
        return select(p)->count(p);
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
        return select<Color>(p)->count_mobility(p);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::detect(const Position<Board>& p)
{
        return select(p)->detect(p);
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
        return count(p) == num_moves && detect(p) == (num_moves > 0);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::reverse_invariant(const Position<Board>& p, int num_reverse)
{
        return count_reverse(p) == num_reverse && detect_reverse(p) == (num_reverse > 0);
}

template<typename Rules, typename Board>
bool Successors<Rules, Board>::promotions_invariant(const Position<Board>& p, int num_promotions)
{
        return count_promotions(p) == num_promotions && detect_promotions(p) == (num_promotions > 0);
}

template<typename Rules, typename Board>
const TemplateMethodInterface<Rules, Board>* Successors<Rules, Board>::select(const Position<Board>& p)
{
        return FACTORY[state(p)];
}

template<typename Rules, typename Board> template<bool Color>
const TemplateMethodInterface<Rules, Board>* Successors<Rules, Board>::select(const Position<Board>& p)
{
        return FACTORY[state<Color>(p)];
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
const TemplateMethodInterface<Rules, Board>* const Successors<Rules, Board>::FACTORY[] = {
        &BLACK_NONE, &BLACK_PAWN, &BLACK_KING, &BLACK_BOTH, 
        &WHITE_NONE, &WHITE_PAWN, &WHITE_KING, &WHITE_BOTH
};

template<typename Rules, typename Board> 
const TemplateMethod<Side::BLACK, Material::NONE, Rules, Board> Successors<Rules, Board>::BLACK_NONE;

template<typename Rules, typename Board> 
const TemplateMethod<Side::BLACK, Material::PAWN, Rules, Board> Successors<Rules, Board>::BLACK_PAWN;

template<typename Rules, typename Board> 
const TemplateMethod<Side::BLACK, Material::KING, Rules, Board> Successors<Rules, Board>::BLACK_KING;

template<typename Rules, typename Board> 
const TemplateMethod<Side::BLACK, Material::BOTH, Rules, Board> Successors<Rules, Board>::BLACK_BOTH;

template<typename Rules, typename Board> 
const TemplateMethod<Side::WHITE, Material::NONE, Rules, Board> Successors<Rules, Board>::WHITE_NONE;

template<typename Rules, typename Board> 
const TemplateMethod<Side::WHITE, Material::PAWN, Rules, Board> Successors<Rules, Board>::WHITE_PAWN;

template<typename Rules, typename Board> 
const TemplateMethod<Side::WHITE, Material::KING, Rules, Board> Successors<Rules, Board>::WHITE_KING;

template<typename Rules, typename Board> 
const TemplateMethod<Side::WHITE, Material::BOTH, Rules, Board> Successors<Rules, Board>::WHITE_BOTH;

}       // namespace generate
}       // namespace dctl
