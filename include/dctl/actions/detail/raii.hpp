#pragma once
#include <dctl/piece.hpp>       // pawn, king
#include <cassert>              // assert
#include <cstddef>              // size_t

namespace dctl {
namespace core {
namespace raii {

template<class Builder>
class toggle_king_targets
{
        Builder& builder;
public:
        ~toggle_king_targets()
        {
                builder.toggle_king_targets();
        }

        toggle_king_targets(Builder& b)
        :
                builder{b}
        {
                builder.toggle_king_targets();
        }
};

template<class Builder>
class launch
{
        Builder& builder;
        std::size_t const square;
public:
        ~launch()
        {
                builder.undo_launch(square);
        }

        launch(Builder& b, std::size_t const sq)
        :
                builder{b},
                square{sq}
        {
                builder.make_launch(square);
        }
};

template<class Builder>
class capture
{
        Builder& builder;
        std::size_t const square;
public:
        ~capture()
        {
                builder.release_piece(square);
        }

        capture(Builder& b, std::size_t const sq)
        :
                builder{b},
                square{sq}
        {
                builder.capture_piece(square);
        }
};

template<class Builder>
class Visit
{
        Builder& builder;
public:
        ~Visit()
        {
                builder.leave();
        }

        Visit(Visit const&) = delete;
        Visit& operator=(Visit const&) = delete;

        Visit(Builder& b, std::size_t sq)
        :
                builder{b}
        {
                builder.visit(sq);
        }
};

template<class Builder>
class SetKingJump
{
        Builder& builder;
public:
        ~SetKingJump()
        {
                assert(builder.is_with(Piece::king) && builder.is_into(Piece::king));
                builder.set_with(Piece::pawn);
                builder.set_into(Piece::pawn);
        }

        SetKingJump(SetKingJump const&) = delete;
        SetKingJump& operator=(SetKingJump const&) = delete;

        SetKingJump(Builder& b)
        :
                builder{b}
        {
                assert(builder.is_with(Piece::pawn) && builder.is_into(Piece::pawn));
                builder.set_with(Piece::king);
                builder.set_into(Piece::king);
        }
};

template<class Builder>
class promotion
{
        Builder& builder;
public:
        ~promotion()
        {
                assert(builder.is_with(Piece::pawn) && builder.is_into(Piece::king));
                builder.set_into(Piece::pawn);
        }

        promotion(Builder& b)
        :
                builder{b}
        {
                assert(builder.is_with(Piece::pawn) && builder.is_into(Piece::pawn));
                builder.set_into(Piece::king);
        }
};

}       // namespace raii
}       // namespace core
}       // namespace dctl
