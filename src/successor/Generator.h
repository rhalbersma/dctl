#pragma once
#include "GeneratorInterface.h"
#include "../node/Stack.h"

namespace dctl {

template<typename> class Position;

namespace successor {

template<bool Color, int Material, typename Rules, typename Board> 
class Generator
: 
        public GeneratorInterface<Rules, Board>
{
private:		
        virtual void do_generate_legal(const Position<Board>&, Stack&) const;
        virtual void do_generate_moves(const Position<Board>&, Stack&) const;
        virtual void do_generate_jumps(const Position<Board>&, Stack&) const;
        virtual void do_generate_reverse(const Position<Board>&, Stack&) const;
        virtual void do_generate_promotions(const Position<Board>&, Stack&) const;

        virtual int do_count_legal(const Position<Board>&) const;
        virtual int do_count_moves(const Position<Board>&) const;
        virtual int do_count_jumps(const Position<Board>&) const;
        virtual int do_count_reverse(const Position<Board>&) const;
        virtual int do_count_promotions(const Position<Board>&) const;

        virtual bool do_detect_legal(const Position<Board>&) const;
        virtual bool do_detect_moves(const Position<Board>&) const;
        virtual bool do_detect_jumps(const Position<Board>&) const;
        virtual bool do_detect_reverse(const Position<Board>&) const;
        virtual bool do_detect_promotions(const Position<Board>&) const;
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "Generator.hpp"
