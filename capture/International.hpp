#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "Value_fwd.hpp"

namespace dctl {

namespace variant { struct International; }

namespace capture {

// explicit specialization for International draughts
template<>
struct Value<variant::International>
{
public:
        // constructors
        Value()
        :
                num_pieces_(0)
        {
                BOOST_ASSERT(invariant());
        }

        // predicates
        bool operator<(const Value<variant::International>& other) const
        {
                return num_pieces_ < other.num_pieces_;
        }

        bool operator==(const Value<variant::International>& other) const
        {
                return num_pieces_ == other.num_pieces_;
        }

        // views
        int count() const
        {
                return num_pieces_;
        }

        // modifiers
        void increment()
        {
                ++num_pieces_;
                BOOST_ASSERT(invariant());
        }

        void decrement()
        {
                --num_pieces_;
                BOOST_ASSERT(invariant());
        }

private:
        // implementation
        bool invariant() const
        {
                return 0 <= num_pieces_;
        }

        // representation
        int num_pieces_;
};

}       // namespace capture
}       // namespace dctl
