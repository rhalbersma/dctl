#pragma once
#include <boost/utility.hpp>
#include "GeneratorInterface.h"
#include "../node/Stack.h"

namespace dctl {

template<typename> class Position;

namespace successors {

template<bool Color, int Material, typename Rules, typename Board> 
class Generator
: 
        public GeneratorInterface<Rules, Board>,
        private boost::noncopyable      // enforce singleton semantics
{
private:		
        virtual void do_generate(const Position<Board>&, Stack&) const;
        virtual void do_generate_non_captures(const Position<Board>&, Stack&) const;
        virtual void do_generate_captures(const Position<Board>&, Stack&) const;
        virtual void do_generate_reverse(const Position<Board>&, Stack&) const;
        virtual void do_generate_promotions(const Position<Board>&, Stack&) const;

        virtual int do_count(const Position<Board>&) const;
        virtual int do_count_non_captures(const Position<Board>&) const;
        virtual int do_count_captures(const Position<Board>&) const;
        virtual int do_count_reverse(const Position<Board>&) const;
        virtual int do_count_promotions(const Position<Board>&) const;

        virtual bool do_detect(const Position<Board>&) const;
        virtual bool do_detect_non_captures(const Position<Board>&) const;
        virtual bool do_detect_captures(const Position<Board>&) const;
        virtual bool do_detect_reverse(const Position<Board>&) const;
        virtual bool do_detect_promotions(const Position<Board>&) const;
};

}       // namespace successors
}       // namespace dctl

// include template definitions inside header
#include "Generator.hpp"
