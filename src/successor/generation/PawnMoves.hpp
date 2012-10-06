#pragma once
#include <functional>                   // function
#include <boost/utility.hpp>            // noncopyable
#include "Generator_fwd.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Material.hpp"
#include "../../node/Promotion.hpp"
#include "../../node/Stack.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves_ generation
template<bool Color, typename Position>
struct generator<Color, Material::pawn, Moves, Position>
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<void(Position const&)>
{
private:
        // typedefs

        typedef typename Position::board_type Board;
        typedef angle::Compass<Color, Board> Compass;

        // representation

        Stack& moves_;

public:
        // structors

        explicit generator(Stack& m)
        :
                moves_(m)
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                if (auto const active_pawns = p.pawns(Color))
                        select(active_pawns, not_occupied(p));
        }

        void select(BitBoard active_pawns, BitBoard not_occupied) const
        {
                branch(active_pawns, not_occupied);
        }

private:
        void branch(BitBoard active_pawns, BitBoard not_occupied) const
        {
                serialize<typename Compass::left_up >(active_pawns, not_occupied);
                serialize<typename Compass::right_up>(active_pawns, not_occupied);
        }

        template<typename Direction>
        void serialize(BitBoard active_pawns, BitBoard not_occupied) const
        {
                for (
                        active_pawns &= Prev<Board, Direction>()(not_occupied);
                        active_pawns;
                        bit::clear_first(active_pawns)
                )
                        find<Direction>(bit::get_first(active_pawns));
        }

        template<typename Direction>
        void find(BitIndex from_sq) const
        {
                auto const dest_sq = Next<Board, Direction>()(from_sq);
                moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq, promotion_sq<Color, Board>(dest_sq)));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
