#include <cctype>       // toupper, tolower

namespace dctl {
namespace setup {

template<char B, char W, char E>
const char TokenSetBase<B, W, E>::COLOR[] = {
        BLACK,
        WHITE
};

template<char B, char W, char E>
const char TokenSetBase<B, W, E>::LOWER[] = {
        static_cast<char>(tolower(BLACK)),
        static_cast<char>(tolower(WHITE))
};

template<char B, char W, char E>
const char TokenSetBase<B, W, E>::UPPER[] = {
        static_cast<char>(toupper(BLACK)),
        static_cast<char>(toupper(WHITE))
};

}       // namespace setup
}       // namespace dctl
