#pragma once

#include <dctl/rules/regular/is_backward_pawn_jump.hpp>
#include <dctl/rules/regular/is_long_ranged_king.hpp>
#include <dctl/rules/regular/is_jump_precedence.hpp>

#include <dctl/rules/irregular/initial_gap.hpp>
#include <dctl/rules/irregular/is_absolute_king_jump_precedence.hpp>
#include <dctl/rules/irregular/is_directly_land_after_piece.hpp>
#include <dctl/rules/irregular/is_directly_halt_after_final_king.hpp>
#include <dctl/rules/irregular/is_en_passant_jump_removal.hpp>
#include <dctl/rules/irregular/is_en_passant_promotion.hpp>
#include <dctl/rules/irregular/is_orthogonal_jump.hpp>
#include <dctl/rules/irregular/is_pawn_jump_king.hpp>

#include <dctl/rules/composite/is_long_ranged_land_after_piece.hpp>
#include <dctl/rules/composite/is_reversible_king_jump_direction.hpp>
#include <dctl/rules/composite/is_unambiguous_pawn_jump.hpp>
#include <dctl/rules/composite/large_jump.hpp>

#include <dctl/rules/traits/max_reversible_moves.hpp>
#include <dctl/rules/traits/max_same_king_moves.hpp>

#include <dctl/rules/traits/is_restricted_reversible_moves.hpp>
#include <dctl/rules/traits/is_restricted_same_king_moves.hpp>

#include <dctl/rules/traits/draw.hpp>
#include <dctl/rules/traits/duplicates.hpp>
