#pragma once
#include <cstddef>                      // size_t
#include <iostream>
#include <iomanip>
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <dctl/search/bound.hpp>
#include <dctl/search/objective.hpp>
#include <dctl/search/score.hpp>
#include <dctl/search/transposition.hpp>
#include <dctl/search/variation.hpp>
#include <dctl/evaluate/score.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/hash/signature_extractor.hpp>
#include <dctl/hash/map.hpp>
#include <dctl/hash/replace.hpp>
#include <dctl/successor/generate.hpp>
#include <dctl/utility/ply.hpp>
#include <dctl/utility/int.hpp>
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

        void resize_hash(std::size_t s)
        {
                TT.resize(s);
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
        int iterative_deepening(Position const&, int);
        int negamax(Position const&, int, int, Variation&);
        template<int> int pvs(Position const&, int, int, int, int, Variation&);

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

                std::cout << " nodes ";
                std::cout << std::setw(11) << std::right << statistics_.nodes();

                std::cout << " time ";
                std::cout << std::setw( 6) << timer.elapsed().count();

                double const nps = static_cast<double>(1000 * statistics_.nodes()) / static_cast<double>(timer.elapsed().count());
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

                MoveArena a;
                auto const moves = successor::generate(p, a);
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

                MoveArena a;
                auto const moves = successor::generate(p, a);
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

// include template definitions inside header
#include <dctl/search/algorithm.hpp>
