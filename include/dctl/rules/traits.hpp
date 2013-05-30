#pragma once

#include <dctl/rules/traits/range/scan.hpp>
#include <dctl/rules/traits/directions/pawn_jump.hpp>
#include <dctl/rules/traits/precedence/jump.hpp>

#include <dctl/rules/traits/range/land.hpp>
#include <dctl/rules/traits/range/halt.hpp>
#include <dctl/rules/traits/orthogonality/king_jump.hpp>
#include <dctl/rules/traits/orthogonality/king_move.hpp>
#include <dctl/rules/traits/is_pawns_jump_kings.hpp>
#include <dctl/rules/traits/directions/is_reversal.hpp>
#include <dctl/rules/traits/phase/capture.hpp>
#include <dctl/rules/traits/phase/promotion.hpp>
#include <dctl/rules/traits/precedence/is_absolute_king.hpp>
#include <dctl/rules/traits/precedence/is_relative_king.hpp>
#include <dctl/rules/traits/initial_gap.hpp>
#include <dctl/rules/traits/max_reversible_moves.hpp>
#include <dctl/rules/traits/max_same_king_moves.hpp>

#include <dctl/rules/traits/directions/king_jump.hpp>
#include <dctl/rules/traits/directions/king_turn.hpp>
#include <dctl/rules/traits/directions/pawn_turn.hpp>
#include <dctl/rules/traits/is_unambiguous_pawn_jump.hpp>
#include <dctl/rules/traits/large_jump.hpp>
#include <dctl/rules/traits/is_precedence.hpp>
#include <dctl/rules/traits/is_restricted_reversible_moves.hpp>
#include <dctl/rules/traits/is_restricted_same_king_moves.hpp>

#include <dctl/rules/traits/draw.hpp>
#include <dctl/rules/traits/duplicates.hpp>
