#pragma once
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/copy/generator_fwd.hpp>
#include <dctl/successor/copy/king_reverse.hpp>
#include <dctl/successor/copy/pawn_reverse.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, Reverse, Position>
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        // typedefs

        typedef generator<Color, Material::king, Reverse, Position> KingMoves;
        typedef generator<Color, Material::pawn, Reverse, Position> PawnMoves;

        // representation

        Vector<Move>& moves_;

public:
        // structors

        /*explicit*/ generator(Vector<Move>& m)
        :
                moves_(m)
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                KingMoves{moves_}(p);
                PawnMoves{moves_}(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
