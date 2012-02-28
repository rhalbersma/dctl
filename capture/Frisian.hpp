#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Value_fwd.hpp"

namespace dctl {

namespace variant { struct Frisian; }

namespace capture {

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
        void increment(bool is_captured_king)
        {
                num_kings_ += is_captured_king;
                ++num_pieces_;
                BOOST_ASSERT(invariant());
        }

        void decrement(bool is_captured_king)
        {
                --num_pieces_;
                num_kings_ -= is_captured_king;
                BOOST_ASSERT(invariant());
        }

        void toggle_with_king()
        {
                with_king_ ^= toggle;
                BOOST_ASSERT(invariant());
        }
        
private:
        // implementation
        bool invariant() const
        {
                return (0 <= num_kings_) && (num_kings_ <= num_pieces_);
        }

        BOOST_STATIC_CONSTANT(auto, toggle = true);

        // representation
        int num_pieces_;
        int num_kings_;
        bool with_king_;
};

}       // namespace capture
}       // namespace dctl
