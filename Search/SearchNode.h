#pragma once
#include "../Utilities/IntegerTypes.h"
#include <string>

class SearchNode
{
public:
        // typedefs
        enum Type {LOWER = 1, UPPER, EXACT};

        // constructors
	SearchNode(void);
	SearchNode(int, Type, size_t, size_t);

	// views
	int value(void) const;                          // value
	int refined_value(int) const;                   // refined value
	Type type(void) const;                          // upper or lower bound, or exact
	size_t depth(void) const;                       // remaining depth to search
	size_t move(void) const;                        // index of the best move

        // predicates
        bool is_depth_greater_equal(size_t) const;      // check for sufficient depth
        bool is_cutoff(int, int) const;                 // check for a cutoff
        bool is_cutoff(int) const;                      // check for a cutoff
        bool is_fail_low(int) const;                    // check for a fail low cutoff
        bool is_fail_high(int) const;                   // check for a fail high cutoff
        bool is_heuristic(void) const;                  // check for a heuristic score
        bool has_move(void) const;                      // check for a best move

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

        static const size_t TYPE_BITS = 3;
        static const size_t DEPTH_BITS = 6;
        static const size_t MOVE_BITS = 7;

        static const size_t TYPE_SHIFT = 0;
        static const size_t DEPTH_SHIFT = TYPE_SHIFT + TYPE_BITS;
        static const size_t MOVE_SHIFT = DEPTH_SHIFT + DEPTH_BITS;

        static const size_t TYPE_MASK = ((1 << TYPE_BITS) - 1);
        static const size_t DEPTH_MASK = ((1 << DEPTH_BITS) - 1);
        static const size_t MOVE_MASK = ((1 << MOVE_BITS) - 1);

        // representation
         int16_t d_value;		                // value
        uint16_t d_rest;                                // type, depth and move
};
