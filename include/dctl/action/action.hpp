#pragma once
#include <dctl/piece.hpp>                       // pawn, king
#include <dctl/rule_traits.hpp>                 // is_quality_precedence, is_ordering_precedence
#include <dctl/utility/logic.hpp>               // implies
#include <dctl/utility/tagged_empty_base.hpp>   // tagged_empty_base
#include <dctl/utility/type_traits.hpp>         // set_t, square_t
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <tuple>                                // make_tuple
#include <type_traits>                          // conditional, enable_if

namespace dctl {
namespace detail {
namespace block_adl {

template<class Board>
struct base_action
{
           set_t<Board> captured_pieces_;
        square_t<Board> from_;
        square_t<Board> dest_;
        Piece with_;
        Piece into_;
};

enum base_tags { quality, ordering };

template<class Board>
struct quality_action
{
        set_t<Board> captured_kings_;
};

template<class Rules, class Board>
using conditional_quality = std::conditional_t<
        is_quality_precedence_or_v<Rules>,
        quality_action<Board>,
        util::tagged_empty_base<base_tags::quality>
>;

template<class Board>
struct ordering_action
{
        set_t<Board> piece_order_;
};

template<class Rules, class Board>
using conditional_ordering = std::conditional_t<
        is_ordering_precedence_or_v<Rules>,
        ordering_action<Board>,
        util::tagged_empty_base<base_tags::ordering>
>;

}       // namespace block_adl

using block_adl::base_action;
using block_adl::conditional_quality;
using block_adl::conditional_ordering;

}       // namespace detail

template<class Rules, class Board>
class action
:
        detail::conditional_ordering<Rules, Board>,
        detail::conditional_quality <Rules, Board>,
        detail::base_action<Board>
{
        using ordering = detail::conditional_ordering<Rules, Board>;
        using quality  = detail::conditional_quality <Rules, Board>;
        using base     = detail::base_action<Board>;

        constexpr auto assert_invariants() const noexcept
        {
                assert(util::implies(from() == dest(), is_jump()));
                assert(!is_demotion());
        }
public:
        using  board_type = Board;
        using  rules_type = Rules;
        using    set_type =    set_t<Board>;
        using square_type = square_t<Board>;

        action() = default;

        constexpr action(std::size_t const src, std::size_t const dst, bool const promotion) noexcept
        :
                ordering{}, quality{}, base{ set_type{}, static_cast<square_type>(src), static_cast<square_type>(dst), Piece::pawn, promotion ? Piece::king : Piece::pawn }
        {
                assert_invariants();
        }

        constexpr action(std::size_t const src, std::size_t const dst) noexcept
        :
                ordering{}, quality{}, base{ set_type{}, static_cast<square_type>(src), static_cast<square_type>(dst), Piece::king, Piece::king }
        {
                assert_invariants();
        }

        constexpr auto capture(std::size_t const sq, bool const is_king) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                capture_quality_ordering_dispatch(sq, is_king, is_quality_precedence_or_t<Rules>{}, is_ordering_precedence_or_t<Rules>{});
                this->captured_pieces_.set(sq);
        }

        constexpr auto release(std::size_t const sq, bool const is_king) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                this->captured_pieces_.reset(sq);
                release_quality_ordering_dispatch(sq, is_king, is_quality_precedence_or_t<Rules>{}, is_ordering_precedence_or_t<Rules>{});
        }

        constexpr auto from(std::size_t const sq) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                this->from_ = static_cast<square_type>(sq);
        }

        constexpr auto from() const noexcept
        {
                return this->from_;
        }

        constexpr auto dest(std::size_t const sq) // Throws: Nothing.
        {
                assert(is_onboard(sq));
                this->dest_ = static_cast<square_type>(sq);
        }

        constexpr auto dest() const noexcept
        {
                return this->dest_;
        }

        constexpr auto with(Piece const p) noexcept
        {
                this->with_ = p;
        }

        constexpr auto with() const noexcept
        {
                return this->with_;
        }

        constexpr auto into(Piece const p) noexcept
        {
                this->into_ = p;
        }

        constexpr auto into() const noexcept
        {
                return this->into_;
        }

        constexpr auto captured_pieces() const noexcept
        {
                return this->captured_pieces_;
        }

        constexpr auto num_captured_pieces() const noexcept
        {
                return captured_pieces().count();
        }

        template<class RulesType = rules_type, std::enable_if_t<is_quality_precedence_or_v<RulesType>>* = nullptr>
        constexpr auto captured_kings() const noexcept
        {
                return this->captured_kings_;
        }

        template<class RulesType = rules_type, std::enable_if_t<is_quality_precedence_or_v<RulesType>>* = nullptr>
        constexpr auto num_captured_kings() const noexcept
        {
                return captured_kings().count();
        }

        template<class RulesType = rules_type, std::enable_if_t<is_ordering_precedence_or_v<RulesType>>* = nullptr>
        constexpr auto piece_order() const noexcept
        {
                return this->piece_order_;
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
private:
        constexpr auto capture_quality_ordering_dispatch(std::size_t const sq, bool const is_king, std::false_type, std::false_type)
        {
                // no-op
        }

        constexpr auto capture_quality_ordering_dispatch(std::size_t const sq, bool const is_king, std::true_type, std::false_type)
        {
                if (is_king) {
                        this->captured_kings_.set(sq);
                }
        }

        constexpr auto capture_quality_ordering_dispatch(std::size_t const /*sq*/, bool const is_king, std::false_type, std::true_type)
        {
                if (is_king) {
                        this->piece_order_.set(set_type::size() - 1 - num_captured_pieces());
                }
        }

        constexpr auto capture_quality_ordering_dispatch(std::size_t const sq, bool const is_king, std::true_type, std::true_type)
        {
                if (is_king) {
                        this->captured_kings_.set(sq);
                        this->piece_order_.set(set_type::size() - 1 - num_captured_pieces());
                }
        }

        constexpr auto release_quality_ordering_dispatch(std::size_t const sq, bool const is_king, std::false_type, std::false_type)
        {
                // no-op
        }

        constexpr auto release_quality_ordering_dispatch(std::size_t const sq, bool const is_king, std::true_type, std::false_type)
        {
                if (is_king) {
                        this->captured_kings_.reset(sq);
                }
        }

        constexpr auto release_quality_ordering_dispatch(std::size_t const /*sq*/, bool const is_king, std::false_type, std::true_type)
        {
                if (is_king) {
                        this->piece_order_.reset(set_type::size() - 1 - num_captured_pieces());
                }
        }

        constexpr auto release_quality_ordering_dispatch(std::size_t const sq, bool const is_king, std::true_type, std::true_type)
        {
                if (is_king) {
                        this->piece_order_.reset(set_type::size() - 1 - num_captured_pieces());
                        this->captured_kings_.reset(sq);
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

template<class Rules, class Board, std::enable_if_t<!is_ordering_precedence_or_v<Rules>>* = nullptr>
constexpr auto as_tuple(action<Rules, Board> const& a) noexcept
{
        return std::make_tuple(a.from(), a.dest(), a.captured_pieces());
}

template<class Rules, class Board, std::enable_if_t< is_ordering_precedence_or_v<Rules>>* = nullptr>
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
