#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>
#include <dctl/core/state/piece.hpp>    // pawn, king
#include <cassert>                      // assert

namespace dctl::core::raii {

template<class Builder>
class lift
{
        Builder& m_builder;
        int const m_square;
public:
        constexpr ~lift() noexcept
        {
                m_builder.drop(m_square);
        }

        constexpr lift(int const sq, Builder& b) noexcept
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
        constexpr ~capture() noexcept
        {
                m_builder.release(m_square);
        }

        constexpr capture(int const sq, Builder& b) noexcept
        :
                m_builder{b},
                m_square{sq}
        {
                m_builder.capture(m_square);
        }
};

template<class Builder>
class king_targets
{
        Builder& m_builder;
public:
        ~king_targets() = default;

        constexpr ~king_targets() noexcept
                requires is_superior_rank_jump_v<rules_t<Builder>>
        {
                m_builder.toggle_king_targets();
        }

        explicit constexpr king_targets(Builder& b) noexcept
        :
                m_builder(b)
        {
                if constexpr (is_superior_rank_jump_v<rules_t<Builder>>) {
                        m_builder.toggle_king_targets();
                }
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
                m_builder(b)
        {
                assert(m_builder.with() == piece::pawn);
                assert(m_builder.into() == piece::pawn);
                m_builder.into(piece::king);
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
                m_builder(b)
        {
                m_builder.visit(sq);
        }
};

}       // namespace dctl::core::raii
