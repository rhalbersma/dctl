#include "Parameters.h"
#include "../Generate/Successors.h"
#include "../Move/String.h"
#include "../Move/Types.h"
#include "../Node/Layout.h"
#include "../Node/Position.h"
#include "../Node/String.h"
#include "../../Evaluation/Evaluate.h"
#include "../../Utilities/Timer.h"
#include <iostream>
#include <iomanip>

namespace Tree {
namespace Search {

// iterative deepening with no move ordering at the root
template<typename Rules, typename Board>
int Root::analyze(const Node::Position<Board>& p, size_t nominal_depth)
{
        return iterative_deepening<Rules, Board>(p, nominal_depth);
}

template<typename Board>
void Root::announce(const Node::Position<Board>& p, size_t nominal_depth)
{
        std::cout << std::endl;
        std::cout << Node::Layout::write<Node::FEN_tag>()(p) << std::endl;
        std::cout << Node::String::write<Node::FEN_tag>()(p) << std::endl << std::endl;
        std::cout << "Searching to nominal depth=" << nominal_depth << std::endl;
        std::cout << std::endl;
}

template<typename Rules, typename Board>
void Root::insert_PV(const Node::Position<Board>& p, const Move::Sequence& pv, int value)
{
        Node::Position<Board> q(p);

        for (size_t i = 0; i < pv.size(); ++i) {
                Move::Stack move_stack;
                Generate::Successors<Rules, Board>::generate(q, move_stack);

                TT.insert(q, Entry(value, Entry::exact(), pv.size() - i, pv[i]));
                value = -Value::stretch(value);

                q.template make<Rules>(move_stack[pv[i]]);
        }
        TT.insert(q, Entry(value, Entry::exact(), 0, Entry::no_move()));
        
        assert(
                (value == Evaluate::evaluate(q)) || 
                (value == Value::loss(0) && !Generate::Successors<Rules, Board>::detect(q))
                // NOTE: with endgame databases, delayed losses can occur at the tips of the PV
        );
}

template<typename Rules, typename Board>
void Root::print_PV(const Node::Position<Board>& p, const Move::Sequence& pv)
{
        Node::Position<Board> q(p);

        for (size_t i = 0; i < pv.size(); ++i) {
                Move::Stack move_stack;
                Generate::Successors<Rules, Board>::generate(q, move_stack);

                if (!(i % 2))                        
                        std::cout << std::setw(2) << std::right << ((i / 2) + 1) << ". ";
                std::cout << Move::String::write<Rules>()(q, move_stack[pv[i]]) << " ";
                if (i % 2)
                        std::cout << std::endl;

                q.template make<Rules>(move_stack[pv[i]]);
        }
        std::cout << std::endl << std::endl;
}

}       // namespace Search
}       // namespace Tree
