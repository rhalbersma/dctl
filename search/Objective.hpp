#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Score.hpp"
#include "../bit/Bit.hpp"
#include "../successor/Selection.hpp"
#include "../successor/Successor.hpp"
#include "../utility/Int2Type.hpp"

namespace dctl {
namespace search {

/* first side to run out of moves, loses */
struct NoMovesLeft;

/* first side to get a king, wins */
struct Kingscourt;

struct Regular;
struct Misere;

struct FirstPlayerWin;
struct SecondPlayerWin;
struct HeuristicDraw;

template
<
        typename TerminalDetection = NoMovesLeft,
        typename TerminalScoring = Regular,
        typename CycleScoring = HeuristicDraw
>
struct GameObjective
{
        template<typename Position>
        static int value(Position const& p)
        {
                if (is_cycle(p))
                        return cycle<CycleScoring>::value(p);

                if (is_terminal<TerminalDetection>()(p))
                        return terminal<TerminalScoring>::value();

                return -infinity();
        }
};

template<typename>
struct is_terminal;

template<>
struct is_terminal<NoMovesLeft>
{
        template<typename Position>
        bool operator()(Position const& p) const
        {
                return !Successor<select::Legal>::detect(p);
        }
};

template<>
struct is_terminal<Kingscourt>
{
        template<typename Position>
        bool operator()(Position const& p) const
        {
                return (
                        (bit::count(active_kings(p)) - bit::count(passive_kings(p)) < 0) ||
                        is_terminal<NoMovesLeft>()(p)
                );
        }
};

template<typename>
struct terminal;

template<>
struct terminal<Regular>
{
        static int value()
        {
                return loss_min();
        }
};

template<>
struct terminal<Misere>
{
        static int value()
        {
                return -terminal<Regular>::value();
        }
};

template<typename Rules, typename Board, template<typename, typename> class Position>
bool is_draw(Position<Rules, Board> const& p)
{
        return (
                is_cycle(p) ||
                is_no_progress<Rules>(p)
        );
}

template<typename Position>
bool is_cycle(Position const& p)
{
        // a cycle needs at least 4 reversible moves
        if (p.reversible_moves() < 4)
                return false;

        // find the parent position at 4 ply above the current position
        auto q = grand_parent(*grand_parent(p));

        // compare the ancestor hash indices with the current hash index
        for (auto i = 4; i <= p.reversible_moves(); i += 2) {
                if (q->hash_index() == p.hash_index())
                        return true;
                q = grand_parent(*q);
        }
        return false;
}

template<typename Rules, typename Board, template<typename, typename> class Position>
bool is_no_progress(Position<Rules, Board> const& p)
{
        // tag dispatching on restrictions on consecutive reversible moves
        return is_no_progress_dispatch<Rules>(
                p,
                Int2Type<rules::is_restricted_reversible_moves<Rules>::value>()
        );
}

// partial specialization for no restrictions on consecutive reversible moves
template<typename Position>
bool is_no_progress_dispatch(Position const& /* p */, Int2Type<false>)
{
        return false;
}

// partial specialization for a maximum of consecutive reversible moves
template<typename Rules, typename Board, template<typename, typename> class Position>
bool is_no_progress_dispatch(Position<Rules, Board> const& p, Int2Type<true>)
{
        return p.reversible_moves() >= rules::max_reversible_moves<Rules>::value;
}

template<typename>
struct cycle;

template<>
struct cycle<HeuristicDraw>
{
        template<typename Position>
        static int value(Position const& /* p */)
        {
                return draw_value();
        }
};

template<>
struct cycle<FirstPlayerWin>
{
        template<typename Position>
        static int value(Position const& p)
        {
                return (p.distance_to_root() % 2)? loss_min() : win_min();
        }
};

template<>
struct cycle<SecondPlayerWin>
{
        template<typename Position>
        static int value(Position const& p)
        {
                return -cycle<FirstPlayerWin>::value(p);
        }
};

}       // namespace search
}       // namespace dctl
