#pragma once
#include <map>                          // map
#include <memory>                       // unique_ptr
#include <string>                       // string
#include <type_traits>                  // is_base_of
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/mpl/for_each.hpp>       // for_each
#include <boost/mpl/identity.hpp>       // identity, make_identity
#include <boost/mpl/placeholders.hpp>   // _1
#include "mixin.hpp"                    // has_header_body_terminator
#include "Registry.hpp"                 // Registry
#include "../mpl/algorithm.hpp"         // all_of

namespace dctl {

template
<
        typename DerivedSequence,
        typename Base,
        typename BasePointer = std::unique_ptr<Base>,
        typename Input = std::string,
        typename Identifier = std::string,
        typename Creator = BasePointer (*)(const Identifier&)
>
struct Factory
{
public:
        // Base must have header / body / terminator functionality mixed in
        BOOST_MPL_ASSERT((mixin::has_header_body_terminator<Base>));

        // all DerivedSequence must be derived from Base
        BOOST_MPL_ASSERT((mpl::all_of<DerivedSequence, std::is_base_of< Base, boost::mpl::_1 > >));

        Factory()
        {
                boost::mpl::for_each<DerivedSequence, boost::mpl::make_identity<> >(
                        call_insert(registry_)
                );
        }

        ~Factory()
        {
                boost::mpl::for_each<DerivedSequence, boost::mpl::make_identity<> >(
                        call_erase(registry_)
                );
        }

        BasePointer create(const Input& input) const
        {
                auto const fun = registry_.find(Base::header(input));
                return fun? (fun)(Base::body(input)) : nullptr;
        }

private:
        typedef Registry<Base, BasePointer, Identifier, Creator> Registry;

        // TODO: refactor into polymorphic lambda expresssion whenever C++11 supports this
        struct call_insert
        {
        public:
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
        public:
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
