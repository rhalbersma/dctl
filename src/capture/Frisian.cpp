#include "Frisian.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

Value<rules::Frisian>::Value()
:
        num_pieces(0),
        num_kings(0),
        with_king(false)
{
}
        
bool Value<rules::Frisian>::is_large(BitBoard) const
{
        return num_pieces >= rules::large_capture<rules::Frisian>::value; 
}
        
// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
bool Value<rules::Frisian>::operator<(const Value<rules::Frisian>& other) const
{
        const int delta_kings = num_kings - other.num_kings;
        const int delta_men = num_pieces - other.num_pieces - delta_kings;

        // Art. 11
        if (delta_kings * delta_men >= 0) {
                if (delta_men + delta_kings < 0)
                        return true;
                if (delta_men + delta_kings > 0)
                        return false;
        } else {
                if (delta_kings > 0)
                        return delta_men + 2 * delta_kings - 1 < 0;
                else
                        return delta_men + 2 * delta_kings     < 0;
        }

        // Art. 12
        return with_king < other.with_king;
}

bool Value<rules::Frisian>::operator==(const Value<rules::Frisian>& other) const
{
        return (
                (num_pieces == other.num_pieces) &&
                 (num_kings == other.num_kings) &&
                 (with_king == other.with_king)
        );
}

void Value<rules::Frisian>::increment(BitBoard target_sq, BitBoard king_targets)
{
        ++num_pieces;
        if (target_sq & king_targets)
                ++num_kings;
}

void Value<rules::Frisian>::decrement(BitBoard target_sq, BitBoard king_targets)
{
        if (target_sq & king_targets)
                --num_kings;
        --num_pieces;
}

void Value<rules::Frisian>::toggle_with_king()
{
        with_king ^= TOGGLE;
}

}       // namespace capture
}       // namespace dctl
