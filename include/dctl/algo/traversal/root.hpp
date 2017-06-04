#pragma once
#include <dctl/algo/node.hpp>
#include <dctl/algo/traversal/transposition.hpp>
#include <dctl/core.hpp>
#include <dctl/util/hash/dual_map.hpp>
#include <dctl/util/hash/extract.hpp>
#include <dctl/util/static_vector.hpp>
#include <dctl/util/statistics.hpp>
#include <dctl/util/stopwatch.hpp>
#include <hash_append/hash_append.h>
#include <hash_append/identity_hash.h>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>                     // distance
#include <memory>
#include <numeric>
#include <utility>
#include <map>
#include <unordered_map>

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
                return std::make_pair(false, std::size_t{0});
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
                return std::make_pair(false, std::size_t{0});
        }

        template<class Actions>
        std::pair<bool, std::size_t> terminal(State const& s, Actions successor, int depth) const
        {
                return std::make_pair(depth == 1, successor.count(s));
        }

        void insert(State const& /* s */, std::size_t /* nodes */, int /* depth */) const
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

        std::pair<bool, std::size_t> find(State const& s, int depth) const
        {
                auto const TT_entry = handle_->TT_.find(s);
                return TT_entry && TT_entry->depth() == depth ?
                        std::make_pair(true, std::size_t(TT_entry->nodes())) :
                        std::make_pair(false, std::size_t{0})
                ;
        }

        template<class Actions>
        std::pair<bool, std::size_t> terminal(State const& s, Actions successor, int depth) const
        {
                return depth == 1 ?
                        std::make_pair(true, std::size_t(successor.count(s))) :
                        std::make_pair(false, std::size_t{0})
                ;
        }

        void insert(State const& s, std::size_t nodes, int depth) const
        {
                handle_->TT_.insert(s, { nodes, depth } );
        }

        value_type* handle_;
};

template<class State, class Actions, class Enhancements>
int64_t walk(State const& s, int depth, int ply, Actions successor, Enhancements e)
{
        // (0)
        e.collect_statistics(ply);

        // (1)
        auto const found = e.find(s, depth);
        if (found.first)
                return found.second;

        int64_t nodes = 0;

        // (2)
        auto const terminal = e.terminal(s, successor, depth);
        if (terminal.first) {
                nodes = terminal.second;
        } else {
                using R = core::rules_t<State>;
                using B = core::board_t<State>;

                static_vector<core::action<R,B>> moves;
                successor.generate(s, moves);
                for (auto const& m : moves)
                        nodes += walk(result(s, m), depth - 1, ply + 1, successor, e);
        }

        // (3)
        e.insert(s, nodes, depth);

        return nodes;
}

template<class Actions, class State>
auto legal_actions(Actions const& successor, State const& s)
{
        static_vector<core::action<core::rules_t<State>, core::board_t<State>>> moves;
        successor.generate(s, moves);
        return moves;
}

template<bool IsBulk, class Actions, class State>
auto perft_inplace(Actions const& successor, State& s, int depth)
        -> int64_t
{
        if constexpr(IsBulk) {
                if (depth == 1) return successor.count(s);
        } else {
                if (depth == 0) return 1;
        }

        auto const moves = legal_actions(successor, s);
        return std::accumulate(moves.cbegin(), moves.cend(), int64_t{0}, [&](auto sum, auto const& a){
                s.make(a);
                auto const res = sum + perft_inplace(successor, s, depth - 1);
                s.undo(a);
                return res;
        });
}

template<bool IsBulk, class Actions, class State>
auto perft_state(Actions const& successor, State const& s, int depth)
        -> int64_t
{
        if constexpr(IsBulk) {
                if (depth == 1) return successor.count(s);
        } else {
                if (depth == 0) return 1;
        }

        static_vector<core::action<core::rules_t<State>, core::board_t<State>>> moves;
        successor.generate(s, moves);
        return std::accumulate(moves.cbegin(), moves.cend(), int64_t{0}, [&](auto sum, auto const& a){
                return sum + perft_state<IsBulk>(successor, result(s, a), depth - 1);
        });
}

template<class Vertex, class ImplicitGraph>
auto out_edges(Vertex const& u, ImplicitGraph const& g)
{
        static_vector<core::action<core::rules_t<core::state_t<Vertex>>, core::board_t<core::state_t<Vertex>>>> edges;
        g.generate(u.state(), edges);
        return edges;
}

template<bool IsBulk, class Actions, class Node>
auto perft_node(Actions const& successor, Node const& n, int depth)
        -> int64_t
{
        if constexpr(IsBulk) {
                if (depth == 1) return successor.count(n.state());
        } else {
                if (depth == 0) return 1;
        }

        auto const moves = out_edges(n, successor);
        return std::accumulate(moves.cbegin(), moves.cend(), int64_t{0}, [&](auto sum, auto const& a){
                return sum + perft_node<IsBulk>(successor, child(n, a), depth - 1);
        });
}

template<bool IsBulk, class Vertex>
class dls_visitor
{
        //std::map<Vertex, std::pair<int, int64_t>> m_color_map;
        std::unordered_map<Vertex, std::pair<int, int64_t>, xstd::uhash<acme::identity_hash>> m_color_map;
        int m_limit {};
public:
        auto is_frontier(Vertex const& u) const noexcept
        {
                return m_limit == u.cost();
        }

        template<class ImplicitGraph>
        auto frontier_vertex(Vertex const& u, ImplicitGraph const& g)
        {
                if constexpr (IsBulk) {
                        return g.count(u.state());
                } else {
                        return 1;
                }
        }

        auto find(Vertex const& u)
                -> std::pair<bool, int64_t>
        {
                auto const it = m_color_map.find(u);
                if (it != m_color_map.end() && it->second.first == u.cost()) {
                        return { true, it->second.second };
                } else {
                        return { false, 0 };
                }
        }

        template<class... Args>
        auto emplace(Args&&... args)
        {
                m_color_map.emplace(std::forward<Args>(args)...);
        }

        auto depth(int d) { m_limit = d - (IsBulk ? 1 : 0); }
        auto clear() { m_color_map.clear(); }
};

template<class ImplicitGraph, class Vertex, class Visitor>
auto dfs_visit(ImplicitGraph const& g, Vertex const& u, Visitor& vis)
        -> int64_t
{
        if (auto const lookup = vis.find(u); lookup.first) {
                return lookup.second;
        }

        if (vis.is_frontier(u)) {
                return vis.frontier_vertex(u, g);
        }

        auto const moves = out_edges(u, g);
        auto const res = std::accumulate(moves.cbegin(), moves.cend(), int64_t{0}, [&](auto sum, auto const& e){
                return sum + dfs_visit(g, child(u, e), vis);
        });

        vis.emplace(u, std::make_pair(u.cost(), res));

        return res;
}

template<class State>
void announce(State const& s, int depth)
{
        std::cout << core::setup::diagram<core::pdn::protocol>()(s);
        std::cout << core::setup::write<core::pdn::protocol>()(s) << '\n';
        std::cout << "Searching to nominal depth=" << depth << "\n\n";
}

template<class State>
void announce(State const& s, int depth, int num_moves)
{
        announce(s, depth);
        std::cout << "Found " << num_moves << " moves, searching each to nominal depth=" << depth - 1 << '\n';
        std::cout << '\n';
}

template<class Action>
void print_move(Action const& move, int i)
{
        std::cout << std::setw(2) << (i + 1) << "." << move << " ";
}

template<class Stopwatch, class Enhancements>
void report(int depth, int64_t leafs, Stopwatch const& stopwatch, Enhancements e)
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

        auto const nps = static_cast<double>(node_count) / static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(lap).count());
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
void xreport(int depth, int64_t leafs, Stopwatch const& stopwatch)
{
        std::cout << "info";

        std::cout << " depth ";
        std::cout << std::setw( 2) << depth;

        std::cout << " leafs ";
        std::cout << std::setw(12) << std::right << leafs;

        auto const lap = stopwatch.lap_time();
        std::cout << " time ";
        std::cout << std::setw( 6) << lap.count();

	auto const knps = static_cast<double>(leafs) / static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(lap).count());
        std::cout << " " << knps << " knps";

/*
        double const hashfull = 1000 * (static_cast<double>(e.handle_->TT_.size()) / static_cast<double>(e.handle_->TT_.capacity()));
        std::cout << " hashfull ";
        std::cout << std::setw( 4) << std::right << hashfull;
*/
        std::cout << std::endl;
}

inline
void summary(int64_t leafs)
{
        std::cout << "Total leafs: " << leafs << "\n\n";
}

template<class State, class Actions, class Enhancements>
auto perft(State const& s, int depth, Actions successor, Enhancements e)
{

        int64_t nodes = 0;
        announce(s, depth);
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto d = 1; d <= depth; ++d) {
                e.reset_statistics();
                nodes = walk(s, d, 0, successor, e);
                stopwatch.split_reset();
                report(d, nodes, stopwatch, e);
        }
        return nodes;
}

template<class Actions, class State>
auto iperft(Actions successor, State const& s, int depth)
{
        announce(s, depth);
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        auto c{s};
        for (auto d = 1; d <= depth; ++d) {
                stopwatch.split_reset();
                auto const nodes = perft_inplace<true>(successor, c, d);
                stopwatch.split_reset();
                xreport(d, nodes, stopwatch);
        }
}

template<class Actions, class State>
auto sperft(Actions successor, State const& s, int depth)
{
        announce(s, depth);
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto d = 1; d <= depth; ++d) {
                stopwatch.split_reset();
                auto const nodes = perft_state<true>(successor, s, d);
                stopwatch.split_reset();
                xreport(d, nodes, stopwatch);
        }
}

template<class Actions, class State>
auto nperft(Actions successor, State const& s, int depth)
{
        announce(s, depth);
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        using Node = node<State, core::action<core::rules_t<State>, core::board_t<State>>>;
        auto const n = root<Node>(s);
        for (auto d = 1; d <= depth; ++d) {
                stopwatch.split_reset();
                auto const nodes = perft_node<true>(successor, n, d);
                stopwatch.split_reset();
                xreport(d, nodes, stopwatch);
        }
}

template<class Actions, class State>
auto dperft(Actions successor, State const& s, int depth)
{
        announce(s, depth);
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        using Node = node<State, core::action<core::rules_t<State>, core::board_t<State>>>;
        auto const n = root<Node>(s);
        dls_visitor<true, Node> vis;
        for (auto d = 1; d <= depth; ++d) {
                stopwatch.split_reset();
                vis.depth(d);
                auto const cnt = dfs_visit(successor, n, vis);
                vis.clear();
                stopwatch.split_reset();
                xreport(d, cnt, stopwatch);
        }
}

template<class State, class Actions, class Enhancements>
auto divide(State const& s, int depth, Actions successor, Enhancements e)
{
        int64_t leaf_nodes = 0;

        std::vector<core::action<core::rules_t<State>, core::board_t<State>>> moves;
        successor.generate(s, moves);

        announce(s, depth, moves.size());
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto const& m : moves) {
                e.reset_statistics();
                auto const i = std::distance(&moves[0], &m);
                print_move(moves[i], i);
                auto const sub_count = walk(result(s, moves[i]), depth - 1, 1, successor, e);
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
