#pragma once
#include <dctl/successor/select/legal.hpp>              // DefaultSelection
#include <dctl/successor/detect/specializations.hpp>    // detect
#include <dctl/successor/material/piece.hpp>            // piece
#include <dctl/node/side.hpp>                           // black, white

namespace dctl {
namespace successor {

template<bool Color, typename Material, typename Select, typename Position>
bool detect(Position const& p)
{
        typedef typename Position::rules_type Rules;
        typedef typename rules::range_king<Rules>::type Range;
        return detail::detect<Color, Material, Select, Range>()(p);
}

template<typename Material, typename Select, typename Position>
bool detect(Position const& p)
{
        return (p.to_move() == Side::black)?
                detect<Side::black, Material, Select>(p) :
                detect<Side::white, Material, Select>(p)
        ;
}

template<typename Position>
bool detect(Position const& p)
{
        return detect<material::piece, select::legal>(p);
}

}       // namespace successor
}       // namespace dctl
