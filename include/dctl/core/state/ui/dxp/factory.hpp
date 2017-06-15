#pragma once
#include <boost/mpl/for_each.hpp>       // for_each
#include <boost/mpl/identity.hpp>       // make_identity
#include <boost/mpl/vector.hpp>         // vector
#include <boost/variant.hpp>            // get, variant
#include <functional>                   // function
#include <iterator>                     // index
#include <map>                          // map
#include <string>                       // string
#include <utility>                      // pair
#include <variant>                      // monostate

namespace dctl::core {

template<class... Types>
class factory
{
        using type_list = boost::mpl::vector<Types...>;
        using input_type = std::string;
        using key_type = std::string;        
        using argument_type = std::string;
        using result_type = boost::variant<std::monostate, Types...>;    // https://bugs.llvm.org//show_bug.cgi?id=33222
        using create_type = std::pair<key_type, result_type>;
        using mapped_type = std::function<result_type(argument_type)>;
        std::map<key_type, mapped_type> m_registry;

public:
        factory()
        {
                boost::mpl::for_each<type_list, boost::mpl::make_identity<>>([&](auto Id) {
                        using T = typename decltype(Id)::type;
                        m_registry.emplace(T::header(), [](argument_type const& arg) { return T{arg}; });
                });
        }

        auto create(input_type const& in) const
                -> value_type
        {
                auto const h = header(in);
                if (auto const it = m_registry.find(h); it != m_registry.end()) {
                        return { it->first, (it->second)(body(in)) };
                } else {
                        return { h, std::monostate{} };
                }
        }
        
        template<class UnaryFunction>
        auto visit(value_type const& v, UnaryFunction fun) const
        {
                boost::mpl::for_each<type_list, boost::mpl::make_identity<>>([&](auto Id) {
                        using T = typename decltype(Id)::type;
                        if (T::header() == v.first) {
                                fun(boost::get<T>(v.second));
                        }
                });
        }
        
private:
        auto header(input_type const& in) const
                -> key_type
        {
                return in.substr(0, 1);
        }

        auto body(input_type const& in) const
                -> argument_type
        {
                return in.substr(1);
        }
};

}       // namespace dctl::core
