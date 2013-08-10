#pragma once

namespace dctl {
namespace hash {

struct ActiveColorExtractor
{
        using result_type = bool;

        template<class Key>
        result_type operator()(Key const& key) const
        {
                return key.active_color();
        }
};

}       // namespace hash
}       // namespace dctl
