#pragma once
#include <functional>                                   // function
#include <map>                                          // map
#include <memory>                                       // unique_ptr
#include <string>                                       // string
#include <dctl/factory/header_body_terminator.hpp>      // has_header_body_terminator

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
public:
        using base_type = Base;
        static_assert(factory::has_header_body_terminator<Base>::value, "");

        // modifiers

        bool insert(Arg const& id, Creator fun)
        {
                return registry_.emplace(id, fun).second;
        }

        bool erase(Arg const& id)
        {
                return registry_.erase(id) == 1;
        }

        // queries

        Ret create(Arg const& input) const
        {
                auto const it = registry_.find(Base::header(input));
                return (it != std::end(registry_)) ? (it->second)(Base::body(input)) : nullptr;
        }

private:
        // representation

        std::map<Arg, Creator> registry_;
};

}       // namespace dctl
