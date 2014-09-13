#pragma once

namespace dctl {
namespace tti {

template<class T>
struct identity
{
        using type = T;
};

}       // namespace tti
}       // namespace dctl
