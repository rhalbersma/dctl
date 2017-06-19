#pragma once

namespace dctl::eval {

template<class Rules, class Board>
struct Weight
{
        static int const material[];
        static int const tempo[];
        static int const center[];
        static int const balance[];
        static int const mobility;
        static int const king_monopoly;
};

template<class Rules, class Board>
int const Weight<Rules, Board>::material[] =
{
        100, 250
};

template<class Rules, class Board>
int const Weight<Rules, Board>::tempo[] =
{
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

template<class Rules, class Board>
int const Weight<Rules, Board>::center[] =
{
        0, 1, 2, 3, 4
};

template<class Rules, class Board>
int const Weight<Rules, Board>::balance[] =
{
        5, 4, 3, 2, 1
};

template<class Rules, class Board>
int const Weight<Rules, Board>::mobility = 1;

template<class Rules, class Board>
int const Weight<Rules, Board>::king_monopoly = 200;

}       // namespace dctl::eval
