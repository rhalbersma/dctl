#include <iostream>
#include <iomanip>
#include "Parameters.h"
#include "../Generate/Successors.h"
#include "../Move/String.h"
#include "../Node/Layout.h"
#include "../Node/Position.h"
#include "../Node/Stack.h"
#include "../Node/String.h"
#include "../../Evaluation/Evaluate.h"
#include "../../Utilities/Timer.h"

namespace tree {
namespace search {

// iterative deepening with no move ordering at the root
template<typename Rules, typename Board>
int Root::analyze(const node::Position<Board>& p, int depth)
{
        return iterative_deepening<Rules, Board>(p, depth);
}

template<typename Board>
void Root::announce(const node::Position<Board>& p, int depth)
{
        std::cout << std::endl;
        std::cout << node::layout::write<node::FEN_tag>()(p) << std::endl;
        std::cout << node::string::write<node::FEN_tag>()(p) << std::endl << std::endl;
        std::cout << "Searching to nominal depth=" << depth << std::endl;
        std::cout << std::endl;
}

template<typename Rules, typename Board>
void Root::insert_PV(const node::Position<Board>& p, const node::Sequence& pv, int value)
{
        auto q(p);

        for (size_t i = 0; i < pv.size(); ++i) {
                node::Stack move_stack;
                generate::Successors<Rules, Board>::generate(q, &move_stack);

                TT.insert(q, Entry(value, Entry::exact(), pv.size() - i, pv[i]));
                value = -score::stretch(value);

                q.template make<Rules>(move_stack[pv[i]]);
        }
        TT.insert(q, Entry(value, Entry::exact(), 0, Entry::no_move()));
        
        assert(
                (value == Evaluate::evaluate(q)) || 
                (value == score::loss_value(0) && !generate::Successors<Rules, Board>::detect(q))
                // NOTE: with endgame databases, delayed losses can occur at the tips of the PV
        );
}

template<typename Rules, typename Board>
void Root::print_PV(const node::Position<Board>& p, const node::Sequence& pv)
{
        auto q(p);

        for (size_t i = 0; i < pv.size(); ++i) {
                node::Stack move_stack;
                generate::Successors<Rules, Board>::generate(q, &move_stack);

                if (!(i % 2))                        
                        std::cout << std::setw(2) << std::right << ((i / 2) + 1) << ". ";
                std::cout << move::string::write<Rules>()(q, move_stack[pv[i]]);
                if (i % 10 == 9)
                        std::cout << std::endl;
                else
                        std::cout << " ";

                q.template make<Rules>(move_stack[pv[i]]);
        }
        std::cout << std::endl << std::endl;
}

}       // namespace search
}       // namespace tree
