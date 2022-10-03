#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/piece.hpp>    // pawn, king
#include <cassert>                      // assert

namespace dctl::core::raii {

template<class Builder>
class lift
{
        Builder& m_builder;
        int const m_square;
public:
        constexpr ~lift() // Throws: Nothing.
        {
                m_builder.drop(m_square);
        }

        constexpr lift(int const sq, Builder& b) // Throws: Nothing.
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
        constexpr ~capture() // Throws: Nothing.
        {
                m_builder.release(m_square);
        }

        constexpr capture(int const sq, Builder& b) // Throws: Nothing.
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
        constexpr ~Visit()
        {
                m_builder.leave();
        }

        constexpr Visit(Builder& b, int sq)
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
        constexpr ~set_king_jump() noexcept
        {
                assert(m_builder.with() == piece::king);
                assert(m_builder.into() == piece::king);
                m_builder.with(piece::pawn);
                m_builder.into(piece::pawn);
        }

        explicit constexpr set_king_jump(Builder& b) noexcept
        :
                m_builder{b}
        {
                assert(m_builder.with() == piece::pawn);
                assert(m_builder.into() == piece::pawn);
                m_builder.with(piece::king);
                m_builder.into(piece::king);
        }
};

template<class Builder>
class promotion
{
        Builder& m_builder;
public:
        constexpr ~promotion() noexcept
        {
                assert(m_builder.with() == piece::pawn);
                assert(m_builder.into() == piece::king);
                m_builder.into(piece::pawn);
        }

        explicit constexpr promotion(Builder& b) noexcept
        :
                m_builder{b}
        {
                assert(m_builder.with() == piece::pawn);
                assert(m_builder.into() == piece::pawn);
                m_builder.into(piece::king);
        }
};

}       // namespace dctl::core::raii
