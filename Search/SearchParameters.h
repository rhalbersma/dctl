#pragma once
#include "../Position/Move.h"

template<typename> class Position;

class SearchParameters
{
public:
        // constructors
        SearchParameters(void);

        // views
        const Move::Sequence& PV(void) const;
        Move::Sequence& PV(void);
        bool do_null_move(void) const;

        // modifiers
        void reset_PV(size_t, const Move::Sequence&);
        void clear_do_null_move(void);
        void set_do_null_move(void);

private:
        // implementation
        static const bool TOGGLE = true;

        // representation
        Move::Sequence d_PV;
        bool d_null_move;
};
