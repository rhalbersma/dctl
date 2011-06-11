#pragma once
#include "Template.h"
#include "../Move/Types.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/TemplateTricks.h"
#include "../../Variants/Rules.h"

namespace tree {

namespace node { template<typename> class Position; }
namespace move { class Stack; }

namespace generate {

template<bool Color, typename Rules, typename Board> 
class Template<Color, node::Pieces::KING, move::MOVES, Rules, Board>
{
public:
        static void generate(const node::Position<Board>&, move::Stack&);
        static void generate_promotions(const node::Position<Board>&, move::Stack&);

        static size_t count(const node::Position<Board>&);
        static size_t count_promotions(const node::Position<Board>&);
        
        static bool detect(const node::Position<Board>&);
        static bool detect_promotions(const node::Position<Board>&);

private:
        // implementation
        
        // tag dispatching for restrictions on consecutive moves with the same king
        static void generate_serial(BitBoard, BitBoard, move::Stack&);
        static void generate_serial(BitBoard, BitBoard, move::Stack&, Int2Type<true>);
        static void generate_serial(BitBoard, BitBoard, move::Stack&, Int2Type<false>);
        
        static void generate_dirs(BitBoard, BitBoard, move::Stack&);
        
        // tag dispatching based on king range
        template<size_t> static void generate_dir(BitBoard, BitBoard, move::Stack&);
        template<size_t> static void generate_dir(BitBoard, BitBoard, move::Stack&, Int2Type<variants::RANGE_1>);
        template<size_t> static void generate_dir(BitBoard, BitBoard, move::Stack&, Int2Type<variants::RANGE_N>);

        static size_t count_dirs(BitBoard, BitBoard);
        
        // tag dispatching based on king range
        template<size_t> static size_t count_dir(BitBoard, BitBoard);
        template<size_t> static size_t count_dir(BitBoard, BitBoard, Int2Type<variants::RANGE_1>);
        template<size_t> static size_t count_dir(BitBoard, BitBoard, Int2Type<variants::RANGE_N>);

        static bool detect_dirs(BitBoard, BitBoard);
        template<size_t> static bool detect_dir(BitBoard, BitBoard);
};

}       // namespace generate
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "KingMoves.hpp"
