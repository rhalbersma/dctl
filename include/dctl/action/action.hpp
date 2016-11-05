#pragma once
#include <dctl/color_piece.hpp>                 // pawn, king
#include <dctl/rule_traits.hpp>                 // rectangular, is_quality_precedence, is_ordering_precedence
#include <dctl/utility/tagged_empty_base.hpp>   // tagged_empty_base
#include <dctl/utility/type_traits.hpp>         // set_t, square_t
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <tuple>                                // make_tuple
#include <type_traits>                          // conditional, enable_if, is_same

namespace dctl {
namespace detail {
namespace block_adl {

template<class Board>
struct BaseQualityPrecedence
{
        set_t<Board> m_captured_kings;
};

template<class Rules, class Board>
using quality_precedence_t = std::conditional_t<
        is_quality_precedence_v<Rules>,
        BaseQualityPrecedence<Board>,
        util::tagged_empty_base<0>
>;

template<class Board>
struct BaseOrderingPrecedence
{
        set_t<Board> m_piece_order;
};

template<class Rules, class Board>
using ordering_precedence_t = std::conditional_t<
        is_ordering_precedence_v<Rules>,
        BaseOrderingPrecedence<Board>,
        util::tagged_empty_base<1>
>;

}       // namespace block_adl

using block_adl::quality_precedence_t;
using block_adl::ordering_precedence_t;

}       // namespace detail

template<class Rules, class Board = rectangular_t<Rules>>
class action
:
        detail::ordering_precedence_t<Rules, Board>,
        detail:: quality_precedence_t<Rules, Board>
{
        using ordering_precedence_t = detail::ordering_precedence_t<Rules, Board>;
        using  quality_precedence_t = detail:: quality_precedence_t<Rules, Board>;
public:
        using  rules_type = Rules;
        using  board_type = Board;
        using    set_type =    set_t<Board>;
        using square_type = square_t<Board>;
private:
        set_type m_captured_pieces;
        square_type m_from;
        square_type m_dest;
        piece m_with;
        piece m_into;

        constexpr auto assert_invariants() const noexcept
        {
                assert(from() != dest() || is_jump());
                assert(!is_demotion());
        }
public:
        action() = default;

        constexpr action(std::size_t const src, std::size_t const dst, bool const promotion) noexcept
        :
                ordering_precedence_t{},
                quality_precedence_t{},
                m_captured_pieces{},
                m_from{static_cast<square_type>(src)},
                m_dest{static_cast<square_type>(dst)},
                m_with{piece::pawn},
                m_into{promotion ? piece::king : piece::pawn}
        {
                assert_invariants();
        }

        constexpr action(std::size_t const src, std::size_t const dst) noexcept
        :
                ordering_precedence_t{},
                quality_precedence_t{},
                m_captured_pieces{},
                m_from{static_cast<square_type>(src)},
                m_dest{static_cast<square_type>(dst)},
                m_with{piece::king},
                m_into{piece::king}
        {
                assert_invariants();
        }

        constexpr auto capture(std::size_t const sq, bool const is_king) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                if constexpr (is_quality_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>) {
                        capture_quality_ordering(sq, is_king);
                }
                m_captured_pieces.set(sq);
        }

        constexpr auto release(std::size_t const sq, bool const is_king) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                m_captured_pieces.reset(sq);
                if constexpr (is_quality_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>) {
                        release_quality_ordering(sq, is_king);
                }
        }

        constexpr auto from(std::size_t const sq) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                m_from = static_cast<square_type>(sq);
        }

        constexpr auto from() const noexcept
        {
                return m_from;
        }

        constexpr auto dest(std::size_t const sq) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                m_dest = static_cast<square_type>(sq);
        }

        constexpr auto dest() const noexcept
        {
                return m_dest;
        }

        constexpr auto with(piece const p) noexcept
        {
                m_with = p;
        }

        constexpr auto with() const noexcept
        {
                return m_with;
        }

        constexpr auto into(piece const p) noexcept
        {
                m_into = p;
        }

        constexpr auto into() const noexcept
        {
                return m_into;
        }

        constexpr auto captured_pieces() const noexcept
        {
                return m_captured_pieces;
        }

        constexpr auto num_captured_pieces() const noexcept
        {
                return captured_pieces().count();
        }

        constexpr auto is_with_king() const noexcept
        {
                return with() == piece::king;
        }

        constexpr auto is_jump() const noexcept
        {
                return captured_pieces().any();
        }

        constexpr auto is_promotion() const noexcept
        {
                return with() == piece::pawn && into() != piece::pawn;
        }

        constexpr auto is_reversible() const noexcept
        {
                return is_with_king() && !is_jump();
        }

        template<class RulesType = rules_type, std::enable_if_t<
                std::is_same<RulesType, rules_type>{} &&
                is_quality_precedence_v<RulesType>
        >...>
        constexpr auto captured_kings() const noexcept
        {
                return this->m_captured_kings;
        }

        template<class RulesType = rules_type, std::enable_if_t<
		std::is_same<RulesType, rules_type>{} &&
        	is_quality_precedence_v<RulesType>
        >...>
        constexpr auto num_captured_kings() const noexcept
        {
                return captured_kings().count();
        }

        template<class RulesType = rules_type, std::enable_if_t<
		std::is_same<RulesType, rules_type>{} &&
		is_ordering_precedence_v<RulesType>
        >...>
        constexpr auto piece_order() const noexcept
        {
                return this->m_piece_order;
        }
private:
        constexpr auto capture_quality_ordering(std::size_t const sq, bool const is_king)
        {
                static_assert(is_quality_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>);
                if (is_king) {
                        if constexpr (is_quality_precedence_v<rules_type>) {
                                this->m_captured_kings.set(sq);
                        }
                        if constexpr (is_ordering_precedence_v<rules_type>) {
                                this->m_piece_order.set(reverse_index());
                        }
                }
        }

        constexpr auto release_quality_ordering(std::size_t const sq, bool const is_king)
        {
                static_assert(is_quality_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>);
                if (is_king) {
                        if constexpr (is_ordering_precedence_v<rules_type>) {
                                this->m_piece_order.reset(reverse_index());
                        }
                        if constexpr (is_quality_precedence_v<rules_type>) {
                                this->m_captured_kings.reset(sq);
                        }
                }
        }

        constexpr auto is_onboard(std::size_t const sq) const noexcept
        {
                return sq < set_type::size();
        }

        constexpr auto is_demotion() const noexcept
        {
                return with() != piece::pawn && into() == piece::pawn;
        }

        constexpr auto reverse_index() const noexcept
        {
                return set_type::size() - 1 - num_captured_pieces();
        }
};

template<class Rules, class Board, std::enable_if_t<
        !is_ordering_precedence_v<Rules>
>...>
constexpr auto as_tuple(action<Rules, Board> const& a) noexcept
{
        return std::make_tuple(a.from(), a.dest(), a.captured_pieces());
}

template<class Rules, class Board, std::enable_if_t<
	is_ordering_precedence_v<Rules>
>...>
constexpr auto as_tuple(action<Rules, Board> const& a) noexcept
{
        return std::make_tuple(a.from(), a.dest(), a.captured_pieces(), a.piece_order());
}

template<class Rules, class Board>
constexpr auto operator==(action<Rules, Board> const& lhs, action<Rules, Board> const& rhs) noexcept
{
        return as_tuple(lhs) == as_tuple(rhs);
}

template<class Rules, class Board>
constexpr auto operator< (action<Rules, Board> const& lhs, action<Rules, Board> const& rhs) noexcept
{
        return as_tuple(lhs) < as_tuple(rhs);
}

template<class Rules, class Board>
constexpr auto operator!=(action<Rules, Board> const& lhs, action<Rules, Board> const& rhs) noexcept
{
        return !(lhs == rhs);
}

template<class Rules, class Board>
constexpr auto operator> (action<Rules, Board> const& lhs, action<Rules, Board> const& rhs) noexcept
{
        return rhs < lhs;
}

template<class Rules, class Board>
constexpr auto operator>=(action<Rules, Board> const& lhs, action<Rules, Board> const& rhs) noexcept
{
        return !(lhs < rhs);
}

template<class Rules, class Board>
constexpr auto operator<=(action<Rules, Board> const& lhs, action<Rules, Board> const& rhs) noexcept
{
        return !(rhs < lhs);
}

}       // namespace dctl
