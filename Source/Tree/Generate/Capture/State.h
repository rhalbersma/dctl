#pragma once
#include "Value.h"
#include "../../Node/Stack.h"
#include "../../../Geometry/Board.h"
#include "../../../Variants/Rules.h"
#include "../../../Utilities/IntegerTypes.h"
#include "../../../Utilities/TemplateTricks.h"
#include "../../../Utilities/VectorArray.h"

namespace tree {

namespace node { 
        template<typename> class Position; 
        class Pieces;
}
        
namespace generate {
namespace capture {

template<typename Rules, typename Board>
class State//: private Board
{
public:
        // constructors
        explicit State(const node::Position<Board>&);

        // views
        template<size_t> BitBoard targets(void) const;
        template<size_t> BitBoard path(void) const;
        BitBoard path(void) const;

        // unary predicates
        template<bool> bool is_promotion(BitBoard) const;
        bool is_promotion(void) const;

        // binary predicates
        bool current_greater_equal_best(void) const;
        bool current_not_equal_to_best(void) const;

        // modifiers before a capture
        void toggle_king_targets(void);                                 // men that cannot capture kings
        void toggle_with_king(void);                                    // king capture precedence
        void toggle_promotion(void);                                    // promotion en-passant

        // modifiers during a capture
        void launch(BitBoard);
        void finish(BitBoard);
        void make(BitBoard);
        void undo(BitBoard);
        void improve_best(node::Stack*);

        // modifiers after a capture
        template<bool> void add_man_capture(BitBoard, node::Stack*);
        template<bool, size_t> void add_king_capture(BitBoard, node::Stack*);

private:
        BitBoard captured_targets(void) const;

        // tag dispatching based on whether men can capture kings
        BitBoard captured_king_targets(BitBoard) const;
        BitBoard captured_king_targets(BitBoard, Int2Type<false>) const;
        BitBoard captured_king_targets(BitBoard, Int2Type<true >) const;

        template<bool> BitBoard promotions(BitBoard) const;

        // tag dispatching on capture removal
        void make(BitBoard, Int2Type<variants::REMOVE_1>);
        void make(BitBoard, Int2Type<variants::REMOVE_N>);

        // tag dispatching on capture removal
        void undo(BitBoard, Int2Type<variants::REMOVE_1>);
        void undo(BitBoard, Int2Type<variants::REMOVE_N>);

        // tag dispatching based on ambiguity of man captures
        template<bool> void add_man_capture(BitBoard, node::Stack*, Int2Type<false>);
        template<bool> void add_man_capture(BitBoard, node::Stack*, Int2Type<true >);

        // tag dispatching based on king halt after final capture
        template<bool, size_t> void add_king_capture(BitBoard, node::Stack*, Int2Type<variants::HALT_K>);
        template<bool, size_t> void add_king_capture(BitBoard, node::Stack*, Int2Type<variants::HALT_1>);
        template<bool, size_t> void add_king_capture(BitBoard, node::Stack*, Int2Type<variants::HALT_N>);

        // tag dispatching based on promotion condition
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, bool, node::Stack*);
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, node::Stack*, Int2Type<variants::PROMOTE_BR>);
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, node::Stack*, Int2Type<variants::PROMOTE_EP>);

        // implementation
	static const bool TOGGLE = true;

        // representation
        Value current_;
        Value best_;
        BitBoard initial_targets_;                                     // targets at the start of a capture
        BitBoard remaining_targets_;                                   // targets at the end of a capture
        BitBoard not_occupied_;
        BitBoard king_targets_;
        BitBoard from_sq_;                                             
};

}       // namespace capture
}       // namespace generate
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "State.hpp"
