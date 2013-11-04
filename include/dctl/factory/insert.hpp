#pragma once
#include <type_traits>                          // is_base_of
#include <boost/mpl/for_each.hpp>               // for_each
#include <boost/mpl/identity.hpp>               // identity, make_identity
#include <boost/mpl/placeholders.hpp>           // _1

namespace dctl {
namespace factory {
namespace detail {

template
<
        class Sequence,
        class Factory
>
struct Insert
{
public:
        Insert(Factory& f)
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
        // implementation

        struct polymorphic_lambda
        {
                explicit polymorphic_lambda(Insert* i): i_(i) {}

                template<class T>
                void operator()(boost::mpl::identity<T> Id)
                {
                        i_->factory_.insert(Id);
                }

                Insert* i_;
        };

        // representation

        Factory& factory_;
};

}       // namespace detail

template<class Sequence, class Factory>
void insert(Factory& f)
{
        return detail::Insert<Sequence, Factory>{f}();
}

}       // namesapce factory
}       // namespace dctl
