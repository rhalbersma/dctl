#pragma once
#include "../Board/Board.h"
#include "../Variant/Rules.h"
#include "../Variant/CaptureValue.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"
#include "../Utilities/VectorArray.h"
#include "../Position/Position.h"
#include "../Position/Move.h"
#include "../Position/Pieces.h"

template<typename Rules, typename Board>
class Propagate: private Board
{
public:
        // constructors
        explicit Propagate(const Position<Board>&);
        Propagate(const Position<Board>&, const Pieces&);

        // views
        BitBoard path(void) const;
        template<size_t> BitBoard path(void) const;
        template<size_t> BitBoard targets(void) const;
        template<size_t> BitBoard man_movers(void) const;
        template<size_t> BitBoard man_jumpers(void) const;

        // unary predicates
        template<bool> bool is_promotion(BitBoard) const;
        bool is_promotion(void) const;

        // binary predicates
        bool current_greater_equal_best(void) const;
        bool current_not_equal_to_best(void) const;

        size_t size(void) const;
        const Pieces operator[](size_t) const;

        // modifiers before a capture
        void toggle_opponent_kings(void);                               // men that cannot capture kings
        void toggle_with_king(void);                                    // king capture precedence
        void toggle_promotion(void);                                    // promotion en-passant

        // modifiers during a capture
        void launch(BitBoard);
        void finish(BitBoard);
        void make(BitBoard);
        void undo(BitBoard);
        void improve_best(void);

        // modifiers after a capture
        template<bool> void add_king_move(BitBoard);
        template<bool> void add_man_move(BitBoard, BitBoard);
        template<bool> void add_man_capture(BitBoard);
        template<bool, size_t> void add_king_capture(BitBoard);

private:
        BitBoard captured_targets(void) const;

        // tag dispatching based on whether men can capture kings
        BitBoard captured_kings(BitBoard) const;
        BitBoard captured_kings(BitBoard, Int2Type<false>) const;
        BitBoard captured_kings(BitBoard, Int2Type<true>) const;

        template<bool> BitBoard promotions(BitBoard) const;

        // tag dispatching based on promotion condition
        void init_promotion(void);
        void init_promotion(Int2Type<Variant::PROMOTE_BR>);
        void init_promotion(Int2Type<Variant::PROMOTE_EP>);

        // tag dispatching on capture removal
        void make(BitBoard, Int2Type<Variant::REMOVE_1>);
        void make(BitBoard, Int2Type<Variant::REMOVE_N>);

        // tag dispatching on capture removal
        void undo(BitBoard, Int2Type<Variant::REMOVE_1>);
        void undo(BitBoard, Int2Type<Variant::REMOVE_N>);

        // tag dispatching based on ambiguity of man captures
        template<bool> void add_man_capture(BitBoard, Int2Type<false>);
        template<bool> void add_man_capture(BitBoard, Int2Type<true>);

        // tag dispatching based on king halt after final capture
        template<bool, size_t> void add_king_capture(BitBoard, Int2Type<Variant::HALT_K>);
        template<bool, size_t> void add_king_capture(BitBoard, Int2Type<Variant::HALT_1>);
        template<bool, size_t> void add_king_capture(BitBoard, Int2Type<Variant::HALT_N>);

        // tag dispatching based on promotion condition
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, bool);
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, Int2Type<Variant::PROMOTE_BR>);
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, Int2Type<Variant::PROMOTE_EP>);

        // implementation
	static const bool TOGGLE = true;

        // representation
        BitBoard initial_targets_;                                     // targets at the start of a capture
        BitBoard remaining_targets_;                                   // targets at the end of a capture
        BitBoard not_occupied_;
        BitBoard opponent_kings_;
        BitBoard from_sq_;                                             
        CaptureValue current_capture_;
        CaptureValue best_capture_;
        bool promotion_;                                               // promotion en-passant
        Move::List move_list_;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Propagate.hpp"
