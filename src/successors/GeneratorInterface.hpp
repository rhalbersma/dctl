namespace dctl {
namespace successors {

template<typename Rules, typename Board> 
void GeneratorInterface<Rules, Board>::generate(const Position<Board>& p, Stack& moves) const 
{
        do_generate(p, moves);
}

template<typename Rules, typename Board> 
void GeneratorInterface<Rules, Board>::generate_moves(const Position<Board>& p, Stack& moves) const 
{ 
        do_generate_moves(p, moves);
}

template<typename Rules, typename Board> 
void GeneratorInterface<Rules, Board>::generate_jumps(const Position<Board>& p, Stack& moves) const 
{ 
        do_generate_jumps(p, moves);
}

template<typename Rules, typename Board> 
void GeneratorInterface<Rules, Board>::generate_reverse(const Position<Board>& p, Stack& moves) const 
{ 
        do_generate_reverse(p, moves);
}

template<typename Rules, typename Board> 
void GeneratorInterface<Rules, Board>::generate_promotions(const Position<Board>& p, Stack& moves) const 
{ 
        do_generate_promotions(p, moves);
}

template<typename Rules, typename Board>
int GeneratorInterface<Rules, Board>::count(const Position<Board>& p) const
{
        return do_count(p);
}

template<typename Rules, typename Board>
int GeneratorInterface<Rules, Board>::count_moves(const Position<Board>& p) const
{
        return do_count_moves(p);
}

template<typename Rules, typename Board>
int GeneratorInterface<Rules, Board>::count_jumps(const Position<Board>& p) const
{
        return do_count_jumps(p);
}

template<typename Rules, typename Board>
int GeneratorInterface<Rules, Board>::count_reverse(const Position<Board>& p) const
{
        return do_count_reverse(p);
}

template<typename Rules, typename Board>
int GeneratorInterface<Rules, Board>::count_promotions(const Position<Board>& p) const
{
        return do_count_promotions(p);
}

template<typename Rules, typename Board> 
bool GeneratorInterface<Rules, Board>::detect(const Position<Board>& p) const 
{ 
        return do_detect(p);
}

template<typename Rules, typename Board> 
bool GeneratorInterface<Rules, Board>::detect_moves(const Position<Board>& p) const 
{ 
        return do_detect_moves(p);
}

template<typename Rules, typename Board> 
bool GeneratorInterface<Rules, Board>::detect_jumps(const Position<Board>& p) const 
{ 
        return do_detect_jumps(p);
}

template<typename Rules, typename Board> 
bool GeneratorInterface<Rules, Board>::detect_reverse(const Position<Board>& p) const 
{
        return do_detect_reverse(p);
}

template<typename Rules, typename Board> 
bool GeneratorInterface<Rules, Board>::detect_promotions(const Position<Board>& p) const 
{
        return do_detect_promotions(p);
}

}       // namespace successors
}       // namespace dctl
