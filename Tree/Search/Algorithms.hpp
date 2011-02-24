#include <cassert>

namespace Tree {
namespace Search {

using Value::stretch;
using Value::squeeze;

// negamax
template<typename Rules, typename Board>
int Root::negamax(const Position<Board>& p, size_t ply, size_t depth, Parameters& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return Value::draw();

        // return evaluation in leaf nodes with valid move_list
        if (depth == 0)
                return !Generate::detect<Rules>(p)? Value::loss(0) : Evaluate::evaluate(p);

        // generate move_list
        Move::List move_list;
        Generate::generate(p, move_list);

        // search move_list
        int value = -Value::infinity();
        int score;
        Parameters child_node;
        Position<Board> q;
        for (size_t i = 0; i < move_list.size(); ++i) {
                q.template copy_make<Rules>(p, move_list[i]);
                score = -squeeze(negamax<Rules>(q, ply + 1, depth - 1, child_node));

                if (score > value) {
                        value = score;
                        parent_node.reset_PV(i, child_node.PV());
                }
        }

        // without a valid move, the position is an immediate loss
        return std::max(Value::loss(0), value);
}

// alpha-beta
template<typename Rules, typename Board>
int Root::alpha_beta(const Position<Board>& p, size_t ply, size_t depth, int alpha, int beta, Parameters& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return Value::draw();

        // mate distance pruning
        if (alpha >= Value::win(1))
                return alpha;
        if (beta <= Value::loss(0))
                return beta;

        // return evaluation in leaf nodes with valid move_list
        if (depth == 0)
                return !Generate::detect<Rules>(p)? Value::loss(0) : Evaluate::evaluate(p);

        // generate move_list
        Move::List move_list;
        Generate::generate(p, move_list);

        // search move_list
        int value = -Value::infinity();
        int score;
        Parameters child_node;
        Position<Board> q;
        for (size_t i = 0; i < move_list.size(); ++i) {
                q.template copy_make<Rules>(p, move_list[i]);
                score = -squeeze(alpha_beta<Rules>(p, ply  + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));

                if (score > value) {
                        if (score >= beta)
                                return score;
                        value = score;
                        if (value > alpha) {
                                alpha = value;
                                parent_node.reset_PV(i, child_node.PV());
                        }
                }
        }

        // without a valid move, the position is an immediate loss
        return std::max(Value::loss(0), value);
}

// principal variation search (PVS) with TT cut-offs, TT move ordering and IID
template<size_t ThisNode, typename Rules, typename Board>
int Root::search(const Position<Board>& p, size_t ply, int depth, int alpha, int beta, Parameters& parent_node)
{
        statistics_.update(ply);
        
        assert(p.non_conversion() <= ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return Value::draw();       

        // return evaluation in leaf nodes with valid move_list
        if (depth <= 0)
                return !Generate<Rules, Board>::detect(p)? Value::loss(0) : Evaluate::evaluate(p);

        assert(depth > 0);
        assert(alpha >= -Value::infinity());
        assert(beta <= Value::infinity());

        // mate distance pruning
        if (alpha >= Value::win(1))
                return alpha;
        if (beta <= Value::loss(0))
                return beta;

        assert(
                ( is_PV(ThisNode) && alpha <  beta - 1) ||
                (!is_PV(ThisNode) && alpha == beta - 1)
        );

        // TT cut-off for exact win/loss scores or for deep enough heuristic scores
        const Node* TT_entry = TT.find(p);
        if (TT_entry && (!TT_entry->is_heuristic() || TT_entry->is_sufficient(depth)) && TT_entry->is_cutoff(alpha, beta))
                return TT_entry->value();

        // generate move_list
        Move::List move_list;
        Generate<Rules, Board>::generate(p, move_list);

        // without a valid move, the position is an immediate loss
        if (!move_list.size()) {
                const int loss_score = Value::loss(0);

                // we can only have an upper bound or an exact value at this point
                assert(loss_score < beta);
                const Node::Type loss_type = (loss_score <= alpha)? Node::upper() : Node::exact();

                TT.insert(p, Node(loss_score, loss_type, depth, Node::no_move()));
                return loss_score;
        }

        // internal iterative deepening
        if (!(TT_entry && TT_entry->has_move())) {
                const int IID_depth = is_PV(ThisNode)? depth - 2 : depth / 2;
                if (IID_depth > 0) {
                        const int IID_value = search<ThisNode, Rules>(p, ply, IID_depth, alpha, beta, parent_node);
                        TT.insert(p, Node(IID_value, Node::exact(), IID_depth, parent_node.best_move()));
                        TT_entry = TT.find(p);
                        assert(TT_entry);
                }
        }

        // TT move ordering
        Move::Order move_order(move_list.size());
        identity_permutation(move_order);                
        if (TT_entry && TT_entry->has_move()) {
                const size_t TT_move = TT_entry->move() % move_list.size();
                std::swap(move_order[0], move_order[TT_move]);
        }

        // search move_list
        int value = -Value::infinity();
        size_t best_move = Node::no_move();
        int score;
        size_t i;
        Parameters child_node;
        const int original_alpha = alpha;

        Position<Board> q;
        for (size_t s = 0; s < move_order.size(); ++s) {
                i = move_order[s];
                // TODO: TT singular extension

                // TODO: futility pruning

                q.template copy_make<Rules>(p, move_list[i]);

                if (is_PV(ThisNode) && s == 0)
                        score = -squeeze(search<PV, Rules>(q, ply + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));
                else {
                        // TODO: late move reductions

                        score = -squeeze(search<ZW, Rules>(q, ply + 1, depth - 1, -stretch(alpha + 1), -stretch(alpha), child_node));
                        if (is_PV(ThisNode) && score > alpha && score < beta)
                                score = -squeeze(search<PV, Rules>(q, ply + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));
                }

                if (score > value) {
                        if (score >= beta) {
                                TT.insert(p, Node(score, Node::lower(), depth, i));
                                return score;
                        }
                        value = score;
                        best_move = i;
                        if (is_PV(ThisNode) && value > alpha) {
                                alpha = value;
                                parent_node.reset_PV(i, child_node.PV());
                        }
                }
        }

        // we must have found a best move with a finite value
        assert(value > -Value::infinity());
        assert(best_move != Node::no_move());

        // we can only have an upper bound or an exact value at this point
        const Node::Type value_type = (value <= original_alpha)? Node::upper() : Node::exact();
        TT.insert(p, Node(value, value_type, depth, best_move));
        return value;
}

/*
// principal variation search (PVS) with TT cut-offs, TT move ordering and IID
template<size_t Node, typename Rules, typename Board>
int Root::quiescence(const Position<Board>& p, size_t ply, int depth, int alpha, int beta, Parameters& parent_node)
{
        statistics_.update(ply);

        // check for a legal draw
        if (p.is_draw<Rules>())
                return Value::draw();

        // check for legal move_list
        if (!Generate::detect(p)) {
                return Value::loss(0);
        }

        // generate captures and promotions
        Move::List move_list;
        Generate::generate_captures_promotions(p, move_list);

        if (!move_list.size())
        {
                if (Generate::detect_pending_captures_promotions(p)) {

                } else {
                        // return eval
                }
        }

        // search generated move_list
}
*/

}       // namespace Search
}       // namespace Tree
