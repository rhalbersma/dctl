#pragma once
#include <cassert>                      // assert
#include <tuple>                        // tie
#include <dctl/bit/algorithm.hpp>       // set_includes

namespace dctl {

template<class Rules, class Board>
struct Move
{
public:
        using board_type = Board;
        using rules_type = Rules;
        using Set = typename Board::set_type;

        // structors

        // default constructor
        constexpr Move() = default;

        // king move
        constexpr Move(int from, int dest)
        :
                from_{from},
                dest_{dest},
                is_with_king_{true}
        {
                assert(from_ != dest_);
                assert(invariant());
        }

        // pawn move
        constexpr Move(int from, int dest, bool is_promotion)
        :
                from_{from},
                dest_{dest},
                is_promotion_{is_promotion}
        {
                assert(from_ != dest_);
                assert(invariant());
        }

        // king jump
        constexpr Move(Set captured_pieces, Set captured_kings, int from, int dest)
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
        constexpr Move(Set captured_pieces, Set captured_kings, int from, int dest, bool is_promotion)
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

        friend /* constexpr */ bool operator==(Move const& lhs, Move const& rhs) noexcept
        {
                return
                        std::tie(lhs.captured_pieces_, lhs.captured_kings_, lhs.from_, lhs.dest_, lhs.is_promotion_) ==
                        std::tie(rhs.captured_pieces_, rhs.captured_kings_, rhs.from_, rhs.dest_, rhs.is_promotion_)
                ;
        }

        friend /* constexpr */ bool operator!=(Move const& lhs, Move const& rhs) noexcept
        {
                return !(lhs == rhs);
        }

        // queries

        constexpr auto captured_pieces() const noexcept
        {
                return captured_pieces_;
        }

        constexpr auto captured_kings() const noexcept
        {
                return captured_kings_;
        }

        constexpr auto from() const noexcept
        {
                return from_;
        }

        constexpr auto dest() const noexcept
        {
                return dest_;
        }

        constexpr auto is_with_king() const noexcept
        {
                return is_with_king_;
        }

        constexpr auto is_jump() const noexcept
        {
                return is_jump_;
        }

        constexpr auto is_promotion() const noexcept
        {
                return is_promotion_;
        }

        constexpr auto is_reversible() const noexcept
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

        Set captured_pieces_{};
        Set captured_kings_{};
        int from_{};
        int dest_{};
        bool is_with_king_{};
        bool is_jump_{};
        bool is_promotion_{};
};

}       // namespace dctl
