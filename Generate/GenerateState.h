#pragma once
#include <cstddef>

template<typename> class Position;
namespace Move { class List; }

template<typename Rules, typename Board> 
class AbstractGenerateState
{
public:
        // interface
        virtual void generate(const Position<Board>&, Move::List&) const = 0;
        virtual void generate_captures(const Position<Board>&, Move::List&) const = 0;
        virtual void generate_promotions(const Position<Board>&, Move::List&) const = 0;

        virtual size_t count(const Position<Board>&) const = 0;
        virtual size_t count_mobility(const Position<Board>&) const = 0;

        virtual bool detect(const Position<Board>&) const = 0;        
        virtual bool detect_captures(const Position<Board>&) const = 0;
        virtual bool detect_promotions(const Position<Board>&) const = 0;

        // virtual destructor
        virtual ~AbstractGenerateState(void) {};
};

template<bool Color, int Material, typename Rules, typename Board> 
class GenerateState: public AbstractGenerateState<Rules, Board>
{
public:		
        GenerateState(void) {};

        virtual void generate(const Position<Board>&, Move::List&) const;
        virtual void generate_captures(const Position<Board>&, Move::List&) const;
        virtual void generate_promotions(const Position<Board>&, Move::List&) const;

        virtual size_t count(const Position<Board>&) const;
        virtual size_t count_mobility(const Position<Board>&) const;

        virtual bool detect(const Position<Board>&) const;
        virtual bool detect_captures(const Position<Board>&) const;
        virtual bool detect_promotions(const Position<Board>&) const;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "GenerateState.hpp"
