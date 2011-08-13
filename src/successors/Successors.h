#pragma once
#include "GeneratorInterface.h"
#include "../node/Material.h"
#include "../node/Side.h"
#include "../node/Stack.h"
#include "../utils/TemplateTricks.h"

namespace dctl {

template<typename> class Position;

template<typename Rules, typename Board>
class Successors
:
        private utils::nonconstructible // enforce static semantics
{
public:
        // interface
        static void generate(const Position<Board>&, Stack&);
        static void generate_moves(const Position<Board>&, Stack&);
        static void generate_jumps(const Position<Board>&, Stack&);
        static void generate_reverse(const Position<Board>&, Stack&);
        static void generate_promotions(const Position<Board>&, Stack&);

        static int count(const Position<Board>&);
        static int count_moves(const Position<Board>&);
        static int count_jumps(const Position<Board>&);
        static int count_reverse(const Position<Board>&);
        static int count_promotions(const Position<Board>&);

        static bool detect(const Position<Board>&);
        static bool detect_moves(const Position<Board>&);
        static bool detect_jumps(const Position<Board>&);
        static bool detect_reverse(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);

        template<bool> static int count_mobility(const Position<Board>&);

private:
        // implementation
        static bool invariant(const Position<Board>&, int);
        static bool reverse_invariant(const Position<Board>&, int);
        static bool promotions_invariant(const Position<Board>&, int);

        static int state(const Position<Board>&);
        template<bool> static int state(const Position<Board>&);
        static int state(bool, BitBoard, BitBoard);

        typedef const successors::GeneratorInterface<Rules, Board>* Generator;
        static const Generator select(const Position<Board>&);
        template<bool> static const Generator select(const Position<Board>&);

        // representation
        typedef const Generator GeneratorArray[8];
        static GeneratorArray& instance();
};

}       // namespace dctl

// include template definitions inside header
#include "Successors.hpp"
