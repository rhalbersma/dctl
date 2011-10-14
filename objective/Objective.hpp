#pragma once

namespace dctl {
namespace objective {

/* first side to run out of moves, loses */
struct Regular;

/* first side to run out of moves, wins */
struct Misere;
typedef Misere Suicide;
typedef Misere Giveaway;
typedef Misere Poddavki;
typedef Misere QuiPerdGagne;

/* first side to get a king, wins */
struct Kingscourt;

}       // namespace objective
}       // namespace dctl
