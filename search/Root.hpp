#pragma once
#include <cstddef>                      // std::size_t
#include <iostream>
#include <iomanip>
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Transposition.hpp"
#include "Score.hpp"
#include "Parameters.hpp"
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
        typename Board
>
class Root
{
public:
        // typdefs
        enum EntryType { ZW, PV };

        Root()
        :
                TT(0),
                interrupted_(false)
        {
        }

        int analyze(const Position<Board>& p, int depth)
        {
                return iterative_deepening(p, depth);
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
        int negamax(const Position<Board>&, int, int, Parameters&);      
        int alpha_beta(const Position<Board>&, int, int, int, int, Parameters&);
        template<typename int> int pvs(const Position<Board>&, int, int, int, int, Parameters&);
        template<typename int> int quiescence(const Position<Board>&, int, int, int, int, Parameters&);

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

                const double nps = (1000 * statistics_.nodes()) / static_cast<double>(timer.lap());
                std::cout << " nps ";
                std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
                std::cout << std::setw( 7) << nps;

                const double hashfull = 1000 * (static_cast<double>((TT.size() - TT.available())) / TT.size());
                std::cout << " hashfull ";
                std::cout << std::setw( 4) << std::right << hashfull;

                std::cout << "\n";
                print_pv(p, pv);
        }

        void insert_pv(const Position<Board>& p, const Sequence& pv, int value)
        {
                auto q = p;

                for (auto i = 0; i < static_cast<int>(pv.size()); ++i) {
                        Stack move_stack;
                        Successor<successor::Legal, Rules>::generate(q, move_stack);

                        TT.insert(q, Transposition(value, Transposition::exact_value, pv.size() - i, pv[i]));
                        value = -stretch(value);

                        q.template make<Rules>(move_stack[pv[i]]);
                }
                TT.insert(q, Transposition(value, Transposition::exact_value, 0, Transposition::no_move()));
        
                BOOST_ASSERT(
                        (value == Evaluate<Rules>::evaluate(q)) || 
                        (value == loss_value(0) && !Successor<successor::Legal, Rules>::detect(q))
                        // NOTE: with endgame databases, delayed losses can occur at the tips of the pv
                );
        }
        
        void print_pv(const Position<Board>& p, const Sequence& pv)
        {
                auto q = p;

                for (auto i = 0; i < static_cast<int>(pv.size()); ++i) {
                        Stack moves;
                        Successor<successor::Legal, Rules>::generate(q, moves);

                        if (!(i % 2))                        
                                std::cout << std::setw(2) << std::right << ((i / 2) + 1) << ". ";
                        std::cout << notation::write<Rules>()(q, moves[pv[i]]);
                        q.template make<Rules>(moves[pv[i]]);
                        //if (q.same_king_moves(!q.to_move()))
                                //std::cout << "^" << q.same_king_moves(!q.to_move());
                        if (i % 10 == 9)
                                std::cout << "\n";
                        else
                                std::cout << " ";

                }
                std::cout << "\n\n";
        }

        bool is_pv(int node)
        {
                return node == PV;
        }

        // implementation
        BOOST_STATIC_CONSTANT(auto, ROOT_ID_INCREMENT = 2);

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
