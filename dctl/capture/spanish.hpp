#pragma once
#include <limits>                       // numeric_limits
#include <utility>                      // get, tuple
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/operators.hpp>          // totally_ordered
#include <dctl/capture/value_fwd.hpp>   // Value (primary template)
#include <dctl/rules/spanish_fwd.hpp>   // Spanish

namespace dctl {
namespace capture {

// partial specialization for Spanish draughts
template<typename Board>
struct Value<rules::Spanish, Board>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<rules::Spanish, Board> >  // < >= > <= == !=
{
public:
        // structors

        Value()
        :
                data_() // std::tuple default constructor zero-intializes
        {
                BOOST_ASSERT(invariant());
        }

        // modifiers

        void increment(bool is_king)
        {
                BOOST_ASSERT(!full());
                num_kings() += is_king;
                ++num_pieces();
                BOOST_ASSERT(invariant());
        }

        void decrement(bool is_king)
        {
                BOOST_ASSERT(!empty(is_king));
                --num_pieces();
                num_kings() -= is_king;
                BOOST_ASSERT(invariant());
        }

        // queries

        int count() const
        {
                return num_pieces();
        }

        // predicates

        // operator>=, operator>, operator<= automatically generated by boost::totally_ordered
        friend bool operator<(Value const& lhs, Value const& rhs)
        {
                // delegate to std::tuple::operator<
                return lhs.data_ < rhs.data_;
        }

        // operator!= automatically generated by boost::totally_ordered
        friend bool operator==(Value const& lhs, Value const& rhs)
        {
                // delegate to std::tuple::operator==
                return lhs.data_ == rhs.data_;
        }

private:
        // modifiers

        int& num_pieces()
        {
                return std::get<num_pieces_>(data_);
        }

        int& num_kings()
        {
                return std::get<num_kings_>(data_);
        }

        // queries

        int const& num_pieces() const
        {
                return std::get<num_pieces_>(data_);
        }

        int const& num_kings() const
        {
                return std::get<num_kings_>(data_);
        }

        // predicates

        bool invariant() const
        {
                return (
                        (0 <= num_kings()) &&
                        (num_kings() <= num_pieces()) &&
                        (num_pieces() <= std::numeric_limits<int>::max())
                );
        }

        bool empty(bool is_king) const
        {
                return (is_king? num_kings() : num_pieces()) == 0;
        }

        bool full() const
        {
                return num_pieces() == std::numeric_limits<int>::max();
        }

        // representation

        enum { num_pieces_, num_kings_ };
        std::tuple<int, int> data_;
};

}       // namespace capture
}       // namespace dctl