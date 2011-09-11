#pragma once
#include <string>       // std::string
#include <boost/config.hpp>
#include "../utility/IntegerTypes.h"

namespace dctl {
namespace search {

class Transposition
{
public:
        // typedefs
        enum Bound { lower_bound = 1, upper_bound = 2, exact_value = 3 };

        // constructors
	Transposition();
	Transposition(int, Bound, int, int);

	// views
	int value() const;                      // value
	int refined_value(int) const;           // refined value
	Bound bound() const;                    // upper or lower bound, or exact
	int depth() const;                      // remaining depth to search
	int move() const;                       // index of the best move

        // predicates
        bool is_cutoff(int, int) const;         // check for a cutoff against a non-null window
        bool is_cutoff(int) const;              // check for a cutoff against a null window
        bool is_fail_low(int) const;            // check for a fail low cutoff
        bool is_fail_high(int) const;           // check for a fail high cutoff
        bool has_move() const;                  // check for a best move

        // output
        std::string print_value() const;
        std::string print_bound() const;
        std::string print_depth() const;
        std::string print_move() const;

        // modifiers
        void set_move(int);

        static int no_move();

private:
        // implementation
        bool is_finite() const;
        bool has_lower_bound() const;
        bool has_upper_bound() const;
        bool is_lower_bound() const;
        bool is_upper_bound() const;
        bool is_exact_value() const;
        bool invariant() const;

        BOOST_STATIC_CONSTANT(auto, BOUND_BITS = 2);
        BOOST_STATIC_CONSTANT(auto, DEPTH_BITS = 7);
        BOOST_STATIC_CONSTANT(auto, MOVE_BITS = 7);

        BOOST_STATIC_CONSTANT(auto, BOUND_SHIFT = 0);
        BOOST_STATIC_CONSTANT(auto, DEPTH_SHIFT = BOUND_SHIFT + BOUND_BITS);
        BOOST_STATIC_CONSTANT(auto, MOVE_SHIFT = DEPTH_SHIFT + DEPTH_BITS);

        BOOST_STATIC_CONSTANT(auto, BOUND_MASK = ((1 << BOUND_BITS) - 1));
        BOOST_STATIC_CONSTANT(auto, DEPTH_MASK = ((1 << DEPTH_BITS) - 1));
        BOOST_STATIC_CONSTANT(auto, MOVE_MASK = ((1 << MOVE_BITS) - 1));

        // representation
         int16_t value_;        // value
        uint16_t rest_;         // bound, depth and move
};

}       // namespace search
}       // namespace dctl
