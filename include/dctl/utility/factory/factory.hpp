#pragma once
#include <dctl/utility/factory/header_body_terminator.hpp>      // has_header_body_terminator
#include <functional>                                   // function
#include <map>                                          // map
#include <memory>                                       // unique_ptr
#include <string>                                       // string

namespace dctl {

template
<
        class Base,
        class Ret = std::unique_ptr<Base>,
        class Arg = std::string,
        class Creator = std::function<Ret(Arg const&)>
>
class Factory
{
        std::map<Arg, Creator> registry;
public:
        using base_type = Base;
        static_assert(factory::has_header_body_terminator<Base>::value, "");

        auto insert(Arg const& id, Creator fun)
        {
                return registry.emplace(id, fun).second;
        }

        auto erase(Arg const& id)
        {
                return registry.erase(id) == 1;
        }

        auto create(Arg const& input) const
        {
                auto const it = registry.find(Base::header(input));
                return (it != std::end(registry)) ? (it->second)(Base::body(input)) : nullptr;
        }
};

}       // namespace dctl
