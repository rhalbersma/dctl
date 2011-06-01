#pragma once
#include <cstddef>
#include "AbstractState.h"

namespace Tree {

namespace Node { template<typename> class Position; }
namespace Move { class Stack; }

namespace Generate {

template<bool Color, int Material, typename Rules, typename Board> 
class State: public AbstractState<Rules, Board>
{
public:		
        State(void) {};

        virtual void generate(const Node::Position<Board>&, Move::Stack&) const;
        virtual void generate_captures(const Node::Position<Board>&, Move::Stack&) const;
        virtual void generate_promotions(const Node::Position<Board>&, Move::Stack&) const;

        virtual size_t count(const Node::Position<Board>&) const;
        virtual size_t count_captures(const Node::Position<Board>&) const;
        virtual size_t count_promotions(const Node::Position<Board>&) const;
        virtual size_t count_mobility(const Node::Position<Board>&) const;

        virtual bool detect(const Node::Position<Board>&) const;
        virtual bool detect_captures(const Node::Position<Board>&) const;
        virtual bool detect_promotions(const Node::Position<Board>&) const;
};

}       // namespace Generate
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "State.hpp"
