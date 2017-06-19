#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/algo/node.hpp>
#include <dctl/algo/search/bound.hpp>
#include <dctl/algo/search/ply.hpp>         // PlyCount
#include <dctl/algo/search/objective.hpp>
#include <dctl/algo/search/score.hpp>
#include <dctl/algo/search/transposition.hpp>
#include <dctl/algo/search/variation.hpp>
#include <dctl/core.hpp>
#include <dctl/eval.hpp>
#include <dctl/util/hash/extract.hpp>
#include <dctl/util/hash/map.hpp>
#include <dctl/util/hash/replace.hpp>
#include <dctl/util/static_vector.hpp>
#include <dctl/util/statistics.hpp>
#include <dctl/util/stopwatch.hpp>
#include <boost/algorithm/cxx11/iota.hpp>
#include <cassert>                      // assert
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <iterator>                     // back_inserter
#include <vector>                       // vector

namespace dctl::algo {
namespace search {

template
<
        class State,
        class Objective
>
class Root
{
        using node_type = node<State, core::action<core::rules_t<State>, core::board_t<State>>>;
public:
        // typdefs
        enum EntryType { ZW, PV };

        Root()
        :
                interrupted_(false)
        {
        }

        template<class Actions>
        int analyze(Actions successor, State const& s, int depth)
        {
                auto const n = node_type{s};
                return iterative_deepening(successor, n, depth);
        }

        int solve(State const& s, int depth)
        {
                return proof_verify(s, depth);
        }

        void resize_hash(std::size_t n)
        {
                TT.resize(n);
        }

        void clear_hash()
        {
                TT.clear();
        }

        void interrupt()
        {
                interrupted_ = true;
        }

        bool is_interrupted() const
        {
                return interrupted_;
        }

private:
        template<class Actions, class Node>
        int iterative_deepening(Actions successor, Node const& n, int depth)
        {
                auto score = -infinity();
                int alpha, beta;

                Variation pv;
                announce(n.state(), depth);
                statistics_.reset();
                util::Stopwatch stopwatch;
                stopwatch.start_stop();
                for (auto i = 1; i <= depth; i += ROOT_ID_INCREMENT) {
                        alpha = -infinity();
                        beta = infinity();
                        score = pvs<PV>(successor, n, alpha, beta, i, 0, pv);
                        insert_pv(successor, n, pv, score);
                        stopwatch.split_reset();
                        report(i, score, stopwatch, successor, n, pv);
                }

                return score;
        }

        template<int NodeType, class Actions, class Node>
        int pvs(Actions successor, Node const& n, int alpha, int beta, int depth, int ply, Variation& refutation)
        {
                statistics_.collect(ply);

                if (is_interrupted()) {
                        return alpha;
                }

                // -INF <= alpha < beta <= +INF
                assert(-infinity() <= alpha && alpha < beta && beta <= infinity());

                // alpha < beta <= +INF implies alpha <= win_min
                // with equality, any finite score will fail low
                if (alpha == win_min()) {
                        return alpha;
                }

                // -INF <= alpha < beta implies loss_min <= beta
                // with equality, any finite score will fail high
                if (beta == loss_min()) {
                        return beta;
                }

                // alpha < beta implies alpha <= beta - 1,
                // with the strict inequality if and only if is_pv(NodeType)
                assert(is_pv(NodeType) == (alpha <  beta - 1));

                // terminal positions
                auto const terminal_value = Objective::value(n.state(), successor);
                if (is_finite(terminal_value)) {
                        if (depth > 0) {
                                auto const type = Bound::type(terminal_value, alpha, beta);
                                TT.insert(n, { terminal_value, type, depth, Transposition::no_move() } );
                                if (type == Bound::exact)
                                        refutation.clear();
                        }
                        return terminal_value;
                }

                // return evaluation in leaf nodes with valid moves
                if (depth <= 0 || ply >= MAX_PLY) {
                        return eval::score(n.state());
                }

                // TT cut-off for exact win/loss scores or for deep enough heuristic scores
                auto TT_entry = TT.find(n);
                if (TT_entry && (is_mate(TT_entry->value()) || TT_entry->depth() >= depth) && TT_entry->is_cutoff(alpha, beta)) {
                        return TT_entry->value();
                }

                // generate moves
                auto const moves = successor.generate(n.state());
                assert(not moves.empty());

                Order move_order;
                boost::algorithm::iota_n(std::back_inserter(move_order), 0, moves.size()); // generate indices [0, moves.size() - 1]

                // internal iterative deepening (IID)
                if (not (TT_entry && TT_entry->has_move())) {
                        auto const IID_depth = is_pv(NodeType) ? depth - 2 : depth / 2;
                        if (IID_depth > 0) {
                                pvs<NodeType>(successor, n, alpha, beta, IID_depth, ply, refutation);
                                TT_entry = TT.find(n);
                                assert(TT_entry);
                        }
                }

                // move ordering
                if (TT_entry && TT_entry->has_move()) {
                        auto const TT_move = static_cast<std::size_t>(TT_entry->move()) % moves.size();
                        std::swap(move_order[0], move_order[TT_move]);
                }

                // search movesa
                auto const original_alpha = alpha;
                auto best_value = -infinity();
                auto best_move = Transposition::no_move();
                int value;

                Variation continuation;

                for (auto const& i : move_order) {
                        auto q = child(n, moves[static_cast<std::size_t>(i)]);

                        // TODO: TT singular extension

                        // TODO: futility pruning

                        if (is_pv(NodeType) && (&i == &move_order[0])) {
                                value = -squeeze(pvs<PV>(successor, q, -stretch(beta), -stretch(alpha), depth - 1, ply + 1, continuation));
                        } else {
                                // TODO: late move reductions

                                value = -squeeze(pvs<ZW>(successor, q, -stretch(alpha + 1), -stretch(alpha), depth - 1, ply + 1, continuation));
                                if (is_pv(NodeType) && alpha < value && value < beta) {
                                        value = -squeeze(pvs<PV>(successor, q, -stretch(beta), -stretch(alpha), depth - 1, ply + 1, continuation));
                                }
                        }

                        if (value > best_value) {
                                best_value = value;
                                best_move = i;
                                if (best_value >= beta) {
                                        break;
                                }
                                if (best_value > alpha) {
                                        alpha = best_value;
                                        //refutation = make_variation(best_move, continuation);
                                        update_variation(refutation, best_move, continuation);
                                }
                        }
                }

                // we must have found a best move with a finite value
                assert(is_finite(best_value));
                assert(best_move != Transposition::no_move());

                auto const type = Bound::type(best_value, original_alpha, beta);
                TT.insert(n, { best_value, type, depth, best_move } );
                return best_value;
        }

        void announce(State const& s, int depth)
        {
                std::cout << core::diag << s;
                std::cout << core::fen << s << '\n';
                std::cout << "Searching to nominal depth=" << depth << "\n\n";
        }

        template<class Stopwatch, class Actions, class Node>
        void report(int depth, int value, Stopwatch const& stopwatch, Actions successor, Node const& n, Variation const& pv)
        {
                std::cout << "info";

                std::cout << " score ";
                std::cout << std::setw( 3) << std::right << print(value);

                std::cout << " depth ";
                std::cout << std::setw( 2) << depth;
                std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(1);
                std::cout << "/" << boost::accumulators::mean(statistics_.ply());
                std::cout << std::setprecision(0);
                std::cout << "/" << boost::accumulators::max(statistics_.ply());

                auto const node_count = boost::accumulators::count(statistics_.nodes());
                std::cout << " nodes ";
                std::cout << std::setw(11) << std::right << node_count;

                auto const split = stopwatch.split_time();
                std::cout << " time ";
                std::cout << std::setw( 6) << split.count();

                auto const nps = static_cast<double>(node_count) / static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(split).count());
                std::cout << " nps ";
                std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
                std::cout << std::setw( 7) << nps;

                auto const hashfull = 1000 * (static_cast<double>(TT.size()) / static_cast<double>(TT.capacity()));
                std::cout << " hashfull ";
                std::cout << std::setw( 4) << std::right << hashfull;

                std::cout << '\n';
                print_pv(successor, n, pv);
        }

        template<class Actions, class Node>
        void insert_pv(Actions successor, Node const& n, Variation const& pv, int value, int ply = 0)
        {
                auto const depth = static_cast<int>(pv.size()) - ply;
                if (depth == 0) {
                        assert(
                                (value == eval::score(n.state())) ||
                                (value == draw_value() && is_draw(n.state())) ||
                                (value == loss_min() && not successor.detect(n.state()))
                                // NOTE: with endgame databases, delayed losses can occur at the tips of the pv
                        );
                        TT.insert(n, Transposition(value, Bound::exact, depth, Transposition::no_move()));
                        return;
                }

                using R = core::rules_t<State>;
                using B = core::board_t<State>;
                std::vector<core::action<R,B>> moves;
                successor.generate(n.state(), moves);
                auto const index = static_cast<std::size_t>(pv[static_cast<std::size_t>(ply)]) % moves.size();
                auto const best_move = moves[index];
                TT.insert(n, Transposition(value, Bound::exact, depth, static_cast<int>(index)));

                insert_pv(successor, child(n, best_move), pv, -stretch(value), ply + 1);
        }

        template<class Actions, class Node>
        void print_pv(Actions successor, Node const& n, Variation const& pv, int ply = 0)
        {
                auto const depth = static_cast<int>(pv.size()) - ply;
                if (depth == 0) {
                        std::cout << std::endl /*'\n'*/;
                        std::cout << core::diag << n.state();
                        return;
                }

                using R = core::rules_t<State>;
                using B = core::board_t<State>;
                std::vector<core::action<R,B>> moves;
                successor.generate(n.state(), moves);
                auto const best_move = moves[static_cast<std::size_t>(pv[static_cast<std::size_t>(ply)]) % moves.size()];

                if (not (ply % 2)) {
                	std::cout << std::setw(2) << std::right << ((ply / 2) + 1) << ". ";
                }
                std::cout << best_move;
                std::cout << ((ply % 10 == 9) ? '\n' : ' ');

                auto q = child(n, best_move);
                //if (q.same_king_push(not q.to_move()))
                        //std::cout << "^" << q.same_king_push(not q.to_move());
                print_pv(successor, q, pv, ply + 1);
        }

        bool is_pv(int node) const
        {
                return node == PV;
        }

        // implementation
        static const auto ROOT_ID_INCREMENT = 2;

        // 8-byte hash entries: 32-bit hash signature, 4-byte {value, type, depth, move} content
        // 8-way buckets on 64-byte cache lines, (1 Gb = 2^27 entries)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        using TranspositionTable = hash::set_associative_cache<
                node_type,
                Transposition,
                4,
                hash::EmptyOldMin<hash::Shallowest>,
                extract::UpperBits,
                extract::Hash
        >;

        TranspositionTable TT;

        // representation
        Statistics statistics_;
        bool interrupted_;
};

}       // namespace search
}       // namespace dctl::algo
