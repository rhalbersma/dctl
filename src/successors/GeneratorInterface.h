#pragma once
#include "../node/Stack.h"

namespace dctl {

template<typename> class Position;

namespace successors {

template<typename Rules, typename Board> 
class GeneratorInterface
{
public:
        // non-virtual interface
        void generate_legal(const Position<Board>&, Stack&) const;
        void generate_moves(const Position<Board>&, Stack&) const;
        void generate_jumps(const Position<Board>&, Stack&) const;
        void generate_reverse(const Position<Board>&, Stack&) const;
        void generate_promotions(const Position<Board>&, Stack&) const;

        int count_legal(const Position<Board>&) const;
        int count_moves(const Position<Board>&) const;
        int count_jumps(const Position<Board>&) const;
        int count_reverse(const Position<Board>&) const;
        int count_promotions(const Position<Board>&) const;

        bool detect_legal(const Position<Board>&) const;
        bool detect_moves(const Position<Board>&) const;
        bool detect_jumps(const Position<Board>&) const;
        bool detect_reverse(const Position<Board>&) const;
        bool detect_promotions(const Position<Board>&) const;

protected:
        // destructor
        ~GeneratorInterface() {};

private:
        // pure virtual implementation
        virtual void do_generate_legal(const Position<Board>&, Stack&) const = 0;
        virtual void do_generate_moves(const Position<Board>&, Stack&) const = 0;
        virtual void do_generate_jumps(const Position<Board>&, Stack&) const = 0;
        virtual void do_generate_reverse(const Position<Board>&, Stack&) const = 0;
        virtual void do_generate_promotions(const Position<Board>&, Stack&) const = 0;

        virtual int do_count_legal(const Position<Board>&) const = 0;
        virtual int do_count_moves(const Position<Board>&) const = 0;
        virtual int do_count_jumps(const Position<Board>&) const = 0;
        virtual int do_count_reverse(const Position<Board>&) const = 0;
        virtual int do_count_promotions(const Position<Board>&) const = 0;

        virtual bool do_detect_legal(const Position<Board>&) const = 0;
        virtual bool do_detect_moves(const Position<Board>&) const = 0;
        virtual bool do_detect_jumps(const Position<Board>&) const = 0;
        virtual bool do_detect_reverse(const Position<Board>&) const = 0;
        virtual bool do_detect_promotions(const Position<Board>&) const = 0;
};

}       // namespace successors
}       // namespace dctl

// include template definitions inside header
#include "GeneratorInterface.hpp"
