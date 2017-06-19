#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/color_piece.hpp> // pawn, king
#include <cassert>              // assert

namespace dctl::core {
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
class set_king_jump
{
        Builder& builder;
public:
        ~set_king_jump()
        {
                assert(builder.with() == piece::kings && builder.into() == piece::kings);
                builder.with(piece::pawns);
                builder.into(piece::pawns);
        }

        set_king_jump(set_king_jump const&) = delete;
        set_king_jump& operator=(set_king_jump const&) = delete;

        set_king_jump(Builder& b)
        :
                builder{b}
        {
                assert(builder.with() == piece::pawns && builder.into() == piece::pawns);
                builder.with(piece::kings);
                builder.into(piece::kings);
        }
};

template<class Builder>
class promotion
{
        Builder& builder;
public:
        ~promotion()
        {
                assert(builder.with() == piece::pawns && builder.into() == piece::kings);
                builder.into(piece::pawns);
        }

        promotion(Builder& b)
        :
                builder{b}
        {
                assert(builder.with() == piece::pawns && builder.into() == piece::pawns);
                builder.into(piece::kings);
        }
};

}       // namespace raii
}       // namespace dctl::core
