#pragma once
#include <memory>                       // unique_ptr
#include <string>                       // string
#include <boost/config.hpp>             // BOOST_STATIC_ASSERT
#include <boost/mpl/for_each.hpp>       // for_each
#include <boost/mpl/identity.hpp>       // identity, make_identity
#include "mixin.hpp"                    // has_header_body
#include "Registry.hpp"                 // Registry
#include "../utility/type_traits.hpp"   // is_base_of_all

namespace dctl {

template
<
        typename ConcreteProducts,
        typename AbstractProduct,
        typename AbstractProductPointer = std::unique_ptr<AbstractProduct>,
        typename Creator = AbstractProductPointer (*)(const std::string&),
        typename Lookup = std::map<std::string, Creator>,
        typename Registry = Registry<AbstractProduct, AbstractProductPointer, Creator, Lookup>
>
struct Factory
{
public:
        // all ConcreteProducts must be derived from AbstractProduct
        BOOST_STATIC_ASSERT((is_base_of_all<AbstractProduct, ConcreteProducts>::type::value));

        Factory()
        {
                boost::mpl::for_each<ConcreteProducts, boost::mpl::make_identity<> >(
                        call_insert(registry_)
                );
        }       

        ~Factory()
        {
                boost::mpl::for_each<ConcreteProducts, boost::mpl::make_identity<> >(
                        call_erase(registry_)
                );
        }

        AbstractProductPointer create(const std::string& input) const
        {
                // AbstractProduct must have header / body functionality mixed in 
                BOOST_STATIC_ASSERT((mixin::has_header_body<AbstractProduct>::value));

                const auto fun = registry_.find(AbstractProduct::header(input));
                return fun? (fun)(AbstractProduct::body(input)) : nullptr;
        }

private:
        // TODO: refactor into polymorphic lambda expresssion whenever C++11 supports this
        struct call_insert
        {
                explicit call_insert(Registry& r): registry_(r) {};

                template<typename T>
                bool operator()(boost::mpl::identity<T> I)
                {
                        return registry_.insert(I);
                }

        private:
                // suppress warning about the compiler-generated assignment operator
                call_insert& operator=(const call_insert&);

                Registry& registry_;
        };

        // TODO: refactor into polymorphic lambda expresssion whenever C++11 supports this
        struct call_erase
        {
                explicit call_erase(Registry& r): registry_(r) {};

                template<typename T>
                bool operator()(boost::mpl::identity<T> I)
                {
                        return registry_.erase(I);
                }

        private:
                // suppress warning about the compiler-generated assignment operator
                call_erase& operator=(const call_erase&);

                Registry& registry_;
        };

        Registry registry_;
};

}       // namespace dctl
