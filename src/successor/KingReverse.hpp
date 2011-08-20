namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::KING, Reverse, Rules, Board>::generate(const Position<Board>& p, Stack& moves)
{
        Active::generate(p, moves);
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::KING, Reverse, Rules, Board>::count(const Position<Board>& p)
{
        return Active::count(p);
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::KING, Reverse, Rules, Board>::detect(const Position<Board>& p)
{
        return Active::detect(p);
}

}       // namespace successor
}       // namespace dctl
