#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename> class Position;
template<typename, typename> class Propagate;

class ManMoves
{
public:
        // independent on side to move
        template<typename R, typename B> static void generate(const Position<B>&, Propagate<R, B>&);
        template<typename R, typename B> static void generate_promotions(const Position<B>&, Propagate<R, B>&);
        template<typename B> static size_t count(const Position<B>&);
        template<typename B> static bool detect(const Position<B>&);
        template<typename B> static bool detect_promotions(const Position<B>&);

        // dependent of side to move
        template<bool, typename R, typename B> static void generate(const Position<B>&, Propagate<R, B>&);
        template<bool, typename R, typename B> static void generate_promotions(const Position<B>&, Propagate<R, B>&);
        template<bool, typename B> static size_t count(const Position<B>&);
        template<bool, typename B> static bool detect(const Position<B>&);
        template<bool, typename B> static bool detect_promotions(const Position<B>&);

private:
        // implementation
        template<bool, typename R, typename B> static void generate_dirs(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static void generate_dir(BitBoard, Propagate<R, B>&);
        template<bool, typename B> static BitBoard promotors(const Position<B>&);

        template<bool, typename> static size_t count_dirs(BitBoard, BitBoard);
        template<size_t, typename> static size_t count_dir(BitBoard, BitBoard);

        template<bool, typename> static bool detect_dirs(BitBoard, BitBoard);
        template<size_t, typename> static bool detect_dir(BitBoard, BitBoard);
};

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "ManMoves.hpp"
