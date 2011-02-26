#pragma once
#include "AbstractState.h"
#include <cstddef>

template<typename> class Position;
namespace Move { class List; }

template<bool Color, int Material, typename Rules, typename Board> 
class State: public AbstractState<Rules, Board>
{
public:		
        State(void) {};

        virtual void generate(const Position<Board>&, Move::List&) const;
        virtual void generate_captures(const Position<Board>&, Move::List&) const;
        virtual void generate_promotions(const Position<Board>&, Move::List&) const;

        virtual size_t count(const Position<Board>&) const;
        virtual size_t count_captures(const Position<Board>&) const;
        virtual size_t count_promotions(const Position<Board>&) const;
        virtual size_t count_mobility(const Position<Board>&) const;

        virtual bool detect(const Position<Board>&) const;
        virtual bool detect_captures(const Position<Board>&) const;
        virtual bool detect_promotions(const Position<Board>&) const;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "State.hpp"
