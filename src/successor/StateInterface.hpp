namespace dctl {
namespace successor {

template<typename Board> 
void StateInterface<Board>::generate(const Position<Board>& p, Stack& moves) const 
{
        do_generate(p, moves);
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

}       // namespace successor
}       // namespace dctl
