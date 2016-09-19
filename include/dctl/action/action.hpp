#pragma once
#include <dctl/piece.hpp>                       // pawn, king
#include <dctl/rule_traits.hpp>                 // rectangular, is_quality_precedence, is_ordering_precedence
#include <dctl/utility/logic.hpp>               // implies
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
struct base_quality_precedence
{
        set_t<Board> captured_kings_;
};

template<class Rules, class Board>
using quality_precedence_t = std::conditional_t<
        is_quality_precedence_v<Rules>,
        base_quality_precedence<Board>,
        util::tagged_empty_base<0>
>;

template<class Board>
struct base_ordering_precedence
{
        set_t<Board> piece_order_;
};

template<class Rules, class Board>
using ordering_precedence_t = std::conditional_t<
        is_ordering_precedence_v<Rules>,
        base_ordering_precedence<Board>,
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
        set_type captured_pieces_;
        square_type from_;
        square_type dest_;
        Piece with_;
        Piece into_;

        constexpr auto assert_invariants() const noexcept
        {
                assert(util::implies(from() == dest(), is_jump()));
                assert(!is_demotion());
        }
public:
        action() = default;

        constexpr action(std::size_t const src, std::size_t const dst, bool const promotion) noexcept
        :
                ordering_precedence_t{},
                quality_precedence_t{},
                captured_pieces_{},
                from_{static_cast<square_type>(src)},
                dest_{static_cast<square_type>(dst)},
                with_{Piece::pawn},
                into_{promotion ? Piece::king : Piece::pawn}
        {
                assert_invariants();
        }

        constexpr action(std::size_t const src, std::size_t const dst) noexcept
        :
                ordering_precedence_t{},
                quality_precedence_t{},
                captured_pieces_{},
                from_{static_cast<square_type>(src)},
                dest_{static_cast<square_type>(dst)},
                with_{Piece::king},
                into_{Piece::king}
        {
                assert_invariants();
        }

        constexpr auto capture(std::size_t const sq, bool const is_king) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                if constexpr (is_quality_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>) {
                        capture_quality_ordering(sq, is_king);
                }
                captured_pieces_.set(sq);
        }

        constexpr auto release(std::size_t const sq, bool const is_king) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                captured_pieces_.reset(sq);
                if constexpr (is_quality_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>) {
                        release_quality_ordering(sq, is_king);
                }
        }

        constexpr auto from(std::size_t const sq) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                from_ = static_cast<square_type>(sq);
        }

        constexpr auto from() const noexcept
        {
                return from_;
        }

        constexpr auto dest(std::size_t const sq) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                dest_ = static_cast<square_type>(sq);
        }

        constexpr auto dest() const noexcept
        {
                return dest_;
        }

        constexpr auto with(Piece const p) noexcept
        {
                with_ = p;
        }

        constexpr auto with() const noexcept
        {
                return with_;
        }

        constexpr auto into(Piece const p) noexcept
        {
                into_ = p;
        }

        constexpr auto into() const noexcept
        {
                return into_;
        }

        constexpr auto captured_pieces() const noexcept
        {
                return captured_pieces_;
        }

        constexpr auto num_captured_pieces() const noexcept
        {
                return captured_pieces().count();
        }

        constexpr auto is_with_king() const noexcept
        {
                return with() == Piece::king;
        }

        constexpr auto is_jump() const noexcept
        {
                return captured_pieces().any();
        }

        constexpr auto is_promotion() const noexcept
        {
                return with() == Piece::pawn && into() != Piece::pawn;
        }

        constexpr auto is_reversible() const noexcept
        {
                return with() == Piece::king && !is_jump();
        }

        template<class RulesType = rules_type, std::enable_if_t<
                is_quality_precedence_v<RulesType> &&
		std::is_same<RulesType, rules_type>{}
        >* = nullptr>
        constexpr auto captured_kings() const noexcept
        {
                return this->captured_kings_;
        }

        template<class RulesType = rules_type, std::enable_if_t<
        	is_quality_precedence_v<RulesType> &&
		std::is_same<RulesType, rules_type>{}
        >* = nullptr>
        constexpr auto num_captured_kings() const noexcept
        {
                return captured_kings().count();
        }

        template<class RulesType = rules_type, std::enable_if_t<
		is_ordering_precedence_v<RulesType> &&
		std::is_same<RulesType, rules_type>{}
        >* = nullptr>
        constexpr auto Piece_order() const noexcept
        {
                return this->piece_order_;
        }
private:
        constexpr auto capture_quality_ordering(std::size_t const sq, bool const is_king)
        {
                static_assert(is_quality_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>);
                if (is_king) {
                        if constexpr (is_quality_precedence_v<rules_type>) {
                                this->captured_kings_.set(sq);
                        }
                        if constexpr (is_ordering_precedence_v<rules_type>) {
                                this->piece_order_.set(set_type::size() - 1 - num_captured_pieces());
                        }
                }
        }

        constexpr auto release_quality_ordering(std::size_t const sq, bool const is_king)
        {
                static_assert(is_quality_precedence_v<rules_type> || is_ordering_precedence_v<rules_type>);
                if (is_king) {
                        if constexpr (is_ordering_precedence_v<rules_type>) {
                                this->piece_order_.reset(set_type::size() - 1 - num_captured_pieces());
                        }
                        if constexpr (is_quality_precedence_v<rules_type>) {
                                this->captured_kings_.reset(sq);
                        }
                }
        }

        constexpr auto is_onboard(std::size_t const sq) const noexcept
        {
                return sq < set_type::size();
        }

        constexpr auto is_demotion() const noexcept
        {
                return with() != Piece::pawn && into() == Piece::pawn;
        }
};

template<class Rules, class Board, std::enable_if_t<
        !is_ordering_precedence_v<Rules>
>* = nullptr>
constexpr auto as_tuple(action<Rules, Board> const& a) noexcept
{
        return std::make_tuple(a.from(), a.dest(), a.captured_pieces());
}

template<class Rules, class Board, std::enable_if_t<
	is_ordering_precedence_v<Rules>
>* = nullptr>
constexpr auto as_tuple(action<Rules, Board> const& a) noexcept
{
        return std::make_tuple(a.from(), a.dest(), a.captured_pieces(), a.Piece_order());
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
