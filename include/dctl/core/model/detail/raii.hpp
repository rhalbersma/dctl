#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/piece.hpp>    // pawn, king
#include <cassert>                      // assert

namespace dctl::core {
namespace raii {

template<class Builder>
class lift
{
        Builder& m_builder;
        int const m_square;
public:
        ~lift() // Throws: Nothing.
        {
                m_builder.drop(m_square);
        }

        lift(int const sq, Builder& b) // Throws: Nothing.
        :
                m_builder{b},
                m_square{sq}
        {
                m_builder.lift(m_square);
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

        capture(int const sq, Builder& b) // Throws: Nothing.
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
                assert(m_builder.with() == piece::king); assert(m_builder.into() == piece::king);
                m_builder.with(piece::pawn);
                m_builder.into(piece::pawn);
        }

        explicit set_king_jump(Builder& b) noexcept
        :
                m_builder{b}
        {
                assert(m_builder.with() == piece::pawn); assert(m_builder.into() == piece::pawn);
                m_builder.with(piece::king);
                m_builder.into(piece::king);
        }
};

template<class Builder>
class promotion
{
        Builder& m_builder;
public:
        ~promotion() noexcept
        {
                assert(m_builder.with() == piece::pawn); assert(m_builder.into() == piece::king);
                m_builder.into(piece::pawn);
        }

        explicit promotion(Builder& b) noexcept
        :
                m_builder{b}
        {
                assert(m_builder.with() == piece::pawn); assert(m_builder.into() == piece::pawn);
                m_builder.into(piece::king);
        }
};

}       // namespace raii
}       // namespace dctl::core
