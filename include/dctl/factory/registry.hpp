#pragma once
#include <functional>                   // function
#include <map>                          // map
#include <memory>                       // unique_ptr
#include <string>                       // string
#include <boost/assert.hpp>		// BOOST_ASSERT
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/mpl/identity.hpp>       // identity
#include <dctl/factory/mixin.hpp>       // has_factory_create

namespace dctl {

template
<
        class Base,
        class BasePointer = std::unique_ptr<Base>,
        class Identifier = std::string,
        class Creator = std::function<BasePointer(Identifier const&)>
>
class Registry
{
public:
	// typedefs

	typedef Identifier key_type;
	typedef Creator mapped_type;
    	typedef std::map<key_type, mapped_type> map_type;
    	typedef typename map_type::value_type value_type;

	// modifiers

        template<class Derived>
        bool insert(boost::mpl::identity<Derived>)
        {
                BOOST_MPL_ASSERT((mixin::has_factory_create<Derived>));
                return insert(Derived::identifier(), Derived::create);
        }

        template<class Derived>
        bool erase(boost::mpl::identity<Derived>)
        {
                BOOST_MPL_ASSERT((mixin::has_factory_create<Derived>));
                return erase(Derived::identifier());
        }

        // queries

        mapped_type find(key_type const& id) const
        {
                auto const it = map_.find(id);
                return (it != std::end(map_))? it->second : nullptr;
        }

private:
        // implementation

        bool insert(key_type const& id, mapped_type fun)
        {
                auto const insertion = map_.insert(value_type(id, fun)).second;
                BOOST_ASSERT(insertion);
                return insertion;
        }

        bool erase(key_type const& id)
        {
                auto const erasure = map_.erase(id) == 1;
                BOOST_ASSERT(erasure);
                return erasure;
        }

        // representation

        map_type map_;
};

}       // namespace dctl
