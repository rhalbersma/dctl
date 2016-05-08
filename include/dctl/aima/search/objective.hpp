#pragma once
#include <dctl/aima/search/score.hpp>
#include <dctl/actions.hpp>
#include <xstd/cstddef.hpp>
#include <type_traits>                  // false_type, true_type

namespace dctl {
namespace aima {
namespace search {

/* first side to run out of moves, loses */
struct NoActionsLeft;

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
struct is_terminal<NoActionsLeft>
{
        template<class State, class Actions>
        bool operator()(State const& p, Actions successor) const
        {
                return !successor.detect(p);
        }
};

template<>
struct is_terminal<Kingscourt>
{
        template<class State>
        bool operator()(State const& p) const
        {
                return
                        (active_kings(p).size() - passive_kings(p).size() < 0) ||
                        is_terminal<NoActionsLeft>()(p)
                ;
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

template<class State>
bool is_cycle(State const& /*p*/)
{/*
        using namespace xstd::support_literals;

        // a cycle needs at least 4 reversible moves
        if (p.reversible_actions() < 4) return false;

        // find the parent position at 4 ply above the current position
        auto q = grand_parent(*grand_parent(p));

        // compare the ancestor hash indices with the current hash index
        for (auto i = 4_zu; i <= p.reversible_actions(); i += 2) {
                if (q->hash() == p.hash())
                        return true;
                q = grand_parent(*q);
        }*/
        return false;
}

template<class>
struct cycle;

template<>
struct cycle<HeuristicDraw>
{
        template<class State>
        static int value(State const& /* p */)
        {
                return draw_value();
        }
};

template<>
struct cycle<FirstPlayerWin>
{
        template<class State>
        static int value(State const& /*p*/)
        {
                return win_min();//(p.distance_to_root() % 2) ? loss_min() : win_min();
        }
};

template<>
struct cycle<SecondPlayerWin>
{
        template<class State>
        static int value(State const& p)
        {
                return -cycle<FirstPlayerWin>::value(p);
        }
};

namespace detail {

template<class State>
bool is_no_progress(State const& /* p */, std::false_type)
{
        return false;
}

template<class State>
bool is_no_progress(State const& /*p*/, std::true_type)
{
        //using Rules = typename State::rules_type;

        return false;//p.reversible_actions() >= max_reversible_moves_v<Rules>;
}

}       // namespace detail

template<class State>
bool is_no_progress(State const& p)
{
        using Rules = typename State::rules_type;

        return detail::is_no_progress(p, is_restricted_reversible_moves<Rules>{});
}

template<class State>
bool is_draw(State const& p)
{
        return is_cycle(p) || is_no_progress(p);
}

template
<
        class TerminalDetection = NoActionsLeft,
        class TerminalScoring = Regular,
        class CycleScoring = HeuristicDraw
>
struct GameObjective
{
        template<class State, class Actions>
        static int value(State const& p, Actions successor)
        {
                if (is_cycle(p))
                        return cycle<CycleScoring>::value(p);

                if (is_terminal<TerminalDetection>()(p, successor))
                        return terminal<TerminalScoring>::value();

                return -infinity();
        }
};

}       // namespace search
}       // namespace aima
}       // namespace dctl
