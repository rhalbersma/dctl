#include "Italian.h"
#include "../../rules/Rules.h"

namespace dctl {
namespace generate {
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
        
template<>
bool operator<(const Value<rules::Italian>& left, const Value<rules::Italian>& right)
{
                // Art. 6.6
                if (left.num_pieces < right.num_pieces)
                        return true;
                if (left.num_pieces > right.num_pieces)
                        return false;

                // Art. 6.7
                if (left.with_king < right.with_king)
                        return true;
                if (left.with_king > right.with_king)
                        return false;

                // Art. 6.8
                if (left.num_kings < right.num_kings)
                        return true;
                if (left.num_kings > right.num_kings)
                        return false;

                // Art. 6.9
                return left.piece_order < right.piece_order;
}

template<>
bool operator==(const Value<rules::Italian>& left, const Value<rules::Italian>& right)
{
        return (
                (left.piece_order == right.piece_order) &&
                 (left.num_pieces == right.num_pieces) &&
                  (left.num_kings == right.num_kings) &&
                  (left.with_king == right.with_king)
        );
}

}       // namespace capture
}       // namespace generate
}       // namespace dctl
