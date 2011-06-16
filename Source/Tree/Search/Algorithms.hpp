#include <cassert>
#include "../Generate/Successors.h"

namespace tree {
namespace search {

using value::stretch;
using value::squeeze;

// iterative deepening with no move ordering at the root
template<typename Rules, typename Board>
int Root::iterative_deepening(const node::Position<Board>& p, int nominal_depth)
{
        int score = -value::infinity();                
        int alpha, beta;

        Parameters root_node;       
        Timer timer;
        announce(p, nominal_depth);
        for (int depth = 1; depth <= nominal_depth; depth += ROOT_ID_INCREMENT) {
                statistics_.reset();
                alpha = -value::infinity();
                beta = value::infinity();
                score = pvs<PV, Rules>(p, 0, depth, alpha, beta, root_node);
                timer.split();
                report(score, depth, timer);
                print_PV<Rules>(p, root_node.PV());
                insert_PV<Rules>(p, root_node.PV(), score);
        }

        return score;
}

// principal variation search (PVS)
template<int ThisNode, typename Rules, typename Board>
int Root::pvs(const node::Position<Board>& p, int ply, int depth, int alpha, int beta, Parameters& parent_node)
{
        statistics_.update(ply);
        
        assert(p.non_conversion() <= ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return value::draw();       

        // return evaluation in leaf nodes with valid move_stack
        if (depth <= 0)
                return !generate::Successors<Rules, Board>::detect(p)? value::loss(0) : Evaluate::evaluate(p);

        assert(depth > 0);
        assert(alpha >= -value::infinity());
        assert(beta <= value::infinity());

        // mate distance pruning
        if (alpha >= value::win(1))
                return alpha;
        if (beta <= value::loss(0))
                return beta;

        assert(
                ( is_PV(ThisNode) && alpha <  beta - 1) ||
                (!is_PV(ThisNode) && alpha == beta - 1)
        );

        // TT cut-off for exact win/loss scores or for deep enough heuristic scores
        const Entry* TT_entry = TT.find(p);
        if (TT_entry && (TT_entry->is_mate() || TT_entry->greater_equal(depth)) && TT_entry->is_cutoff(alpha, beta))
                return TT_entry->value();

        // generate moves
        move::Stack move_stack;
        generate::Successors<Rules, Board>::generate(p, &move_stack);

        // without a valid move, the position is an immediate loss
        if (move_stack.empty()) {
                const int loss_score = value::loss(0);

                // we can only have an upper bound or an exact value at this point
                assert(loss_score < beta);
                const Entry::Bound bound = (loss_score <= alpha)? Entry::upper() : Entry::exact();

                TT.insert(p, Entry(loss_score, bound, depth, Entry::no_move()));
                return loss_score;
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
        move::Order move_order(move_stack.size());
        identity_permutation(move_order);                
        if (TT_entry && TT_entry->has_move()) {
                const size_t TT_move = TT_entry->move() % move_stack.size();
                std::swap(move_order[0], move_order[TT_move]);
        }

        // search moves
        int value = -value::infinity();
        size_t best_move = Entry::no_move();
        int score;
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
                        score = -squeeze(pvs<PV, Rules>(q, ply + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));
                else {
                        // TODO: late move reductions

                        score = -squeeze(pvs<ZW, Rules>(q, ply + 1, depth - 1, -stretch(alpha + 1), -stretch(alpha), child_node));
                        if (is_PV(ThisNode) && score > alpha && score < beta)
                                score = -squeeze(pvs<PV, Rules>(q, ply + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));
                }

                if (score > value) {
                        value = score;
                        best_move = i; 
                        if (is_PV(ThisNode) && value > alpha) {
                                parent_node.set_PV(best_move, child_node.PV());
                                alpha = value;
                        }
                        if (value >= beta)
                                break;                      
                }
        }

        // we must have found a best move with a finite value
        assert(best_move != Entry::no_move());
        assert(value > -value::infinity());

        // determine the bound type of the value
        const Entry::Bound bound = 
                value <= original_alpha ? Entry::upper() : 
                value >= beta ? Entry::lower() : Entry::exact();
        TT.insert(p, Entry(value, bound, depth, best_move));
        return value;
}

/*
// principal variation search (PVS) with TT cut-offs, TT move ordering and IID
template<size_t Entry, typename Rules, typename Board>
int Root::quiescence(const node::Position<Board>& p, int ply, int depth, int alpha, int beta, Parameters& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.is_draw<Rules>())
                return value::draw();

        // check for legal move_stack
        if (!generate::detect(p)) {
                return value::loss(0);
        }

        // generate captures and promotions
        move::Stack move_stack;
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
                return value::draw();

        // return evaluation in leaf nodes with valid move_stack
        if (depth == 0)
                return !generate::detect<Rules>(p)? value::loss(0) : Evaluate::evaluate(p);

        // generate moves
        move::Stack move_stack;
        generate::generate(p, move_stack);

        // search moves
        int value = -value::infinity();
        int score;
        Parameters child_node;
        Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                score = -squeeze(negamax<Rules>(q, ply + 1, depth - 1, child_node));

                if (score > value) {
                        value = score;
                        parent_node.set_PV(i, child_node.PV());
                }
        }

        // without a valid move, the position is an immediate loss
        return std::max(value::loss(0), value);
}

// alpha-beta
template<typename Rules, typename Board>
int Root::alpha_beta(const node::Position<Board>& p, int ply, int depth, int alpha, int beta, Parameters& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return value::draw();

        // mate distance pruning
        if (alpha >= value::win(1))
                return alpha;
        if (beta <= value::loss(0))
                return beta;

        // return evaluation in leaf nodes with valid move_stack
        if (depth == 0)
                return !generate::detect<Rules>(p)? value::loss(0) : Evaluate::evaluate(p);

        // generate moves
        move::Stack move_stack;
        generate::generate(p, move_stack);

        // search moves
        int value = -value::infinity();
        int score;
        Parameters child_node;
        Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                score = -squeeze(alpha_beta<Rules>(p, ply  + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));

                if (score > value) {
                        if (score >= beta)
                                return score;
                        value = score;
                        if (value > alpha) {
                                alpha = value;
                                parent_node.set_PV(i, child_node.PV());
                        }
                }
        }

        // without a valid move, the position is an immediate loss
        return std::max(value::loss(0), value);
}

}       // namespace search
}       // namespace tree
