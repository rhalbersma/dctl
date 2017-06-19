#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/variant.hpp>    // variant
#include <functional>           // function
#include <map>                  // map
#include <string>               // string
#include <utility>              // forward
#include <variant>              // monostate

namespace dctl::core {

template<class... Types>
class factory
{
        using input_type = std::string;
        using key_type = std::string;
        using argument_type = std::string;
        using result_type = boost::variant<std::monostate, Types...>;           // TODO: https://bugs.llvm.org//show_bug.cgi?id=33222
        using mapped_type = std::function<result_type(argument_type)>;
        std::map<key_type, mapped_type> m_registry;

public:
        factory()
        :
                m_registry{ { Types::header(), construct<Types>{} }...  }       // TODO: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=47226
        {}

        auto create(input_type const& in) const
                -> result_type
        {
                if (auto const it = m_registry.find(in.substr(0, 1)); it != m_registry.end()) {
                        return (it->second)(in.substr(1));
                } else {
                        return {};
                }
        }

private:
        template<class T>
        struct construct
        {
                auto operator()(argument_type&& arg) const
                {
                        return T(std::forward<argument_type>(arg));
                }
        };
};

}       // namespace dctl::core
