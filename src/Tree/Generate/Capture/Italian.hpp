#include "../Tree/Generate/Capture/Templates.h"
#include "../Tree/Generate/Capture/Value.h"

namespace tree {
namespace generate {
namespace capture {

template<>
struct init<rules::Italian>
{
        void operator()(Value& value) const
        {
                value.piece_order = BitBoard(0);
                value.num_pieces = value.num_kings = 0;
                value.with_king = false;
        }
};

template<>
struct copy<rules::Italian>
{
        void operator()(Value& left, const Value& right) const
        {
                left.piece_order = right.piece_order;
                left.num_pieces = right.num_pieces;
                left.num_kings = right.num_kings;
                left.with_king = right.with_king;
        }
};

template<>
struct increment<rules::Italian>
{
        void operator()(Value& value, BitBoard target_sq, BitBoard king_targets) const
        {
                ++value.num_pieces;
                if (target_sq & king_targets) {
                        ++value.num_kings;
                        value.piece_order ^= BitBoard(1) << (8 * sizeof(BitBoard) - value.num_pieces);
                }
        }
};

template<>
struct decrement<rules::Italian>
{
        void operator()(Value& value, BitBoard target_sq, BitBoard king_targets) const
        {
                if (target_sq & king_targets) {
                        value.piece_order ^= BitBoard(1) << (8 * sizeof(BitBoard) - value.num_pieces);
                        --value.num_kings;
                }
                --value.num_pieces;
        }
};

template<>
struct equal_to<rules::Italian>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                return (
                       (left.piece_order == right.piece_order) &&
                        (left.num_pieces == right.num_pieces) &&
                         (left.num_kings == right.num_kings) &&
                         (left.with_king == right.with_king)
                );
        }
};

template<>
struct greater_equal<rules::Italian>: public std::binary_function<Value, Value, bool>
{
        // http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
        bool operator()(const Value& left, const Value& right) const
        {
                // Art. 6.6
                if (left.num_pieces > right.num_pieces)
                        return true;
                if (left.num_pieces < right.num_pieces)
                        return false;

                // Art. 6.7
                if (left.with_king > right.with_king)
                        return true;
                if (left.with_king < right.with_king)
                        return false;

                // Art. 6.8
                if (left.num_kings > right.num_kings)
                        return true;
                if (left.num_kings < right.num_kings)
                        return false;

                // Art. 6.9
                return left.piece_order >= right.piece_order;
        }
};

}       // namespace capture
}       // namespace generate
}       // namespace tree
