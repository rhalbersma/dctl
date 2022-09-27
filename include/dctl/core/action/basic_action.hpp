#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/mask.hpp>             // basic_mask
#include <dctl/core/rules/type_traits.hpp>      // is_contents_precedence, is_ordering_precedence
#include <dctl/core/state/piece.hpp>            // pawn, king
#include <dctl/util/type_traits.hpp>            // set_t, square_t
#include <xstd/array.hpp>                       // or_empty
#include <cassert>                              // assert
#include <concepts>                             // same_as
#include <tuple>                                // tie

namespace dctl::core {
namespace detail {
namespace block_adl {

template<class Board>
struct base_action
{
        set_t<basic_mask<Board>> m_captured_pieces;
        square_t<basic_mask<Board>> m_from;
        square_t<basic_mask<Board>> m_dest;
        piece m_with;
        piece m_into;
};

template<class Board>
struct base_contents_precedence
{
        square_t<basic_mask<Board>> m_num_captured_kings;
};

template<class Board>
struct base_ordering_precedence
{
        set_t<basic_mask<Board>> m_piece_order;
};

template<class Rules, class Board>
using conditional_base_contents_precedence = xstd::or_empty<
        is_contents_precedence_v<Rules>,
        base_contents_precedence<Board>
>;

template<class Rules, class Board>
using conditional_base_ordering_precedence = xstd::or_empty<
        is_ordering_precedence_v<Rules>,
        base_ordering_precedence<Board>
>;

}       // namespace block_adl

using block_adl::base_action;
using block_adl::conditional_base_contents_precedence;
using block_adl::conditional_base_ordering_precedence;

}       // namespace detail

template<class Rules, class Board>
class basic_action
:
        detail::conditional_base_ordering_precedence<Rules, Board>,
        detail::conditional_base_contents_precedence<Rules, Board>,
        detail::base_action<Board>
{
        using conditional_base_ordering_precedence = detail::conditional_base_ordering_precedence<Rules, Board>;
        using conditional_base_contents_precedence = detail::conditional_base_contents_precedence<Rules, Board>;
        using base_action = detail::base_action<Board>;

        constexpr auto assert_invariants() const noexcept
        {
                assert(from() != dest() || is_jump());
                assert(!is_demotion());
        }
public:
        using  rules_type = Rules;
        using  board_type = Board;
        using   mask_type = basic_mask<board_type>;
        using    set_type =    set_t<mask_type>;
        using square_type = square_t<mask_type>;

        basic_action() = default;

        constexpr auto operator==(basic_action const& other) const noexcept
        {
                return this->tied() == other.tied();
        }

        constexpr auto operator<=>(basic_action const& other) const noexcept
        {
                return this->tied() <=> other.tied();
        }

        constexpr basic_action(int src, int dst, bool is_promotion) noexcept
        :
                conditional_base_ordering_precedence{},
                conditional_base_contents_precedence{},
                base_action{{}, static_cast<square_type>(src), static_cast<square_type>(dst), piece::pawn, is_promotion ? piece::king : piece::pawn}
        {
                assert_invariants();
        }

        constexpr basic_action(int src, int dst) noexcept
        :
                conditional_base_ordering_precedence{},
                conditional_base_contents_precedence{},
                base_action{{}, static_cast<square_type>(src), static_cast<square_type>(dst), piece::king, piece::king}
        {
                assert_invariants();
        }

        constexpr auto capture(int sq, bool is_king [[maybe_unused]]) // Throws: Nothing.
        {
                assert(board_type::is_onboard(sq));
                if constexpr (is_contents_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>) {
                        capture_contents_ordering(is_king);
                }
                this->m_captured_pieces.add(sq);
        }

        constexpr auto release(int sq, bool is_king [[maybe_unused]]) // Throws: Nothing.
        {
                assert(board_type::is_onboard(sq));
                this->m_captured_pieces.pop(sq);
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
                return captured_pieces().ssize();
        }

        constexpr auto from(int sq) // Throws: Nothing.
        {
                assert(board_type::is_onboard(sq));
                this->m_from = static_cast<square_type>(sq);
        }

        constexpr auto from() const noexcept
        {
                return this->m_from;
        }

        constexpr auto dest(int sq) // Throws: Nothing.
        {
                assert(board_type::is_onboard(sq));
                this->m_dest = static_cast<square_type>(sq);
        }

        constexpr auto dest() const noexcept
        {
                return this->m_dest;
        }

        constexpr auto with(piece p) noexcept
        {
                this->m_with = p;
        }

        constexpr auto with() const noexcept
        {
                return this->m_with;
        }

        constexpr auto into(piece p) noexcept
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
                return !captured_pieces().empty();
        }

        constexpr auto is_promotion() const noexcept
        {
                return with() == piece::pawn && into() != piece::pawn;
        }

        constexpr auto is_reversible() const noexcept
        {
                return is_with_king() && !is_jump();
        }

        constexpr auto num_captured_kings() const noexcept
                requires is_contents_precedence_v<Rules>
        {
                return this->m_num_captured_kings;
        }

        constexpr auto piece_order() const noexcept
                requires is_ordering_precedence_v<Rules>
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
        constexpr auto capture_contents_ordering(bool is_king)
                requires (is_contents_precedence_v<Rules> || is_ordering_precedence_v<Rules>)
        {
                if (is_king) {
                        if constexpr (is_contents_precedence_v<Rules>) {
                                ++this->m_num_captured_kings;
                        }
                        if constexpr (is_ordering_precedence_v<Rules>) {
                                this->m_piece_order.add(reverse_index());
                        }
                }
        }

        constexpr auto release_contents_ordering(bool is_king)
                requires (is_contents_precedence_v<Rules> || is_ordering_precedence_v<Rules>)
        {
                if (is_king) {
                        if constexpr (is_ordering_precedence_v<Rules>) {
                                this->m_piece_order.pop(reverse_index());
                        }
                        if constexpr (is_contents_precedence_v<Rules>) {
                                --this->m_num_captured_kings;
                        }
                }
        }

        constexpr auto is_demotion() const noexcept
        {
                return with() != piece::pawn && into() == piece::pawn;
        }

        constexpr auto reverse_index() const noexcept
        {
                return static_cast<int>(set_type::max_size()) - 1 - num_captured_pieces();
        }
};

}       // namespace dctl::core
