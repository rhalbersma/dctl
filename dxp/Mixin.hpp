#pragma once

namespace dctl {
namespace dxp {

/*----------------------------------------------------------------------------*/
// Boiler-plate code for enforcing factory creation and defining a header
/*----------------------------------------------------------------------------*/

#define MIXIN_HEADER_FACTORY_CREATION(Header, ConcreteProduct)                          \
public:                                                                                 \
        template<typename Factory>                                                      \
        static bool registered(const Factory& factory)                                  \
        {                                                                               \
                return factory.registered(header());                                    \
        }                                                                               \
                                                                                        \
        template<typename Factory>                                                      \
        static bool register_class(Factory& factory)                                    \
        {                                                                               \
                return factory.register_class(header(), create);                        \
        }                                                                               \
                                                                                        \
        template<typename Factory>                                                      \
        static bool unregister_class(Factory& factory)                                  \
        {                                                                               \
                return factory.unregister_class(header());                              \
        }                                                                               \
                                                                                        \
private:                                                                                \
        static std::unique_ptr<MessageInterface> create(const std::string& message)     \
        {                                                                               \
                return std::unique_ptr<ConcreteProduct>(new ConcreteProduct(message));  \
        }                                                                               \
                                                                                        \
        virtual std::string do_header() const                                           \
        {                                                                               \
                return header();                                                        \
        }                                                                               \
                                                                                        \
        static std::string header()                                                     \
        {                                                                               \
                return std::string(1, header_);                                         \
        }                                                                               \
                                                                                        \
        BOOST_STATIC_CONSTANT(auto, header_ = Header);

/*----------------------------------------------------------------------------*/
// END OF MIXIN_HEADER_FACTORY_CREATION
/*----------------------------------------------------------------------------*/

}       // namespace dxp
}       // namespace dctl
