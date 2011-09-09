#include <boost/assert.hpp>

namespace dctl {
namespace successor {

template<typename Board> 
void StateInterface<Board>::generate(const Position<Board>& p, Stack& moves) const 
{
        do_generate(p, moves);
        BOOST_ASSERT(invariant(p, moves.size()));
}

template<typename Board>
int StateInterface<Board>::count(const Position<Board>& p) const
{
        return do_count(p);
}

template<typename Board> 
bool StateInterface<Board>::detect(const Position<Board>& p) const 
{ 
        return do_detect(p);
}

template<typename Board> 
bool StateInterface<Board>::invariant(const Position<Board>& p, int number) const
{
        return (
                (count(p) == number) && 
                (detect(p) == (number > 0))
        );
}

}       // namespace successor
}       // namespace dctl
