#include <cassert>
/*
// negamax
template<typename Rules, typename Board>
int Search::negamax(Position<Board>& p, size_t ply, size_t depth, SearchParameters& parent_node)
{
        update_statistics(ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return SearchValue::draw();

        // return evaluation in leaf nodes with valid moves
        if (depth == 0)
                return !Generate::detect<Rules>(p)? SearchValue::loss(0) : Evaluate::evaluate(p);

        // generate moves
        Propagate<Rules, Board> moves(p);
        Generate::generate(p, moves);

        // search moves
        int value = -SearchValue::infinity();
        int score;
        SearchParameters child_node;
        for (size_t i = 0; i < moves.size(); ++i) {
                p.template make<Rules>(moves[i]);
                score = -squeeze(negamax<Rules>(p, ply + 1, depth - 1, child_node));
                //p.template undo<Rules>(moves[i]);

                if (score > value) {
                        value = score;
                        parent_node.reset_PV(i, child_node.PV());
                }
        }

        // without a valid move, the position is an immediate loss
        return std::max(SearchValue::loss(0), value);
}

// alpha-beta
template<typename Rules, typename Board>
int Search::alpha_beta(Position<Board>& p, size_t ply, size_t depth, int alpha, int beta, SearchParameters& parent_node)
{
        update_statistics(ply);

        // check for a legal draw
        if (p.template is_draw<Rules>())
                return SearchValue::draw();

        // mate distance pruning
        if (alpha >= SearchValue::win(1))
                return alpha;
        if (beta <= SearchValue::loss(0))
                return beta;

        // return evaluation in leaf nodes with valid moves
        if (depth == 0)
                return !Generate::detect<Rules>(p)? SearchValue::loss(0) : Evaluate::evaluate(p);

        // generate moves
        Propagate<Rules, Board> moves(p);
        Generate::generate(p, moves);

        // search moves
        int value = -SearchValue::infinity();
        int score;
        SearchParameters child_node;
        for (size_t i = 0; i < moves.size(); ++i) {
                p.template make<Rules>(moves[i]);
                score = -squeeze(alpha_beta<Rules>(p, ply  + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));
                //p.template undo<Rules>(moves[i]);

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
        return std::max(SearchValue::loss(0), value);
}
*/
// principal variation search (PVS) with TT cut-offs, TT move ordering and IID
template<size_t Node, typename Rules, typename Board>
int Search::search(Position<Board>& p, size_t ply, int depth, int alpha, int beta, SearchParameters& parent_node)
{
        update_statistics(ply);
        
        assert(p.non_conversion() <= ply);

        ///*
        // check for a legal draw
        if (p.template is_draw<Rules>())
                return SearchValue::draw();       
        //*/

        // return evaluation in leaf nodes with valid moves
        if (depth <= 0)
                return !Generate::detect<Rules>(p)? SearchValue::loss(0) : Evaluate::evaluate(p);

        assert(depth > 0);
        assert(alpha >= -SearchValue::infinity());
        assert(beta <= SearchValue::infinity());

        // mate distance pruning
        if (alpha >= SearchValue::win(1))
                return alpha;
        if (beta <= SearchValue::loss(0))
                return beta;

        assert(
                ( is_PV(Node) && alpha <  beta - 1) ||
                (!is_PV(Node) && alpha == beta - 1)
        );

        // TT cut-off for exact win/loss scores or for deep enough heuristic scores
        const SearchNode* TT_entry = TT.find(p);
        if (TT_entry && (!TT_entry->is_heuristic() || TT_entry->is_depth_greater_equal(depth)) && TT_entry->is_cutoff(alpha, beta))
                return TT_entry->value();

        // generate moves
        Propagate<Rules, Board> moves(p);
        Generate::generate(p, moves);

        // without a valid move, the position is an immediate loss
        if (!moves.size()) {
                const int loss_score = SearchValue::loss(0);

                // we can only have an upper bound or an exact value at this point
                assert(loss_score < beta);
                const SearchNode::Type loss_type = (loss_score <= alpha)? SearchNode::upper() : SearchNode::exact();

                TT.insert(p, SearchNode(loss_score, loss_type, depth, SearchNode::no_move()));
                return loss_score;
        }

        /*
        // internal iterative deepening
        if (!(TT_entry && TT_entry->has_move())) {
                const int IID_depth = is_PV(Node)? depth - 2 : depth / 2;
                if (IID_depth > 0) {
                        const int IID_value = search<Node, Rules>(p, ply, IID_depth, alpha, beta, parent_node);
                        TT.insert(p, SearchNode(IID_value, SearchNode::exact(), IID_depth, parent_node.best_move()));
                        TT_entry = TT.find(p);
                        assert(TT_entry);
                }
        }
        */

        // TT move ordering
        Move::Order move_order(moves.size());
        identity_permutation(move_order);                
        if (TT_entry && TT_entry->has_move()) {
                const size_t TT_move = TT_entry->move() % moves.size();
                std::swap(move_order[0], move_order[TT_move]);
        }

        // search moves
        int value = -SearchValue::infinity();
        size_t best_move = SearchNode::no_move();
        int score;
        size_t i;
        SearchParameters child_node;
        const int original_alpha = alpha;

        Position<Board> copy_p(p);
        for (size_t s = 0; s < move_order.size(); ++s) {
                i = move_order[s];
                // TODO: TT singular extension

                // TODO: futility pruning

                copy_p = p;
                copy_p.link(p);
                copy_p.template make<Rules>(moves[i]);

                if (is_PV(Node) && s == 0)
                        score = -squeeze(search<PV, Rules>(copy_p, ply + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));
                else {
                        // TODO: late move reductions

                        score = -squeeze(search<ZW, Rules>(copy_p, ply + 1, depth - 1, -stretch(alpha + 1), -stretch(alpha), child_node));
                        if (is_PV(Node) && score > alpha && score < beta)
                                score = -squeeze(search<PV, Rules>(copy_p, ply + 1, depth - 1, -stretch(beta), -stretch(alpha), child_node));
                }
                //p.template undo<Rules>(moves[i]);

                if (score > value) {
                        if (score >= beta) {
                                TT.insert(p, SearchNode(score, SearchNode::lower(), depth, i));
                                return score;
                        }
                        value = score;
                        best_move = i;
                        if (is_PV(Node) && value > alpha) {
                                alpha = value;
                                parent_node.reset_PV(i, child_node.PV());
                        }
                }
        }

        // we must have found a best move with a finite value
        assert(value > -SearchValue::infinity());
        assert(best_move != SearchNode::no_move());

        // we can only have an upper bound or an exact value at this point
        const SearchNode::Type value_type = (value <= original_alpha)? SearchNode::upper() : SearchNode::exact();
        TT.insert(p, SearchNode(value, value_type, depth, best_move));
        return value;
}

/*
// principal variation search (PVS) with TT cut-offs, TT move ordering and IID
template<size_t Node, typename Rules, typename Board>
int Search::quiescence(Position<Board>& p, size_t ply, int depth, int alpha, int beta, SearchParameters& parent_node)
{
        update_statistics(ply);

        // check for a legal draw
        if (p.is_draw<Rules>())
                return SearchValue::draw();

        // check for legal moves
        if (!Generate::detect(p)) {
                return SearchValue::loss(0);
        }

        // generate captures and promotions
        Propagate<Rules, Board> moves(p);
        Generate::generate_captures_promotions(p, moves);

        if (!moves.size())
        {
                if (Generate::detect_pending_captures_promotions(p)) {

                } else {
                        // return eval
                }
        }

        // search generated moves
}
*/