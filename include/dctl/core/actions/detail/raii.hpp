#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/color_piece.hpp>      // pawn, king
#include <cassert>                              // assert

namespace dctl::core {
namespace raii {

template<class Builder>
class launch
{
        Builder& m_builder;
        int const m_square;
public:
        ~launch() // Throws: Nothing.
        {
                m_builder.undo_launch(m_square);
        }

        launch(Builder& b, int const sq) // Throws: Nothing.
        :
                m_builder{b},
                m_square{sq}
        {
                m_builder.make_launch(m_square);
        }
};

template<class Builder>
class capture
{
        Builder& m_builder;
        int const m_square;
public:
        ~capture() // Throws: Nothing.
        {
                m_builder.release(m_square);
        }

        capture(Builder& b, int const sq) // Throws: Nothing.
        :
                m_builder{b},
                m_square{sq}
        {
                m_builder.capture(m_square);
        }
};

template<class Builder>
class Visit
{
        Builder& m_builder;
public:
        ~Visit()
        {
                m_builder.leave();
        }

        Visit(Builder& b, int sq)
        :
                m_builder{b}
        {
                m_builder.visit(sq);
        }
};

template<class Builder>
class set_king_jump
{
        Builder& m_builder;
public:
        ~set_king_jump() noexcept
        {
                assert(m_builder.with() == piece::kings); assert(m_builder.into() == piece::kings);
                m_builder.with(piece::pawns);
                m_builder.into(piece::pawns);
        }

        explicit set_king_jump(Builder& b) noexcept
        :
                m_builder{b}
        {
                assert(m_builder.with() == piece::pawns); assert(m_builder.into() == piece::pawns);
                m_builder.with(piece::kings);
                m_builder.into(piece::kings);
        }
};

template<class Builder>
class promotion
{
        Builder& m_builder;
public:
        ~promotion() noexcept
        {
                assert(m_builder.with() == piece::pawns); assert(m_builder.into() == piece::kings);
                m_builder.into(piece::pawns);
        }

        explicit promotion(Builder& b) noexcept
        :
                m_builder{b}
        {
                assert(m_builder.with() == piece::pawns); assert(m_builder.into() == piece::pawns);
                m_builder.into(piece::kings);
        }
};

}       // namespace raii
}       // namespace dctl::core
