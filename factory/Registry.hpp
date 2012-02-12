#pragma once
#include <map>                          // map
#include <memory>                       // unique_ptr
#include <string>                       // string
#include <type_traits>                  // is_base_of
#include <boost/config.hpp>             // BOOST_STATIC_ASSERT
#include <boost/mpl/identity.hpp>       // identity
#include "mixin.hpp"                    // has_identifier_create

namespace dctl {

template
<
        typename Base,
        typename BasePointer = std::unique_ptr<Base>,
        typename Creator = BasePointer (*)(const std::string&),
        typename Lookup = std::map<std::string, Creator>
>
struct Registry
{
public:
        Creator find(const std::string& id) const
        {
                const auto it = lookup_.find(id);
                return (it != lookup_.end())? it->second : nullptr;
        }

        template<typename Derived>
        bool insert(boost::mpl::identity<Derived>)
        {
                BOOST_STATIC_ASSERT((std::is_base_of<Base, Derived>::value));
                BOOST_STATIC_ASSERT((mixin::has_identifier_create<Derived>::value));

                return insert(Derived::identifier(), Derived::create);
        }

        template<typename Derived>
        bool erase(boost::mpl::identity<Derived>)
        {
                BOOST_STATIC_ASSERT((std::is_base_of<Base, Derived>::value));
                BOOST_STATIC_ASSERT((mixin::has_identifier_create<Derived>::value));

                return erase(Derived::identifier());
        }

private:
        // implementation
        bool insert(const std::string& id, Creator fun)
        {
                return lookup_.insert(Lookup::value_type(id, fun)).second;
        }

        bool erase(const std::string& id)
        {
                return lookup_.erase(id) == 1;
        }

        // representation
        Lookup lookup_;
};

}       // namespace dctl
