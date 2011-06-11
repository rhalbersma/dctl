#pragma once
#include <cstddef>

namespace tree {

namespace node { template<typename> class Position; }
namespace move { class Stack; }

namespace generate {

template<typename Rules, typename Board> 
class AbstractState
{
public:
        // interface
        virtual void generate(const node::Position<Board>&, move::Stack&) const = 0;
        virtual void generate_captures(const node::Position<Board>&, move::Stack&) const = 0;
        virtual void generate_promotions(const node::Position<Board>&, move::Stack&) const = 0;

        virtual size_t count(const node::Position<Board>&) const = 0;
        virtual size_t count_captures(const node::Position<Board>&) const = 0;
        virtual size_t count_promotions(const node::Position<Board>&) const = 0;
        virtual size_t count_mobility(const node::Position<Board>&) const = 0;

        virtual bool detect(const node::Position<Board>&) const = 0;        
        virtual bool detect_captures(const node::Position<Board>&) const = 0;
        virtual bool detect_promotions(const node::Position<Board>&) const = 0;

        // virtual destructor
        virtual ~AbstractState(void) {};
};

}       // namespace generate
}       // namespace tree
