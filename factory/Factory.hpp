#pragma once
#include <memory>                       // unique_ptr
#include <string>                       // string
#include <boost/config.hpp>             // BOOST_STATIC_ASSERT
#include <boost/mpl/fold.hpp>           // fold
#include <boost/mpl/for_each.hpp>       // for_each
#include <boost/mpl/identity.hpp>       // identity, make_identity
#include <boost/mpl/lambda.hpp>         // lambda, _1, _2
#include <boost/mpl/logical.hpp>        // and_, true_
#include "Registry.hpp"                 // Registry

namespace dctl {

template
<
        typename ConcreteProducts,
        typename AbstractProduct,
        size_t HeaderLength = 1,
        size_t MaxBodyLength = 126,
        typename AbstractProductPointer = std::unique_ptr<AbstractProduct>,
        typename Creator = AbstractProductPointer (*)(const std::string&),
        typename Lookup = std::map<std::string, Creator>,
        typename Registry = Registry<
                AbstractProduct, AbstractProductPointer, Creator, Lookup
        >
>
struct Factory
{
public:
        BOOST_STATIC_ASSERT((
                boost::mpl::fold<
                        ConcreteProducts,
                        boost::mpl::true_,
                        boost::mpl::lambda<
                                boost::mpl::and_<
                                        boost::mpl::_1,
                                        std::is_base_of< AbstractProduct, boost::mpl::_2 >
                                >
                        >                
                >::type::value
        ));

        BOOST_STATIC_ASSERT((
                std::is_base_of< 
                        mixin::HeaderBody< HeaderLength, MaxBodyLength >, 
                        AbstractProduct 
                >::value
        ));

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
                call_erase& operator=(const call_erase&);
                Registry& registry_;
        };

        Registry registry_;
};

}       // namespace dctl
