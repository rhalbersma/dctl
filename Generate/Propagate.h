#pragma once
#include "../Board/Board.h"
#include "../Rules/Rules.h"
#include "../Rules/CaptureValue.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"
#include "../Containers/Generic/VectorBucket.h"
#include "../Position/Position.h"
#include "../Position/Move.h"
#include "../Position/Reversible/Pieces.h"

template<typename Rules, typename Board>
class Propagate: private Board
{
public:
        // constructors
        explicit Propagate(const Position<Board>&);
        Propagate(const Position<Board>&, bool);

        // views
        BitBoard path(void) const;
        template<size_t> BitBoard path(void) const;
        template<size_t> BitBoard movers(void) const;
        template<size_t> BitBoard targets(void) const;
        template<size_t> BitBoard jumpers(void) const;

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
        BitBoard captured_kings(BitBoard) const;
        BitBoard captured_kings(BitBoard, Int2Type<false>) const;       // men that cannot capture kings
        BitBoard captured_kings(BitBoard, Int2Type<true>) const;        // men that can capture kings

        template<bool> BitBoard promotions(BitBoard) const;

        void init_promotion(void);
        void init_promotion(Int2Type<PROMOTE_BR>) {};                   // men that promote on the back row
        void init_promotion(Int2Type<PROMOTE_EP>);

        void make(BitBoard, Int2Type<REMOVE_1>);
        void make(BitBoard, Int2Type<REMOVE_N>);
        void undo(BitBoard, Int2Type<REMOVE_1>);
        void undo(BitBoard, Int2Type<REMOVE_N>);

        template<bool> void add_man_capture(BitBoard, Int2Type<false>);
        template<bool> void add_man_capture(BitBoard, Int2Type<true>);

        template<bool, size_t> void add_king_capture(BitBoard, Int2Type<HALT_K>);
        template<bool, size_t> void add_king_capture(BitBoard, Int2Type<HALT_1>);
        template<bool, size_t> void add_king_capture(BitBoard, Int2Type<HALT_N>);

        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, bool);
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, Int2Type<PROMOTE_BR>);
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, Int2Type<PROMOTE_EP>);

        // implementation
	static const bool TOGGLE = true;

        // representation
        BitBoard d_initial_targets;                                     // targets at the start of a capture
        BitBoard d_remaining_targets;                                   // targets at the end of a capture
        BitBoard d_not_occupied;
        BitBoard d_opponent_kings;
        BitBoard d_from_sq;                                             
        CaptureValue d_current_capture;
        CaptureValue d_best_capture;
        bool d_promotion;                                               // promotion en-passant
        Move::List d_move_list;
};

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "Propagate.hpp"
