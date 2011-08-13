namespace dctl {
namespace successors {

template<typename Rules, typename Board> 
void GenerateInterface<Rules, Board>::generate(const Position<Board>& p, Stack& moves) const 
{
        do_generate(p, moves);
}

template<typename Rules, typename Board> 
void GenerateInterface<Rules, Board>::generate_captures(const Position<Board>& p, Stack& moves) const 
{ 
        do_generate_captures(p, moves);
}

template<typename Rules, typename Board> 
void GenerateInterface<Rules, Board>::generate_reverse(const Position<Board>& p, Stack& moves) const 
{ 
        do_generate_reverse(p, moves);
}

template<typename Rules, typename Board> 
void GenerateInterface<Rules, Board>::generate_promotions(const Position<Board>& p, Stack& moves) const 
{ 
        do_generate_promotions(p, moves);
}

template<typename Rules, typename Board>
int GenerateInterface<Rules, Board>::count(const Position<Board>& p) const
{
        return do_count(p);
}

template<typename Rules, typename Board>
int GenerateInterface<Rules, Board>::count_captures(const Position<Board>& p) const
{
        return do_count_captures(p);
}

template<typename Rules, typename Board>
int GenerateInterface<Rules, Board>::count_reverse(const Position<Board>& p) const
{
        return do_count_reverse(p);
}

template<typename Rules, typename Board>
int GenerateInterface<Rules, Board>::count_promotions(const Position<Board>& p) const
{
        return do_count_promotions(p);
}

template<typename Rules, typename Board>
int GenerateInterface<Rules, Board>::count_mobility(const Position<Board>& p) const
{
        return do_count_mobility(p);
}

template<typename Rules, typename Board> 
bool GenerateInterface<Rules, Board>::detect(const Position<Board>& p) const 
{ 
        return do_detect(p);
}

template<typename Rules, typename Board> 
bool GenerateInterface<Rules, Board>::detect_captures(const Position<Board>& p) const 
{ 
        return do_detect_captures(p);
}

template<typename Rules, typename Board> 
bool GenerateInterface<Rules, Board>::detect_reverse(const Position<Board>& p) const 
{
        return do_detect_reverse(p);
}

template<typename Rules, typename Board> 
bool GenerateInterface<Rules, Board>::detect_promotions(const Position<Board>& p) const 
{
        return do_detect_promotions(p);
}

}       // namespace successors
}       // namespace dctl
