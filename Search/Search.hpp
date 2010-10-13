#include "SearchParameters.h"
#include "../Generate/Propagate.h"
#include "../Generate/Generate.h"
#include "../Evaluation/Evaluate.h"
#include "../IO/PositionIO.h"
#include "../IO/MoveIO.h"
#include <ctime>
#include <iostream>
#include <iomanip>

// iterative deepening with no move ordering at the root
template<typename Rules, typename Board>
void Search::root(Position<Board>& p, size_t nominal_depth)
{
        double start_time, time_used;
        int value, alpha, beta;
        SearchParameters root_node;

        announce(p, nominal_depth);
        for (size_t depth = 1; depth <= nominal_depth; depth += ROOT_ID_INCREMENT) {
                start_time = clock();
                reset_statistics();
                alpha = -SearchValue::infinity();
                beta = SearchValue::infinity();
                value = search<PV, Rules>(p, 0, depth, alpha, beta, root_node);
                time_used = (clock() + 1 - start_time) / CLOCKS_PER_SEC;
                report(value, depth, alpha, beta, time_used);
                print_PV<Rules>(root_node, p, true);
                insert_PV<Rules>(root_node, p, value);
        }
}

template<typename Board>
void Search::announce(const Position<Board>& p, size_t nominal_depth)
{
        std::cout << std::endl;
        std::cout << write_position_stream<FEN>()(p) << std::endl;
        std::cout << write_position_string<FEN>()(p) << std::endl << std::endl;
        std::cout << "Searching to nominal depth=" << nominal_depth << std::endl;
        std::cout << std::endl;
}

template<typename Rules, typename Board>
void Search::insert_PV(const SearchParameters& node, const Position<Board>& p, int value)
{
        const Move::Sequence& line = node.PV();
        Position<Board> pp(p);

        for(size_t i = 0; i < line.size(); ++i) {
                TT.insert(pp, SearchNode(value, SearchNode::exact(), line.size() - i, line[i]));
                Propagate<Rules, Board> moves(pp);
                Generate::generate<Rules>(pp, moves);
                pp.template make<Rules>(moves[line[i]]);
                value = -SearchValue::stretch(value);
        }
        TT.insert(pp, SearchNode(value, SearchNode::exact(), 0, SearchNode::no_move()));
}

template<typename Rules, typename Board>
void Search::print_PV(const SearchParameters& node, const Position<Board>& p, bool print_TT_info)
{
        const Move::Sequence& line = node.PV();
        Position<Board> pp(p);
        const SearchNode* TT_entry;
        int eval_score;
        size_t same_king;
        size_t non_conversion;

        for(size_t i = 0; i < line.size(); ++i) {
                Propagate<Rules, Board> moves(pp);
                Generate::generate<Rules>(pp, moves);
                assert(line[i] < moves.size());

                if (pp.to_move())
                        std::cout << write_move_string<Rules>()(pp, moves[line[i]]);
                else {
                        std::cout << "(";
                        std::cout << write_move_string<Rules>()(pp, moves[line[i]]);
                        std::cout << ")";
                }

                pp.template make<Rules>(moves[line[i]]);

                same_king = pp.same_moves(!pp.to_move());
                ///*
                if (same_king)
                        std::cout << "^" << std::setw(1) << same_king;
                else
                        std::cout << "  ";
                //*/
                non_conversion = pp.non_conversion_moves();
                /*
                if (non_conversion)
                        std::cout << "#" << std::setw(1) << non_conversion;
                else
                        std::cout << "  ";
                */
                TT_entry = TT.find(pp);

                eval_score = Evaluate::evaluate(pp);
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
