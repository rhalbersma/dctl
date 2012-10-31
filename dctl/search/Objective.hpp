#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/bool.hpp>           // false_, true_
#include <boost/mpl/identity.hpp>       // identity
#include <dctl/search/Score.hpp>
#include <dctl/bit/Bit.hpp>
#include <dctl/successor/Detect.hpp>

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

template<typename>
struct is_terminal;

template<>
struct is_terminal<NoMovesLeft>
{
        template<typename Position>
        bool operator()(Position const& p) const
        {
                return (!successor::detect(p));
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
                return (loss_min());
        }
};

template<>
struct terminal<Misere>
{
        static int value()
        {
                return (-terminal<Regular>::value());
        }
};

template<typename Position>
bool is_cycle(Position const& p)
{
        // a cycle needs at least 4 reversible moves
        if (p.reversible_moves() < 4)
                return (false);

        // find the parent position at 4 ply above the current position
        auto q = grand_parent(*grand_parent(p));

        // compare the ancestor hash indices with the current hash index
        for (auto i = 4; i <= p.reversible_moves(); i += 2) {
                if (q->hash_index() == p.hash_index())
                        return (true);
                q = grand_parent(*q);
        }
        return (false);
}

template<typename>
struct cycle;

template<>
struct cycle<HeuristicDraw>
{
        template<typename Position>
        static int value(Position const& /* p */)
        {
                return (draw_value());
        }
};

template<>
struct cycle<FirstPlayerWin>
{
        template<typename Position>
        static int value(Position const& p)
        {
                return ((p.distance_to_root() % 2)? loss_min() : win_min());
        }
};

template<>
struct cycle<SecondPlayerWin>
{
        template<typename Position>
        static int value(Position const& p)
        {
                return (-cycle<FirstPlayerWin>::value(p));
        }
};

namespace detail {

// overload for no restrictions on consecutive reversible moves
template<typename Position>
bool is_no_progress(Position const& /* p */, boost::mpl::false_)
{
        return (false);
}

// overload for a maximum of consecutive reversible moves
template<typename Position>
bool is_no_progress(Position const& p, boost::mpl::true_)
{
        typedef typename Position::rules_type Rules;

        return (p.reversible_moves() >= rules::max_reversible_moves<Rules>::value);
}

}       // namespace detail

template<typename Position>
bool is_no_progress(Position const& p)
{
        typedef typename Position::rules_type Rules;

        // tag dispatching on restrictions on consecutive reversible moves
        return (detail::is_no_progress(p, boost::mpl::bool_<rules::is_restricted_reversible_moves<Rules>::value>()));
}

template<typename Position>
bool is_draw(Position const& p)
{
        return (is_cycle(p) || is_no_progress(p));
}

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
                        return (cycle<CycleScoring>::value(p));

                if (is_terminal<TerminalDetection>()(p))
                        return (terminal<TerminalScoring>::value());

                return (-infinity());
        }
};

}       // namespace search
}       // namespace dctl
