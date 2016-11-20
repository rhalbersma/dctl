#pragma once
#include <dctl/color_piece.hpp> // pawn, king
#include <cassert>              // assert

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
        int const square;
public:
        ~launch()
        {
                builder.undo_launch(square);
        }

        launch(Builder& b, int const sq)
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
        int const square;
public:
        ~capture()
        {
                builder.release(square);
        }

        capture(Builder& b, int const sq)
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

        Visit(Builder& b, int sq)
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
                assert(builder.is_with(piece::king) && builder.is_into(piece::king));
                builder.with(piece::pawn);
                builder.into(piece::pawn);
        }

        Setking_jump(Setking_jump const&) = delete;
        Setking_jump& operator=(Setking_jump const&) = delete;

        Setking_jump(Builder& b)
        :
                builder{b}
        {
                assert(builder.is_with(piece::pawn) && builder.is_into(piece::pawn));
                builder.with(piece::king);
                builder.into(piece::king);
        }
};

template<class Builder>
class promotion
{
        Builder& builder;
public:
        ~promotion()
        {
                assert(builder.is_with(piece::pawn) && builder.is_into(piece::king));
                builder.into(piece::pawn);
        }

        promotion(Builder& b)
        :
                builder{b}
        {
                assert(builder.is_with(piece::pawn) && builder.is_into(piece::pawn));
                builder.into(piece::king);
        }
};

}       // namespace raii
}       // namespace dctl
