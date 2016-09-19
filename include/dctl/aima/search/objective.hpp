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
        bool operator()(State const& s, Actions successor) const
        {
                return !successor.detect(s);
        }
};

template<>
struct is_terminal<Kingscourt>
{
        template<class State>
        bool operator()(State const& s) const
        {
                return
                        (active_kings(s).size() - passive_kings(s).size() < 0) ||
                        is_terminal<NoActionsLeft>()(s)
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
        static int value(State const& s)
        {
                return -cycle<FirstPlayerWin>::value(s);
        }
};

namespace detail {

template<class State>
bool is_no_progress(State const& /* s */, std::false_type)
{
        return false;
}

template<class State>
bool is_no_progress(State const& /* s */, std::true_type)
{
        //using Rules = typename State::rules_type;

        return false;//s.reversible_actions() >= max_reversible_moves_v<Rules>;
}

}       // namespace detail

template<class State>
bool is_no_progress(State const& s)
{
        using Rules = typename State::rules_type;

        return detail::is_no_progress(s, is_restricted_reversible_moves<Rules>{});
}

template<class State>
bool is_draw(State const& s)
{
        return is_cycle(s) || is_no_progress(s);
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
        static int value(State const& s, Actions successor)
        {
                if (is_cycle(s))
                        return cycle<CycleScoring>::value(s);

                if (is_terminal<TerminalDetection>()(s, successor))
                        return terminal<TerminalScoring>::value();

                return -infinity();
        }
};

}       // namespace search
}       // namespace aima
}       // namespace dctl
