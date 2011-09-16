#pragma once
#include <boost/config.hpp>
#include "Value.hpp"
#include "../node/Stack.hpp"
#include "../rules/Rules.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/TemplateTricks.hpp"

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

        // tag dispatching on whether men can capture kings
        BitBoard captured_king_targets(BitBoard) const;
        BitBoard captured_king_targets(BitBoard, Int2Type<false>) const;
        BitBoard captured_king_targets(BitBoard, Int2Type<true >) const;

        // tag dispatching on capture removal
        void make(BitBoard, Int2Type<rules::remove_ep>);
        void make(BitBoard, Int2Type<rules::remove_af>);

        // tag dispatching on capture removal
        void undo(BitBoard, Int2Type<rules::remove_ep>);
        void undo(BitBoard, Int2Type<rules::remove_af>);

        // tag dispatching on ambiguity of man captures
        template<bool> void add_man_capture(BitBoard, Stack&, Int2Type<false>);
        template<bool> void add_man_capture(BitBoard, Stack&, Int2Type<true >);

        // tag dispatching on king halt after final capture
        template<bool, int> void add_king_capture(BitBoard, Stack&, Int2Type<rules::halt_1K>);
        template<bool, int> void add_king_capture(BitBoard, Stack&, Int2Type<rules::halt_1 >);
        template<bool, int> void add_king_capture(BitBoard, Stack&, Int2Type<rules::halt_N >);

        // tag dispatching on promotion condition
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, bool, Stack&);
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, Stack&, Int2Type<rules::promote_ep>);
        template<bool> void add_king_capture(BitBoard, BitBoard, BitBoard, Stack&, Int2Type<rules::promote_af>);

        // implementation
	BOOST_STATIC_CONSTANT(auto, TOGGLE = true);

        // representation
        BitBoard initial_targets_;      // targets before a capture
        BitBoard remaining_targets_;    // targets after a capture
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
