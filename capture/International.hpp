#pragma once
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

        int count() const
        {
                return num_pieces_; 
        }

        // modifiers
        void do_increment()
        {
                ++num_pieces_;
        }

        void do_decrement()
        {
                --num_pieces_;
        }      

private:
        // representation
        int num_pieces_;
};
   
template<typename Rules>
void increment(Value<Rules>&);

template<> inline
void increment(Value<variant::International>& v)
{
        v.do_increment();
}
 
template<typename Rules>
void decrement(Value<Rules>&);

template<> inline
void decrement(Value<variant::International>& v)
{
        v.do_decrement();
}

}       // namespace capture
}       // namespace dctl
