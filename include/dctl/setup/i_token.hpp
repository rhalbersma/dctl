#pragma once
#include <cctype>       // tolower, toupper

namespace dctl {
namespace setup {

template<class>
struct TokenSet;

template<class Protocol>
struct Token
{
        using type = TokenSet<Protocol>;
};

template<char B, char W, char E>
struct TokenInterface
{
        static const auto black = B;
        static const auto white = W;
        static const auto empty = E;

        static char const color[];
        static char const lower[];
        static char const upper[];
};

template<char B, char W, char E>
char const TokenInterface<B, W, E>::color[] = {
        black,
        white
};

template<char B, char W, char E>
char const TokenInterface<B, W, E>::lower[] = {
        static_cast<char>(tolower(black)),
        static_cast<char>(tolower(white))
};

template<char B, char W, char E>
char const TokenInterface<B, W, E>::upper[] = {
        static_cast<char>(toupper(black)),
        static_cast<char>(toupper(white))
};

}       // namespace maskup
}       // namespace dctl
