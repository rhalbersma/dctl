#pragma once
#include <memory>       // make_unique, unique_ptr
#include <string>       // string
#include <type_traits>  // is_base_of

namespace dctl::core {
namespace factory {

template
<
        class Base,
        class Derived,
        char ID,
        class Ret = std::unique_ptr<Base>,
        class Arg = std::string
>
struct make_creatable
:
        public Base
{
        static auto identifier()
        {
                // NOT: Arg{1, identifier_} because 1 is converted without narrowing to '\1'
                return Arg(1, identifier_);
        }

        static auto create(Arg const& arg)
        {
                return std::make_unique<Derived>(arg);
        }

        constexpr static auto identifier_ = ID;
};

template
<
        class Base,
        class Derived
>
struct is_creatable
:
        std::is_base_of<
                make_creatable<
                        Base,
                        Derived,
                        Derived::identifier_
                >,
                Derived
        >
{};

}       // namespace factory
}       // namespace dctl::core
