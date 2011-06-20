#pragma once
#include <cstddef>
#include "../Node/Stack.h"

namespace tree {

namespace node { template<typename> class Position; }

namespace generate {

template<typename Rules, typename Board> 
class TemplateMethodInterface
{
public:
        // interface
        virtual void generate(const node::Position<Board>&, node::Stack*) const = 0;
        virtual void generate_captures(const node::Position<Board>&, node::Stack*) const = 0;
        virtual void generate_reverse(const node::Position<Board>&, node::Stack*) const = 0;
        virtual void generate_promotions(const node::Position<Board>&, node::Stack*) const = 0;

        virtual size_t count(const node::Position<Board>&) const = 0;
        virtual size_t count_captures(const node::Position<Board>&) const = 0;
        virtual size_t count_reverse(const node::Position<Board>&) const = 0;
        virtual size_t count_promotions(const node::Position<Board>&) const = 0;
        virtual size_t count_mobility(const node::Position<Board>&) const = 0;

        virtual bool detect(const node::Position<Board>&) const = 0;        
        virtual bool detect_captures(const node::Position<Board>&) const = 0;
        virtual bool detect_reverse(const node::Position<Board>&) const = 0;        
        virtual bool detect_promotions(const node::Position<Board>&) const = 0;

        // virtual destructor
        virtual ~TemplateMethodInterface(void) throw() {};
};

}       // namespace generate
}       // namespace tree
