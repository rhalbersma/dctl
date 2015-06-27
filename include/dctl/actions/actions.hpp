#pragma once
#include <dctl/player.hpp>                               // Player, black, white
#include <dctl/actions/generate/specializations.hpp>  // Generate
#include <dctl/actions/count/specializations.hpp>     // Count
#include <dctl/actions/detect/specializations.hpp>    // Detect
#include <dctl/actions/select/legal.hpp>              // legal

namespace dctl {
namespace actions {

template<class Select = select::legal, bool RemoveDuplicateJumps = true, bool Reverse = false>
class Successor
{
        template<Player ToMove, class State>
        auto invariant(State const& s, std::size_t n)
        {
                return
                        (count <ToMove>(s) ==  n     ) &&
                        (detect<ToMove>(s) == (n > 0))
                ;
        }

public:
        template<Player ToMove, class State, class Sequence>
        auto generate(State const& s, Sequence& moves)
        {
                Generate<ToMove, Select, RemoveDuplicateJumps, Reverse>{}(s, moves);
                assert((invariant<ToMove>(s, moves.size())));
        }

        template<class State, class Sequence>
        auto generate(State const& s, Sequence& moves)
        {
                return
                        (s.to_move() == Player::black) ?
                        generate<Player::black>(s, moves) :
                        generate<Player::white>(s, moves)
                ;
        }

        template<Player ToMove, class State>
        auto count(State const& s)
        {
                return Count<ToMove, Select, RemoveDuplicateJumps, Reverse>{}(s);
        }

        template<class State>
        auto count(State const& s)
        {
                return
                        (s.to_move() == Player::black) ?
                        count<Player::black>(s) :
                        count<Player::white>(s)
                ;
        }

        template<Player ToMove, class State>
        auto detect(State const& s)
        {
                return Detect<ToMove, Select, Reverse>{}(s);
        }

        template<class State>
        auto detect(State const& s)
        {
                return
                        (s.to_move() == Player::black) ?
                        detect<Player::black>(s) :
                        detect<Player::white>(s)
                ;
        }
};

}       // namespace actions
}       // namespace dctl
