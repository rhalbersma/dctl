#pragma once

namespace dctl {

template
<
        typename Rules,
        typename Board
>
struct Weight
{
        static int const material[];
        static int const tempo[];
        static int const center[];
        static int const balance[];
        static int const mobility;
};

template<typename Rules, typename Board>
int const Weight<Rules, Board>::material[] =
{
        100, 200
};

template<typename Rules, typename Board>
int const Weight<Rules, Board>::tempo[] =
{
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

template<typename Rules, typename Board>
int const Weight<Rules, Board>::center[] =
{
        0, 1, 2, 3, 4
};

template<typename Rules, typename Board>
int const Weight<Rules, Board>::balance[] =
{
        5, 4, 3, 2, 1
};

template<typename Rules, typename Board>
int const Weight<Rules, Board>::mobility = 1;

}       // namespace dctl