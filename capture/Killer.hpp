#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"

namespace dctl {

namespace variant { struct Killer; }

namespace capture {

// explicit specialization for Killer draughts
template<>
struct Value<variant::Killer>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<variant::Killer> >
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
        bool operator<(const Value<variant::Killer>& other) const
        {
                return num_pieces_ < other.num_pieces_;
        }

        bool operator==(const Value<variant::Killer>& other) const
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
        bool invariant() const
        {
                return 0 <= num_pieces_;
        }

        // representation
        int num_pieces_;
};

}       // namespace capture
}       // namespace dctl
