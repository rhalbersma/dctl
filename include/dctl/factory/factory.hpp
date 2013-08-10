#pragma once
#include <functional>                                   // function
#include <map>                                          // map
#include <memory>                                       // unique_ptr
#include <string>                                       // string
#include <boost/mpl/assert.hpp>                         // BOOST_MPL_ASSERT
#include <boost/mpl/identity.hpp>                       // identity
#include <dctl/factory/creatable.hpp>                   // is_creatable
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
        BOOST_MPL_ASSERT((factory::has_header_body_terminator<Base>));

        // modifiers

        template<class Derived>
        bool insert(boost::mpl::identity<Derived>)
        {
                BOOST_MPL_ASSERT((factory::is_creatable<Base, Derived>));
                return insert(Derived::identifier(), Derived::create);
        }

        bool insert(Arg const& id, Creator fun)
        {
                return registry_.emplace(id, fun).second;
        }

        template<class Derived>
        bool erase(boost::mpl::identity<Derived>)
        {
                BOOST_MPL_ASSERT((factory::is_creatable<Base, Derived>));
                return erase(Derived::identifier());
        }

        bool erase(Arg const& id)
        {
                return registry_.erase(id) == 1;
        }

        // queries

        Ret create(Arg const& input) const
        {
                auto const it = registry_.find(Base::header(input));
                return (it != std::end(registry_))? (it->second)(Base::body(input)) : nullptr;
        }

private:
        // representation

        std::map<Arg, Creator> registry_;
};

}       // namespace dctl
