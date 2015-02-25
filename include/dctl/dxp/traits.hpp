#pragma once

namespace dctl {
namespace dxp {

// type traits
template<class>
struct port
{
        static const unsigned short value = 27531;
};

template<class>
struct terminator
{
        static const auto value = '\0';
};

}       // namespace dxp
}       // namespace dctl
