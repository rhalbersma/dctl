#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"                // Value (primary template)

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
        // structors

        Value()
        :
                num_pieces_(0)
        {
                BOOST_ASSERT(invariant());
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

        // queries

        int count() const
        {
                return num_pieces_;
        }

        // predicates

        bool operator<(const Value& other) const
        {
                return num_pieces_ < other.num_pieces_;
        }

        bool operator==(const Value& other) const
        {
                return num_pieces_ == other.num_pieces_;
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
