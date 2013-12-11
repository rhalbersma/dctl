#pragma once
#include <cassert>                      // assert
#include <type_traits>
#include <tuple>
#include <utility>
#include <boost/operators.hpp>          // equality_comparable, xorable
#include <dctl/node/move_fwd.hpp>
#include <dctl/node/side.hpp>
#include <dctl/rules/traits.hpp>

#include <dctl/bit/algorithm.hpp>
#include <dctl/bit/predicates.hpp>

namespace dctl {

template<class Rules, class Board>
struct Move
        // http://www.boost.org/doc/libs/1_51_0/libs/utility/operators.htm#chaining
        // use base class chaining to ensure Empty Base Optimization
:       boost::equality_comparable1< Move<Rules, Board>
,       boost::xorable1< Move<Rules, Board>
        > >
{
public:
        using board_type = Board;
        using rules_type = Rules;
        using T = typename Board::bit_type;

        // structors

        // default constructor
        Move() = default;

        // king move
        Move(int from, int dest)
        :
                from_{from},
                dest_{dest},
                is_with_king_{true}
        {
                assert(from_ != dest_);
                assert(invariant());
        }

        // pawn move
        Move(int from, int dest, bool is_promotion)
        :
                from_{from},
                dest_{dest},
                is_promotion_{is_promotion}
        {
                assert(from_ != dest_);
                assert(invariant());
        }

        // king jump
        Move(T captured_pieces, T captured_kings, int from, int dest)
        :
                captured_pieces_{captured_pieces},
                captured_kings_{captured_kings},
                from_{from},
                dest_{dest},
                is_with_king_{true},
                is_jump_{true}
        {
                assert(invariant());
        }

        // pawn jump
        Move(T captured_pieces, T captured_kings, int from, int dest, bool is_promotion)
        :
                captured_pieces_{captured_pieces},
                captured_kings_{captured_kings},
                from_{from},
                dest_{dest},
                is_jump_{true},
                is_promotion_{is_promotion}
        {
                assert(invariant());
        }

        // predicates

        // operator!= provided by boost::equality_comparable1
        friend bool operator==(Move const& lhs, Move const& rhs)
        {
                return
                        std::tie(lhs.captured_pieces_, lhs.captured_kings_, lhs.from_, lhs.dest_, lhs.is_promotion_) ==
                        std::tie(rhs.captured_pieces_, rhs.captured_kings_, rhs.from_, rhs.dest_, rhs.is_promotion_)
                ;
        }

        // queries

        auto captured_pieces() const noexcept
        {
                return captured_pieces_;
        }

        auto captured_kings() const noexcept
        {
                return captured_kings_;
        }

        auto from() const noexcept
        {
                return from_;
        }

        auto dest() const noexcept
        {
                return dest_;
        }

        auto is_with_king() const noexcept
        {
                return is_with_king_;
        }

        auto is_jump() const noexcept
        {
                return is_jump_;
        }

        auto is_promotion() const noexcept
        {
                return is_promotion_;
        }

        auto is_reversible() const noexcept
        {
                return is_with_king_ && !is_jump_;
        }

private:
        // implementation
        bool invariant() const
        {
                return (
                        bit::set_includes(captured_pieces_, captured_kings_) &&
                        is_jump_ == !captured_pieces_.empty() &&
                        !(is_with_king_ && is_promotion_)
                );
        }

        // representation

        T captured_pieces_{};
        T captured_kings_{};
        int from_{};
        int dest_{};
        bool is_with_king_{};
        bool is_jump_{};
        bool is_promotion_{};
};

}       // namespace dctl
