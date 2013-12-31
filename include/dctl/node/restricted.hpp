#pragma once
#include <dctl/node/side.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/zobrist/restricted.hpp>

namespace dctl {

template<class Rules, class Board>
class KingMoves
{
public:
        enum { M = rules::max_same_king_moves<Rules>::value };
        enum { N = Board::set_type::N };

        // structors

        constexpr KingMoves() noexcept = default;

        // modifiers

        constexpr void init(int dest)
        {
                assert(0 <= dest && dest < N);
                king_ = dest + 1;
                moves_ = 1;
                assert(0 < moves_ && moves_ <= M);
        }

        constexpr void increment(int dest)
        {
                assert(0 <= dest && dest < N);
                assert(0 < moves_ && moves_ <= M);
                king_ = dest + 1;
                ++moves_;
                assert(1 < moves_ && move_ <= M);
        }

        constexpr void reset() noexcept
        {
                king_ = 0;
                moves_ = 0;
        }

        // predicates

        constexpr auto is_max() const noexcept
        {
                return moves_ == M;
        }

        // queries

        constexpr auto king() const noexcept
        {
                return king_ - 1;
        }

        constexpr auto index() const
        {
                return king_;
        }

        constexpr auto moves() const noexcept
        {
                return moves_;
        }

private:
        // representation

        int king_{};
        int moves_{};
};

template<class Rules, class Board>
using Restricted = KingMoves<Rules, Board>[2];

template<class Rules, class Board>
auto zobrist_hash(KingMoves<Rules, Board> const& king_moves, bool to_move)
{
        enum { M = KingMoves<Rules, Board>::M };
        enum { N = KingMoves<Rules, Board>::N };
        using Zobrist = zobrist::Restricted<M, N>;
        using Index = typename Zobrist::index_type;

        return
                Zobrist::index[to_move][static_cast<std::size_t>(king_moves.index())] ^
                Zobrist::moves[to_move][static_cast<std::size_t>(king_moves.moves())]
        ;
}

template<class Rules, class Board>
auto zobrist_hash(Restricted<Rules, Board> const& restricted)
{
        return
                zobrist_hash(restricted[Side::black], bool(Side::black)) ^
                zobrist_hash(restricted[Side::white], bool(Side::white))
        ;
}

}       // namespace dctl
