#pragma once
#include <type_traits>                  // false_type, true_type
#include <dctl/search/score.hpp>
#include <dctl/successor/detect.hpp>

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

template<class>
struct is_terminal;

template<>
struct is_terminal<NoMovesLeft>
{
        template<class Position>
        bool operator()(Position const& p) const
        {
                return !successor::detect(p);
        }
};

template<>
struct is_terminal<Kingscourt>
{
        template<class Position>
        bool operator()(Position const& p) const
        {
                return (
                        (active_kings(p).size() - passive_kings(p).size() < 0) ||
                        is_terminal<NoMovesLeft>()(p)
                );
        }
};

template<class>
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

template<class Position>
bool is_cycle(Position const& p)
{
        // a cycle needs at least 4 reversible moves
        if (p.reversible_moves() < 4) return false;

        // find the parent position at 4 ply above the current position
        auto q = grand_parent(*grand_parent(p));

        // compare the ancestor hash indices with the current hash index
        for (auto i = 4; i <= p.reversible_moves(); i += 2) {
                if (q->hash() == p.hash())
                        return true;
                q = grand_parent(*q);
        }
        return false;
}

template<class>
struct cycle;

template<>
struct cycle<HeuristicDraw>
{
        template<class Position>
        static int value(Position const& /* p */)
        {
                return draw_value();
        }
};

template<>
struct cycle<FirstPlayerWin>
{
        template<class Position>
        static int value(Position const& p)
        {
                return (p.distance_to_root() % 2) ? loss_min() : win_min();
        }
};

template<>
struct cycle<SecondPlayerWin>
{
        template<class Position>
        static int value(Position const& p)
        {
                return -cycle<FirstPlayerWin>::value(p);
        }
};

namespace detail {

// overload for no restrictions on consecutive reversible moves
template<class Position>
bool is_no_progress(Position const& /* p */, std::false_type)
{
        return false;
}

// overload for a maximum of consecutive reversible moves
template<class Position>
bool is_no_progress(Position const& p, std::true_type)
{
        using Rules = typename Position::rules_type;

        return p.reversible_moves() >= rules::max_reversible_moves<Rules>::value;
}

}       // namespace detail

template<class Position>
bool is_no_progress(Position const& p)
{
        using Rules = typename Position::rules_type;

        // tag dispatching on restrictions on consecutive reversible moves
        return detail::is_no_progress(p, rules::is_restricted_reversible_moves<Rules>{});
}

template<class Position>
bool is_draw(Position const& p)
{
        return is_cycle(p) || is_no_progress(p);
}

template
<
        class TerminalDetection = NoMovesLeft,
        class TerminalScoring = Regular,
        class CycleScoring = HeuristicDraw
>
struct GameObjective
{
        template<class Position>
        static int value(Position const& p)
        {
                if (is_cycle(p))
                        return cycle<CycleScoring>::value(p);

                if (is_terminal<TerminalDetection>()(p))
                        return terminal<TerminalScoring>::value();

                return -infinity();
        }
};

}       // namespace search
}       // namespace dctl
