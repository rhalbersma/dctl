#pragma once
#include "../Rules/Rules.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
template<typename, typename> class Propagate;

class ManCaptures
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
        template<bool, typename R, typename B> static void generate_targets(const Position<B>&, Propagate<R, B>&);
        template<bool, typename R, typename B> static void generate_targets(const Position<B>&, Propagate<R, B>&, Int2Type<false>);
        template<bool, typename R, typename B> static void generate_targets(const Position<B>&, Propagate<R, B>&, Int2Type<true>);

        template<bool, typename R, typename B> static void generate_dirs(BitBoard, Propagate<R, B>&);
        template<bool, typename R, typename B> static void generate_dirs(BitBoard, Propagate<R, B>&, Int2Type<DIRS_2>);
        template<bool, typename R, typename B> static void generate_dirs(BitBoard, Propagate<R, B>&, Int2Type<DIRS_4>);
        template<bool, typename R, typename B> static void generate_dirs(BitBoard, Propagate<R, B>&, Int2Type<DIRS_8>);

        template<bool, size_t, typename R, typename B> static void generate_dir(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static void generate_next(BitBoard, Propagate<R, B>&);

        template<bool, size_t, typename R, typename B> static bool scan_next(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static bool scan_next(BitBoard, Propagate<R, B>&, Int2Type<PROMOTE_BR>);
        template<bool, size_t, typename R, typename B> static bool scan_next(BitBoard, Propagate<R, B>&, Int2Type<PROMOTE_EP>);

        template<bool, size_t, typename R, typename B> static bool scan_dirs(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static bool scan_dirs(BitBoard, Propagate<R, B>&, Int2Type<DIRS_2>);
        template<bool, size_t, typename R, typename B> static bool scan_dirs(BitBoard, Propagate<R, B>&, Int2Type<DIRS_4>);
        template<bool, size_t, typename R, typename B> static bool scan_dirs(BitBoard, Propagate<R, B>&, Int2Type<DIRS_8>);

        template<bool, size_t, typename R, typename B> static bool scan_dir(BitBoard, Propagate<R, B>&);
        template<bool, size_t, typename R, typename B> static bool scan(BitBoard, Propagate<R, B>&);

        template<bool, typename, typename B> static bool detect_targets(const Position<B>&);
        template<bool, typename, typename B> static bool detect_targets(const Position<B>&, Int2Type<false>);
        template<bool, typename, typename B> static bool detect_targets(const Position<B>&, Int2Type<true>);

        template<bool, typename, typename> static bool detect_dirs(BitBoard, BitBoard, BitBoard);
        template<bool, typename, typename> static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<DIRS_2>);
        template<bool, typename, typename> static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<DIRS_4>);
        template<bool, typename, typename> static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<DIRS_8>);

        template<size_t, typename> static bool detect_dir(BitBoard, BitBoard, BitBoard);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "ManCaptures.hpp"
