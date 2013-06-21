#pragma once
#include <type_traits>                          // is_base_of
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/for_each.hpp>               // for_each
#include <boost/mpl/identity.hpp>               // identity, make_identity
#include <boost/mpl/placeholders.hpp>           // _1
#include <dctl/mpl/algorithm.hpp>               // all_of

namespace dctl {
namespace factory {
namespace detail {

template
<
        class Sequence,
        class Factory
>
struct Erase
{
public:
        // all of Sequence must be derived from Base
        BOOST_MPL_ASSERT((mpl::all_of<Sequence, std::is_base_of< typename Factory::base_type, boost::mpl::_1 > >));

        Erase(Factory& f)
        :
                factory_(f)
        {}

        void operator()()
        {
                boost::mpl::for_each<Sequence, boost::mpl::make_identity<> >(
                        polymorphic_lambda(this)
                );
        }

private:
        struct polymorphic_lambda
        {
                explicit polymorphic_lambda(Erase* e): e_(e) {}

                template<class T>
                void operator()(boost::mpl::identity<T> Id)
                {
                        e_->factory_.erase(Id);
                }

                Erase* e_;
        };

        Factory& factory_;
};

}       // namespace detail

template<class Sequence, class Factory>
void erase(Factory& f)
{
        return detail::Erase<Sequence, Factory>{f}();
}

}       // namespace factory
}       // namespace dctl
