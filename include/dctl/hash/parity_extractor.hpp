#pragma once

namespace dctl {
namespace hash {

struct ActiveColorExtractor
{
        typedef bool result_type;

        template<class Key>
        result_type operator()(Key const& key) const
        {
                return key.active_color();
        }
};

}       // namespace hash
}       // namespace dctl
