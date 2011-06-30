#pragma once
#include <string>
#include "../../Utilities/IntegerTypes.h"

namespace tree {
namespace search {

class Entry
{
public:
        // typedefs
        enum Bound { LOWER = 1, UPPER = 2, EXACT = 3 };

        // constructors
	Entry(void);
	Entry(int, Bound, int, size_t);

	// views
	int value(void) const;                  // value
	int refined_value(int) const;           // refined value
	Bound bound(void) const;                // upper or lower bound, or exact
	int depth(void) const;                  // remaining depth to search
	size_t move(void) const;                // index of the best move

        // predicates
        bool is_cutoff(int, int) const;         // check for a cutoff against a non-null window
        bool is_cutoff(int) const;              // check for a cutoff against a null window
        bool is_fail_low(int) const;            // check for a fail low cutoff
        bool is_fail_high(int) const;           // check for a fail high cutoff
        bool has_move(void) const;              // check for a best move

        // output
        std::string print_value(void) const;
        std::string print_bound(void) const;
        std::string print_depth(void) const;
        std::string print_move(void) const;

        // modifiers
        void set_move(size_t);

        static Bound lower(void);
        static Bound upper(void);
        static Bound exact(void);
        static size_t no_move(void);

private:
        // implementation
        bool is_finite(void) const;
        bool has_lower(void) const;
        bool has_upper(void) const;
        bool is_lower(void) const;
        bool is_upper(void) const;
        bool is_exact(void) const;
        bool invariant(void) const;

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
}       // namespace tree
