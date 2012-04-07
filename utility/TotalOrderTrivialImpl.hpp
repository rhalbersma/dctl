#pragma once

namespace dctl {

template<typename Host>
struct TotalOrderTrivialImpl
{
        // predicates
        bool less(const Host& /* other */) const
        {
                return false;
        }

        bool equal(const Host& /* other */) const
        {

                return true;
        }
};

}       // namespace dctl
