#pragma once

namespace dctl {
namespace hash {

struct ActiveColorExtractor
{
        typedef bool parity_type;

        template<typename U>
        parity_type operator()(U const& u) const
        {
                return (u.active_color());
        }
};

}       // namespace hash
}       // namespace dctl
