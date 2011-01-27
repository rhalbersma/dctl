#pragma once
#include "../Utilities/IntegerTypes.h"

class CaptureValue
{
        // modifiers
        template<typename> friend struct init;
        template<typename> friend struct copy;
        template<typename> friend struct increment;
        template<typename> friend struct decrement;

        // unary predicate
        template<typename> friend class large;

        // binary predicates
        template<typename> friend struct equal_to;
        template<typename> friend struct not_equal_to;
        template<typename> friend struct greater_equal;

public:
        void toggle_with_king(void);

private:
        // implementation
        static const bool TOGGLE = true;

        // representation
        BitBoard piece_order;
        size_t num_pieces;
        size_t num_kings;
        bool with_king;
};
