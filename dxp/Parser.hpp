#pragma once
#include <memory>
#include <string>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/utility.hpp>
#include "Factory.hpp"
#include "MessageInterface.hpp"
#include "Types.hpp"

namespace dctl {
namespace dxp {

/*

        The Factory class is the <ConcreteCreator> in a <Factory Method>
        Design Pattern, with the Product class as the <Product>, and classes 
        derived from that as instances of a <ConcreteProduct>. 

        All derived message classes MUST register themselves with the factory.

*/

template
<
        typename ConcreteProducts,
        typename Product,
        typename ProductPointer = std::unique_ptr<Product>,
        typename Input = std::string,
        typename Factory = Factory<Product, ProductPointer, Input>
>
class Parser
{
public:
        Parser()
        {
                boost::mpl::for_each<ConcreteProducts, boost::mpl::make_identity<> >(
                        call_register_class<Factory>(factory_)
                );
        }       

        ~Parser()
        {
                boost::mpl::for_each<ConcreteProducts, boost::mpl::make_identity<> >(
                        call_unregister_class<Factory>(factory_)
                );
        }

        ProductPointer parse(const Input& input) const
        {
                return factory_.create_object(input);
        }

private:
        Factory factory_;
};

template<typename Factory>
class call_register_class
{
public:
        call_register_class(Factory& f): factory_(f) {}

        template<typename T>
        void operator()(boost::mpl::identity<T>) const
        {
                T::register_class(factory_);
        }

private:
        call_register_class& operator=(const call_register_class&);
        Factory& factory_;
};

template<typename Factory>
class call_unregister_class
{
public:
        call_unregister_class(Factory& f): factory_(f) {}

        template<typename T>
        void operator()(boost::mpl::identity<T>) const
        {
                T::unregister_class(factory_);
        }

private:
        call_unregister_class& operator=(const call_unregister_class&);
        Factory& factory_;
};

}       // namespace dxp
}       // namespace dctl
