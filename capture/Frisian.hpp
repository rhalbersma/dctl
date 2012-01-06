#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "../utility/IntegerTypes.hpp"

namespace dctl {

namespace variant { struct Frisian; }

namespace capture {

// forward declaration of the primary template
template<typename> struct Value;

// explicit specialization for Frisian draughts
template<>
struct Value<variant::Frisian>
{
public:
        // constructors
        Value()
        :
                num_pieces_(0),
                num_kings_(0),
                with_king_(false)
        {
                BOOST_ASSERT(invariant());
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

        int count() const
        {
                return num_pieces_; 
        }

        // modifiers
        void do_increment(BitBoard target_sq, BitBoard king_targets)
        {
                ++num_pieces_;
                if (target_sq & king_targets)
                        ++num_kings_;
                BOOST_ASSERT(invariant());
        }

        void do_decrement(BitBoard target_sq, BitBoard king_targets)
        {
                if (target_sq & king_targets)
                        --num_kings_;
                --num_pieces_;
                BOOST_ASSERT(invariant());
        }

        void do_toggle_with_king()
        {
                with_king_ ^= toggle;
        }
        
private:
        // implementation
        bool invariant() const
        {
                return num_kings_ <= num_pieces_;
        }

        BOOST_STATIC_CONSTANT(auto, toggle = true);

        // representation
        PieceCount num_pieces_;
        PieceCount num_kings_;
        bool with_king_;
};

template<typename Rules>
void increment(Value<Rules>&, BitBoard, BitBoard);

template<> inline
void increment(Value<variant::Frisian>& v, BitBoard target_sq, BitBoard king_targets)
{
        v.do_increment(target_sq, king_targets);
}
 
template<typename Rules>
void decrement(Value<Rules>&, BitBoard, BitBoard);

template<> inline
void decrement(Value<variant::Frisian>& v, BitBoard target_sq, BitBoard king_targets)
{
        v.do_decrement(target_sq, king_targets);
}

template<typename Rules>
void toggle_with_king(Value<Rules>&);

template<> inline
void toggle_with_king(Value<variant::Frisian>& v)
{
        v.do_toggle_with_king();
}

}       // namespace capture
}       // namespace dctl
