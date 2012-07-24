#pragma once
#include <functional>                   // function
#include <map>                          // map
#include <memory>                       // unique_ptr
#include <string>                       // string
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/mpl/identity.hpp>       // identity
#include "mixin.hpp"                    // has_factory_create

namespace dctl {

template
<
        typename Base,
        typename BasePointer = std::unique_ptr<Base>,
        typename Identifier = std::string,
        typename Creator = std::function<BasePointer(Identifier const&)>
>
struct Registry
{
public:
        // modifiers

        template<typename Derived>
        bool insert(boost::mpl::identity<Derived>)
        {
                BOOST_MPL_ASSERT((mixin::has_factory_create<Derived>));
                return insert(Derived::identifier(), Derived::create);
        }

        template<typename Derived>
        bool erase(boost::mpl::identity<Derived>)
        {
                BOOST_MPL_ASSERT((mixin::has_factory_create<Derived>));
                return erase(Derived::identifier());
        }

        // queries

        Creator find(Identifier const& id) const
        {
                auto const it = lookup_.find(id);
                return (it != std::end(lookup_))? it->second : nullptr;
        }

private:
        // implementation

        bool insert(Identifier const& id, Creator fun)
        {
                return lookup_.insert(Lookup::value_type(id, fun)).second;
        }

        bool erase(Identifier const& id)
        {
                return lookup_.erase(id) == 1;
        }

        // representation
        
        typedef std::map<Identifier, Creator> Lookup;
        Lookup lookup_;
};

}       // namespace dctl
