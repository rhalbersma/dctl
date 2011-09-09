#pragma once
#include <tuple>

namespace dctl {
namespace board {

template
<
        template<typename, typename, int> class, 
        typename Board, 
        typename, 
        int = Board::ExternalGrid::size - 1
>
struct init_predicate;

template<typename>
struct init_squares;

template<typename Board, bool>
struct init_initial;

template<typename Board, bool, int>
struct init_row_mask;

template<typename Board, bool, int>
struct init_col_mask;

template<typename Board, int>
struct init_man_jump_group;

template<typename Board, int>
struct init_jumpable;

}       // namespace board
}       // namespace dctl

// include template definitions inside header
#include "MetaTemplates.hpp"
