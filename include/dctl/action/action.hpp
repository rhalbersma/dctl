#pragma once
#include <dctl/color_piece.hpp>                 // pawn, king
#include <dctl/rule_traits.hpp>                 // rectangular, is_quality_precedence, is_ordering_precedence
#include <dctl/utility/concepts.hpp>            // is_trivial_special_members
#include <dctl/utility/conditional_base.hpp>    // conditional_base
#include <dctl/utility/type_traits.hpp>         // set_t, square_t
#include <cassert>                              // assert
#include <tuple>                                // tie
#include <type_traits>                          // conditional, enable_if, is_same

namespace dctl {
namespace detail {
namespace block_adl {

template<class Board>
struct base_action
{
        set_t<Board> m_captured_pieces;
        square_t<Board> m_from;
        square_t<Board> m_dest;
        piece m_with;
        piece m_into;
};

template<class Board>
struct base_quality_precedence
{
        set_t<Board> m_captured_kings;
};

template<class Rules, class Board>
using conditional_base_quality_precedence = util::conditional_base<
        is_quality_precedence_v<Rules>,
        base_quality_precedence<Board>
>;

template<class Board>
struct base_ordering_precedence
{
        set_t<Board> m_piece_order;
};

template<class Rules, class Board>
using conditional_base_ordering_precedence = util::conditional_base<
        is_ordering_precedence_v<Rules>,
        base_ordering_precedence<Board>
>;

}       // namespace block_adl

using block_adl::base_action;
using block_adl::conditional_base_quality_precedence;
using block_adl::conditional_base_ordering_precedence;

}       // namespace detail

template<class Rules, class Board = rectangular_t<Rules>>
class action
:
        detail::conditional_base_ordering_precedence<Rules, Board>,
        detail::conditional_base_quality_precedence<Rules, Board>,
        detail::base_action<Board>
{
        using conditional_base_ordering_precedence = detail::conditional_base_ordering_precedence<Rules, Board>;
        using conditional_base_quality_precedence = detail::conditional_base_quality_precedence<Rules, Board>;
        using base_action = detail::base_action<Board>;

        static constexpr auto static_assert_type_traits() noexcept
        {
                using T = action<Rules, Board>;
                static_assert(util::is_trivial_special_members_v<T>);
                static_assert(std::is_pod<T>{});
        }

        constexpr auto assert_invariants() const noexcept
        {
                assert(from() != dest() || is_jump());
                assert(!is_demotion());
        }
public:
        using  rules_type = Rules;
        using  board_type = Board;
        using    set_type =    set_t<Board>;
        using square_type = square_t<Board>;

        action() = default;

        constexpr action(int const src, int const dst, bool const promotion) noexcept
        :
                conditional_base_ordering_precedence{},
                conditional_base_quality_precedence{},
                base_action{{}, static_cast<square_type>(src), static_cast<square_type>(dst), piece::pawn, promotion ? piece::king : piece::pawn}
        {
                assert_invariants();
        }

        constexpr action(int const src, int const dst) noexcept
        :
                conditional_base_ordering_precedence{},
                conditional_base_quality_precedence{},
                base_action{{}, static_cast<square_type>(src), static_cast<square_type>(dst), piece::king, piece::king}
        {
                assert_invariants();
        }

        constexpr auto capture(int const sq, bool const is_king) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                if constexpr (is_quality_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>) {
                        capture_quality_ordering(sq, is_king);
                }
                this->m_captured_pieces.set(sq);
        }

        constexpr auto release(int const sq, bool const is_king) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                this->m_captured_pieces.reset(sq);
                if constexpr (is_quality_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>) {
                        release_quality_ordering(sq, is_king);
                }
        }

        constexpr auto captured_pieces() const noexcept
        {
                return this->m_captured_pieces;
        }

        constexpr auto num_captured_pieces() const noexcept
        {
                return captured_pieces().count();
        }

        constexpr auto from(int const sq) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                this->m_from = static_cast<square_type>(sq);
        }

        constexpr auto from() const noexcept
        {
                return this->m_from;
        }

        constexpr auto dest(int const sq) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                this->m_dest = static_cast<square_type>(sq);
        }

        constexpr auto dest() const noexcept
        {
                return this->m_dest;
        }

        constexpr auto with(piece const p) noexcept
        {
                this->m_with = p;
        }

        constexpr auto with() const noexcept
        {
                return this->m_with;
        }

        constexpr auto into(piece const p) noexcept
        {
                this->m_into = p;
        }

        constexpr auto into() const noexcept
        {
                return this->m_into;
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

        constexpr auto tied() const noexcept
        {
                if constexpr (is_ordering_precedence_v<Rules>) {
                        return std::tie(this->m_from, this->m_dest, this->m_captured_pieces, this->m_piece_order);
                } else {
                        return std::tie(this->m_from, this->m_dest, this->m_captured_pieces);
                }
        }

private:
        constexpr auto capture_quality_ordering(int const sq, bool const is_king)
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

        constexpr auto release_quality_ordering(int const sq, bool const is_king)
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

        constexpr auto is_onboard(int const sq) const noexcept
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

template<class Rules, class Board>
constexpr auto operator==(action<Rules, Board> const& lhs, action<Rules, Board> const& rhs) noexcept
{
        return lhs.tied() == rhs.tied();
}

template<class Rules, class Board>
constexpr auto operator< (action<Rules, Board> const& lhs, action<Rules, Board> const& rhs) noexcept
{
        return lhs.tied() < rhs.tied();
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
