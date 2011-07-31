#include "Frisian.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

Value<rules::Frisian>::Value()
:
        num_pieces_(0),
        num_kings_(0),
        with_king_(false)
{
}
        
bool Value<rules::Frisian>::is_large(BitBoard) const
{
        return num_pieces_ >= rules::large_capture<rules::Frisian>::value; 
}
        
// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
bool Value<rules::Frisian>::operator<(const Value<rules::Frisian>& other) const
{
        const int delta_kings = num_kings_ - other.num_kings_;
        const int delta_men = num_pieces_ - other.num_pieces_ - delta_kings;

        // Art. 11
        if (delta_kings * delta_men >= 0) {
                // delta_kings and delta_men have equal sign when both are non-zero
                if (delta_men + delta_kings < 0)
                        return true;
                if (delta_men + delta_kings > 0)
                        return false;
        } else {
                // delta_kings and delta_men are both non-zero and have opposite sign
                if (delta_kings > 0)
                        // [n] kings are worth more than [2 n - 1] men
                        return delta_men + 2 * delta_kings - 1 < 0;
                else
                        // [n] kings are worth less than [2 n] men
                        return delta_men + 2 * delta_kings     < 0;
        }

        // Art. 12
        return with_king_ < other.with_king_;
}

bool Value<rules::Frisian>::operator==(const Value<rules::Frisian>& other) const
{
        return (
                (num_pieces_ == other.num_pieces_) &&
                 (num_kings_ == other.num_kings_) &&
                 (with_king_ == other.with_king_)
        );
}

void Value<rules::Frisian>::increment(BitBoard target_sq, BitBoard king_targets)
{
        ++num_pieces_;
        if (target_sq & king_targets)
                ++num_kings_;
}

void Value<rules::Frisian>::decrement(BitBoard target_sq, BitBoard king_targets)
{
        if (target_sq & king_targets)
                --num_kings_;
        --num_pieces_;
}

void Value<rules::Frisian>::toggle_with_king()
{
        with_king_ ^= TOGGLE;
}

}       // namespace capture
}       // namespace dctl
