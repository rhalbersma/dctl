#pragma once
#include <cassert>                      // assert
#include <tuple>                        // tie
#include <dctl/bit.hpp>                 // set_includes

namespace dctl {

template<class Rules, class Board>
struct Move
{
public:
        using board_type = Board;
        using rules_type = Rules;
        using Set = typename Board::set_type;

        // structors

        // king move
        constexpr Move(int src, int dst, bool color)
        :
                from_{src},
                dest_{dst},
                active_color_{color},
                is_with_king_{true}
        {
                assert(from_ != dest_);
                assert(invariant());
        }

        // pawn move
        constexpr Move(int src, int dst, bool color, bool prom)
        :
                from_{src},
                dest_{dst},
                active_color_{color},
                is_promotion_{prom}
        {
                assert(from_ != dest_);
                assert(invariant());
        }

        // king jump
        constexpr Move(Set pieces, Set kings, int src, int dst, bool color)
        :
                captured_pieces_{pieces},
                captured_kings_{kings},
                from_{src},
                dest_{dst},
                active_color_{color},
                is_with_king_{true},
                is_jump_{true}
        {
                assert(invariant());
        }

        // pawn jump
        constexpr Move(Set pieces, Set kings, int src, int dst, bool color, bool prom)
        :
                captured_pieces_{pieces},
                captured_kings_{kings},
                from_{src},
                dest_{dst},
                active_color_{color},
                is_jump_{true},
                is_promotion_{prom}
        {
                assert(invariant());
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

        constexpr auto active_color() const
        {
                return active_color_;
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

        // predicates

        friend /* constexpr */ bool
        operator==(Move const& lhs, Move const& rhs) noexcept
        {
                return
                        std::tie(lhs.from_, lhs.dest_, lhs.captured_pieces_) ==
                        std::tie(rhs.from_, rhs.dest_, rhs.captured_pieces_)
                ;
        }

        friend /* constexpr */ bool
        operator!=(Move const& lhs, Move const& rhs) noexcept
        {
                return !(lhs == rhs);
        }

private:
        // contracts

        bool invariant() const
        {
                return
                        set_includes(captured_pieces_, captured_kings_) &&
                        is_jump_ == !captured_pieces_.empty() &&
                        !(is_with_king_ && is_promotion_)
                ;
        }

        // representation

        Set captured_pieces_{};
        Set captured_kings_{};
        int from_{};
        int dest_{};
        bool active_color_{};
        bool is_with_king_{};
        bool is_jump_{};
        bool is_promotion_{};
};

}       // namespace dctl
