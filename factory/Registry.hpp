#pragma once
#include <map>                          // map
#include <memory>                       // unique_ptr
#include <string>                       // string
#include <type_traits>                  // is_base_of
#include <boost/config.hpp>             // BOOST_STATIC_ASSERT
#include <boost/mpl/identity.hpp>       // identity

namespace dctl {

template
<
        typename AbstractProduct,
        typename AbstractProductPointer = std::unique_ptr<AbstractProduct>,
        typename Creator = AbstractProductPointer (*)(const std::string&),
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

        template<typename ConcreteProduct>
        bool insert(boost::mpl::identity<ConcreteProduct>)
        {
                BOOST_STATIC_ASSERT((std::is_base_of<AbstractProduct, ConcreteProduct>::value));
                return insert(ConcreteProduct::identifier(), ConcreteProduct::create_object);
        }

        template<typename ConcreteProduct>
        bool erase(boost::mpl::identity<ConcreteProduct>)
        {
                BOOST_STATIC_ASSERT((std::is_base_of<AbstractProduct, ConcreteProduct>::value));
                return erase(ConcreteProduct::identifier());
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
