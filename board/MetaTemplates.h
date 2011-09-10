#pragma once
#include <tuple>

namespace dctl {
namespace board {

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename Tuple, 
        int SQ
>
struct test_predicate;

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename Tuple, 
        int SQ = Board::ExternalGrid::size - 1
>
struct init_predicate;

template<typename Board>
struct init_squares;

template<typename Board, bool Color>
struct init_initial;

template<typename Board, bool Color, int Row>
struct init_row_mask;

template<typename Board, bool Color, int Column>
struct init_col_mask;

template<typename Board, int Group>
struct init_jump_group;

template<typename Board, int Index>
struct init_jump_start;

}       // namespace board
}       // namespace dctl

// include template definitions inside header
#include "MetaTemplates.hpp"
