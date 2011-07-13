#pragma once

namespace board {

template<typename Board, int = Board::ExternalGrid::SIZE- 1>
class init_squares;

template<typename Board, bool, int = Board::ExternalGrid::SIZE- 1>
class init_initial;

template<typename Board, bool, int, int = Board::ExternalGrid::SIZE- 1>
class init_row_mask;

template<typename Board, bool, int, int = Board::ExternalGrid::SIZE- 1>
class init_col_mask;

template<typename Board, int, int = Board::ExternalGrid::SIZE- 1>
class init_man_jump_group;

template<typename Board, int, int = Board::ExternalGrid::SIZE- 1>
class init_jumpable;

}       // namespace board

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "MetaTemplates.hpp"
