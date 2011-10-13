#pragma once

namespace dctl {

template
<
        typename Rules,
        typename Board
>
struct Weight
{
        static const int material[];
        static const int tempo[];
        static const int center[];
        static const int balance[];
        static const int mobility;
};

template<typename Rules, typename Board>
const int Weight<Rules, Board>::material[] =
{
        100, 200
};

template<typename Rules, typename Board>
const int Weight<Rules, Board>::tempo[] =
{
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

template<typename Rules, typename Board>
const int Weight<Rules, Board>::center[] =
{
        0, 1, 2, 3, 4
};

template<typename Rules, typename Board>
const int Weight<Rules, Board>::balance[] =
{
        5, 4, 3, 2, 1
};

template<typename Rules, typename Board>
const int Weight<Rules, Board>::mobility = 1;

}       // namespace dctl
