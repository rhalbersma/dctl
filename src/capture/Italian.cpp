#include "Italian.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

Value<rules::Italian>::Value()
:
        piece_order_(0),
        num_pieces_(0),
        num_kings_(0),
        with_king_(false)
{
}
        
bool Value<rules::Italian>::is_large(BitBoard) const
{
        return num_pieces_ >= rules::large_capture<rules::Italian>::value; 
}

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
bool Value<rules::Italian>::operator<(const Value<rules::Italian>& other) const
{
                // Art. 6.6
                if (num_pieces_ < other.num_pieces_)
                        return true;
                if (num_pieces_ > other.num_pieces_)
                        return false;

                // Art. 6.7
                if (with_king_ < other.with_king_)
                        return true;
                if (with_king_ > other.with_king_)
                        return false;

                // Art. 6.8
                if (num_kings_ < other.num_kings_)
                        return true;
                if (num_kings_ > other.num_kings_)
                        return false;

                // Art. 6.9
                return piece_order_ < other.piece_order_;
}

bool Value<rules::Italian>::operator==(const Value<rules::Italian>& other) const
{
        return (
                (piece_order_ == other.piece_order_) &&
                 (num_pieces_ == other.num_pieces_) &&
                  (num_kings_ == other.num_kings_) &&
                  (with_king_ == other.with_king_)
        );
}

void Value<rules::Italian>::increment(BitBoard target_sq, BitBoard king_targets)
{
        ++num_pieces_;
        if (target_sq & king_targets) {
                ++num_kings_;
                piece_order_ ^= BitBoard(1) << (8 * sizeof(BitBoard) - num_pieces_);
        }
}

void Value<rules::Italian>::decrement(BitBoard target_sq, BitBoard king_targets)
{
        if (target_sq & king_targets) {
                piece_order_ ^= BitBoard(1) << (8 * sizeof(BitBoard) - num_pieces_);
                --num_kings_;
        }
        --num_pieces_;
}

void Value<rules::Italian>::toggle_with_king()
{
        with_king_ ^= TOGGLE;
}

}       // namespace capture
}       // namespace dctl
