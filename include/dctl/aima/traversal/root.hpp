#pragma once
#include <dctl/aima/traversal/transposition.hpp>
#include <dctl/utility/hash/dual_map.hpp>
#include <dctl/utility/hash/extract.hpp>
#include <dctl/utility/static_vector.hpp>
#include <dctl/aima/node.hpp>
#include <dctl/actions.hpp>
#include <dctl/utility/statistics.hpp>
#include <dctl/utility/stopwatch.hpp>

#include <dctl/setup/diagram.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/action/ostream.hpp>
#include <boost/container/static_vector.hpp>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>                     // distance
#include <memory>
#include <numeric>
#include <utility>

namespace dctl {
namespace aima {
namespace traversal {

template<class Tag, class State>
struct Data;

template<class Tag, class State>
struct Enhancements;

struct default_tag {};

template<class State>
struct Data<default_tag, State>
{
        Statistics statistics_;
};

template<class State>
struct Enhancements<default_tag, State>
{
        using value_type = Data<default_tag, State>;

        explicit Enhancements(value_type* p): handle_(p) {}

        void reset_statistics() { handle_->statistics_.reset(); }
        void collect_statistics(int ply) { handle_->statistics_.collect(ply); }

        std::pair<bool, std::size_t> find(State const& /* p */, int /* depth */) const
        {
                return std::make_pair(false, std::size_t(0));
        }

        template<class Actions>
        std::pair<bool, std::size_t> terminal(State const& /* p */, Actions /* successor */, int depth) const
        {
                return std::make_pair(depth == 0, std::size_t(1));
        }

        void insert(State const& /* p */, std::size_t /* nodes */, int /* depth */) const
        {
                /* no-op */
        }

        value_type* handle_;
};

struct bulk_tag {};

template<class State>
struct Data<bulk_tag, State>
:
        Data<default_tag, State>
{};

template<class State>
struct Enhancements<bulk_tag, State>
{
        using value_type = Data<bulk_tag, State>;

        explicit Enhancements(value_type* p): handle_(p) {}

        void reset_statistics() { handle_->statistics_.reset(); }
        void collect_statistics(int ply) { handle_->statistics_.collect(ply); }

        std::pair<bool, std::size_t> find(State const& /* p */, int /* depth */) const
        {
                return std::make_pair(false, std::size_t(0));
        }

        template<class Actions>
        std::pair<bool, std::size_t> terminal(State const& p, Actions successor, int depth) const
        {
                return std::make_pair(depth == 1, successor.count(p));
        }

        void insert(State const& /* p */, std::size_t /* nodes */, int /* depth */) const
        {
                /* no-op */
        }

        value_type* handle_;
};

struct hash_tag {};

template<class State>
struct Data<hash_tag, State>
{
        hash::set_associative_cache<
                State,
                Transposition,
                4,
                dctl::hash::EmptyOldMin<hash::Smallest>,
                extract::UpperBits,
                extract::Hash
        > TT_;

        Statistics statistics_;
};

template<class State>
struct Enhancements<hash_tag, State>
{
        using value_type = Data<hash_tag, State>;

        explicit Enhancements(value_type* p): handle_(p) {}

        void reset_statistics() { handle_->statistics_.reset(); }
        void collect_statistics(int ply) { handle_->statistics_.collect(ply); }

        void clear_TT() { handle_->TT_.clear(); }
        void resize_TT(std::size_t n) { handle_->TT_.resize(n); }

        std::pair<bool, std::size_t> find(State const& p, int depth) const
        {
                auto const TT_entry = handle_->TT_.find(p);
                return (TT_entry && TT_entry->depth() == depth) ?
                        std::make_pair(true, std::size_t(TT_entry->nodes())) :
                        std::make_pair(false, std::size_t(0))
                ;
        }

        template<class Actions>
        std::pair<bool, std::size_t> terminal(State const& p, Actions successor, int depth) const
        {
                return (depth == 1) ?
                        std::make_pair(true, std::size_t(successor.count(p))) :
                        std::make_pair(false, std::size_t(0))
                ;
        }

        void insert(State const& p, std::size_t nodes, int depth) const
        {
                handle_->TT_.insert(p, { nodes, depth } );
        }

        value_type* handle_;
};

template<class State, class Actions, class Enhancements>
std::size_t walk(State const& p, int depth, int ply, Actions successor, Enhancements e)
{
        // (0)
        e.collect_statistics(ply);

        // (1)
        auto const found = e.find(p, depth);
        if (found.first)
                return found.second;

        std::size_t nodes = 0;

        // (2)
        auto const terminal = e.terminal(p, successor, depth);
        if (terminal.first) {
                nodes = terminal.second;
        } else {
                using R = rules_t<State>;
                using B = board_t<State>;

                static_vector<action<R,B>> moves;
                successor.generate(p, moves);
                for (auto const& m : moves)
                        nodes += walk(result(p, m), depth - 1, ply + 1, successor, e);
        }

        // (3)
        e.insert(p, nodes, depth);

        return nodes;
}

template<class Actions, class State>
auto perft_nobulk_counting(Actions successor, State const& state, int depth)
{
        if (depth == 0)
                return std::size_t{1};

        static_vector<action<rules_t<State>, board_t<State>>> moves;
        successor.generate(state, moves);
        return std::accumulate(moves.cbegin(), moves.cend(), std::size_t{0}, [&](auto n, auto const& a){
                return n + perft_nobulk_counting(successor, result(state, a), depth - 1);
        });
}

template<class Actions, class State>
auto iperft_bulk_counting(Actions const& successor, State& state, int depth)
{
        if (depth == 1)
                return successor.count(state);

        static_vector<action<rules_t<State>, board_t<State>>> moves;
        successor.generate(state, moves);
        return std::accumulate(moves.cbegin(), moves.cend(), std::size_t{0}, [&](auto n, auto const& a){
                state.make(a);
                auto const res = n + iperft_bulk_counting(successor, state, depth - 1);
                state.undo(a);
                return res;
        });
}

template<class Actions, class State>
auto perft_bulk_counting(Actions const& successor, State const& state, int depth)
{
        if (depth == 1)
                return successor.count(state);

        static_vector<action<rules_t<State>, board_t<State>>> moves;
        successor.generate(state, moves);
        return std::accumulate(moves.cbegin(), moves.cend(), std::size_t{0}, [&](auto n, auto const& a){
                return n + perft_bulk_counting(successor, result(state, a), depth - 1);
        });
}

template<class Actions, class Node>
auto perft_node_bulk_counting(Actions const& successor, Node const& node, int depth)
{
        if (depth == 1)
                return successor.count(node.state);

        static_vector<action<rules_t<decltype(node.state)>, board_t<decltype(node.state)>>> moves;
        successor.generate(node.state, moves);
        return std::accumulate(moves.cbegin(), moves.cend(), std::size_t{0}, [&](auto n, auto const& a){
                return n + perft_node_bulk_counting(successor, child(node, a), depth - 1);
        });
}

template<class State>
void announce(State const& p, int depth)
{
        std::cout << setup::diagram<pdn::protocol>()(p);
        std::cout << setup::write<pdn::protocol>()(p) << '\n';
        std::cout << "Searching to nominal depth=" << depth << "\n\n";
}

template<class State>
void announce(State const& p, int depth, int num_moves)
{
        announce(p, depth);
        std::cout << "Found " << num_moves << " moves, searching each to nominal depth=" << depth - 1 << '\n';
        std::cout << '\n';
}

template<class Action>
void print_move(Action const& move, int i)
{
        std::cout << std::setw(2) << (i + 1) << "." << move << " ";
}

template<class Stopwatch, class Enhancements>
void report(int depth, std::size_t leafs, Stopwatch const& stopwatch, Enhancements e)
{
        std::cout << "info";

        std::cout << " depth ";
        std::cout << std::setw( 2) << depth;

        std::cout << " leafs ";
        std::cout << std::setw(12) << std::right << leafs;

        auto const node_count = boost::accumulators::count(e.handle_->statistics_.nodes());
        std::cout << " nodes ";
        std::cout << std::setw(12) << std::right << node_count;

        auto const lap = stopwatch.lap_time();
        std::cout << " time ";
        std::cout << std::setw( 6) << lap.count();

        auto const nps = static_cast<double>(node_count) / std::chrono::duration_cast<std::chrono::seconds>(lap).count();
        std::cout << " nps ";
        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
        std::cout << std::setw( 7) << nps;
/*
        double const hashfull = 1000 * (static_cast<double>(e.handle_->TT_.size()) / static_cast<double>(e.handle_->TT_.capacity()));
        std::cout << " hashfull ";
        std::cout << std::setw( 4) << std::right << hashfull;
*/
        std::cout << std::endl;
}

template<class Stopwatch>
void xreport(int depth, std::size_t leafs, Stopwatch const& stopwatch)
{
        std::cout << "info";

        std::cout << " depth ";
        std::cout << std::setw( 2) << depth;

        std::cout << " leafs ";
        std::cout << std::setw(12) << std::right << leafs;

        auto const lap = stopwatch.lap_time();
        std::cout << " time ";
        std::cout << std::setw( 6) << lap.count();

/*
        double const hashfull = 1000 * (static_cast<double>(e.handle_->TT_.size()) / static_cast<double>(e.handle_->TT_.capacity()));
        std::cout << " hashfull ";
        std::cout << std::setw( 4) << std::right << hashfull;
*/
        std::cout << std::endl;
}

inline
void summary(std::size_t leafs)
{
        std::cout << "Total leafs: " << leafs << "\n\n";
}

template<class State, class Actions, class Enhancements>
std::size_t perft(State const& p, int depth, Actions successor, Enhancements e)
{

        std::size_t nodes = 0;
        announce(p, depth);
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto d = 1; d <= depth; ++d) {
                e.reset_statistics();
                nodes = walk(p, d, 0, successor, e);
                stopwatch.split_reset();
                report(d, nodes, stopwatch, e);
        }
        return nodes;
}
/*
template<class State, class Actions>
auto iperft(State const& s_in, int depth, Actions successor)
{
        auto s{s_in};
        announce(s, depth);
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto d = 1; d <= depth; ++d) {
                stopwatch.split_reset();
                auto const nodes = iperft_bulk_counting(successor, s, d);
                stopwatch.split_reset();
                xreport(d, nodes, stopwatch);
        }
}
*/
template<class State, class Actions>
auto sperft(State const& s, int depth, Actions successor)
{
        announce(s, depth);
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto d = 1; d <= depth; ++d) {
                stopwatch.split_reset();
                auto const nodes = perft_bulk_counting(successor, s, d);
                stopwatch.split_reset();
                xreport(d, nodes, stopwatch);
        }
}

template<class State, class Actions>
auto nperft(State const& s, int depth, Actions successor)
{
        announce(s, depth);
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        using Node = node<State, action<rules_t<State>, board_t<State>>>;
        auto const n = root<Node>(s);
        for (auto d = 1; d <= depth; ++d) {
                stopwatch.split_reset();
                auto const nodes = perft_node_bulk_counting(successor, n, d);
                stopwatch.split_reset();
                xreport(d, nodes, stopwatch);
        }
}

template<class State, class Actions, class Enhancements>
std::size_t divide(State const& p, int depth, Actions successor, Enhancements e)
{
        std::size_t leaf_nodes = 0;

        std::vector<action<rules_t<State>, board_t<State>>> moves;
        successor.generate(p, moves);

        announce(p, depth, moves.size());
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto const& m : moves) {
                e.reset_statistics();
                auto const i = std::distance(&moves[0], &m);
                print_move(moves[i], i);
                auto const sub_count = walk(result(p, moves[i]), depth - 1, 1, successor, e);
                leaf_nodes += sub_count;
                stopwatch.split_reset();
                report(depth - 1, sub_count, stopwatch, e);
        }
        summary(leaf_nodes);
        return leaf_nodes;
}

}       // namespace traversal
}       // namespace aima
}       // namespace dctl
