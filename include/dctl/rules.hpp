#pragma once
#include <tuple>        // make_tuple

namespace dctl {
namespace rules {

// http://www.usacheckers.com/rulesofcheckers.php
struct Checkers
{
        // THE DRAUGHTBOARD AND ITS ARRANGEMENT
        static constexpr auto is_algebraic_notation = false;    // 1.5

        // DRAUGHT PIECES AND THEIR ARRANGEMENT
        static constexpr auto initial_gap = 2;                  // 1.11

        // Ordinary Move Of A King
        static constexpr auto is_long_ranged_king = false;      // 1.17

        // Capturing Move Of A Man
        static constexpr auto is_backward_pawn_jump = false;    // 1.18
        static constexpr auto is_pawns_jump_only_pawns = false; // 1.18

        // Capturing In General
        static constexpr auto is_passing_promotion = false;     // 1.19
        static constexpr auto is_passing_capture = false;       // 1.19
        static constexpr auto is_quantity_precedence = false;   // 1.20

        // Definition of a Draw
        static constexpr auto max_repetitions = 3;              // 1.32.1
        static constexpr auto max_reversible_moves = 80;        // 1.32.2
};

// http://www.damweb.cz/pravidla/cdfull.html
struct Czech
{
        static constexpr auto is_algebraic_notation = true;     // 2.6
        static constexpr auto jumpsep = ':';

        static constexpr auto is_long_ranged_king = true;       // 3.9
        static constexpr auto is_backward_pawn_jump = false;    // 4.1
        static constexpr auto is_modality_precedence = true;    // 4.14

        struct precedence_tuple
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return a.is_with_king();
                }
        };
};

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct Frisian
{
        static constexpr auto is_backward_pawn_jump = true;
        static constexpr auto is_long_ranged_king = true;

        static constexpr auto is_orthogonal_jump = true;
        static constexpr auto max_same_king_push = 3;

        static constexpr auto is_quantity_precedence = true;
        static constexpr auto is_quality_precedence = true;
        static constexpr auto is_modality_precedence = true;

        struct precedence_tuple
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        auto const k = a.num_captured_kings();
                        auto const n = a.num_captured();
                        constexpr auto N = 65;
                        auto const v = N * (n + k) - k;
                        return std::make_tuple(
                                v, a.is_with_king()
                        );
                }
        };
};

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc
struct International
{
        // 2. The material
        static constexpr auto initial_gap = 2;                  // 2.8

        // 3. The movement of the piece
        static constexpr auto is_long_ranged_king = true;       // 3.9

        // 4. The capture
        static constexpr auto is_backward_pawn_jump = true;     // 4.1
        static constexpr auto is_pawns_jump_only_pawns = false; // 4.2 (implied by 3.1)
        static constexpr auto is_passing_capture = false;       // 4.11
        static constexpr auto is_quantity_precedence = true;    // 4.13
        static constexpr auto is_quality_precedence = false;    // 4.13
        static constexpr auto is_modality_precedence = false;   // 4.14

        struct precedence_tuple
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return a.num_captured();
                }
        };

        static constexpr auto is_passing_promotion = false;     // 4.15 (and 3.5)

        // 6. The draw
        static constexpr auto max_repetitions = 3;              // 6.1
        static constexpr auto max_reversible_moves = 50;        // 6.2

        // 8. Notation
        static constexpr auto is_algebraic_notation = false;    // 8.1 (and 2.6)
        static constexpr auto pushsep = '-';                    // 8.2.2
        static constexpr auto jumpsep = 'x';                    // 8.2.3
};

using Brazilian  = International;
using Philippine = International;
using Canadian   = International;
using SriLankan  = International;

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
struct Italian
{
        static constexpr auto is_long_ranged_king = false;      // 4.7

        static constexpr auto is_backward_pawn_jump = false;    // 5.3(a)
        static constexpr auto is_pawns_jump_only_pawns = true; // 5.3(b)

        static constexpr auto is_quantity_precedence = true;    // 6.6
        static constexpr auto is_modality_precedence = true;    // 6.7
        static constexpr auto is_quality_precedence = true;     // 6.8
        static constexpr auto is_ordering_precedence = true;    // 6.9

        struct precedence_tuple
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return std::make_tuple(
                                a.num_captured(), a.is_with_king(), a.num_captured_kings(), a.piece_order()
                        );
                }
        };

        static constexpr auto max_repetitions = 4;              // 9.3(b1)
        static constexpr auto max_reversible_moves = 80;        // 10.4
};

// http://americanpoolcheckers.us/americanpoolcheckers/index.php/history/apca-tournament-rules-of-play
struct Pool
{
        static constexpr auto initial_gap = 2;                  // 5

        static constexpr auto is_backward_pawn_jump = true;     // 14
        static constexpr auto is_long_ranged_king = true;       // 15
        static constexpr auto is_pawns_jump_only_pawns = false; // 16
        static constexpr auto is_passing_promotion = false;     // 16 (and 22 and 23)
        static constexpr auto is_passing_capture = false;       // 19 (and 17 and 18)
        static constexpr auto is_quantity_precedence = false;   // 20

        static constexpr auto max_repetitions = 3;              // 26
        static constexpr auto max_reversible_moves = 30;        // 26
};

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct Russian
{
        static constexpr auto is_long_ranged_king = true;       // 1.4.5

        static constexpr auto is_backward_pawn_jump = true;     // 1.5.3
        static constexpr auto is_passing_promotion = true;      // 1.5.15

        static constexpr auto is_algebraic_notation = true;
        static constexpr auto jumpsep = ':';
};

// http://www.fpdamas.pt/downloads/Regras%20Damas%20Cl%C3%A1ssicas%20-%20atualizado.pdf
struct Spanish
{
        static constexpr auto is_long_ranged_king = true;
        static constexpr auto is_backward_pawn_jump = false;

        static constexpr auto is_quantity_precedence = true;
        static constexpr auto is_quality_precedence = true;

        struct precedence_tuple
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return std::make_tuple(
                                a.num_captured(), a.num_captured_kings()
                        );
                }
        };
};

struct Thai
{
        static constexpr auto is_long_ranged_king = true;
        static constexpr auto is_backward_pawn_jump = false;

        static constexpr auto initial_position_gap = 4;
        static constexpr auto is_land_behind_piece = true;
        static constexpr auto is_passing_capture = true;
        static constexpr auto jumpsep = '-';
};

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules
template<class Parent = International>
struct Killer
:
        Parent
{
        static constexpr auto is_halt_behind_king = true;
};

}       // namespace rules
}       // namespace dctl
