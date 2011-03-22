#pragma once
#include <cstddef>

template<typename> class Position;
namespace Move { class Stack; }

template<typename Rules, typename Board> 
class AbstractState
{
public:
        // interface
        virtual void generate(const Position<Board>&, Move::Stack&) const = 0;
        virtual void generate_captures(const Position<Board>&, Move::Stack&) const = 0;
        virtual void generate_promotions(const Position<Board>&, Move::Stack&) const = 0;

        virtual size_t count(const Position<Board>&) const = 0;
        virtual size_t count_captures(const Position<Board>&) const = 0;
        virtual size_t count_promotions(const Position<Board>&) const = 0;
        virtual size_t count_mobility(const Position<Board>&) const = 0;

        virtual bool detect(const Position<Board>&) const = 0;        
        virtual bool detect_captures(const Position<Board>&) const = 0;
        virtual bool detect_promotions(const Position<Board>&) const = 0;

        // virtual destructor
        virtual ~AbstractState(void) {};
};
