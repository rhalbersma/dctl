#include "Parameters.h"
#include "../Move/String.h"
#include "../Move/Types.h"
#include "../Generate/Successors.h"
#include "../../Evaluation/Evaluate.h"
#include "../../IO/FEN.h"
#include "../../Utilities/StopWatch.h"
#include <iostream>
#include <iomanip>

namespace Tree {
namespace Search {

// iterative deepening with no move ordering at the root
template<typename Rules, typename Board>
int Root::analyze(const Node::Position<Board>& p, size_t nominal_depth)
{
        int value = -Value::infinity();                
        int alpha, beta;
        Parameters root_node;
        StopWatch timer;

        timer.start();
        announce(p, nominal_depth);
        for (size_t depth = 1; depth <= nominal_depth; depth += ROOT_ID_INCREMENT) {
                statistics_.reset();
                alpha = -Value::infinity();
                beta = Value::infinity();
                value = search<PV, Rules>(p, 0, depth, alpha, beta, root_node);
                timer.split();
                report(depth, alpha, beta, value, timer);
                print_PV<Rules>(root_node, p, true);
                insert_PV<Rules>(root_node, p, value);
        }

        return value;
}

template<typename Board>
void Root::announce(const Node::Position<Board>& p, size_t nominal_depth)
{
        std::cout << std::endl;
        std::cout << write_position_layout<FEN_tag>()(p) << std::endl;
        std::cout << write_position_string<FEN_tag>()(p) << std::endl << std::endl;
        std::cout << "Searching to nominal depth=" << nominal_depth << std::endl;
        std::cout << std::endl;
}

template<typename Rules, typename Board>
void Root::insert_PV(const Parameters& node, const Node::Position<Board>& q, int value)
{
        const Move::Sequence& line = node.PV();
        Node::Position<Board> p(q);

        for (size_t i = 0; i < line.size(); ++i) {
                TT.insert(p, Entry(value, Entry::exact(), line.size() - i, line[i]));
                Move::Stack move_stack;
                Generate::Successors<Rules, Board>::generate(p, move_stack);
                p.template make<Rules>(move_stack[line[i]]);
                value = -Value::stretch(value);
        }
        TT.insert(p, Entry(value, Entry::exact(), 0, Entry::no_move()));
}

template<typename Rules, typename Board>
void Root::print_PV(const Parameters& node, const Node::Position<Board>& q, bool print_TT_info)
{
        const Move::Sequence& line = node.PV();
        Node::Position<Board> p(q);
        const Entry* TT_entry;
        int eval_score;
        size_t repeated_kings;
        size_t non_conversion;

        for (size_t i = 0; i < line.size(); ++i) {
                Move::Stack move_stack;
                Generate::Successors<Rules, Board>::generate(p, move_stack);
                assert(line[i] < move_stack.size());

                if (p.to_move())
                        std::cout << Move::String::write<Rules>()(p, move_stack[line[i]]);
                else {
                        std::cout << "(";
                        std::cout << Move::String::write<Rules>()(p, move_stack[line[i]]);
                        std::cout << ")";
                }

                p.template make<Rules>(move_stack[line[i]]);

                repeated_kings = p.repeated_moves(!p.to_move());
                ///*
                if (repeated_kings)
                        std::cout << "^" << std::setw(1) << repeated_kings;
                else
                        std::cout << "  ";
                //*/
                non_conversion = p.non_conversion();
                ///*
                if (non_conversion)
                        std::cout << "#" << std::setw(1) << non_conversion;
                else
                        std::cout << "  ";
                //*/
                TT_entry = TT.find(p);

                eval_score = Evaluate::evaluate(p);
                std::cout << " [";
                std::cout << std::setw(4) << Value::print(eval_score);

                if (print_TT_info && TT_entry) {
                        std::cout << ", ";
                        std::cout << std::setw(4) << (TT_entry->print_value());
                        std::cout << ", ";
                        std::cout << std::setw(2) << (TT_entry->print_type());
                        std::cout << ", ";
                        std::cout << std::setw(2) << (TT_entry->print_depth());
                        std::cout << ", ";
                        std::cout << std::setw(3) << (TT_entry->print_move());
                }
                std::cout << "] ";

                if ((i % 2 != 0))
                        std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
}

}       // namespace Search
}       // namespace Tree
