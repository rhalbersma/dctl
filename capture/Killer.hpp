#pragma once

namespace dctl {

namespace variant { struct Killer; }

namespace capture {

// forward declaration of the primary template
template<typename> struct Value;

// explicit specialization for Killer draughts
template<>
struct Value<variant::Killer>
{
public:
        // constructors
        Value()
        :
                num_pieces_(0)
        {
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
void increment(Value<Rules>&, BitBoard, BitBoard);

template<> inline
void increment(Value<variant::Killer>& v)
{
        v.do_increment();
}
 
template<typename Rules>
void decrement(Value<Rules>&, BitBoard, BitBoard);

template<> inline
void decrement(Value<variant::Killer>& v)
{
        v.do_decrement();
}

}       // namespace capture
}       // namespace dctl
