#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "Value_fwd.hpp"

namespace dctl {

namespace variant { struct Spanish; }

namespace capture {

// explicit specialization for Spanish draughts
template<>
struct Value<variant::Spanish>
{
public:
        // constructors
        Value()
        :
                num_pieces_(0),
                num_kings_(0)
        {
                BOOST_ASSERT(invariant());
        }

        // predicates
        bool operator<(const Value<variant::Spanish>& other) const
        {
                // 15 b)
                if (num_pieces_ < other.num_pieces_)
                        return true;
                if (num_pieces_ > other.num_pieces_)
                        return false;

                // 15 c)
                return num_kings_ < other.num_kings_;
        }

        bool operator==(const Value<variant::Spanish>& other) const
        {
                return (
                        (num_pieces_ == other.num_pieces_) &&
                         (num_kings_ == other.num_kings_)
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

private:
        // implementation
        bool invariant() const
        {
                return (0 <= num_kings_) && (num_kings_ <= num_pieces_);
        }

        // representation
        int num_pieces_;
        int num_kings_;
};

}       // namespace capture
}       // namespace dctl
