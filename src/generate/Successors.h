#pragma once
#include "TemplateMethod.h"
#include "../node/Material.h"
#include "../node/Side.h"
#include "../node/Stack.h"

namespace dctl {

template<typename> class Position;

namespace generate {
        
template<typename Rules, typename Board>
class Successors
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
        static const TemplateMethodInterface<Rules, Board>* select(const Position<Board>&);
        template<bool> static const TemplateMethodInterface<Rules, Board>* select(const Position<Board>&);

        static int state(const Position<Board>&);
        template<bool> static int state(const Position<Board>&);
        static int state(bool, BitBoard, BitBoard);

        static bool invariant(const Position<Board>&, int);
        static bool reverse_invariant(const Position<Board>&, int);
        static bool promotions_invariant(const Position<Board>&, int);

        // representation
        static const TemplateMethodInterface<Rules, Board>* const FACTORY[];

        static const TemplateMethod<Side::BLACK, Material::NONE, Rules, Board> BLACK_NONE;
        static const TemplateMethod<Side::BLACK, Material::PAWN, Rules, Board> BLACK_PAWN;
        static const TemplateMethod<Side::BLACK, Material::KING, Rules, Board> BLACK_KING;
        static const TemplateMethod<Side::BLACK, Material::BOTH, Rules, Board> BLACK_BOTH;
        static const TemplateMethod<Side::WHITE, Material::NONE, Rules, Board> WHITE_NONE;
        static const TemplateMethod<Side::WHITE, Material::PAWN, Rules, Board> WHITE_PAWN;
        static const TemplateMethod<Side::WHITE, Material::KING, Rules, Board> WHITE_KING;
        static const TemplateMethod<Side::WHITE, Material::BOTH, Rules, Board> WHITE_BOTH;
};

}       // namespace generate
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Successors.hpp"
