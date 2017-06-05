#pragma once
#include <dctl/core/rules/traits.hpp>           // rectangular, is_contents_precedence, is_ordering_precedence
#include <dctl/core/state/color_piece.hpp>      // pawn, king
#include <dctl/util/conditional_base.hpp>       // conditional_base
#include <dctl/util/type_traits.hpp>            // set_t, square_t
#include <cassert>                              // assert
#include <tuple>                                // tie
#include <type_traits>                          // conditional, enable_if, is_pod, is_same

namespace dctl {
namespace core {
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
struct base_contents_precedence
{
        square_t<Board> m_num_captured_kings;
};

template<class Rules, class Board>
using conditional_base_contents_precedence = util::conditional_base<
        is_contents_precedence_v<Rules>,
        base_contents_precedence<Board>
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
using block_adl::conditional_base_contents_precedence;
using block_adl::conditional_base_ordering_precedence;

}       // namespace detail

template<class Rules, class Board = rectangular_t<Rules>>
class action
:
        detail::conditional_base_ordering_precedence<Rules, Board>,
        detail::conditional_base_contents_precedence<Rules, Board>,
        detail::base_action<Board>
{
        using conditional_base_ordering_precedence = detail::conditional_base_ordering_precedence<Rules, Board>;
        using conditional_base_contents_precedence = detail::conditional_base_contents_precedence<Rules, Board>;
        using base_action = detail::base_action<Board>;

        static constexpr auto static_assert_type_traits() noexcept
        {
                static_assert(std::is_pod_v<action>);
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
                conditional_base_contents_precedence{},
                base_action{{}, static_cast<square_type>(src), static_cast<square_type>(dst), piece::pawns, promotion ? piece::kings : piece::pawns}
        {
                assert_invariants();
        }

        constexpr action(int const src, int const dst) noexcept
        :
                conditional_base_ordering_precedence{},
                conditional_base_contents_precedence{},
                base_action{{}, static_cast<square_type>(src), static_cast<square_type>(dst), piece::kings, piece::kings}
        {
                assert_invariants();
        }

        constexpr auto capture(int const sq, [[maybe_unused]] bool const is_king) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                if constexpr (is_contents_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>) {
                        capture_contents_ordering(is_king);
                }
                this->m_captured_pieces.insert(sq);
        }

        constexpr auto release(int const sq, [[maybe_unused]] bool const is_king) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                this->m_captured_pieces.erase(sq);
                if constexpr (is_contents_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>) {
                        release_contents_ordering(is_king);
                }
        }

        constexpr auto captured_pieces() const noexcept
        {
                return this->m_captured_pieces;
        }

        constexpr auto num_captured_pieces() const noexcept
        {
                return captured_pieces().size();
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
                return with() == piece::kings;
        }

        constexpr auto is_jump() const noexcept
        {
                return !captured_pieces().empty();
        }

        constexpr auto is_promotion() const noexcept
        {
                return with() == piece::pawns && into() != piece::pawns;
        }

        constexpr auto is_reversible() const noexcept
        {
                return is_with_king() && !is_jump();
        }

        template<class RulesType = rules_type, std::enable_if_t<
		std::is_same_v<RulesType, rules_type> &&
        	is_contents_precedence_v<RulesType>
        >...>
        constexpr auto num_captured_kings() const noexcept
        {
                return this->m_num_captured_kings;
        }

        template<class RulesType = rules_type, std::enable_if_t<
		std::is_same_v<RulesType, rules_type> &&
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
        constexpr auto capture_contents_ordering(bool const is_king)
        {
                static_assert(is_contents_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>);
                if (is_king) {
                        if constexpr (is_contents_precedence_v<rules_type>) {
                                ++this->m_num_captured_kings;
                        }
                        if constexpr (is_ordering_precedence_v<rules_type>) {
                                this->m_piece_order.insert(reverse_index());
                        }
                }
        }

        constexpr auto release_contents_ordering(bool const is_king)
        {
                static_assert(is_contents_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>);
                if (is_king) {
                        if constexpr (is_ordering_precedence_v<rules_type>) {
                                this->m_piece_order.erase(reverse_index());
                        }
                        if constexpr (is_contents_precedence_v<rules_type>) {
                                --this->m_num_captured_kings;
                        }
                }
        }

        constexpr auto is_onboard(int const sq) const noexcept
        {
                return static_cast<unsigned>(sq) < static_cast<unsigned>(set_type::max_size());
        }

        constexpr auto is_demotion() const noexcept
        {
                return with() != piece::pawns && into() == piece::pawns;
        }

        constexpr auto reverse_index() const noexcept
        {
                return set_type::max_size() - 1 - num_captured_pieces();
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

}       // namespace core
}       // namespace dctl
