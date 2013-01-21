#pragma once
#include <algorithm>                    // accumulate
#include <cstddef>                      // size_t
#include <iomanip>
#include <iostream>
#include <iterator>                     // distance
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <dctl/walk/transposition.hpp>
#include <dctl/hash/dual_map.hpp>
#include <dctl/hash/signature_extractor.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/successor/count.hpp>
#include <dctl/successor/generate.hpp>
#include <dctl/utility/int.hpp>
#include <dctl/utility/statistics.hpp>
#include <dctl/utility/timer.hpp>

#include <dctl/setup/diagram.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/notation/string.hpp>

#include <memory>
#include <utility>

namespace dctl {
namespace walk {

template<typename Tag, typename Position>
struct Data;

template<typename Tag, typename Position>
struct Enhancements;

struct default_tag {};

template<typename Position>
struct Data<default_tag, Position>
{
        Statistics statistics_;
};

template<typename Position>
struct Enhancements<default_tag, Position>
{
        typedef Data<default_tag, Position> value_type;

        explicit Enhancements(value_type* p): handle_(p) {}

        void reset_statistics() { handle_->statistics_.reset(); }
        void update_statistics(int ply) { handle_->statistics_.update(ply); }

        std::pair<bool, NodeCount> find(Position const& /* p */, int /* depth */) const
        {
                return std::make_pair(false, NodeCount(0));
        }

        std::pair<bool, NodeCount> terminal(Position const& /* p */, int depth) const
        {
                return std::make_pair(depth == 0, NodeCount(1));
        }

        void insert(Position const& /* p */, NodeCount /* nodes */, int /* depth */) const
        {
                /* no-op */
        }

        value_type* handle_;
};

struct bulk_tag {};

template<typename Position>
struct Data<bulk_tag, Position>
:
        Data<default_tag, Position>
{};

template<typename Position>
struct Enhancements<bulk_tag, Position>
{
        typedef Data<bulk_tag, Position> value_type;

        explicit Enhancements(value_type* p): handle_(p) {}

        void reset_statistics() { handle_->statistics_.reset(); }
        void update_statistics(int ply) { handle_->statistics_.update(ply); }

        std::pair<bool, NodeCount> find(Position const& /* p */, int /* depth */) const
        {
                return std::make_pair(false, NodeCount(0));
        }

        std::pair<bool, NodeCount> terminal(Position const& p, int depth) const
        {
                return std::make_pair(depth == 1, successor::count(p));
        }

        void insert(Position const& /* p */, NodeCount /* nodes */, int /* depth */) const
        {
                /* no-op */
        }

        value_type* handle_;
};

struct hash_tag {};

template<typename Position>
struct Data<hash_tag, Position>
{
        hash::Map<
                Position,
                Transposition,
                hash::UpperHashBitsExtractor,
                hash::EmptyOldUnderCutMin<hash::Smallest>
        > TT_;

        Statistics statistics_;
};

template<typename Position>
struct Enhancements<hash_tag, Position>
{
        typedef Data<hash_tag, Position> value_type;

        explicit Enhancements(value_type* p): handle_(p) {}

        void reset_statistics() { handle_->statistics_.reset(); }
        void update_statistics(int ply) { handle_->statistics_.update(ply); }

        void clear_TT() { handle_->TT_.clear(); };
        void resize_TT(std::size_t n) { handle_->TT_.resize(n); }

        std::pair<bool, NodeCount> find(Position const& p, int depth) const
        {
                auto const TT_entry = handle_->TT_.find(p);
                return (TT_entry && TT_entry->depth() == depth)?
                        std::make_pair(true, NodeCount(TT_entry->nodes())) :
                        std::make_pair(false, NodeCount(0))
                ;
        }

        std::pair<bool, NodeCount> terminal(Position const& p, int depth) const
        {
                return (depth == 1)?
                        std::make_pair(true, NodeCount(successor::count(p))) :
                        std::make_pair(false, NodeCount(0))
                ;
        }

        void insert(Position const& p, NodeCount nodes, int depth) const
        {
                handle_->TT_.insert(p, { nodes, depth } );
        }

        value_type* handle_;
};

template<typename Position, typename Enhancements>
NodeCount walk(Position const& p, int depth, int ply, Enhancements e)
{
        // (0)
        e.update_statistics(ply);

        // (1)
        auto const find = e.find(p, depth);
        if (find.first)
                return find.second;

        NodeCount nodes = 0;

        // (2)
        auto const terminal = e.terminal(p, depth);
        if (terminal.first) {
                nodes = terminal.second;
        } else {
                Arena<Move> a;
                auto const moves = successor::generate(p, a);
                for (auto const& m: moves)
                        nodes += walk(successor::make_copy(p, m), depth - 1, ply + 1, e);
        }

        // (3)
        e.insert(p, nodes, depth);

        return nodes;
}

template<typename Position>
void announce(Position const& p, int depth)
{
        std::cout << setup::diagram<pdn::protocol>()(p);
        std::cout << setup::write<pdn::protocol>()(p) << "\n";
        std::cout << "Searching to nominal depth=" << depth << "\n\n";
}

template<typename Position>
void announce(Position const& p, int depth, int num_moves)
{
        announce(p, depth);
        std::cout << "Found " << num_moves << " moves, searching each to nominal depth=" << depth - 1 << "\n";
        std::cout << "\n";
}

void print_move(std::string const& move, int i)
{
        std::cout << std::setw(2) << (i + 1) << "." << move << " ";
}

template<typename Enhancements>
void report(int depth, NodeCount leafs, Timer const& timer, Enhancements e)
{
        std::cout << "info";

        std::cout << " depth ";
        std::cout << std::setw( 2) << depth;

        std::cout << " leafs ";
        std::cout << std::setw(12) << std::right << leafs;

        std::cout << " nodes ";
        std::cout << std::setw(12) << std::right << e.handle_->statistics_.nodes();

        std::cout << " time ";
        std::cout << std::setw( 6) << timer.lap().count();

        double const nps = static_cast<double>(1000 * e.handle_->statistics_.nodes()) / static_cast<double>(timer.lap().count());
        std::cout << " nps ";
        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
        std::cout << std::setw( 7) << nps;

        double const hashfull = 1000 * (static_cast<double>(e.handle_->TT_.size()) / static_cast<double>(e.handle_->TT_.capacity()));
        std::cout << " hashfull ";
        std::cout << std::setw( 4) << std::right << hashfull;

        std::cout << std::endl;
}

void summary(NodeCount leafs)
{
        std::cout << "Total leafs: " << leafs << "\n\n";
}

template<typename Position, typename Enhancements>
NodeCount perft(Position const& p, int depth, Enhancements e)
{
        NodeCount nodes = 0;
        Timer timer;
        announce(p, depth);
        for (auto d = 1; d <= depth; ++d) {
                e.reset_statistics();
                nodes = walk(p, d, 0, e);
                timer.split();
                report(d, nodes, timer, e);
        }
        return nodes;
}

template<typename Position, typename Enhancements>
NodeCount divide(Position const& p, int depth, Enhancements e)
{
        NodeCount leaf_nodes = 0;
        NodeCount sub_count;

        Timer timer;
        Arena<Move> a;
        auto const moves = successor::generate(p, a);

        announce(p, depth, moves.size());
        for (auto const& m: moves) {
                e.reset_statistics();
                auto const i = std::distance(&moves[0], &m);
                print_move(notation::write(p, moves[i]), i);
                sub_count = walk(successor::make_copy(p, moves[i]), depth - 1, 1, e);
                leaf_nodes += sub_count;

                timer.split();
                report(depth - 1, sub_count, timer, e);
        }
        summary(leaf_nodes);
        return leaf_nodes;
}

}       // namespace walk
}       // namespace dctl
