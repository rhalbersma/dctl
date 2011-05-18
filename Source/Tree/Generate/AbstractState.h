#pragma once
#include <cstddef>

namespace Tree {

namespace Node { template<typename> class Position; }
namespace Move { class Stack; }

namespace Generate {

template<typename Rules, typename Board> 
class AbstractState
{
public:
        // interface
        virtual void generate(const Node::Position<Board>&, Move::Stack&) const = 0;
        virtual void generate_captures(const Node::Position<Board>&, Move::Stack&) const = 0;
        virtual void generate_promotions(const Node::Position<Board>&, Move::Stack&) const = 0;

        virtual size_t count(const Node::Position<Board>&) const = 0;
        virtual size_t count_captures(const Node::Position<Board>&) const = 0;
        virtual size_t count_promotions(const Node::Position<Board>&) const = 0;
        virtual size_t count_mobility(const Node::Position<Board>&) const = 0;

        virtual bool detect(const Node::Position<Board>&) const = 0;        
        virtual bool detect_captures(const Node::Position<Board>&) const = 0;
        virtual bool detect_promotions(const Node::Position<Board>&) const = 0;

        // virtual destructor
        virtual ~AbstractState(void) {};
};

}       // namespace Generate
}       // namespace Tree
