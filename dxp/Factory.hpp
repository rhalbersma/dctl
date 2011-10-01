#pragma once
#include <map>                          // std::map
#include <memory>                       // std::unique_ptr
#include <string>                       // std::string

namespace dctl {
namespace dxp {

/*

        The Factory class is the <ConcreteCreator> in a <Factory Method>
        design pattern, with the Product class as the <Product>, and classes 
        derived from that as instances of a <ConcreteProduct>. 

        All derived classes MUST be registered with a factory.

*/

template
<
        typename Product,
        typename ProductPointer = std::unique_ptr<Product>,
        typename Input = std::string,
        typename Identifier = std::string,
        typename Parameter = std::string,
        typename Creator = ProductPointer (*)(const Parameter&),
        typename Lookup = std::map<Identifier, Creator>
>
class Factory
{
public:
        ProductPointer create_object(const Input& input) const
        {
                auto i = lookup_.find(Product::identifier(input));
                return (i != lookup_.end())? (i->second)(Product::parameter(input)) : nullptr;
        }

        bool registered(const Identifier& identifier) const
        {
                return lookup_.find(Product::identifier(input)) != lookup_.end();
        }

        bool register_class(const Identifier& identifier, Creator creator)
        {
                return lookup_.insert(Lookup::value_type(identifier, creator)).second;
        }

        bool unregister_class(const Identifier& identifier)
        {
                return lookup_.erase(identifier) == 1;
        }

private:
        // representation
        Lookup lookup_;
};

}       // namespace dxp
}       // namespace dctl
