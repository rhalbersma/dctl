#pragma once
#include "../Rules/Rules.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
template<typename, typename> class Propagate;

class ManKingCaptures
{
public:
        // independent of side to move
        template<typename R, typename B> static void generate(const Position<B>&, Propagate<R, B>&);
        template<typename, typename B> static size_t count(const Position<B>&);
        template<typename, typename B> static bool detect(const Position<B>&);

        // dependent on side to move
        template<bool, typename R, typename B> static void generate(const Position<B>&, Propagate<R, B>&);
        template<bool, typename, typename B> static size_t count(const Position<B>&);
        template<bool, typename, typename B> static bool detect(const Position<B>&);

private:
        // implementation
        template<bool, typename R, typename B> static void generate_precede(const Position<B>&, Propagate<R, B>&);
        template<bool, typename R, typename B> static void generate_precede(const Position<B>&, Propagate<R, B>&, Int2Type<false>);
        template<bool, typename R, typename B> static void generate_precede(const Position<B>&, Propagate<R, B>&, Int2Type<true>);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "ManKingCaptures.hpp"
