#pragma once
#include <cstddef>                      // std::size_t
#include <iostream>
#include <iomanip>
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Bound.hpp"
#include "Objective.hpp"
#include "Score.hpp"
#include "Transposition.hpp"
#include "Variation.hpp"
#include "../evaluate/Evaluate.hpp"
#include "../node/Position.hpp"
#include "../node/Stack.hpp"
#include "../hash/Map.hpp"
#include "../successor/Successor.hpp"
#include "../utility/Ply.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/Statistics.hpp"
#include "../utility/Timer.hpp"
#include "../setup/Diagram.hpp"
#include "../setup/String.hpp"
#include "../notation/String.hpp"

namespace dctl {
namespace search {

template
<
        typename Rules,
        typename Board,
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

        int analyze(const Position<Board>& p, int depth)
        {
                return iterative_deepening(p, depth);
        }

        int solve(const Position<Board>& p, int depth)
        {
                return proof_verify(p, depth);
        }

        void resize_hash(std::size_t s)
        {
                return TT.resize(s);
        }

        void clear_hash()
        {
                return TT.clear();
        }

        void interrupt()
        {
                interrupted_ = true;
        }

        bool is_interrupted()
        {
                return interrupted_;
        }

private:
        int iterative_deepening(const Position<Board>&, int);        
        int proof_verify(const Position<Board>&, int);        
        int negamax(const Position<Board>&, int, int, Variation&);      
        template<typename int> int verify(const Position<Board>&, int, int, int, int, Variation&);
        template<typename int> int pvs(const Position<Board>&, int, int, int, int, Variation&);

        void announce(const Position<Board>& p, int depth)
        {
                std::cout << setup::diagram<pdn::protocol>()(p);
                std::cout << setup::write<pdn::protocol>()(p) << "\n";
                std::cout << "Searching to nominal depth=" << depth << "\n\n";
        }
        
        void report(int depth, int value, const Timer& timer, const Position<Board>& p, const Sequence& pv)
        {
                std::cout << "info";

                std::cout << " score ";
                std::cout << std::setw( 3) << std::right << print(value);

                std::cout << " depth ";
                std::cout << std::setw( 2) << depth;

                std::cout << " nodes ";
                std::cout << std::setw(11) << std::right << statistics_.nodes();

                std::cout << " time ";
                std::cout << std::setw( 6) << timer.elapsed();

                const double nps = (1000 * statistics_.nodes()) / static_cast<double>(timer.elapsed());
                std::cout << " nps ";
                std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
                std::cout << std::setw( 7) << nps;

                const double hashfull = 1000 * (static_cast<double>((TT.size() - TT.available())) / TT.size());
                std::cout << " hashfull ";
                std::cout << std::setw( 4) << std::right << hashfull;

                std::cout << "\n";
                print_pv(p, pv);
        }

        void insert_pv(const Position<Board>& p, const Sequence& pv, int value, int ply = 0)
        {
                const auto depth = static_cast<int>(pv.size()) - ply;
                if (depth == 0) {
                        BOOST_ASSERT(
                                (value == Evaluate<Rules>::evaluate(p)) ||
                                (value == draw_value() && is_draw<Rules>(p)) ||
                                (value == loss_min() && !Successor<select::Legal, Rules>::detect(p))
                                // NOTE: with endgame databases, delayed losses can occur at the tips of the pv
                        );
                        TT.insert(p, Transposition(value, Bound::exact, depth, Transposition::no_move()));
                        return;
                }

                Stack moves;
                Successor<select::Legal, Rules>::generate(p, moves);
                const auto index = pv[ply] % moves.size();
                const auto best_move = moves[index];
                TT.insert(p, Transposition(value, Bound::exact, depth, index));

                auto q = p;
                q.attach(p);
                q.template make<Rules>(best_move);
                insert_pv(q, pv, -stretch(value), ply + 1);
        }
        
        void print_pv(const Position<Board>& p, const Sequence& pv, int ply = 0)
        {
                const auto depth = static_cast<int>(pv.size()) - ply;
                if (depth == 0) {
                        std::cout << '\n';
                        std::cout << setup::diagram<pdn::protocol>()(p);
                        return;
                }

                Stack moves;
                Successor<select::Legal, Rules>::generate(p, moves);
                const auto best_move = moves[pv[ply] % moves.size()];

                if (!(ply % 2)) std::cout << std::setw(2) << std::right << ((ply / 2) + 1) << ". ";
                std::cout << notation::write<Rules>()(p, best_move);
                std::cout << ((ply % 10 == 9)? '\n' : ' ');

                auto q = p;
                q.attach(p);
                q.template make<Rules>(best_move);
                //if (q.same_king_moves(!q.to_move()))
                        //std::cout << "^" << q.same_king_moves(!q.to_move());
                print_pv(q, pv, ply + 1);
        }

        bool is_pv(int node)
        {
                return node == PV;
        }

        // implementation
        BOOST_STATIC_CONSTANT(auto, ROOT_ID_INCREMENT = 1);

        // 8-byte hash entries: 32-bit hash signature, 4-byte {value, type, depth, move} content
        // 8-way buckets on 64-byte cache lines, (1 Gb = 2^27 entries) 
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef hash::Map<uint32_t, Transposition> TranspositionTable;
        TranspositionTable TT;

        // representation
        Statistics statistics_;
        bool interrupted_;
};

}       // namespace search
}       // namespace dctl

// include template definitions inside header
#include "Algorithms.hpp"
