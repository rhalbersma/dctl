#include "Frisian.h"
#include "../../rules/Rules.h"

namespace dctl {
namespace generate {
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

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
template<>
bool operator<(const Value<rules::Frisian>& left, const Value<rules::Frisian>& right)
{
        const int delta_kings = left.num_kings - right.num_kings;
        const int delta_men = left.num_pieces - right.num_pieces - delta_kings;

        // Art. 11
        if (delta_kings * delta_men >= 0) {
                if (delta_men + delta_kings > 0)
                        return !true;
                if (delta_men + delta_kings < 0)
                        return !false;
        } else {
                if (delta_kings > 0)
                        return !(delta_men + 2 * delta_kings - 1 >= 0);
                else
                        return !(delta_men + 2 * delta_kings     >= 0);
        }

        // Art. 12
        return !(left.with_king >= right.with_king);
}

template<>
bool operator==(const Value<rules::Frisian>& left, const Value<rules::Frisian>& right)
{
        return (
                (left.num_pieces == right.num_pieces) &&
                 (left.num_kings == right.num_kings) &&
                 (left.with_king == right.with_king)
        );
}

}       // namespace capture
}       // namespace generate
}       // namespace dctl
