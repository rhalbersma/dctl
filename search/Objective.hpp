#pragma once
#include "Score.hpp"

namespace dctl {
namespace search {

template<typename Objective>
struct MinimalLoss;

template<typename Objective>
struct MinimalWin;

struct Regular;
struct Suicide;

template<>
struct MinimalLoss<Regular>
{
        int operator()() const
        {
                return loss_value(0);
        }
};

template<>
struct MinimalWin<Regular>
{
        int operator()() const
        {
                return win_value(1);
        }
};

template<>
struct MinimalLoss<Suicide>
{
        int operator()() const
        {
                return loss_value(1);
        }
};

template<>
struct MinimalWin<Suicide>
{
        int operator()() const
        {
                return win_value(0);
        }
};

}       // namespace search
}       // namespace dctl
