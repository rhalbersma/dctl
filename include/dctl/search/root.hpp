#pragma once
#include <algorithm>                    // generate
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <iterator>                     // back_inserter
#include <vector>                       // vector

#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT

#include <dctl/search/bound.hpp>
#include <dctl/search/objective.hpp>
#include <dctl/search/score.hpp>
#include <dctl/search/transposition.hpp>
#include <dctl/search/variation.hpp>
#include <dctl/evaluate/score.hpp>
#include <dctl/utility/stack_vector.hpp>
#include <dctl/hash/signature_extractor.hpp>
#include <dctl/hash/map.hpp>
#include <dctl/hash/replace.hpp>
#include <dctl/successor/copy.hpp>
#include <dctl/successor/count.hpp>

#include <dctl/utility/algorithm.hpp>
#include <dctl/utility/ply.hpp>
#include <dctl/utility/int.hpp>
#include <dctl/utility/stack_vector.hpp>
#include <dctl/utility/statistics.hpp>
#include <dctl/utility/timer.hpp>

#include <dctl/setup/diagram.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/notation/string.hpp>

namespace dctl {
namespace search {

template
<
        typename Position,
        typename Objective
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

        int analyze(Position const& p, int depth)
        {
                return iterative_deepening(p, depth);
        }

        int solve(Position const& p, int depth)
        {
                return proof_verify(p, depth);
        }

        void resize_hash(int s)
        {
                TT.resize(static_cast<std::size_t>(s));
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
        int iterative_deepening(Position const& p, int depth)
        {
                auto score = -infinity();
                int alpha, beta;

                Arena<int>::type iar;
                Alloc<int>::type ial(iar);
                Variation pv(ial);
                Timer timer;
                announce(p, depth);
                statistics_.reset();
                for (auto i = 1; i <= depth; i += ROOT_ID_INCREMENT) {
                        alpha = -infinity();
                        beta = infinity();
                        score = pvs<PV>(p, alpha, beta, i, 0, pv);
                        insert_pv(p, pv, score);
                        timer.split();
                        report(i, score, timer, p, pv);
                }

                return (score);
        }

        template<int NodeType>
        int pvs(Position const& p, int alpha, int beta, int depth, int ply, Variation& refutation)
        {
                statistics_.collect(ply);

                if (is_interrupted())
                        return alpha;

                // -INF <= alpha < beta <= +INF
                BOOST_ASSERT(-infinity() <= alpha && alpha < beta && beta <= infinity());

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
                BOOST_ASSERT(is_pv(NodeType) == (alpha <  beta - 1));

                // terminal positions
                auto const terminal_value = Objective::value(p);
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
                Arena<Move>::type a;
                auto const moves = successor::copy(p, a);
                BOOST_ASSERT(!moves.empty());

                Arena<int>::type oar;
                Alloc<int>::type oal(oar);
                Order move_order(oal);
                move_order.reserve(moves.size());                                       // reserve enough room for all indices
                algorithm::iota_n(std::back_inserter(move_order), moves.size(), 0);     // generate indices [0, moves.size() - 1]

                // internal iterative deepening (IID)
                if (!(TT_entry && TT_entry->has_move())) {
                        auto const IID_depth = is_pv(NodeType)? depth - 2 : depth / 2;
                        if (IID_depth > 0) {
                                pvs<NodeType>(p, alpha, beta, IID_depth, ply, refutation);
                                TT_entry = TT.find(p);
                                BOOST_ASSERT(TT_entry);
                        }
                }

                // move ordering
                if (TT_entry && TT_entry->has_move()) {
                        auto const TT_move = TT_entry->move() % moves.size();
                        std::swap(move_order[0], move_order[TT_move]);
                }

                // search moves
                auto const original_alpha = alpha;
                auto best_value = -infinity();
                auto best_move = Transposition::no_move();
                int value;

                Arena<int>::type car;
                Alloc<int>::type cal(car);
                Variation continuation(cal);
                continuation.reserve(DCTL_PP_STACK_RESERVE);

                for (auto const& i: move_order) {
                        auto q = successor::make_copy(p, moves[i]);

                        // TODO: TT singular extension

                        // TODO: futility pruning

                        if (is_pv(NodeType) && (&i == &move_order[0]))
                                value = -squeeze(pvs<PV>(q, -stretch(beta), -stretch(alpha), depth - 1, ply + 1, continuation));
                        else {
                                // TODO: late move reductions

                                value = -squeeze(pvs<ZW>(q, -stretch(alpha + 1), -stretch(alpha), depth - 1, ply + 1, continuation));
                                if (is_pv(NodeType) && alpha < value && value < beta)
                                        value = -squeeze(pvs<PV>(q, -stretch(beta), -stretch(alpha), depth - 1, ply + 1, continuation));
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
                BOOST_ASSERT(is_finite(best_value));
                BOOST_ASSERT(best_move != Transposition::no_move());

                auto const type = Bound::type(best_value, original_alpha, beta);
                TT.insert(p, { best_value, type, depth, best_move } );
                return best_value;
        }
        void announce(Position const& p, int depth)
        {
                std::cout << setup::diagram<pdn::protocol>()(p);
                std::cout << setup::write<pdn::protocol>()(p) << "\n";
                std::cout << "Searching to nominal depth=" << depth << "\n\n";
        }

        void report(int depth, int value, Timer const& timer, Position const& p, Variation const& pv)
        {
                std::cout << "info";

                std::cout << " score ";
                std::cout << std::setw( 3) << std::right << print(value);

                std::cout << " depth ";
                std::cout << std::setw( 2) << depth;
                std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(1);
                std::cout << "/" << boost::accumulators::mean(statistics_.ply());
                std::cout << "/" << std::setw( 2) << boost::accumulators::max(statistics_.ply());

                auto const node_count = boost::accumulators::count(statistics_.nodes());
                std::cout << " nodes ";
                std::cout << std::setw(11) << std::right << node_count;

                std::cout << " time ";
                std::cout << std::setw( 6) << timer.elapsed().count();

                double const nps = static_cast<double>(1000 * node_count) / static_cast<double>(timer.elapsed().count());
                std::cout << " nps ";
                std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
                std::cout << std::setw( 7) << nps;

                double const hashfull = 1000 * (static_cast<double>(TT.size()) / static_cast<double>(TT.capacity()));
                std::cout << " hashfull ";
                std::cout << std::setw( 4) << std::right << hashfull;

                std::cout << "\n";
                print_pv(p, pv);
        }

        void insert_pv(Position const& p, Variation const& pv, int value, int ply = 0)
        {
                auto const depth = static_cast<int>(pv.size()) - ply;
                if (depth == 0) {
                        BOOST_ASSERT(
                                (value == evaluate::score(p)) ||
                                (value == draw_value() && is_draw(p)) ||
                                (value == loss_min() && !successor::detect(p))
                                // NOTE: with endgame databases, delayed losses can occur at the tips of the pv
                        );
                        TT.insert(p, Transposition(value, Bound::exact, depth, Transposition::no_move()));
                        return;
                }

                Arena<Move>::type a;
                auto const moves = successor::copy(p, a);
                int const index = pv[ply] % moves.size();
                auto const best_move = moves[index];
                TT.insert(p, Transposition(value, Bound::exact, depth, index));

                insert_pv(successor::make_copy(p, best_move), pv, -stretch(value), ply + 1);
        }

        void print_pv(Position const& p, Variation const& pv, int ply = 0)
        {
                auto const depth = static_cast<int>(pv.size()) - ply;
                if (depth == 0) {
                        std::cout << std::endl /*'\n'*/;
                        std::cout << setup::diagram<pdn::protocol>()(p);
                        return;
                }

                Arena<Move>::type a;
                auto const moves = successor::copy(p, a);
                auto const best_move = moves[pv[ply] % moves.size()];

                if (!(ply % 2)) std::cout << std::setw(2) << std::right << ((ply / 2) + 1) << ". ";
                std::cout << notation::write(p, best_move);
                std::cout << ((ply % 10 == 9)? '\n' : ' ');

                auto q = successor::make_copy(p, best_move);
                //if (q.same_king_moves(!q.to_move()))
                        //std::cout << "^" << q.same_king_moves(!q.to_move());
                print_pv(q, pv, ply + 1);
        }

        bool is_pv(int node) const
        {
                return node == PV;
        }

        // implementation
        BOOST_STATIC_CONSTANT(auto, ROOT_ID_INCREMENT = 2);

        // 8-byte hash entries: 32-bit hash signature, 4-byte {value, type, depth, move} content
        // 8-way buckets on 64-byte cache lines, (1 Gb = 2^27 entries)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef hash::Map< 
                Position, 
                Transposition, 
                hash::UpperHashBitsExtractor, 
                hash::EmptyOldUnderCutMin<hash::Shallowest> 
        > TranspositionTable;

        TranspositionTable TT;

        // representation
        Statistics statistics_;
        bool interrupted_;
};

}       // namespace search
}       // namespace dctl
