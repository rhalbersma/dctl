#include <cassert>
#include "../Generate/Successors.h"

namespace tree {
namespace search {

using score::stretch;
using score::squeeze;

// iterative deepening with no move ordering at the root
template<typename Rules, typename Board>
int Root::iterative_deepening(const node::Position<Board>& p, int depth)
{
        int score = -score::infinity();                
        int alpha, beta;

        Parameters root_node;       
        Timer timer;
        announce(p, depth);
        for (auto i = 1; i <= depth; i += ROOT_ID_INCREMENT) {
                statistics_.reset();
                alpha = -score::infinity();
                beta = score::infinity();
                score = pvs<PV, Rules>(p, 0, i, alpha, beta, root_node);
                timer.split();
                report(i, score, timer);
                print_PV<Rules>(p, root_node.PV());
                insert_PV<Rules>(p, root_node.PV(), score);
        }

        return score;
}

// principal variation search (PVS)
template<int ThisNode, typename Rules, typename Board>
int Root::pvs(const node::Position<Board>& p, int ply, int depth, int alpha, int beta, Parameters& parent_node)
{
        //if (is_interrupted())
        //        return alpha;

        statistics_.update(ply);
        
        // check for a legal draw
        if (p.template is_draw<Rules>())
                return score::draw();       

        // return evaluation in leaf nodes with valid moves
        if (depth <= 0)
                return !generate::Successors<Rules, Board>::detect(p)? score::loss_value(0) : Evaluate::evaluate(p);

        assert(depth > 0);
        assert(alpha >= -score::infinity());
        assert(beta <= score::infinity());

        // mate distance pruning
        if (alpha >= score::win_value(1))
                return alpha;
        if (beta <= score::loss_value(0))
                return beta;

        assert(
                ( is_PV(ThisNode) && alpha <  beta - 1) ||
                (!is_PV(ThisNode) && alpha == beta - 1)
        );

        // TT cut-off for exact win/loss scores or for deep enough heuristic scores
        auto TT_entry = TT.find(p);
        if (TT_entry && (TT_entry->is_mate() || TT_entry->greater_equal(depth)) && TT_entry->is_cutoff(alpha, beta))
                return TT_entry->value();

        // generate moves
        node::Stack move_stack;
        generate::Successors<Rules, Board>::generate(p, &move_stack);

        // without a valid move, the position is an immediate loss
        if (move_stack.empty()) {
                const int value = score::loss_value(0);

                // we can only have an upper bound or an exact value at this point
                assert(value < beta);
                const Entry::Bound bound = (value <= alpha)? Entry::upper() : Entry::exact();

                TT.insert(p, Entry(value, bound, depth, Entry::no_move()));
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
        node::Order move_order(move_stack.size());
        identity_permutation(move_order);                
        if (TT_entry && TT_entry->has_move()) {
                const size_t TT_move = TT_entry->move() % move_stack.size();
                std::swap(move_order[0], move_order[TT_move]);
        }

        // search moves
        int best_value = -score::infinity();
        size_t best_move = Entry::no_move();
        int value;
        size_t i;
        Parameters child_node;
        const int original_alpha = alpha;

        node::Position<Board> q;
        for (size_t s = 0; s < move_order.size(); ++s) {
                i = move_order[s];
                // TODO: TT singular extension

                // TODO: futility pruning

                q.template copy_make<Rules>(p, move_stack[i]);

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
        assert(score::is_finite(best_value));
        assert(best_move != Entry::no_move());

        // determine the bound type of the value
        const Entry::Bound bound = 
                best_value <= original_alpha ? Entry::upper() : 
                best_value >= beta ? Entry::lower() : Entry::exact();
        TT.insert(p, Entry(best_value, bound, depth, best_move));
        return best_value;
}

/*
// principal variation search (PVS) with TT cut-offs, TT move ordering and IID
template<size_t Entry, typename Rules, typename Board>
int Root::quiescence(const node::Position<Board>& p, int ply, int depth, int alpha, int beta, Parameters& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.is_draw<Rules>())
                return score::draw();

        // check for legal move_stack
        if (!generate::detect(p)) {
                return score::loss_value(0);
        }

        // generate captures and promotions
        node::Stack move_stack;
        generate::generate_captures_promotions(p, move_stack);

        if (move_stack.empty())
        {
                if (generate::detect_pending_captures_promotions(p)) {

                } else {
                        // return eval
                }
        }

        // search generated moves
}
*/

// negamax
template<typename Rules, typename Board>
int Root::negamax(const node::Position<Board>& p, int ply, int depth, Parameters& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return score::draw();

        // return evaluation in leaf nodes with valid move_stack
        if (depth == 0)
                return !generate::detect<Rules>(p)? score::loss_value(0) : Evaluate::evaluate(p);

        // generate moves
        node::Stack move_stack;
        generate::generate(p, move_stack);

        // search moves
        int best_value = -score::infinity();
        int value;
        Parameters child_node;
        Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                value = -squeeze(negamax<Rules>(q, ply + 1, depth - 1, child_node));

                if (value > best_value) {
                        best_value = value;
                        parent_node.set_PV(i, child_node.PV());
                }
        }

        // without a valid move, the position is an immediate loss
        return std::max(score::loss_value(0), best_value);
}

// alpha-beta
template<typename Rules, typename Board>
int Root::alpha_beta(const node::Position<Board>& p, int ply, int depth, int alpha, int beta, Parameters& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return score::draw();

        // mate distance pruning
        if (alpha >= score::win_value(1))
                return alpha;
        if (beta <= score::loss_value(0))
                return beta;

        // return evaluation in leaf nodes with valid move_stack
        if (depth == 0)
                return !generate::detect<Rules>(p)? score::loss_value(0) : Evaluate::evaluate(p);

        // generate moves
        node::Stack move_stack;
        generate::generate(p, move_stack);

        // search moves
        int best_value = -score::infinity();
        int value;
        Parameters child_node;
        Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
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
        return std::max(score::loss_value(0), best_value);
}

}       // namespace search
}       // namespace tree
