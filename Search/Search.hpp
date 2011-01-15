#include "SearchParameters.h"
#include "../Generate/Propagate.h"
#include "../Generate/Generate.h"
#include "../Evaluation/Evaluate.h"
#include "../IO/PositionIO.h"
#include "../IO/MoveIO.h"
#include "../Utilities/StopWatch.h"
#include <iostream>
#include <iomanip>

// iterative deepening with no move ordering at the root
template<typename Rules, typename Board>
int Search::root(const Position<Board>& p, size_t nominal_depth)
{
        int value = -SearchValue::infinity();                
        int alpha, beta;
        SearchParameters root_node;
        StopWatch timer;

        timer.start();
        announce(p, nominal_depth);
        for (size_t depth = 1; depth <= nominal_depth; depth += ROOT_ID_INCREMENT) {
                reset_statistics();
                alpha = -SearchValue::infinity();
                beta = SearchValue::infinity();
                value = search<PV, Rules>(p, 0, depth, alpha, beta, root_node);
                timer.split();
                report(depth, alpha, beta, value, timer);
                print_PV<Rules>(root_node, p, true);
                insert_PV<Rules>(root_node, p, value);
        }

        assert(p == pp);
        return value;
}

template<typename Board>
void Search::announce(const Position<Board>& p, size_t nominal_depth)
{
        std::cout << std::endl;
        std::cout << write_position_layout<FEN>()(p) << std::endl;
        std::cout << write_position_string<FEN>()(p) << std::endl << std::endl;
        std::cout << "Searching to nominal depth=" << nominal_depth << std::endl;
        std::cout << std::endl;
}

template<typename Rules, typename Board>
void Search::insert_PV(const SearchParameters& node, const Position<Board>& q, int value)
{
        const Move::Sequence& line = node.PV();
        Position<Board> p(q);

        for (size_t i = 0; i < line.size(); ++i) {
                TT.insert(p, SearchNode(value, SearchNode::exact(), line.size() - i, line[i]));
                Propagate<Rules, Board> moves(p);
                Generate::generate<Rules>(p, moves);
                p.template make<Rules>(moves[line[i]]);
                value = -SearchValue::stretch(value);
        }
        TT.insert(p, SearchNode(value, SearchNode::exact(), 0, SearchNode::no_move()));
}

template<typename Rules, typename Board>
void Search::print_PV(const SearchParameters& node, const Position<Board>& q, bool print_TT_info)
{
        const Move::Sequence& line = node.PV();
        Position<Board> p(q);
        const SearchNode* TT_entry;
        int eval_score;
        size_t repeated_kings;
        size_t non_conversion;

        for (size_t i = 0; i < line.size(); ++i) {
                Propagate<Rules, Board> moves(p);
                Generate::generate<Rules>(p, moves);
                assert(line[i] < moves.size());

                if (p.to_move())
                        std::cout << write_move_string<Rules>()(p, moves[line[i]]);
                else {
                        std::cout << "(";
                        std::cout << write_move_string<Rules>()(p, moves[line[i]]);
                        std::cout << ")";
                }

                p.template make<Rules>(moves[line[i]]);

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
                std::cout << std::setw(4) << SearchValue::print(eval_score);

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
