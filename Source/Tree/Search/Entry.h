#pragma once
#include "../../Utilities/IntegerTypes.h"
#include <string>

namespace Tree {
namespace Search {

class Entry
{
public:
        // typedefs
        enum Type { LOWER = 1, UPPER = 2, EXACT = 3 };

        // constructors
	Entry(void);
	Entry(int, Type, size_t, size_t);

	// views
	int value(void) const;                  // value
	int refined_value(int) const;           // refined value
	Type type(void) const;                  // upper or lower bound, or exact
	size_t depth(void) const;               // remaining depth to search
	size_t move(void) const;                // index of the best move

        // predicates
        bool equal_to(size_t) const;            // check for equal depth
        bool greater_equal(size_t) const;       // check for greater or equal depth
        bool is_cutoff(int, int) const;         // check for a cutoff against a non-null window
        bool is_cutoff(int) const;              // check for a cutoff against a null window
        bool is_fail_low(int) const;            // check for a fail low cutoff
        bool is_fail_high(int) const;           // check for a fail high cutoff
        bool is_mate(void) const;               // check for a mate score
        bool has_move(void) const;              // check for a best move

        // output
        std::string print_value(void) const;
        std::string print_type(void) const;
        std::string print_depth(void) const;
        std::string print_move(void) const;

        // modifiers
        void set_move(size_t);

        static Type lower(void);
        static Type upper(void);
        static Type exact(void);
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

        static const size_t TYPE_BITS = 2;
        static const size_t DEPTH_BITS = 7;
        static const size_t MOVE_BITS = 7;

        static const size_t TYPE_SHIFT = 0;
        static const size_t DEPTH_SHIFT = TYPE_SHIFT + TYPE_BITS;
        static const size_t MOVE_SHIFT = DEPTH_SHIFT + DEPTH_BITS;

        static const size_t TYPE_MASK = ((1 << TYPE_BITS) - 1);
        static const size_t DEPTH_MASK = ((1 << DEPTH_BITS) - 1);
        static const size_t MOVE_MASK = ((1 << MOVE_BITS) - 1);

        // representation
         int16_t value_;                        // value
        uint16_t rest_;                         // type, depth and move
};

}       // namespace Search
}       // namespace Tree
