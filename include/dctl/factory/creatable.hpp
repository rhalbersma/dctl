#pragma once
#include <memory>                       // unique_ptr
#include <string>                       // string
#include <type_traits>                  // is_base_of
#include <dctl/cpp14/make_unique.hpp>   // make_unique

namespace dctl {
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
        static Arg identifier()
        {
                // TODO: find out why "return {1, identifier};" fails here
                return Arg(1, identifier_);
        }

        static Ret create(Arg const& arg)
        {
                return cpp14::make_unique<Derived>(arg);
        }

        static constexpr auto identifier_ = ID;
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
}       // namespace dctl
