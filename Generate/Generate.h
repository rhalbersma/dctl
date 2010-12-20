#pragma once
#include "../Position/Reversible/Pieces.h"
#include "../Position/Move.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
template<typename, typename> class Propagate;

class Generate
{
public:
        // independent of side to move
        template<typename R, typename B> static void generate(const Position<B>&, Propagate<R, B>&);
        template<typename R, typename B> static void generate_captures(const Position<B>&, Propagate<R, B>&);
        template<typename R, typename B> static void generate_promotions(const Position<B>&, Propagate<R, B>&);

        template<typename, typename B> static size_t count(const Position<B>&);

        template<typename, typename B> static bool detect(const Position<B>&);
        template<typename, typename B> static bool detect_captures(const Position<B>&);
        template<typename, typename B> static bool detect_promotions(const Position<B>&);

        template<typename, typename B> static bool invariant(const Position<B>&, size_t);

        // dependent on side to move
        template<bool, typename R, typename B> static void generate(const Position<B>&, Propagate<R, B>&);
        template<bool, typename R, typename B> static void generate_captures(const Position<B>&, Propagate<R, B>&);
        template<bool, typename R, typename B> static void generate_promotions(const Position<B>&, Propagate<R, B>&);

        template<bool, typename, typename B> static size_t count(const Position<B>&);
        template<bool, typename, typename B> static size_t count_moves(const Position<B>&);

        template<bool, typename, typename B> static bool detect(const Position<B>&);
        template<bool, typename, typename B> static bool detect_captures(const Position<B>&);
        template<bool, typename, typename B> static bool detect_promotions(const Position<B>&);

        template<bool, typename, typename B> static bool invariant(const Position<B>&, size_t);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Generate.hpp"
