#pragma once

namespace dctl {
namespace killer {

// partial specialization for Killer draughts
template<class T, class U = typename T::value_type>
class Value
:
        public U
{
        using U::U;
};

}       // namespace successor
}       // namespace dctl
