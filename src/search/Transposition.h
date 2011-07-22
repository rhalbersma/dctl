#pragma once
#include <string>
#include "../utils/IntegerTypes.h"

namespace dctl {
namespace search {

class Transposition
{
public:
        // typedefs
        enum Bound { LOWER = 1, UPPER = 2, EXACT = 3 };

        // constructors
	Transposition();
	Transposition(int, Bound, int, size_t);

	// views
	int value() const;                  // value
	int refined_value(int) const;           // refined value
	Bound bound() const;                // upper or lower bound, or exact
	int depth() const;                  // remaining depth to search
	size_t move() const;                // index of the best move

        // predicates
        bool is_cutoff(int, int) const;         // check for a cutoff against a non-null window
        bool is_cutoff(int) const;              // check for a cutoff against a null window
        bool is_fail_low(int) const;            // check for a fail low cutoff
        bool is_fail_high(int) const;           // check for a fail high cutoff
        bool has_move() const;              // check for a best move

        // output
        std::string print_value() const;
        std::string print_bound() const;
        std::string print_depth() const;
        std::string print_move() const;

        // modifiers
        void set_move(size_t);

        static Bound lower();
        static Bound upper();
        static Bound exact();
        static size_t no_move();

private:
        // implementation
        bool is_finite() const;
        bool has_lower() const;
        bool has_upper() const;
        bool is_lower() const;
        bool is_upper() const;
        bool is_exact() const;
        bool invariant() const;

        static const size_t BOUND_BITS = 2;
        static const size_t DEPTH_BITS = 7;
        static const size_t MOVE_BITS = 7;

        static const size_t BOUND_SHIFT = 0;
        static const size_t DEPTH_SHIFT = BOUND_SHIFT + BOUND_BITS;
        static const size_t MOVE_SHIFT = DEPTH_SHIFT + DEPTH_BITS;

        static const size_t BOUND_MASK = ((1 << BOUND_BITS) - 1);
        static const size_t DEPTH_MASK = ((1 << DEPTH_BITS) - 1);
        static const size_t MOVE_MASK = ((1 << MOVE_BITS) - 1);

        // representation
         int16_t value_;                        // value
        uint16_t rest_;                         // bound, depth and move
};

}       // namespace search
}       // namespace dctl
