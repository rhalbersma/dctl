#pragma once
#include <dctl/piece.hpp>                       // pawn, king
#include <dctl/rule_traits.hpp>                 // is_quality_precedence, is_ordering_precedence
#include <dctl/utility/tagged_empty_base.hpp>   // tagged_empty_base
#include <dctl/utility/logic.hpp>               // implies
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/cstdint.hpp>                     // uint_fast
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <tuple>                                // forward_as_tuple
#include <type_traits>                          // conditional, enable_if

namespace dctl {
namespace detail {

template<class Board>
struct base_action
{
        using    set_type = set_t<Board>;
        using square_type = xstd::uint_fast_t<set_type::size()>;
        set_type captured_pieces_;
        square_type from_;
        square_type dest_;
        Piece with_;
        Piece into_;
};

template<class Board>
struct quality_action
{
        set_t<Board> captured_kings_;
};

template<class Board>
struct ordering_action
{
        set_t<Board> piece_order_;
};

template<class Rules, class Board>
using conditional_quality = std::conditional_t<
        is_quality_precedence_or_v<Rules>,
        quality_action<Board>,
        util::tagged_empty_base<0>
>;

template<class Rules, class Board>
using conditional_ordering = std::conditional_t<
        is_ordering_precedence_or_v<Rules>,
        ordering_action<Board>,
        util::tagged_empty_base<1>
>;

}       // namespace detail

template<class Rules, class Board>
class Action
:
        std::tuple<
                detail::conditional_quality<Rules, Board>,
                detail::conditional_ordering<Rules, Board>,
                detail::base_action<Board>
        >
{
        enum tuple_idx { quality, ordering, push_jump_promote };

        using base = std::tuple<
                detail::conditional_quality<Rules, Board>,
                detail::conditional_ordering<Rules, Board>,
                detail::base_action<Board>
        >;

        constexpr auto assert_invariant() const noexcept
        {
                assert(util::implies(from() == dest(), is_jump()));
        }
public:
        using  board_type = Board;
        using  rules_type = Rules;
        using    set_type = set_t<Board>;
        using square_type = xstd::uint_fast_t<set_type::size()>;

        Action() = default;

        constexpr Action(std::size_t const src, std::size_t const dst, bool const promotion) noexcept
        :
                base{ {}, {}, { set_type{}, static_cast<square_type>(src), static_cast<square_type>(dst), Piece::pawn, promotion ? Piece::king : Piece::pawn } }

        {
                assert_invariant();
        }

        constexpr Action(std::size_t const src, std::size_t const dst) noexcept
        :
                base{ {}, {}, { set_type{}, static_cast<square_type>(src), static_cast<square_type>(dst), Piece::king, Piece::king } }
        {
                assert_invariant();
        }

        auto capture_piece(std::size_t const sq, bool const is_king)
        {
                capture_quality_ordering_dispatch(sq, is_king, is_quality_precedence_or_t<Rules>{}, is_ordering_precedence_or_t<Rules>{});
                captured_pieces().set(sq);
        }

        auto release_piece(std::size_t const sq, bool const is_king)
        {
                captured_pieces().reset(sq);
                release_quality_ordering_dispatch(sq, is_king, is_quality_precedence_or_t<Rules>{}, is_ordering_precedence_or_t<Rules>{});
        }

        auto set_from(std::size_t const src) { from() = static_cast<square_type>(src); }
        auto set_dest(std::size_t const dst) { dest() = static_cast<square_type>(dst); }
        auto set_with(Piece const p) { with() = p; }
        auto set_into(Piece const p) { into() = p; }

        template<class RulesType = rules_type, std::enable_if_t<is_quality_precedence_or_v<RulesType>>* = nullptr>
        constexpr auto captured_kings() const noexcept
        {
                return std::get<tuple_idx::quality>(*this).captured_kings_;
        }

        template<class RulesType = rules_type, std::enable_if_t<is_quality_precedence_or_v<RulesType>>* = nullptr>
        constexpr auto num_captured_kings() const noexcept
        {
                return captured_kings().count();
        }

        template<class RulesType = rules_type, std::enable_if_t<is_ordering_precedence_or_v<RulesType>>* = nullptr>
        constexpr auto piece_order() const noexcept
        {
                return std::get<tuple_idx::ordering>(*this).piece_order_;
        }

        constexpr auto captured_pieces() const noexcept
        {
                return std::get<tuple_idx::push_jump_promote>(*this).captured_pieces_;
        }

        constexpr auto from() const noexcept
        {
                return static_cast<std::size_t>(std::get<tuple_idx::push_jump_promote>(*this).from_);
        }

        constexpr auto dest() const noexcept
        {
                return static_cast<std::size_t>(std::get<tuple_idx::push_jump_promote>(*this).dest_);
        }

        constexpr auto with() const noexcept
        {
                return std::get<tuple_idx::push_jump_promote>(*this).with_;
        }

        constexpr auto into() const noexcept
        {
                return std::get<tuple_idx::push_jump_promote>(*this).into_;
        }

        constexpr auto is_with(Piece const p) const noexcept
        {
                return with() == p;
        }

        constexpr auto is_with_king() const noexcept
        {
                return is_with(Piece::king);
        }

        constexpr auto is_into(Piece const p) const noexcept
        {
                return into() == p;
        }

        constexpr auto is_jump() const noexcept
        {
                return captured_pieces().any();
        }

        constexpr auto is_promotion() const noexcept
        {
                return is_with(Piece::pawn) && !is_into(Piece::pawn);
        }

        constexpr auto is_reversible() const noexcept
        {
                return is_with(Piece::king) && !is_jump();
        }

        constexpr auto num_captured_pieces() const noexcept
        {
                return captured_pieces().count();
        }

private:
        template<class RulesType = rules_type, std::enable_if_t<is_quality_precedence_or_v<RulesType>>* = nullptr>
        constexpr auto& captured_kings() noexcept
        {
                return std::get<tuple_idx::quality>(*this).captured_kings_;
        }

        template<class RulesType = rules_type, std::enable_if_t<is_ordering_precedence_or_v<RulesType>>* = nullptr>
        constexpr auto& piece_order() noexcept
        {
                return std::get<tuple_idx::ordering>(*this).piece_order_;
        }

        constexpr auto& captured_pieces() noexcept
        {
                return std::get<tuple_idx::push_jump_promote>(*this).captured_pieces_;
        }

        constexpr auto& from() noexcept
        {
                return std::get<tuple_idx::push_jump_promote>(*this).from_;
        }

        constexpr auto& dest() noexcept
        {
                return std::get<tuple_idx::push_jump_promote>(*this).dest_;
        }

        constexpr auto& with() noexcept
        {
                return std::get<tuple_idx::push_jump_promote>(*this).with_;
        }

        constexpr auto& into() noexcept
        {
                return std::get<tuple_idx::push_jump_promote>(*this).into_;
        }

        auto capture_quality_ordering_dispatch(std::size_t const sq, bool const is_king, std::false_type, std::false_type)
        {
                // no-op
        }

        auto capture_quality_ordering_dispatch(std::size_t const sq, bool const is_king, std::true_type, std::false_type)
        {
                if (is_king) {
                        captured_kings().set(sq);
                }
        }

        auto capture_quality_ordering_dispatch(std::size_t const /*sq*/, bool const is_king, std::false_type, std::true_type)
        {
                if (is_king) {
                        piece_order().set(set_type::size() - 1 - num_captured_pieces());
                }
        }

        auto capture_quality_ordering_dispatch(std::size_t const sq, bool const is_king, std::true_type, std::true_type)
        {
                if (is_king) {
                        captured_kings().set(sq);
                        piece_order().set(set_type::size() - 1 - num_captured_pieces());
                }
        }

        auto release_quality_ordering_dispatch(std::size_t const sq, bool const is_king, std::false_type, std::false_type)
        {
                // no-op
        }

        auto release_quality_ordering_dispatch(std::size_t const sq, bool const is_king, std::true_type, std::false_type)
        {
                if (is_king) {
                        captured_kings().reset(sq);
                }
        }

        auto release_quality_ordering_dispatch(std::size_t const /*sq*/, bool const is_king, std::false_type, std::true_type)
        {
                if (is_king) {
                        piece_order().reset(set_type::size() - 1 - num_captured_pieces());
                }
        }

        auto release_quality_ordering_dispatch(std::size_t const sq, bool const is_king, std::true_type, std::true_type)
        {
                if (is_king) {
                        piece_order().reset(set_type::size() - 1 - num_captured_pieces());
                        captured_kings().reset(sq);
                }
        }
};

template<class Rules, class Board, std::enable_if_t<!is_ordering_precedence_or_v<Rules>>* = nullptr>
auto as_tuple(Action<Rules, Board> const& a) noexcept
{
        return std::make_tuple(a.from(), a.dest(), a.captured_pieces());
}

template<class Rules, class Board, std::enable_if_t< is_ordering_precedence_or_v<Rules>>* = nullptr>
auto as_tuple(Action<Rules, Board> const& a) noexcept
{
        return std::make_tuple(a.from(), a.dest(), a.captured_pieces(), a.piece_order());
}

template<class Rules, class Board>
constexpr auto operator==(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return as_tuple(lhs) == as_tuple(rhs);
}

template<class Rules, class Board>
constexpr auto operator< (Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return as_tuple(lhs) < as_tuple(rhs);
}

template<class Rules, class Board>
constexpr auto operator!=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(lhs == rhs);
}

template<class Rules, class Board>
constexpr auto operator> (Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return rhs < lhs;
}

template<class Rules, class Board>
constexpr auto operator>=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(lhs < rhs);
}

template<class Rules, class Board>
constexpr auto operator<=(Action<Rules, Board> const& lhs, Action<Rules, Board> const& rhs) noexcept
{
        return !(rhs < lhs);
}

}       // namespace dctl
