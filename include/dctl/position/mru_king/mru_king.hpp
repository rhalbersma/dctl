#pragma once
#include <dctl/position/mru_king/zobrist.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/set_type.hpp>

namespace dctl {

template<class Rules, class Board>
class MostRecentlyUsedKing
{
public:
        enum { M = max_same_king_push_v<Rules> };
        enum { N = set_type<Board>::size() };

        // constructors

        constexpr MostRecentlyUsedKing() = default;

        // modifiers

        constexpr void activate()
        {
                assert(is_min());
                is_active_ = true;
        }

        constexpr void deactivate()
        {
                assert(is_min());
                is_active_ = false;
        }

        constexpr void init(std::size_t dest_sq)
        {
                assert(is_active());
                assert(0 <= dest_sq && dest_sq < N);
                index_ = dest_sq + 1;
                moves_ = 1;
                assert(0 < moves_ && moves_ <= M);
        }

        constexpr void increment(std::size_t dest_sq)
        {
                assert(is_active());
                assert(0 <= dest_sq && dest_sq < N);
                assert(0 < moves_ && moves_ < M);
                index_ = dest_sq + 1;
                ++moves_;
                assert(1 < moves_ && moves_ <= M);
        }

        constexpr void reset()
        {
                assert(is_active());
                index_ = 0;
                moves_ = 0;
        }

        // observers

        constexpr auto square() const noexcept
        {
                return index_ - 1;
        }

        constexpr auto index() const noexcept
        {
                return index_;
        }

        constexpr auto moves() const noexcept
        {
                return moves_;
        }

        // predicates

        constexpr auto is_active() const noexcept
        {
                return is_active_;
        }

        constexpr auto is_min() const noexcept
        {
                return moves_ == 0;
        }

        constexpr auto is_max() const noexcept
        {
                return moves_ == M;
        }

private:
        // representation

        std::size_t index_{};
        std::size_t moves_{};
        bool is_active_{};
};

template<class TabulationHash, class Rules, class Board>
auto hash_xor_accumulate(TabulationHash const& h, MostRecentlyUsedKing<Rules, Board> const& mru_king, bool to_move)
{
        return
                h.index[to_move][mru_king.index()] ^
                h.moves[to_move][mru_king.moves()]
        ;
}

}       // namespace dctl
