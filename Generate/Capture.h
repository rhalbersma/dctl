#pragma once
#include "../Board/Board.h"
#include "../Variant/Rules.h"
#include "../Variant/CaptureValue.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"
#include "../Utilities/VectorArray.h"
#include "../Position/Position.h"
#include "../Position/Pieces.h"

class MoveList;

template<typename Rules, typename Board>
class Capture: private Board
{
public:
        // constructors
        explicit Capture(const Position<Board>&);

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

        size_t size(void) const;
        const Pieces& operator[](size_t) const;

        // modifiers before a capture
        void toggle_king_targets(void);                               // men that cannot capture kings
        void toggle_with_king(void);                                    // king capture precedence
        void toggle_promotion(void);                                    // promotion en-passant

        // modifiers during a capture
        void launch(BitBoard);
        void finish(BitBoard);
        void make(BitBoard);
        void undo(BitBoard);
        void improve_best(MoveList&);

        // modifiers after a capture
        template<bool> void add_man_capture(BitBoard, MoveList&);
        template<bool, size_t> void add_king_capture(BitBoard, MoveList&);

private:
        BitBoard captured_targets(void) const;

        // tag dispatching based on whether men can capture kings
        BitBoard captured_kings(BitBoard) const;
        BitBoard captured_kings(BitBoard, Int2Type<false>) const;
        BitBoard captured_kings(BitBoard, Int2Type<true>) const;

        template<bool> BitBoard promotions(BitBoard) const;

        // tag dispatching on capture removal
        void make(BitBoard, Int2Type<Variant::REMOVE_1>);
        void make(BitBoard, Int2Type<Variant::REMOVE_N>);

        // tag dispatching on capture removal
        void undo(BitBoard, Int2Type<Variant::REMOVE_1>);
        void undo(BitBoard, Int2Type<Variant::REMOVE_N>);

        // tag dispatching based on ambiguity of man captures
        template<bool> void add_man_capture(BitBoard, MoveList&, Int2Type<false>);
        template<bool> void add_man_capture(BitBoard, MoveList&, Int2Type<true>);

        // tag dispatching based on king halt after final capture
        template<bool, size_t> void add_king_capture(BitBoard, MoveList&, Int2Type<Variant::HALT_K>);
        template<bool, size_t> void add_king_capture(BitBoard, MoveList&, Int2Type<Variant::HALT_1>);
        template<bool, size_t> void add_king_capture(BitBoard, MoveList&, Int2Type<Variant::HALT_N>);

        // tag dispatching based on promotion condition
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, bool, MoveList&);
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, MoveList&, Int2Type<Variant::PROMOTE_BR>);
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, MoveList&, Int2Type<Variant::PROMOTE_EP>);

        // implementation
	static const bool TOGGLE = true;

        // representation
        CaptureValue current_;
        CaptureValue best_;
        BitBoard initial_targets_;                                     // targets at the start of a capture
        BitBoard remaining_targets_;                                   // targets at the end of a capture
        BitBoard not_occupied_;
        BitBoard king_targets_;
        BitBoard from_sq_;                                             
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Capture.hpp"
