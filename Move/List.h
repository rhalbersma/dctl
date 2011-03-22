#pragma once
#include "Types.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"
#include "../Variants/Rules.h"

namespace Move {

class List
{
public:
        // predicate
        template<typename> bool non_unique_back(void);

        size_t size(void) const;
        const Pieces& operator[](size_t) const;

        bool empty(void) const;

        // modifiers
        template<bool> void push_back(BitBoard);                                          // king move
        template<bool> void push_back(BitBoard, BitBoard);                                // man move
        template<bool, typename> void push_back(BitBoard, BitBoard, BitBoard);            // king capture
        template<bool, typename> void push_back(BitBoard, BitBoard, BitBoard, BitBoard);  // man capture
        void pop_back(void);
        void clear(void);

private:
        // implementation
        bool non_unique_back(Int2Type<false>);
        bool non_unique_back(Int2Type<true >);

        Vector move_list_;
};

}       // namespace Move

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "List.hpp"
