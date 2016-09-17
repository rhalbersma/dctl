#pragma once
#include <dctl/piece.hpp>       // pawn, king
#include <cassert>              // assert
#include <cstddef>              // size_t

namespace dctl {
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
                builder.release(square);
        }

        capture(Builder& b, std::size_t const sq)
        :
                builder{b},
                square{sq}
        {
                builder.capture(square);
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
class Setking_jump
{
        Builder& builder;
public:
        ~Setking_jump()
        {
                assert(builder.is_with(Piece::king) && builder.is_into(Piece::king));
                builder.with(Piece::pawn);
                builder.into(Piece::pawn);
        }

        Setking_jump(Setking_jump const&) = delete;
        Setking_jump& operator=(Setking_jump const&) = delete;

        Setking_jump(Builder& b)
        :
                builder{b}
        {
                assert(builder.is_with(Piece::pawn) && builder.is_into(Piece::pawn));
                builder.with(Piece::king);
                builder.into(Piece::king);
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
                builder.into(Piece::pawn);
        }

        promotion(Builder& b)
        :
                builder{b}
        {
                assert(builder.is_with(Piece::pawn) && builder.is_into(Piece::pawn));
                builder.into(Piece::king);
        }
};

}       // namespace raii
}       // namespace dctl
