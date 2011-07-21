#pragma once
#include <cstddef>
#include "TemplateMethodInterface.h"
#include "../node/Stack.h"

namespace tree {

template<typename> class Position;

namespace generate {

template<bool Color, int Material, typename Rules, typename Board> 
class TemplateMethod: public TemplateMethodInterface<Rules, Board>
{
public:		
        TemplateMethod() {};

        virtual void generate(const Position<Board>&, Stack&) const;
        virtual void generate_captures(const Position<Board>&, Stack&) const;
        virtual void generate_reverse(const Position<Board>&, Stack&) const;
        virtual void generate_promotions(const Position<Board>&, Stack&) const;

        virtual size_t count(const Position<Board>&) const;
        virtual size_t count_captures(const Position<Board>&) const;
        virtual size_t count_reverse(const Position<Board>&) const;
        virtual size_t count_promotions(const Position<Board>&) const;
        virtual size_t count_mobility(const Position<Board>&) const;

        virtual bool detect(const Position<Board>&) const;
        virtual bool detect_captures(const Position<Board>&) const;
        virtual bool detect_reverse(const Position<Board>&) const;
        virtual bool detect_promotions(const Position<Board>&) const;
};

}       // namespace generate
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "TemplateMethod.hpp"
