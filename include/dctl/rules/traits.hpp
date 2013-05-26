#pragma once

#include <dctl/rules/traits/range_king.hpp>
#include <dctl/rules/traits/directions_pawn_jump.hpp>
#include <dctl/rules/traits/precedence_jump.hpp>

#include <dctl/rules/traits/range_land.hpp>
#include <dctl/rules/traits/range_halt.hpp>
#include <dctl/rules/traits/orthogonality_king_jump.hpp>
#include <dctl/rules/traits/orthogonality_king_move.hpp>
#include <dctl/rules/traits/is_pawns_jump_kings.hpp>
#include <dctl/rules/traits/is_jump_direction_reversal.hpp>
#include <dctl/rules/traits/phase_capture.hpp>
#include <dctl/rules/traits/phase_promotion.hpp>
#include <dctl/rules/traits/is_absolute_king_precedence.hpp>
#include <dctl/rules/traits/is_relative_king_precedence.hpp>
#include <dctl/rules/traits/initial_gap.hpp>
#include <dctl/rules/traits/max_reversible_moves.hpp>
#include <dctl/rules/traits/max_same_king_moves.hpp>

#include <dctl/rules/traits/directions_king_jump.hpp>
#include <dctl/rules/traits/directions_king_turn.hpp>
#include <dctl/rules/traits/directions_pawn_turn.hpp>
#include <dctl/rules/traits/is_unambiguous_pawn_jump.hpp>
#include <dctl/rules/traits/large_jump.hpp>
#include <dctl/rules/traits/is_precedence.hpp>
#include <dctl/rules/traits/is_restricted_reversible_moves.hpp>
#include <dctl/rules/traits/is_restricted_same_king_moves.hpp>

#include <dctl/rules/traits/draw.hpp>
#include <dctl/rules/traits/duplicates.hpp>
