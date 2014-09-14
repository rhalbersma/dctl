#pragma once

namespace dctl {
namespace bit {
namespace detail {

template<int Nb>
class BaseSet;

template<>
class BaseSet<1>;

template<>
class BaseSet<0>;

}       // namespace detail
}       // namespace bit
}       // namespace dctl
