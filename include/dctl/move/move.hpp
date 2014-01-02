#pragma once
#include <cassert>                      // assert
#include <tuple>                        // tie
#include <dctl/bit.hpp>                 // set_includes
#include <dctl/board/algebraic.hpp>
#include <dctl/grid/coordinates.hpp>

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
        constexpr Move(int src, int dst)
        :
                from_{src},
                dest_{dst},
                is_with_king_{true}
        {
                assert(from_ != dest_);
                assert(invariant());
        }

        // pawn move
        constexpr Move(int src, int dst, bool prom)
        :
                from_{src},
                dest_{dst},
                is_promotion_{prom}
        {
                assert(from_ != dest_);
                assert(invariant());
        }

        // king jump
        constexpr Move(Set pieces, Set kings, int src, int dst)
        :
                captured_pieces_{pieces},
                captured_kings_{kings},
                from_{src},
                dest_{dst},
                is_with_king_{true},
                is_jump_{true}
        {
                assert(invariant());
        }

        // pawn jump
        constexpr Move(Set pieces, Set kings, int src, int dst, bool prom)
        :
                captured_pieces_{pieces},
                captured_kings_{kings},
                from_{src},
                dest_{dst},
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
        // implementation
        bool invariant() const
        {
                return
                        bit::set_includes(captured_pieces_, captured_kings_) &&
                        is_jump_ == !captured_pieces_.empty() &&
                        !(is_with_king_ && is_promotion_)
                ;
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
