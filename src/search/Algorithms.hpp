#include <algorithm>    // std::generate
#include <cassert>
#include <iterator>     // std::back_inserter
#include <vector>       // std::vector
#include "../successors/Successors.h"
#include "../node/Position.h"
#include "../node/Stack.h"
#include "../utils/Iota.h"

namespace dctl {
namespace search {

// iterative deepening with no move ordering at the root
template<typename Rules, typename Board>
int Root::iterative_deepening(const Position<Board>& p, int depth)
{
        int score = -infinity();                
        int alpha, beta;

        Parameters root_node;       
        Timer timer;
        announce(p, depth);
        for (auto i = 1; i <= depth; i += ROOT_ID_INCREMENT) {
                statistics_.reset();
                alpha = -infinity();
                beta = infinity();
                score = pvs<PV, Rules>(p, 0, i, alpha, beta, root_node);
                insert_PV<Rules>(p, root_node.PV(), score);
                timer.split();
                report<Rules>(i, score, timer, p, root_node.PV());
        }

        return score;
}

// principal variation search (PVS)
template<int ThisNode, typename Rules, typename Board>
int Root::pvs(const Position<Board>& p, int ply, int depth, int alpha, int beta, Parameters& parent_node)
{
        //if (is_interrupted())
        //        return alpha;

        statistics_.update(ply);
        
        // check for a legal draw
        if (is_draw<Rules>(p))
                return draw();       

        // return evaluation in leaf nodes with valid moves
        if (depth <= 0)
                return !Successors<Rules, Board>::detect_legal(p)? loss_value(0) : Evaluate::evaluate(p);

        assert(depth > 0);
        assert(alpha >= -infinity());
        assert(beta <= infinity());

        // mate distance pruning
        if (alpha >= win_value(1))
                return alpha;
        if (beta <= loss_value(0))
                return beta;

        assert(
                ( is_PV(ThisNode) && alpha <  beta - 1) ||
                (!is_PV(ThisNode) && alpha == beta - 1)
        );

        // TT cut-off for exact win/loss scores or for deep enough heuristic scores
        auto TT_entry = TT.find(p);
        if (TT_entry && (is_mate(TT_entry->value()) || TT_entry->depth() >= depth) && TT_entry->is_cutoff(alpha, beta))
                return TT_entry->value();

        // generate moves
        Stack moves;
        moves.reserve(32);
        Successors<Rules, Board>::generate_legal(p, moves);

        // without a valid move, the position is an immediate loss
        if (moves.empty()) {
                const int value = loss_value(0);

                // we can only have an upper bound or an exact value at this point
                assert(value < beta);
                const Transposition::Bound bound = (value <= alpha)? Transposition::upper() : Transposition::exact();

                TT.insert(p, Transposition(value, bound, depth, Transposition::no_move()));
                return value;
        }

        // internal iterative deepening
        if (!(TT_entry && TT_entry->has_move())) {
                const int IID_depth = is_PV(ThisNode)? depth - 2 : depth / 2;
                if (IID_depth > 0) {
                        pvs<ThisNode, Rules>(p, ply, IID_depth, alpha, beta, parent_node);
                        TT_entry = TT.find(p);
                        assert(TT_entry);
                }
        }

        // move ordering
        std::vector<int> move_order;
        move_order.reserve(moves.size());                               // reserve enough room for all indices
        iota_n(std::back_inserter(move_order), moves.size(), 0);        // generate indices [0, moves.size() - 1]

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
        Parameters child_node;

        for (auto s = 0; s < static_cast<int>(move_order.size()); ++s) {
                i = move_order[s];
                // TODO: TT singular extension

                // TODO: futility pruning

                auto q(p);
                q.attach(p);
                q.template make<Rules>(moves[i]);

                if (is_PV(ThisNode) && s == 0)
                        value = -squeeze(pvs<PV, Rules>(q, ply + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));
                else {
                        // TODO: late move reductions

                        value = -squeeze(pvs<ZW, Rules>(q, ply + 1, depth - 1, -stretch(alpha + 1), -stretch(alpha), child_node));
                        if (is_PV(ThisNode) && value > alpha && value < beta)
                                value = -squeeze(pvs<PV, Rules>(q, ply + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));
                }

                if (value > best_value) {
                        best_value = value;
                        best_move = i; 
                        if (is_PV(ThisNode) && best_value > alpha) {
                                alpha = best_value;
                                parent_node.set_PV(best_move, child_node.PV());
                        }
                        if (best_value >= beta)
                                break;                      
                }
        }

        // we must have found a best move with a finite value
        assert(is_finite(best_value));
        assert(best_move != Transposition::no_move());

        // determine the bound type of the value
        const Transposition::Bound bound = 
                best_value <= original_alpha ? Transposition::upper() : 
                best_value >= beta ? Transposition::lower() : Transposition::exact();
        TT.insert(p, Transposition(best_value, bound, depth, best_move));
        return best_value;
}

/*
// principal variation search (PVS) with TT cut-offs, TT move ordering and IID
template<int ThisNode, typename Rules, typename Board>
int Root::quiescence(const Position<Board>& p, int ply, int depth, int alpha, int beta, Parameters& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.is_draw<Rules>())
                return draw();

        // check for legal move_stack
        if (!successors::detect(p)) {
                return loss_value(0);
        }

        // generate captures and promotions
        Stack move_stack;
        successors::generate_captures_promotions(p, move_stack);

        if (move_stack.empty())
        {
                if (successors::detect_pending_captures_promotions(p)) {

                } else {
                        // return eval
                }
        }

        // search generated moves
}
*/

// negamax
template<typename Rules, typename Board>
int Root::negamax(const Position<Board>& p, int ply, int depth, Parameters& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return draw();

        // return evaluation in leaf nodes with valid moves
        if (depth == 0)
                return !Successors<Rules, Board>::detect(p)? loss_value(0) : Evaluate::evaluate(p);

        // generate moves
        Stack moves;
        Successors<Rules, Board>::generate_legal(p, moves);

        // search moves
        auto best_value = -infinity();
        int value;
        Parameters child_node;
        for (auto i = 0; i < static_cast<int>(moves.size()); ++i) {
                auto q(p);
                q.attach(p);
                q.template make<Rules>(moves[i]);

                value = -squeeze(negamax<Rules>(q, ply + 1, depth - 1, child_node));

                if (value > best_value) {
                        best_value = value;
                        parent_node.set_PV(i, child_node.PV());
                }
        }

        // without a valid move, the position is an immediate loss
        return std::max(loss_value(0), best_value);
}

// alpha-beta
template<typename Rules, typename Board>
int Root::alpha_beta(const Position<Board>& p, int ply, int depth, int alpha, int beta, Parameters& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return draw();

        // mate distance pruning
        if (alpha >= win_value(1))
                return alpha;
        if (beta <= loss_value(0))
                return beta;

        // return evaluation in leaf nodes with valid moves
        if (depth == 0)
                return !Successors<Rules, Board>::detect(p)? loss_value(0) : Evaluate::evaluate(p);

        // generate moves
        Stack moves;
        Successors<Rules, Board>::generate_legal(p, moves);

        // search moves
        auto best_value = -infinity();
        int value;
        Parameters child_node;
        for (auto i = 0; i < static_cast<int>(moves.size()); ++i) {
                auto q(p);
                q.attach(p);
                q.template make<Rules>(moves[i]);

                value = -squeeze(alpha_beta<Rules>(p, ply  + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));

                if (value > best_value) {
                        best_value = value;
                        if (best_value > alpha) {
                                alpha = best_value;
                                parent_node.set_PV(i, child_node.PV());
                        }
                        if (best_value >= beta)
                                break;                      
                }
        }

        // without a valid move, the position is an immediate loss
        return std::max(loss_value(0), best_value);
}

}       // namespace search
}       // namespace dctl
