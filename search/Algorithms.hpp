#include <algorithm>    // std::generate
#include <iterator>     // std::back_inserter
#include <vector>       // std::vector
#include <boost/assert.hpp>
#include "../successor/Selection.hpp"
#include "../successor/Successor.hpp"
#include "../node/Position.hpp"
#include "../node/Stack.hpp"
#include "../utility/Iota.hpp"

namespace dctl {
namespace search {

// iterative deepening with no move ordering at the root
template<typename Rules, typename Board, typename Objective>
int Root<Rules, Board, Objective>::iterative_deepening(const Position<Board>& p, int depth)
{
        auto score = -infinity();                
        int alpha, beta;

        Variation pv;       
        Timer timer;
        announce(p, depth);
        for (auto i = 1; i <= depth; i += ROOT_ID_INCREMENT) {
                statistics_.reset();
                alpha = -infinity();
                beta = infinity();
                score = pvs<PV>(p, 0, i, alpha, beta, pv);
                insert_pv(p, pv.sequence(), score);
                timer.split();
                report(i, score, timer, p, pv.sequence());
        }

        return score;
}

// principal variation search (PVS)
template<typename Rules, typename Board, typename Objective> template<int NodeType>
int Root<Rules, Board, Objective>::pvs(
        const Position<Board>& p, int ply, int depth, int alpha, int beta, Variation& refutation
)
{
        statistics_.update(ply);
        
        if (is_interrupted())
                return alpha;

        // -INF <= alpha < beta <= +INF
        BOOST_ASSERT(alpha >= -infinity());
        BOOST_ASSERT(alpha <   beta);
        BOOST_ASSERT(beta  <=  infinity());

        BOOST_ASSERT(
                ( is_pv(NodeType) && alpha <  beta - 1) ||
                (!is_pv(NodeType) && alpha == beta - 1)
        );

        // mate distance pruning
        if (alpha >= MinimalWin<Objective>()())
                return alpha;
        if (beta <= MinimalLoss<Objective>()())
                return beta;

        // check for a legal draw
        if (is_draw<Rules>(p)) {
                const auto value = draw_value();        // TODO treat first/second player loss
                const auto type = Bound::type(value, alpha, beta);
                TT.insert(p, Transposition(value, type, depth, Transposition::no_move()));
                if (type == Bound::exact)
                        refutation.clear();
                return value;
        }

        // return evaluation in leaf nodes with valid moves (TODO treat regular/suicide wins
        if (depth <= 0)
                return !Successor<successor::Legal, Rules>::detect(p)? MinimalLoss<Objective>()() : Evaluate<Rules>::evaluate(p);

        // TT cut-off for exact win/loss scores or for deep enough heuristic scores
        auto TT_entry = TT.find(p);
        if (TT_entry && (is_mate(TT_entry->value()) || TT_entry->depth() >= depth) && TT_entry->is_cutoff(alpha, beta))
                return TT_entry->value();

        // generate moves
        Stack moves;
        moves.reserve(32);
        Successor<successor::Legal, Rules>::generate(p, moves);

        // without a valid move, the position is an immediate loss
        if (moves.empty()) {
                const auto value = MinimalLoss<Objective>()();
                const auto type = Bound::type(value, alpha, beta);
                TT.insert(p, Transposition(value, type, depth, Transposition::no_move()));

                // we can only have an upper bound or an exact value at this point
                BOOST_ASSERT(type != Bound::lower);

                return value;
        }
                
        std::vector<int> move_order;
        move_order.reserve(moves.size());                               // reserve enough room for all indices
        iota_n(std::back_inserter(move_order), moves.size(), 0);        // generate indices [0, moves.size() - 1]

        // internal iterative deepening (IID)
        if (!(TT_entry && TT_entry->has_move())) {
                const auto IID_depth = is_pv(NodeType)? depth - 2 : depth / 2;
                if (IID_depth > 0) {
                        pvs<NodeType>(p, ply, IID_depth, alpha, beta, refutation);
                        TT_entry = TT.find(p);
                        BOOST_ASSERT(TT_entry);
                }
        }

        // move ordering
        if (TT_entry && TT_entry->has_move()) {
                const auto TT_move = TT_entry->move() % moves.size();
                std::swap(move_order[0], move_order[TT_move]);
        }

        // search moves
        const auto original_alpha = alpha;
        auto best_value = -infinity();
        auto best_move = Transposition::no_move();
        int value;
        int i;
                
        Variation continuation;
        for (auto s = 0; s < static_cast<int>(move_order.size()); ++s) {
                i = move_order[s];
                // TODO: TT singular extension

                // TODO: futility pruning

                auto q = p;
                q.attach(p);
                q.template make<Rules>(moves[i]);

                if (is_pv(NodeType) && s == 0)
                        value = -squeeze(pvs<PV>(q, ply + 1, depth - 1, -stretch(beta), -stretch(alpha), continuation));
                else {
                        // TODO: late move reductions

                        value = -squeeze(pvs<ZW>(q, ply + 1, depth - 1, -stretch(alpha + 1), -stretch(alpha), continuation));
                        if (is_pv(NodeType) && value > alpha && value < beta)
                                value = -squeeze(pvs<PV>(q, ply + 1, depth - 1, -stretch(beta), -stretch(alpha), continuation));
                }

                if (value > best_value) {
                        best_value = value;
                        best_move = i; 
                        if (best_value >= beta)
                                break;
                        if (best_value > alpha) {
                                alpha = best_value;
                                refutation.set(best_move, continuation.sequence());
                        }                    
                }
        }

        // we must have found a best move with a finite value
        BOOST_ASSERT(is_finite(best_value));
        BOOST_ASSERT(best_move != Transposition::no_move());

        const auto type = Bound::type(best_value, original_alpha, beta);
        TT.insert(p, Transposition(best_value, type, depth, best_move));
        return best_value;
}

/*
// principal variation search (PVS) with TT cut-offs, TT move ordering and IID
template<int NodeType, typename Rules, typename Board>
int Root<Rules, Board>::quiescence(const Position<Board>& p, int ply, int depth, int alpha, int beta, Variation& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.is_draw<Rules>())
                return draw_value();

        // check for legal move_stack
        if (!successor::detect(p)) {
                return loss_value(0);
        }

        // generate captures and promotions
        Stack move_stack;
        successor::generate_captures_promotions(p, move_stack);

        if (move_stack.empty())
        {
                if (successor::detect_pending_captures_promotions(p)) {

                } else {
                        // return eval
                }
        }

        // search generated moves
}

// negamax
template<typename Rules, typename Board>
int Root<Rules, Board>::negamax(const Position<Board>& p, int ply, int depth, Variation& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return draw_value();

        // return evaluation in leaf nodes with valid moves
        if (depth == 0)
                return !Successor<successor::Legal, Rules>::detect(p)? loss_value(0) : Evaluate<Rules>::evaluate(p);

        // generate moves
        Stack moves;
        Successor<successor::Legal, Rules>::generate(p, moves);

        // search moves
        auto best_value = -infinity();
        int value;
        Variation child_node;
        for (auto i = 0; i < static_cast<int>(moves.size()); ++i) {
                auto q = p;
                q.attach(p);
                q.template make<Rules>(moves[i]);

                value = -squeeze(negamax<Rules>(q, ply + 1, depth - 1, child_node));

                if (value > best_value) {
                        best_value = value;
                        parent_node.set_pv(i, child_node.pv());
                }
        }

        // without a valid move, the position is an immediate loss
        return std::max(loss_value(0), best_value);
}

// alpha-beta
template<typename Rules, typename Board>
int Root<Rules, Board>::alpha_beta(const Position<Board>& p, int ply, int depth, int alpha, int beta, Variation& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return draw_value();

        // mate distance pruning
        if (alpha >= win_value(1))
                return alpha;
        if (beta <= loss_value(0))
                return beta;

        // return evaluation in leaf nodes with valid moves
        if (depth == 0)
                return !Successor<successor::Legal, Rules>::detect(p)? loss_value(0) : Evaluate<Rules>::evaluate(p);

        // generate moves
        Stack moves;
        Successor<successor::Legal, Rules>::generate(p, moves);

        // search moves
        auto best_value = -infinity();
        int value;
        Variation child_node;
        for (auto i = 0; i < static_cast<int>(moves.size()); ++i) {
                auto q = p;
                q.attach(p);
                q.template make<Rules>(moves[i]);

                value = -squeeze(alpha_beta<Rules>(p, ply  + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));

                if (value > best_value) {
                        best_value = value;
                        if (best_value > alpha) {
                                alpha = best_value;
                                parent_node.set_pv(i, child_node.pv());
                        }
                        if (best_value >= beta)
                                break;                      
                }
        }

        // without a valid move, the position is an immediate loss
        return std::max(loss_value(0), best_value);
}
*/
}       // namespace search
}       // namespace dctl