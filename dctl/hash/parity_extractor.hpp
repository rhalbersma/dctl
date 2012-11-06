#pragma once

namespace dctl {
namespace hash {

struct ActiveColorExtractor
{
        typedef bool value_type;

        template<typename Key>
        value_type operator()(Key const& key) const
        {
                return (key.active_color());
        }
};

}       // namespace hash
}       // namespace dctl
