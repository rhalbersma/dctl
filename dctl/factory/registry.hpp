#pragma once
#include <functional>                   // function
#include <map>                          // map
#include <memory>                       // unique_ptr
#include <string>                       // string
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/mpl/identity.hpp>       // identity
#include <dctl/factory/mixin.hpp>       // has_factory_create

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
        void insert(boost::mpl::identity<Derived>)
        {
                BOOST_MPL_ASSERT((mixin::has_factory_create<Derived>));
                insert(Derived::identifier(), Derived::create);
        }

        template<typename Derived>
        void erase(boost::mpl::identity<Derived>)
        {
                BOOST_MPL_ASSERT((mixin::has_factory_create<Derived>));
                erase(Derived::identifier());
        }

        // queries

        Creator find(Identifier const& id) const
        {
                auto const it = lookup_.find(id);
                return ((it != std::end(lookup_))? it->second : nullptr);
        }

private:
        // implementation

        void insert(Identifier const& id, Creator fun)
        {
                lookup_.insert(typename Lookup::value_type(id, fun));
        }

        void erase(Identifier const& id)
        {
                lookup_.erase(id);
        }

        // representation

        typedef std::map<Identifier, Creator> Lookup;
        Lookup lookup_;
};

}       // namespace dctl
