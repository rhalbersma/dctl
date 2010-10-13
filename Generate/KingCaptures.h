#pragma once
#include "../Board/Board.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
template<typename, typename> class Propagate;

class KingCaptures
{
public:
        // independent of side to move
        template<typename R, typename B> static void generate(const Position<B>&, Propagate<R, B>&);
        template<typename R, typename B> static size_t count(const Position<B>&);
        template<typename R, typename B> static bool detect(const Position<B>&);

        // dependent on side to move
        template<bool, typename R, typename B> static void generate(const Position<B>&, Propagate<R, B>&);
        template<bool, typename R, typename B> static size_t count(const Position<B>&);
        template<bool, typename R, typename B> static bool detect(const Position<B>&);

        template<bool, size_t, typename R, typename B> static bool promotions_en_passant(BitBoard, Propagate<R, B>&);

private:
        // implementation
        template<bool, typename R, typename B> static void generate_precede(const Position<B>&, Propagate<R, B>&);
        template<bool, typename R, typename B> static void generate_precede(const Position<B>&, Propagate<R, B>&, Int2Type<true>);
        template<bool, typename R, typename B> static void generate_precede(const Position<B>&, Propagate<R, B>&, Int2Type<false>);

        template<bool, typename R, typename B> static void generate_serial(BitBoard, Propagate<R, B>&);

        template<bool, typename R, typename B> static void generate_dirs(BitBoard, Propagate<R, B>&);
        template<bool, typename R, typename B> static void generate_dirs(BitBoard, Propagate<R, B>&, Int2Type<DIRS_4>);
        template<bool, typename R, typename B> static void generate_dirs(BitBoard, Propagate<R, B>&, Int2Type<DIRS_8>);

        template<bool, size_t, typename R, typename B> static void generate_dir(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static void generate_dir(BitBoard, Propagate<R, B>&, Int2Type<RANGE_1>);
        template<bool, size_t, typename R, typename B> static void generate_dir(BitBoard, Propagate<R, B>&, Int2Type<RANGE_N>);

        template<bool, size_t, typename R, typename B> static void generate_next(BitBoard, Propagate<R, B>&);

        template<bool, size_t, typename R, typename B> static bool scan_next(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static bool scan_next(BitBoard, Propagate<R, B>&, Int2Type<RANGE_1>);
        template<bool, size_t, typename R, typename B> static bool scan_next(BitBoard, Propagate<R, B>&, Int2Type<RANGE_N>);

        template<bool, size_t, typename R, typename B> static bool scan_long(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static bool scan_long(BitBoard, Propagate<R, B>&, Int2Type<false>);
        template<bool, size_t, typename R, typename B> static bool scan_long(BitBoard, Propagate<R, B>&, Int2Type<true>);

        template<bool, size_t, typename R, typename B> static bool scan_reverse(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static bool scan_forward(BitBoard, Propagate<R, B>&);

        template<bool, size_t, typename R, typename B> static bool scan_dirs(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static bool scan_dirs(BitBoard, Propagate<R, B>&, Int2Type<DIRS_4>);
        template<bool, size_t, typename R, typename B> static bool scan_dirs(BitBoard, Propagate<R, B>&, Int2Type<DIRS_8>);

        template<bool, size_t, typename R, typename B> static bool scan_dir(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static bool scan_dir(BitBoard, Propagate<R, B>&, Int2Type<RANGE_1>);
        template<bool, size_t, typename R, typename B> static bool scan_dir(BitBoard, Propagate<R, B>&, Int2Type<RANGE_N>);

        template<bool, size_t, typename R, typename B> static bool scan(BitBoard, Propagate<R, B>&);

        template<bool, typename, typename> static bool detect_dirs(BitBoard, BitBoard, BitBoard);
        template<bool, typename, typename> static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<DIRS_4>);
        template<bool, typename, typename> static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<DIRS_8>);

        template<size_t, typename, typename> static bool detect_dir(BitBoard, BitBoard, BitBoard);
        template<size_t, typename> static bool detect_dir(BitBoard, BitBoard, BitBoard, Int2Type<RANGE_1>);
        template<size_t, typename> static bool detect_dir(BitBoard, BitBoard, BitBoard, Int2Type<RANGE_N>);
};

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "KingCaptures.hpp"
