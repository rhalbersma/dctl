#pragma once
#include <memory>                       // unique_ptr
#include <string>                       // string
#include <type_traits>                  // is_base_of
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <dctl/utility/make_unique.hpp> // make_unique

namespace dctl {
namespace mixin {

template
<
        size_t HeaderLength,
        size_t MaxBodyLength,
        char Terminator
>
struct HeaderBodyTerminator
{
        static std::string header(std::string const& input)
        {
                return input.substr(0, HeaderLength);
        }

        static std::string body(std::string const& input)
        {
                return input.substr(HeaderLength);
        }

        static char terminator()
        {
                return terminator_;
        }

        BOOST_STATIC_CONSTANT(auto, header_length_ = HeaderLength);
        BOOST_STATIC_CONSTANT(auto, max_body_length_ = MaxBodyLength);
        BOOST_STATIC_CONSTANT(auto, terminator_ = Terminator);
};

template
<
        char ID,
        typename Derived,
        typename Base,
        typename Identifier = std::string
>
struct FactoryCreate
:
        public Base
{
        // NOTE: do NOT insert BOOST_MPL_ASSERT((std::is_base_of<Base, Derived>)) here,
        // because Derived is undefined class with FactoryCreate mixed in

        static Identifier identifier()
        {
                return Identifier(1, identifier_);
        }

        static std::unique_ptr<Base> create(std::string const& parameter)
        {
                return std::make_unique<Derived>(parameter);
        }

        typedef Base base;
        BOOST_STATIC_CONSTANT(auto, identifier_ = ID);
};

template<typename T>
struct has_header_body_terminator
:
        std::is_base_of<
                HeaderBodyTerminator<
                        T::header_length_,
                        T::max_body_length_,
                        T::terminator_
                >,
                T
        >
{};

template<typename T>
struct has_factory_create
:
        std::is_base_of<
                FactoryCreate<
                        T::identifier_,
                        T,
                        typename T::base
                >,
                T
        >
{};

}       // namespace mixin
}       // namespace dctl
