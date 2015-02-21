#pragma once
#include <dctl/walk/transposition.hpp>
#include <dctl/hash/dual_map.hpp>
#include <dctl/hash/extract.hpp>
#include <dctl/utility/stack_vector.hpp>
#include <dctl/position/make_copy.hpp>
#include <dctl/successor.hpp>
#include <dctl/utility/statistics.hpp>
#include <dctl/utility/stopwatch.hpp>

#include <dctl/setup/diagram.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/move/ostream.hpp>

#include <algorithm>                    // accumulate
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>                     // distance
#include <memory>
#include <utility>

namespace dctl {
namespace walk {

template<class Tag, class Position>
struct Data;

template<class Tag, class Position>
struct Enhancements;

struct default_tag {};

template<class Position>
struct Data<default_tag, Position>
{
        Statistics statistics_;
};

template<class Position>
struct Enhancements<default_tag, Position>
{
        using value_type = Data<default_tag, Position>;

        explicit Enhancements(value_type* p): handle_(p) {}

        void reset_statistics() { handle_->statistics_.reset(); }
        void update_statistics(int ply) { handle_->statistics_.update(ply); }

        std::pair<bool, std::size_t> find(Position const& /* p */, int /* depth */) const
        {
                return std::make_pair(false, std::size_t(0));
        }

        std::pair<bool, std::size_t> terminal(Position const& /* p */, int depth) const
        {
                return std::make_pair(depth == 0, std::size_t(1));
        }

        void insert(Position const& /* p */, std::size_t /* nodes */, int /* depth */) const
        {
                /* no-op */
        }

        value_type* handle_;
};

struct bulk_tag {};

template<class Position>
struct Data<bulk_tag, Position>
:
        Data<default_tag, Position>
{};

template<class Position>
struct Enhancements<bulk_tag, Position>
{
        using value_type = Data<bulk_tag, Position>;

        explicit Enhancements(value_type* p): handle_(p) {}

        void reset_statistics() { handle_->statistics_.reset(); }
        void collect_statistics(int ply) { handle_->statistics_.collect(ply); }

        std::pair<bool, std::size_t> find(Position const& /* p */, int /* depth */) const
        {
                return std::make_pair(false, std::size_t(0));
        }

        std::pair<bool, std::size_t> terminal(Position const& p, int depth) const
        {
                return std::make_pair(depth == 1, successor::count(p));
        }

        void insert(Position const& /* p */, std::size_t /* nodes */, int /* depth */) const
        {
                /* no-op */
        }

        value_type* handle_;
};

struct hash_tag {};

template<class Position>
struct Data<hash_tag, Position>
{
        hash::Map<
                Position,
                Transposition,
                extract::UpperBits,
                hash::EmptyOldUnderCutMin<hash::Smallest>
        > TT_;

        Statistics statistics_;
};

template<class Position>
struct Enhancements<hash_tag, Position>
{
        using value_type = Data<hash_tag, Position>;

        explicit Enhancements(value_type* p): handle_(p) {}

        void reset_statistics() { handle_->statistics_.reset(); }
        void collect_statistics(int ply) { handle_->statistics_.collect(ply); }

        void clear_TT() { handle_->TT_.clear(); }
        void resize_TT(std::size_t n) { handle_->TT_.resize(n); }

        std::pair<bool, std::size_t> find(Position const& p, int depth) const
        {
                auto const TT_entry = handle_->TT_.find(p);
                return (TT_entry && TT_entry->depth() == depth) ?
                        std::make_pair(true, std::size_t(TT_entry->nodes())) :
                        std::make_pair(false, std::size_t(0))
                ;
        }

        std::pair<bool, std::size_t> terminal(Position const& p, int depth) const
        {
                return (depth == 1) ?
                        std::make_pair(true, std::size_t(successor::count(p))) :
                        std::make_pair(false, std::size_t(0))
                ;
        }

        void insert(Position const& p, std::size_t nodes, int depth) const
        {
                handle_->TT_.insert(p, { nodes, depth } );
        }

        value_type* handle_;
};

template<class Position, class Enhancements>
std::size_t walk(Position const& p, int depth, int ply, Enhancements e)
{
        // (0)
        e.collect_statistics(ply);

        // (1)
        auto const found = e.find(p, depth);
        if (found.first)
                return found.second;

        std::size_t nodes = 0;

        // (2)
        auto const terminal = e.terminal(p, depth);
        if (terminal.first) {
                nodes = terminal.second;
        } else {
                using R = typename Position::rules_type;
                using B = typename Position::board_type;
                Arena<Move<R,B> > a;
                auto const moves = successor::generate(p, Alloc<Move<R, B> >{a});
                for (auto&& m : moves)
                        nodes += walk(make_copy(p, m), depth - 1, ply + 1, e);
        }

        // (3)
        e.insert(p, nodes, depth);

        return nodes;
}

template<class Position>
void announce(Position const& p, int depth)
{
        std::cout << setup::diagram<pdn::protocol>()(p);
        std::cout << setup::write<pdn::protocol>()(p) << '\n';
        std::cout << "Searching to nominal depth=" << depth << "\n\n";
}

template<class Position>
void announce(Position const& p, int depth, int num_moves)
{
        announce(p, depth);
        std::cout << "Found " << num_moves << " moves, searching each to nominal depth=" << depth - 1 << '\n';
        std::cout << '\n';
}

template<class Move>
void print_move(Move const& move, int i)
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

inline
void summary(std::size_t leafs)
{
        std::cout << "Total leafs: " << leafs << "\n\n";
}

template<class Position, class Enhancements>
std::size_t perft(Position const& p, int depth, Enhancements e)
{
        std::size_t nodes = 0;
        announce(p, depth);
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto d = 1; d <= depth; ++d) {
                e.reset_statistics();
                nodes = walk(p, d, 0, e);
                stopwatch.split_reset();
                report(d, nodes, stopwatch, e);
        }
        return nodes;
}

template<class Position, class Enhancements>
std::size_t divide(Position const& p, int depth, Enhancements e)
{
        std::size_t leaf_nodes = 0;
        std::size_t sub_count;

        using R = typename Position::rules_type;
        using B = typename Position::board_type;
        Arena<Move<R,B> > a;
        auto const moves = successor::generate(p, Alloc<Move<R, B> >{a});

        announce(p, depth, moves.size());
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto&& m : moves) {
                e.reset_statistics();
                auto const i = std::distance(&moves[0], &m);
                print_move(moves[i], i);
                sub_count = walk(make_copy(p, moves[i]), depth - 1, 1, e);
                leaf_nodes += sub_count;
                stopwatch.split_reset();
                report(depth - 1, sub_count, stopwatch, e);
        }
        summary(leaf_nodes);
        return leaf_nodes;
}

}       // namespace walk
}       // namespace dctl
