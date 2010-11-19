#pragma once
#include "../Rules/Rules.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
template<typename, typename> class Propagate;

class KingMoves
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
        template<bool, typename R, typename B> static void generate_serial(BitBoard, Propagate<R, B>&);
        template<bool, typename R, typename B> static void generate_serial(BitBoard, Propagate<R, B>&, Int2Type<true>);
        template<bool, typename R, typename B> static void generate_serial(BitBoard, Propagate<R, B>&, Int2Type<false>);
        template<bool, typename R, typename B> static void generate_dirs(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static void generate_dir(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static void generate_dir(BitBoard, Propagate<R, B>&, Int2Type<RANGE_1>);
        template<bool, size_t, typename R, typename B> static void generate_dir(BitBoard, Propagate<R, B>&, Int2Type<RANGE_N>);

        template<bool, typename, typename> static size_t count_dirs(BitBoard, BitBoard);
        template<size_t, typename, typename> static size_t count_dir(BitBoard, BitBoard);
        template<size_t, typename> static size_t count_dir(BitBoard, BitBoard, Int2Type<RANGE_1>);
        template<size_t, typename> static size_t count_dir(BitBoard, BitBoard, Int2Type<RANGE_N>);

        template<bool, typename> static bool detect_dirs(BitBoard, BitBoard);
        template<size_t, typename> static bool detect_dir(BitBoard, BitBoard);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "KingMoves.hpp"
