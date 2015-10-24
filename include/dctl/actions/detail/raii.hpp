#pragma once
#include <dctl/piece.hpp>       // pawn, king
#include <cassert>              // assert
#include <cstddef>              // size_t

namespace dctl {
namespace actions {
namespace raii {

template<class Builder>
class Launch
{
        Builder& builder;
public:
        ~Launch()
        {
                builder.clear();
        }

        Launch(Launch const&) = delete;
        Launch& operator=(Launch const&) = delete;

        Launch(Builder& b, std::size_t sq)
        :
                builder{b}
        {
                builder.launch(sq);
        }
};

template<class Builder>
class Capture
{
        Builder& builder;
        using square_type = square_t<Builder>;
        square_type square;
public:
        ~Capture()
        {
                builder.release(square);
        }

        Capture(Capture const&) = delete;
        Capture& operator=(Capture const&) = delete;

        Capture(Builder& b, std::size_t sq)
        :
                builder{b}, square{static_cast<square_type>(sq)}
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
class ToggleKingTargets
{
        Builder& builder;
public:
        ~ToggleKingTargets()
        {
                builder.toggle_king_targets();
        }

        ToggleKingTargets(ToggleKingTargets const&) = delete;
        ToggleKingTargets& operator=(ToggleKingTargets const&) = delete;

        ToggleKingTargets(Builder& b)
        :
                builder{b}
        {
                builder.toggle_king_targets();
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
class SetPromotion
{
        Builder& builder;
public:
        ~SetPromotion()
        {
                assert(builder.is_with(Piece::pawn) && builder.is_into(Piece::king));
                builder.set_into(Piece::pawn);
        }

        SetPromotion(SetPromotion const&) = delete;
        SetPromotion& operator=(SetPromotion const&) = delete;

        SetPromotion(Builder& b)
        :
                builder{b}
        {
                assert(builder.is_with(Piece::pawn) && builder.is_into(Piece::pawn));
                builder.set_into(Piece::king);
        }
};

}       // namespace raii
}       // namespace actions
}       // namespace dctl
