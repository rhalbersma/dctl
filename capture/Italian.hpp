#pragma once
#include <boost/config.hpp>
#include "ValueInterface.hpp"
#include "../rules/Rules.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

namespace variant { struct Italian; }

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

// explicit specialization for Italian draughts
template<>
class Value<variant::Italian>
: 
        public ValueInterface
{
public:
        // constructors
        Value()
        :
                piece_order_(0),
                num_pieces_(0),
                num_kings_(0),
                with_king_(false)
        {
        }

        // predicates
        bool operator<(const Value<variant::Italian>& other) const
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

        bool operator==(const Value<variant::Italian>& other) const
        {
                return (
                        (piece_order_ == other.piece_order_) &&
                         (num_pieces_ == other.num_pieces_) &&
                          (num_kings_ == other.num_kings_) &&
                          (with_king_ == other.with_king_)
                );
        }

private:
        // implementation
        virtual bool do_is_large(BitBoard /* captured_pieces */ ) const
        {
                return num_pieces_ >= rules::large_capture<variant::Italian>::value; 
        }

        virtual void do_increment(BitBoard target_sq, BitBoard king_targets)
        {
                ++num_pieces_;
                if (target_sq & king_targets) {
                        ++num_kings_;
                        piece_order_ ^= bit::reverse_singlet<BitBoard>(num_pieces_);
                }
        }

        virtual void do_decrement(BitBoard target_sq, BitBoard king_targets)
        {
                if (target_sq & king_targets) {
                        piece_order_ ^= bit::reverse_singlet<BitBoard>(num_pieces_);
                        --num_kings_;
                }
                --num_pieces_;
        }

        virtual void do_toggle_with_king()
        {
                with_king_ ^= TOGGLE;
        }

        BOOST_STATIC_CONSTANT(auto, TOGGLE = true);

        // representation
        BitBoard piece_order_;
        PieceCount num_pieces_;
        PieceCount num_kings_;
        bool with_king_;
};
        
}       // namespace capture
}       // namespace dctl
