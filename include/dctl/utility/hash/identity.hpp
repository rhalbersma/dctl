#pragma once
#include <hash_append/endian.h> // endian
#include <cstddef>              // size_t

namespace dctl {
namespace hash {

class identity
{
        std::size_t m_state;
public:
        static constexpr xstd::endian endian = xstd::endian::native;
        using result_type = std::size_t;

        constexpr auto operator()(void const* key, std::size_t /* len */) noexcept
        {
                m_state = *static_cast<result_type const*>(key);
        }

        explicit constexpr operator auto() const noexcept
        {
                return m_state;
        }
};

}       // namespace hash
}       // namespace dctl
