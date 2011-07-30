#include "Italian.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

Value<rules::Italian>::Value()
:
        piece_order(0),
        num_pieces(0),
        num_kings(0),
        with_king(false)
{
}
        
bool Value<rules::Italian>::is_large(BitBoard) const
{
        return num_pieces >= rules::large_capture<rules::Italian>::value; 
}

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
bool Value<rules::Italian>::operator<(const Value<rules::Italian>& other) const
{
                // Art. 6.6
                if (num_pieces < other.num_pieces)
                        return true;
                if (num_pieces > other.num_pieces)
                        return false;

                // Art. 6.7
                if (with_king < other.with_king)
                        return true;
                if (with_king > other.with_king)
                        return false;

                // Art. 6.8
                if (num_kings < other.num_kings)
                        return true;
                if (num_kings > other.num_kings)
                        return false;

                // Art. 6.9
                return piece_order < other.piece_order;
}

bool Value<rules::Italian>::operator==(const Value<rules::Italian>& other) const
{
        return (
                (piece_order == other.piece_order) &&
                 (num_pieces == other.num_pieces) &&
                  (num_kings == other.num_kings) &&
                  (with_king == other.with_king)
        );
}

void Value<rules::Italian>::increment(BitBoard target_sq, BitBoard king_targets)
{
        ++num_pieces;
        if (target_sq & king_targets) {
                ++num_kings;
                piece_order ^= BitBoard(1) << (8 * sizeof(BitBoard) - num_pieces);
        }
}

void Value<rules::Italian>::decrement(BitBoard target_sq, BitBoard king_targets)
{
        if (target_sq & king_targets) {
                piece_order ^= BitBoard(1) << (8 * sizeof(BitBoard) - num_pieces);
                --num_kings;
        }
        --num_pieces;
}

void Value<rules::Italian>::toggle_with_king()
{
        with_king ^= TOGGLE;
}

}       // namespace capture
}       // namespace dctl
