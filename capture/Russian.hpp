#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT

namespace dctl {

namespace variant { struct Russian; }

namespace capture {

// forward declaration of the primary template
template<typename> struct Value;

// explicit specialization for Russian draughts
template<>
struct Value<variant::Russian>
{
public:
        // constructors
        Value()
        :
                promotion_(false)
        {
        }

        // predicates
        bool do_is_promotion() const
        {
                return promotion_;
        }

        // modifiers
        void do_toggle_promotion()
        {
                promotion_ ^= toggle;
        } 

private:
        // implementation
        BOOST_STATIC_CONSTANT(auto, toggle = true);

        // representation
        bool promotion_;
};

template<typename Rules>
bool is_promotion(const Value<Rules>&);

template<> inline
bool is_promotion(const Value<variant::Russian>& v)
{
        return v.do_is_promotion();
}

template<typename Rules>
void toggle_promotion(Value<Rules>&);

template<> inline
void toggle_promotion(Value<variant::Russian>& v)
{
        v.do_toggle_promotion();
}

}       // namespace capture
}       // namespace dctl
