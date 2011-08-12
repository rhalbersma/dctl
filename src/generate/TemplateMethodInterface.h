#pragma once
#include "../node/Stack.h"

namespace dctl {

template<typename> class Position;

namespace generate {

template<typename Rules, typename Board> 
class TemplateMethodInterface
{
public:
        // virtual destructor
        virtual ~TemplateMethodInterface() {};

        // interface
        virtual void generate(const Position<Board>&, Stack&) const = 0;
        virtual void generate_captures(const Position<Board>&, Stack&) const = 0;
        virtual void generate_reverse(const Position<Board>&, Stack&) const = 0;
        virtual void generate_promotions(const Position<Board>&, Stack&) const = 0;

        virtual int count(const Position<Board>&) const = 0;
        virtual int count_captures(const Position<Board>&) const = 0;
        virtual int count_reverse(const Position<Board>&) const = 0;
        virtual int count_promotions(const Position<Board>&) const = 0;
        virtual int count_mobility(const Position<Board>&) const = 0;

        virtual bool detect(const Position<Board>&) const = 0;        
        virtual bool detect_captures(const Position<Board>&) const = 0;
        virtual bool detect_reverse(const Position<Board>&) const = 0;        
        virtual bool detect_promotions(const Position<Board>&) const = 0;
};

}       // namespace generate
}       // namespace dctl
