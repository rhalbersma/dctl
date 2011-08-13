#pragma once
#include "../node/Stack.h"

namespace dctl {

template<typename> class Position;

namespace successors {

template<typename Rules, typename Board> 
class GenerateInterface
{
public:
        // non-virtual interface
        void generate(const Position<Board>&, Stack&) const;
        void generate_captures(const Position<Board>&, Stack&) const;
        void generate_reverse(const Position<Board>&, Stack&) const;
        void generate_promotions(const Position<Board>&, Stack&) const;

        int count(const Position<Board>&) const;
        int count_captures(const Position<Board>&) const;
        int count_reverse(const Position<Board>&) const;
        int count_promotions(const Position<Board>&) const;
        int count_mobility(const Position<Board>&) const;

        bool detect(const Position<Board>&) const;
        bool detect_captures(const Position<Board>&) const;
        bool detect_reverse(const Position<Board>&) const;
        bool detect_promotions(const Position<Board>&) const;

protected:
        // destructor
        ~GenerateInterface() {};

private:
        // pure virtual implementation
        virtual void do_generate(const Position<Board>&, Stack&) const = 0;
        virtual void do_generate_captures(const Position<Board>&, Stack&) const = 0;
        virtual void do_generate_reverse(const Position<Board>&, Stack&) const = 0;
        virtual void do_generate_promotions(const Position<Board>&, Stack&) const = 0;

        virtual int do_count(const Position<Board>&) const = 0;
        virtual int do_count_captures(const Position<Board>&) const = 0;
        virtual int do_count_reverse(const Position<Board>&) const = 0;
        virtual int do_count_promotions(const Position<Board>&) const = 0;
        virtual int do_count_mobility(const Position<Board>&) const = 0;

        virtual bool do_detect(const Position<Board>&) const = 0;
        virtual bool do_detect_captures(const Position<Board>&) const = 0;
        virtual bool do_detect_reverse(const Position<Board>&) const = 0;
        virtual bool do_detect_promotions(const Position<Board>&) const = 0;
};

}       // namespace successors
}       // namespace dctl

// include template definitions inside header
#include "GenerateInterface.hpp"
