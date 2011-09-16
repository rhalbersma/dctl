#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "ValueInterface.hpp"
#include "../rules/Rules.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

namespace variant { struct Frisian; }

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

// explicit specialization for Frisian draughts
template<>
class Value<variant::Frisian>
: 
        public ValueInterface
{
public:
        // constructors
        Value()
        :
                num_pieces_(0),
                num_kings_(0),
                with_king_(false)
        {
        }

        // predicates
        bool operator<(const Value<variant::Frisian>& other) const
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

        bool operator==(const Value<variant::Frisian>& other) const
        {
                return (
                        (num_pieces_ == other.num_pieces_) &&
                         (num_kings_ == other.num_kings_) &&
                         (with_king_ == other.with_king_)
                );
        }

private:
        // implementation
        virtual bool do_is_large(BitBoard /* captured_pieces */ ) const
        {
                return num_pieces_ >= rules::large_capture<variant::Frisian>::value; 
        }

        virtual void do_increment(BitBoard target_sq, BitBoard king_targets)
        {
                ++num_pieces_;
                if (target_sq & king_targets)
                        ++num_kings_;
        }

        virtual void do_decrement(BitBoard target_sq, BitBoard king_targets)
        {
                if (target_sq & king_targets)
                        --num_kings_;
                --num_pieces_;
        }

        virtual void do_toggle_with_king()
        {
                with_king_ ^= TOGGLE;
        }
        
        BOOST_STATIC_CONSTANT(auto, TOGGLE = true);

        // representation
        PieceCount num_pieces_;
        PieceCount num_kings_;
        bool with_king_;
};
        
}       // namespace capture
}       // namespace dctl
