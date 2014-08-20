#pragma once
#include <dctl/type_traits.hpp>         // board_type_t, rules_type_t
#include <cassert>                      // assert
#include <tuple>                        // forward_as_tuple

namespace dctl {

template<class Rules, class Board>
struct Move
{
public:
        using board_type = Board;
        using rules_type = Rules;
        using Set = set_type_t<Board>;

        // constructors

        // king move
        constexpr Move(int src, int dst, bool color)
        :
                from_{src},
                dest_{dst},
                is_with_king_{true},
                active_color_{color}
        {
                assert(from_ != dest_);
                assert(invariant());
        }

        // pawn move
        constexpr Move(int src, int dst, bool prom, bool color)
        :
                from_{src},
                dest_{dst},
                is_promotion_{prom},
                active_color_{color}
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
                is_with_king_{true},
                active_color_{color}
        {
                assert(invariant());
        }

        // pawn jump
        constexpr Move(Set pieces, Set kings, int src, int dst, bool prom, bool color)
        :
                captured_pieces_{pieces},
                captured_kings_{kings},
                from_{src},
                dest_{dst},
                is_promotion_{prom},
                active_color_{color}
        {
                assert(invariant());
        }

        template<class U>
        explicit Move(U const& u)
        :
                captured_pieces_{u.captured_pieces()},
                captured_kings_{u.captured_kings()},
                from_{u.from()},
                dest_{u.dest()},
                is_with_king_{u.is_with_king()},
                is_promotion_{u.is_promotion()},
                active_color_{u.active_color()}
        {
                assert(invariant());
        }

        // observers

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

        constexpr auto is_promotion() const noexcept
        {
                return is_promotion_;
        }

        constexpr auto active_color() const noexcept
        {
                return active_color_;
        }

        constexpr auto is_jump() const noexcept
        {
                return !captured_pieces().empty();
        }

        constexpr auto is_reversible() const noexcept
        {
                return is_with_king() && !is_jump();
        }

        constexpr auto num_pieces() const
        {
                return captured_pieces_.size();
        }

        constexpr auto num_kings() const
        {
                return captured_kings_.size();
        }

        // predicates

        friend constexpr auto
        operator==(Move const& lhs, Move const& rhs) noexcept
        {
                return
                        std::tie(lhs.from_, lhs.dest_, lhs.captured_pieces_) ==
                        std::tie(rhs.from_, rhs.dest_, rhs.captured_pieces_)
                ;
        }

        friend constexpr auto
        operator!=(Move const& lhs, Move const& rhs) noexcept
        {
                return !(lhs == rhs);
        }

        friend constexpr auto
        operator<(Move const& lhs, Move const& rhs) noexcept
        {
                return
                        std::tie(lhs.from_, lhs.dest_, lhs.captured_pieces_) <
                        std::tie(rhs.from_, rhs.dest_, rhs.captured_pieces_)
                ;
        }

private:
        // contracts

        bool invariant() const
        {
                return
                        captured_kings_.is_subset_of(captured_pieces_) &&
                        !(is_with_king_ && is_promotion_)
                ;
        }

        // representation

        Set captured_pieces_{};
        Set captured_kings_{};
        int from_{};
        int dest_{};
        bool is_with_king_{};
        bool is_promotion_{};
        bool active_color_{};
};

template<class T>
using Move_t = Move<rules_type_t<T>, board_type_t<T>>;

}       // namespace dctl
