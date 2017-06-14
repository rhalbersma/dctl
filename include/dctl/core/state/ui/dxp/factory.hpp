#pragma once
#include <boost/mpl/for_each.hpp>       // for_each
#include <boost/mpl/identity.hpp>       // make_identity
#include <boost/mpl/vector.hpp>         // vector
#include <boost/variant.hpp>            // get, variant
#include <functional>                   // function
#include <iterator>                     // index
#include <map>                          // map
#include <string>                       // string
#include <variant>                      // monostate

namespace dctl::core {

template<class... Types>
class factory
{
        using type_list = boost::mpl::vector<Types...>;
        using value_type = boost::variant<std::monostate, Types...>;    // TODO: file bug report in Clang/GCC against libstdc++ std::variant
        using creator_fn = std::function<value_type(std::string)>;
        std::map<std::string, creator_fn> m_registry;

public:
        factory()
        {
                boost::mpl::for_each<type_list, boost::mpl::make_identity<>>([&](auto Id) {
                        using T = typename decltype(Id)::type;
                        insert(T::header(), [](auto const& arg) { return T{arg}; });
                });
        }

        template<class UnaryFunction>
        auto process(std::string const& input, UnaryFunction fun) const
        {
                auto const h = header(input);
                boost::mpl::for_each<type_list, boost::mpl::make_identity<>>([&](auto Id) {
                        using T = typename decltype(Id)::type;
                        if (T::header() == h) {
                                fun(boost::get<T>(create(input)));
                        }
                });
        }

private:
        auto header(std::string const& a) const
        {
                return a.substr(0, 1);
        }

        auto body(std::string const& a) const
        {
                return a.substr(1);
        }

        auto create(std::string const& input) const
                -> value_type
        {
                auto const it = m_registry.find(header(input));
                return it != m_registry.end() ? (it->second)(body(input)) : std::monostate{};
        }

        template<class UnaryFunction>
        auto insert(std::string const& id, UnaryFunction fun)
        {
                return m_registry.emplace(id, fun).second;
        }
};

}       // namespace dctl::core
