#pragma once
#include <dctl/search/bound.hpp>
#include <dctl/search/objective.hpp>
#include <dctl/search/score.hpp>
#include <dctl/search/transposition.hpp>
#include <dctl/search/variation.hpp>
#include <dctl/eval/score.hpp>
#include <dctl/utility/stack_vector.hpp>
#include <dctl/hash/extract.hpp>
#include <dctl/hash/map.hpp>
#include <dctl/hash/replace.hpp>
#include <dctl/result.hpp>
#include <dctl/actions.hpp>

#include <dctl/utility/algorithm.hpp>
#include <dctl/utility/ply.hpp>         // PlyCount
#include <dctl/utility/stack_vector.hpp>
#include <dctl/utility/statistics.hpp>
#include <dctl/utility/stopwatch.hpp>

#include <dctl/setup/diagram.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/action/ostream.hpp>

#include <xstd/cstddef.hpp>
#include <range/v3/all.hpp>
#include <cassert>                      // assert
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <iterator>                     // back_inserter
#include <vector>                       // vector

namespace dctl {
namespace search {

template
<
        class State,
        class Objective
>
class Root
{
public:
        // typdefs
        enum EntryType { ZW, PV };

        Root()
        :
                interrupted_(false)
        {
        }

        template<class Successor>
        int analyze(State const& p, Successor successor, int depth)
        {
                return iterative_deepening(p, successor, depth);
        }

        int solve(State const& p, int depth)
        {
                return proof_verify(p, depth);
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
        template<class Successor>
        int iterative_deepening(State const& p, Successor successor, int depth)
        {
                auto score = -infinity();
                int alpha, beta;

                Arena<int> iar;
                Alloc<int> ial(iar);
                Variation pv(ial);
                announce(p, depth);
                statistics_.reset();
                util::Stopwatch stopwatch;
                stopwatch.start_stop();
                for (auto i = 1; i <= depth; i += ROOT_ID_INCREMENT) {
                        alpha = -infinity();
                        beta = infinity();
                        score = pvs<PV>(p, successor, alpha, beta, i, 0, pv);
                        insert_pv(p, successor, pv, score);
                        stopwatch.split_reset();
                        report(i, score, stopwatch, p, successor, pv);
                }

                return score;
        }

        template<int NodeType, class Successor>
        int pvs(State const& p, Successor successor, int alpha, int beta, int depth, int ply, Variation& refutation)
        {
                using namespace xstd::support_literals;
                statistics_.collect(ply);

                if (is_interrupted())
                        return alpha;

                // -INF <= alpha < beta <= +INF
                assert(-infinity() <= alpha && alpha < beta && beta <= infinity());

                // alpha < beta <= +INF implies alpha <= win_min
                // with equality, any finite score will fail low
                if (alpha == win_min())
                        return alpha;

                // -INF <= alpha < beta implies loss_min <= beta
                // with equality, any finite score will fail high
                if (beta == loss_min())
                        return beta;

                // alpha < beta implies alpha <= beta - 1,
                // with the strict inequality if and only if is_pv(NodeType)
                assert(is_pv(NodeType) == (alpha <  beta - 1));

                // terminal positions
                auto const terminal_value = Objective::value(p, successor);
                if (is_finite(terminal_value)) {
                        if (depth > 0) {
                                auto const type = Bound::type(terminal_value, alpha, beta);
                                TT.insert(p, { terminal_value, type, depth, Transposition::no_move() } );
                                if (type == Bound::exact)
                                        refutation.clear();
                        }
                        return terminal_value;
                }

                // return evaluation in leaf nodes with valid moves
                if (depth <= 0 || ply >= MAX_PLY)
                        return evaluate::score(p);

                // TT cut-off for exact win/loss scores or for deep enough heuristic scores
                auto TT_entry = TT.find(p);
                if (TT_entry && (is_mate(TT_entry->value()) || TT_entry->depth() >= depth) && TT_entry->is_cutoff(alpha, beta))
                        return TT_entry->value();

                // generate moves
                using R = typename State::rules_type;
                using B = typename State::board_type;

                Arena<Action<R,B> > a;
                auto moves = stack_vector<Action<R,B>>(Alloc<Action<R,B>>{a});
                moves.reserve(DCTL_PP_STACK_RESERVE);
                successor.generate(p, moves);
                assert(!moves.empty());

                Arena<int> oar;
                auto move_order = Order(Alloc<int>{oar});
                move_order.reserve(moves.size());                                                               // reserve enough room for all indices
                move_order |= ranges::action::push_back(ranges::view::iota(0, static_cast<int>(moves.size()))); // generate indices [0, moves.size() - 1]

                // internal iterative deepening (IID)
                if (!(TT_entry && TT_entry->has_move())) {
                        auto const IID_depth = is_pv(NodeType) ? depth - 2 : depth / 2;
                        if (IID_depth > 0) {
                                pvs<NodeType>(p, successor, alpha, beta, IID_depth, ply, refutation);
                                TT_entry = TT.find(p);
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

                Arena<int> car;
                auto continuation = Variation(Alloc<int>{car});
                continuation.reserve(DCTL_PP_STACK_RESERVE);

                for (auto const& i : move_order) {
                        auto q = result(p, moves[static_cast<std::size_t>(i)]);

                        // TODO: TT singular extension

                        // TODO: futility pruning

                        if (is_pv(NodeType) && (&i == &move_order[0]))
                                value = -squeeze(pvs<PV>(q, successor, -stretch(beta), -stretch(alpha), depth - 1, ply + 1, continuation));
                        else {
                                // TODO: late move reductions

                                value = -squeeze(pvs<ZW>(q, successor, -stretch(alpha + 1), -stretch(alpha), depth - 1, ply + 1, continuation));
                                if (is_pv(NodeType) && alpha < value && value < beta)
                                        value = -squeeze(pvs<PV>(q, successor, -stretch(beta), -stretch(alpha), depth - 1, ply + 1, continuation));
                        }

                        if (value > best_value) {
                                best_value = value;
                                best_move = i;
                                if (best_value >= beta)
                                        break;
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
                TT.insert(p, { best_value, type, depth, best_move } );
                return best_value;
        }
        void announce(State const& p, int depth)
        {
                std::cout << setup::diagram<pdn::protocol>()(p);
                std::cout << setup::write<pdn::protocol>()(p) << '\n';
                std::cout << "Searching to nominal depth=" << depth << "\n\n";
        }

        template<class Stopwatch, class Successor>
        void report(int depth, int value, Stopwatch const& stopwatch, State const& p, Successor successor, Variation const& pv)
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

                auto const nps = static_cast<double>(node_count) / std::chrono::duration_cast<std::chrono::seconds>(split).count();
                std::cout << " nps ";
                std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
                std::cout << std::setw( 7) << nps;

                auto const hashfull = 1000 * (static_cast<double>(TT.size()) / TT.capacity());
                std::cout << " hashfull ";
                std::cout << std::setw( 4) << std::right << hashfull;

                std::cout << '\n';
                print_pv(p, successor, pv);
        }

        template<class Successor>
        void insert_pv(State const& p, Successor successor, Variation const& pv, int value, int ply = 0)
        {
                auto const depth = static_cast<int>(pv.size()) - ply;
                if (depth == 0) {
                        assert(
                                (value == evaluate::score(p)) ||
                                (value == draw_value() && is_draw(p)) ||
                                (value == loss_min() && !successor.detect(p))
                                // NOTE: with endgame databases, delayed losses can occur at the tips of the pv
                        );
                        TT.insert(p, Transposition(value, Bound::exact, depth, Transposition::no_move()));
                        return;
                }

                using R = typename State::rules_type;
                using B = typename State::board_type;
                std::vector<Action<R,B>> moves;
                successor.generate(p, moves);
                auto const index = static_cast<std::size_t>(pv[static_cast<std::size_t>(ply)]) % moves.size();
                auto const best_move = moves[index];
                TT.insert(p, Transposition(value, Bound::exact, depth, static_cast<int>(index)));

                insert_pv(result(p, best_move), successor, pv, -stretch(value), ply + 1);
        }

        template<class Successor>
        void print_pv(State const& p, Successor successor, Variation const& pv, int ply = 0)
        {
                auto const depth = static_cast<int>(pv.size()) - ply;
                if (depth == 0) {
                        std::cout << std::endl /*'\n'*/;
                        std::cout << setup::diagram<pdn::protocol>()(p);
                        return;
                }

                using R = typename State::rules_type;
                using B = typename State::board_type;
                std::vector<Action<R,B>> moves;
                successor.generate(p, moves);
                auto const best_move = moves[static_cast<std::size_t>(pv[static_cast<std::size_t>(ply)]) % moves.size()];

                if (!(ply % 2)) std::cout << std::setw(2) << std::right << ((ply / 2) + 1) << ". ";
                std::cout << best_move;
                std::cout << ((ply % 10 == 9) ? '\n' : ' ');

                auto q = result(p, best_move);
                //if (q.same_king_push(!q.to_move()))
                        //std::cout << "^" << q.same_king_push(!q.to_move());
                print_pv(q, successor, pv, ply + 1);
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
                State,
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
}       // namespace dctl
