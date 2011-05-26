#include "../Node/Pieces.h"
#include <cassert>

namespace Tree {
namespace Generate {

template<bool Color, int Material, Move::Type MoveType, typename Rules, typename Board>
void Template<Color, Material, MoveType, Rules, Board>::generate(const Node::Position<Board>&, Move::Stack&) 
{ 
        assert(Material == Node::Pieces::NONE);
        return;
}

template<bool Color, int Material, Move::Type MoveType, typename Rules, typename Board>
size_t Template<Color, Material, MoveType, Rules, Board>::count(const Node::Position<Board>&) 
{ 
        assert(Material == Node::Pieces::NONE);
        return 0; 
}
        
template<bool Color, int Material, Move::Type MoveType, typename Rules, typename Board>
bool Template<Color, Material, MoveType, Rules, Board>::detect(const Node::Position<Board>&) 
{ 
        assert(Material == Node::Pieces::NONE);
        return false; 
}

}       // namespace Generate
}       // namespace Tree
