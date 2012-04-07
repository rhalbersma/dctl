#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "Value_fwd.hpp"
#include "../utility/TotalOrderInterface.hpp"

namespace dctl {

namespace variant { struct International; }

namespace capture {

// explicit specialization for International draughts
template<>
struct Value<variant::International>
:
        // Curiously Recurring Template Pattern (CRTP)
        public TotalOrderInterface< Value<variant::International> >
{
public:
        // constructors
        Value()
        :
                num_pieces_(0)
        {
                BOOST_ASSERT(invariant());
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
        friend struct TotalOrderInterface< Value<variant::International> >;

        // predicates
        bool less(const Value<variant::International>& other) const
        {
                return num_pieces_ < other.num_pieces_;
        }

        bool equal(const Value<variant::International>& other) const
        {
                return num_pieces_ == other.num_pieces_;
        }

        bool invariant() const
        {
                return 0 <= num_pieces_;
        }

        // representation
        int num_pieces_;
};

}       // namespace capture
}       // namespace dctl
