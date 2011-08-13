#pragma once
#include <boost/utility.hpp>    // boost::noncopyable            
#include "GenerateInterface.h"
#include "../node/Material.h"
#include "../node/Side.h"
#include "../node/Stack.h"

namespace dctl {

template<typename> class Position;

template<typename Rules, typename Board>
class Successors
:
        private boost::noncopyable      // enforce singleton semantics
{
public:
        // interface
        static void generate(const Position<Board>&, Stack&);
        static void generate_captures(const Position<Board>&, Stack&);
        static void generate_reverse(const Position<Board>&, Stack&);
        static void generate_promotions(const Position<Board>&, Stack&);

        static int count(const Position<Board>&);
        static int count_captures(const Position<Board>&);
        static int count_reverse(const Position<Board>&);
        static int count_promotions(const Position<Board>&);

        template<bool> static int count_mobility(const Position<Board>&);

        static bool detect(const Position<Board>&);
        static bool detect_captures(const Position<Board>&);
        static bool detect_reverse(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);

private:
        // implementation
        static const successors::GenerateInterface<Rules, Board>* select(const Position<Board>&);
        template<bool> static const successors::GenerateInterface<Rules, Board>* select(const Position<Board>&);

        static int state(const Position<Board>&);
        template<bool> static int state(const Position<Board>&);
        static int state(bool, BitBoard, BitBoard);

        static bool invariant(const Position<Board>&, int);
        static bool reverse_invariant(const Position<Board>&, int);
        static bool promotions_invariant(const Position<Board>&, int);

        // representation
        typedef const successors::GenerateInterface<Rules, Board>* Generator;
        typedef const Generator GeneratorArray[8];
        static GeneratorArray& instance();
};

}       // namespace dctl

// include template definitions inside header
#include "Successors.hpp"
