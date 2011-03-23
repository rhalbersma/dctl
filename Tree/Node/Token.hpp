#include <cctype>

namespace Tree {
namespace Node {

template<char B, char W, char E>
const char Triplet<B, W, E>::COLOR[2] = {
        BLACK,
        WHITE
};

template<char B, char W, char E>
const char Triplet<B, W, E>::LOWER[2] = {
        static_cast<char>(tolower(BLACK)),
        static_cast<char>(tolower(WHITE))
};

template<char B, char W, char E>
const char Triplet<B, W, E>::UPPER[2] = {
        static_cast<char>(toupper(BLACK)),
        static_cast<char>(toupper(WHITE))
};

}       // namespace Node
}       // namespace Tree
