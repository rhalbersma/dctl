#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/mask.hpp>             // basic_mask
#include <dctl/core/rules/type_traits.hpp>      // is_contents_precedence, is_ordering_precedence
#include <dctl/core/state/piece.hpp>            // pawn, king
#include <dctl/util/type_traits.hpp>            // set_t, square_t
#include <xstd/type_traits.hpp>                 // optional_type
#include <cassert>                              // assert

namespace dctl::core {

template<class Rules, class Board>
class basic_action
{
public:
        using  rules_type = Rules;
        using  board_type = Board;
        using   mask_type = basic_mask<Board>;
        using    set_type =    set_t<mask_type>;
        using square_type = square_t<mask_type>;

private:
        set_type m_captured_pieces;
        square_type m_from;
        square_type m_dest;
        piece m_with;
        piece m_into;
        xstd::optional_type<
                is_contents_precedence_v<Rules>,
                square_type
        > m_num_captured_kings [[no_unique_address]] {};
        xstd::optional_type<
                is_ordering_precedence_v<Rules>,
                set_type
        > m_piece_order [[no_unique_address]];

        [[nodiscard]] constexpr auto is_demotion() const noexcept
        {
                return m_with != piece::pawn && m_into == piece::pawn;
        }

        [[nodiscard]] constexpr auto assert_invariants() const noexcept
        {
                assert(m_from != m_dest || is_jump());
                assert(!is_demotion());
        }

        [[nodiscard]] constexpr auto reverse_index() const noexcept
        {
                return static_cast<int>(set_type::max_size()) - 1 - num_captured_pieces();
        }

public:
        basic_action() = default;
        bool operator==(basic_action const& other) const = default;

        [[nodiscard]] constexpr basic_action(int src, int dst, bool is_promotion) noexcept
        :
                m_captured_pieces(),
                m_from(static_cast<square_type>(src)),
                m_dest(static_cast<square_type>(dst)),
                m_with(piece::pawn),
                m_into(is_promotion ? piece::king : piece::pawn)
        {
                assert_invariants();
        }

        [[nodiscard]] constexpr basic_action(int src, int dst) noexcept
        :
                m_captured_pieces(),
                m_from(static_cast<square_type>(src)),
                m_dest(static_cast<square_type>(dst)),
                m_with(piece::king),
                m_into(piece::king)
        {
                assert_invariants();
        }

        [[nodiscard]] constexpr auto capture(int sq, bool is_king [[maybe_unused]]) noexcept
        {
                assert(board_type::is_onboard(sq));
                if constexpr (is_contents_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>) {
                        if (is_king) {
                                if constexpr (is_contents_precedence_v<Rules>) {
                                        ++m_num_captured_kings;
                                }
                                if constexpr (is_ordering_precedence_v<Rules>) {
                                        m_piece_order.add(reverse_index());
                                }
                        }
                }
                m_captured_pieces.add(sq);
        }

        [[nodiscard]] constexpr auto release(int sq, bool is_king [[maybe_unused]]) noexcept
        {
                assert(board_type::is_onboard(sq));
                m_captured_pieces.pop(sq);
                if constexpr (is_contents_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>) {
                        if (is_king) {
                                if constexpr (is_ordering_precedence_v<Rules>) {
                                        m_piece_order.pop(reverse_index());
                                }
                                if constexpr (is_contents_precedence_v<Rules>) {
                                        --m_num_captured_kings;
                                }
                        }
                }
        }

        [[nodiscard]] constexpr auto captured_pieces() const noexcept
        {
                return m_captured_pieces;
        }

        [[nodiscard]] constexpr auto num_captured_pieces() const noexcept
        {
                return m_captured_pieces.ssize();
        }

        [[nodiscard]] constexpr auto from(int sq) noexcept
        {
                assert(board_type::is_onboard(sq));
                m_from = static_cast<square_type>(sq);
        }

        [[nodiscard]] constexpr auto from() const noexcept
        {
                return m_from;
        }

        [[nodiscard]] constexpr auto dest(int sq) noexcept
        {
                assert(board_type::is_onboard(sq));
                m_dest = static_cast<square_type>(sq);
        }

        [[nodiscard]] constexpr auto dest() const noexcept
        {
                return m_dest;
        }

        [[nodiscard]] constexpr auto with(piece p) noexcept
        {
                m_with = p;
        }

        [[nodiscard]] constexpr auto with() const noexcept
        {
                return m_with;
        }

        [[nodiscard]] constexpr auto into(piece p) noexcept
        {
                m_into = p;
        }

        [[nodiscard]] constexpr auto into() const noexcept
        {
                return m_into;
        }

        [[nodiscard]] constexpr auto is_with_king() const noexcept
        {
                return m_with == piece::king;
        }

        [[nodiscard]] constexpr auto is_jump() const noexcept
        {
                return !m_captured_pieces.empty();
        }

        [[nodiscard]] constexpr auto is_promotion() const noexcept
        {
                return m_with == piece::pawn && m_into != piece::pawn;
        }

        [[nodiscard]] constexpr auto is_reversible() const noexcept
        {
                return is_with_king() && !is_jump();
        }

        [[nodiscard]] constexpr auto num_captured_kings() const noexcept
                requires is_contents_precedence_v<Rules>
        {
                return m_num_captured_kings;
        }

        [[nodiscard]] constexpr auto piece_order() const noexcept
                requires is_ordering_precedence_v<Rules>
        {
                return m_piece_order;
        }
};

}       // namespace dctl::core
