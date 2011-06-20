#pragma once
#include <cstddef>
#include "TemplateMethodInterface.h"
#include "../Node/Stack.h"

namespace tree {

namespace node { template<typename> class Position; }

namespace generate {

template<bool Color, int Material, typename Rules, typename Board> 
class TemplateMethod: public TemplateMethodInterface<Rules, Board>
{
public:		
        TemplateMethod(void) {};

        virtual void generate(const node::Position<Board>&, node::Stack*) const;
        virtual void generate_captures(const node::Position<Board>&, node::Stack*) const;
        virtual void generate_reverse(const node::Position<Board>&, node::Stack*) const;
        virtual void generate_promotions(const node::Position<Board>&, node::Stack*) const;

        virtual size_t count(const node::Position<Board>&) const;
        virtual size_t count_captures(const node::Position<Board>&) const;
        virtual size_t count_reverse(const node::Position<Board>&) const;
        virtual size_t count_promotions(const node::Position<Board>&) const;
        virtual size_t count_mobility(const node::Position<Board>&) const;

        virtual bool detect(const node::Position<Board>&) const;
        virtual bool detect_captures(const node::Position<Board>&) const;
        virtual bool detect_reverse(const node::Position<Board>&) const;
        virtual bool detect_promotions(const node::Position<Board>&) const;
};

}       // namespace generate
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "TemplateMethod.hpp"
