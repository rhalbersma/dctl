#pragma once
#include <boost/config.hpp>
#include "Value.h"
#include "../node/Stack.h"
#include "../rules/Rules.h"
#include "../utility/IntegerTypes.h"
#include "../utility/TemplateTricks.h"

namespace dctl {

template<typename> class Position; 
        
namespace capture {

template<typename Rules, typename Board>
class State
{
public:
        // constructors
        explicit State(const Position<Board>&);

        // views
        template<int> BitBoard targets() const;
        template<int> BitBoard path() const;
        BitBoard path() const;
        const Value<Rules>& current() const;
        const Value<Rules>& best() const;

        // unary predicates
        template<bool> bool is_promotion(BitBoard) const;
        bool is_promotion() const;

        // modifiers before a capture
        void toggle_king_targets();     // men that cannot capture kings
        void toggle_with_king();        // king capture precedence
        void toggle_promotion();        // promotion en-passant

        // modifiers during a capture
        void launch(BitBoard);
        void finish(BitBoard);
        void make(BitBoard);
        void undo(BitBoard);
        Value<Rules>& best();

        // modifiers after a capture
        template<bool> void add_man_capture(BitBoard, Stack&);
        template<bool, int> void add_king_capture(BitBoard, Stack&);

private:
        BitBoard captured_targets() const;

        // tag dispatching based on whether men can capture kings
        BitBoard captured_king_targets(BitBoard) const;
        BitBoard captured_king_targets(BitBoard, Int2Type<false>) const;
        BitBoard captured_king_targets(BitBoard, Int2Type<true >) const;

        // tag dispatching on capture removal
        void make(BitBoard, Int2Type<rules::REMOVE_1>);
        void make(BitBoard, Int2Type<rules::REMOVE_N>);

        // tag dispatching on capture removal
        void undo(BitBoard, Int2Type<rules::REMOVE_1>);
        void undo(BitBoard, Int2Type<rules::REMOVE_N>);

        // tag dispatching based on ambiguity of man captures
        template<bool> void add_man_capture(BitBoard, Stack&, Int2Type<false>);
        template<bool> void add_man_capture(BitBoard, Stack&, Int2Type<true >);

        // tag dispatching based on king halt after final capture
        template<bool, int> void add_king_capture(BitBoard, Stack&, Int2Type<rules::HALT_K>);
        template<bool, int> void add_king_capture(BitBoard, Stack&, Int2Type<rules::HALT_1>);
        template<bool, int> void add_king_capture(BitBoard, Stack&, Int2Type<rules::HALT_N>);

        // tag dispatching based on promotion condition
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, bool, Stack&);
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, Stack&, Int2Type<rules::PROMOTE_BR>);
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, Stack&, Int2Type<rules::PROMOTE_EP>);

        // implementation
	BOOST_STATIC_CONSTANT(auto, TOGGLE = true);

        // representation
        BitBoard initial_targets_;      // targets at the start of a capture
        BitBoard remaining_targets_;    // targets at the end of a capture
        BitBoard not_occupied_;
        BitBoard king_targets_;
        BitBoard from_sq_;                                             
        Value<Rules> current_;
        Value<Rules> best_;
};

}       // namespace capture
}       // namespace dctl

// include template definitions inside header
#include "State.hpp"
