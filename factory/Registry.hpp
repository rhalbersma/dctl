#pragma once
#include <map>                          // map
#include <memory>                       // unique_ptr
#include <string>                       // string
#include <boost/config.hpp>             // BOOST_STATIC_ASSERT
#include <boost/mpl/identity.hpp>       // identity
#include "mixin.hpp"                    // has_factory_create

namespace dctl {

template
<
        typename Base,
        typename BasePointer = std::unique_ptr<Base>,
        typename Identifier = std::string,
        typename Creator = BasePointer (*)(const Identifier&)
>
struct Registry
{
public:
        Creator find(const Identifier& id) const
        {
                const auto it = lookup_.find(id);
                return (it != std::end(lookup_))? it->second : nullptr;
        }

        template<typename Derived>
        bool insert(boost::mpl::identity<Derived>)
        {
                BOOST_STATIC_ASSERT(mixin::has_factory_create<Derived>::value);
                return insert(Derived::identifier(), Derived::create);
        }

        template<typename Derived>
        bool erase(boost::mpl::identity<Derived>)
        {
                BOOST_STATIC_ASSERT(mixin::has_factory_create<Derived>::value);
                return erase(Derived::identifier());
        }

private:
        // implementation
        bool insert(const Identifier& id, Creator fun)
        {
                return lookup_.insert(Lookup::value_type(id, fun)).second;
        }

        bool erase(const Identifier& id)
        {
                return lookup_.erase(id) == 1;
        }

        // representation
        typedef std::map<Identifier, Creator> Lookup;
        Lookup lookup_;
};

}       // namespace dctl
