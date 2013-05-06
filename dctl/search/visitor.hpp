/*

// principal variation search (PVS)
template<typename Objective, typename Position, typename Enhancement>
template<int NodeType>
int search(Position const& p, std::pair<int, int> window, int depth, int ply, Variation& refutation, Enhancement& e)
{
        // GUI interrupt
        auto interrupt = e.interrupt(p);
        if (interrupt.first) return interrupt.second;

        // repetition, hash table, opening book, endgame database
        auto lookup = e.lookup(p);
        if (lookup.first) return lookup.second;

        // razoring, static null move pruning, null move, probcut
        auto prune_position = e.prune(p);
        if (prune_position.first) return prune_position.second;

        // generate moves
        MoveArena a;
        auto const moves = successor::copy(p, a);
        BOOST_ASSERT(!moves.empty());

        // hash move, IID, killer, history
        auto move_order = e.order(moves);

        // ETC, multi-cut
        auto prune_moves = e.prune(moves, move_order);
        if (prune_moves.first) return prune_moves.second;

        // search moves
        auto const original_alpha = alpha;
        auto best_value = -infinity();
        auto best_move = Transposition::no_move();
        int value;

        e.init_search(moves);

        for (auto const& i: move_order) {
                auto prune_move = e.prune(moves[i]);
                if (prune_move.first) continue;

                auto R = e.reduction(moves[i]);

                auto value = -e.search(successor::make_copy(p, moves[i]), window, depth, R);

                // update PV, hash table, history, killers
                auto cutoff = e.cutoff(moves[i], value, window);
                if (cutoff.first) break;
        }

        // we must have found a best move with a finite value
        BOOST_ASSERT(is_finite(best_value));
        BOOST_ASSERT(best_move != Transposition::no_move());

        auto const type = Bound::type(best_value, original_alpha, beta);
        TT.insert(p, { best_value, type, depth, best_move } );
        return best_value;
}
*/
