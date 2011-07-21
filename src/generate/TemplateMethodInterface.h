#pragma once
#include <cstddef>
#include "../node/Stack.h"

namespace dtl {

template<typename> class Position;

namespace generate {

template<typename Rules, typename Board> 
class TemplateMethodInterface
{
public:
        // interface
        virtual void generate(const Position<Board>&, Stack&) const = 0;
        virtual void generate_captures(const Position<Board>&, Stack&) const = 0;
        virtual void generate_reverse(const Position<Board>&, Stack&) const = 0;
        virtual void generate_promotions(const Position<Board>&, Stack&) const = 0;

        virtual size_t count(const Position<Board>&) const = 0;
        virtual size_t count_captures(const Position<Board>&) const = 0;
        virtual size_t count_reverse(const Position<Board>&) const = 0;
        virtual size_t count_promotions(const Position<Board>&) const = 0;
        virtual size_t count_mobility(const Position<Board>&) const = 0;

        virtual bool detect(const Position<Board>&) const = 0;        
        virtual bool detect_captures(const Position<Board>&) const = 0;
        virtual bool detect_reverse(const Position<Board>&) const = 0;        
        virtual bool detect_promotions(const Position<Board>&) const = 0;

        // virtual destructor
        virtual ~TemplateMethodInterface() {};
};

}       // namespace generate
}       // namespace dtl
