#include "../Node/Pieces.h"

namespace tree {
namespace generate {

template<bool Color, int Material, node::Type MoveType, typename Rules, typename Board>
void Driver<Color, Material, MoveType, Rules, Board>::generate(const node::Position<Board>&, node::Stack&) 
{ 
        static_assert(Material == node::Pieces::NONE, "Template function already defined.");
        return;
}

template<bool Color, int Material, node::Type MoveType, typename Rules, typename Board>
void Driver<Color, Material, MoveType, Rules, Board>::generate_reverse(const node::Position<Board>&, node::Stack&) 
{ 
        static_assert(Material == node::Pieces::NONE, "Template function already defined.");
        return;
}

template<bool Color, int Material, node::Type MoveType, typename Rules, typename Board>
void Driver<Color, Material, MoveType, Rules, Board>::generate_promotions(const node::Position<Board>&, node::Stack&) 
{ 
        static_assert(Material == node::Pieces::NONE, "Template function already defined.");
        return;
}

template<bool Color, int Material, node::Type MoveType, typename Rules, typename Board>
size_t Driver<Color, Material, MoveType, Rules, Board>::count(const node::Position<Board>&) 
{ 
        static_assert(Material == node::Pieces::NONE, "Template function already defined.");
        return 0; 
}
        
template<bool Color, int Material, node::Type MoveType, typename Rules, typename Board>
size_t Driver<Color, Material, MoveType, Rules, Board>::count_reverse(const node::Position<Board>&) 
{ 
        static_assert(Material == node::Pieces::NONE, "Template function already defined.");
        return 0; 
}

template<bool Color, int Material, node::Type MoveType, typename Rules, typename Board>
size_t Driver<Color, Material, MoveType, Rules, Board>::count_promotions(const node::Position<Board>&) 
{ 
        static_assert(Material == node::Pieces::NONE, "Template function already defined.");
        return 0; 
}

template<bool Color, int Material, node::Type MoveType, typename Rules, typename Board>
bool Driver<Color, Material, MoveType, Rules, Board>::detect(const node::Position<Board>&) 
{ 
        static_assert(Material == node::Pieces::NONE, "Template function already defined.");
        return false; 
}

template<bool Color, int Material, node::Type MoveType, typename Rules, typename Board>
bool Driver<Color, Material, MoveType, Rules, Board>::detect_reverse(const node::Position<Board>&) 
{ 
        static_assert(Material == node::Pieces::NONE, "Template function already defined.");
        return false; 
}

template<bool Color, int Material, node::Type MoveType, typename Rules, typename Board>
bool Driver<Color, Material, MoveType, Rules, Board>::detect_promotions(const node::Position<Board>&) 
{ 
        static_assert(Material == node::Pieces::NONE, "Template function already defined.");
        return false; 
}

}       // namespace generate
}       // namespace tree
