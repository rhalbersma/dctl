#pragma once
#include <cstddef>                              // ptrdiff_t
#include <iterator>                             // input_iterator_tag
#include <boost/iterator/iterator_facade.hpp>   // iterator_facade

namespace dctl {
namespace util {

class InputRangeCoreAccess
{
public:
        constexpr InputRangeCoreAccess() noexcept = delete;

        template<class Facade>
        static void pop_front(Facade* f)
        {
                f->pop_front();
        }

        template<class Facade>
        static auto front(Facade const* f)
        {
                return f->front();
        }

        template<class Facade>
        static auto empty(Facade const* f) noexcept
        {
                return f->empty();
        }
};

template
<
        class Derived,
        class Value,
        class Reference = Value&,
        class Difference = std::ptrdiff_t
>
class InputRangeFacade
{
public:
        class iterator
        :
                public boost::iterator_facade
                <
                        iterator,
                        Value,
                        std::input_iterator_tag,
                        Reference,
                        Difference
                >
        {
        public:
                // structors

                iterator() = default;

                explicit iterator(Derived* b) noexcept
                :
                        range_{b}
                {}

        private:
                friend class boost::iterator_core_access;

                // operator++() and operator++(int) provided by boost::iterator_facade
                void increment()
                {
                        InputRangeCoreAccess::pop_front(range_);
                }

                // operator* provided by boost::iterator_facade
                auto dereference() const
                {
                        return InputRangeCoreAccess::front(range_);
                }

                // operator== and operator!= provided by boost::iterator_facade
                auto equal(iterator const& other) const noexcept
                {
                        return
                                        range_ ?
                                ( other.range_ || InputRangeCoreAccess::empty(      range_)) :
                                (!other.range_ || InputRangeCoreAccess::empty(other.range_))
                        ;
                }

        private:
                // representation

                Derived* range_{};
        };

        auto begin() noexcept
        {
                return iterator{static_cast<Derived*>(this)};
        }

        auto end() noexcept
        {
                return iterator{};
        }
};

template<class Derived, class Value, class Reference, class Difference>
auto begin(InputRangeFacade<Derived, Value, Reference, Difference>& r) noexcept
{
        return r.begin();
}

template<class Derived, class Value, class Reference, class Difference>
auto end(InputRangeFacade<Derived, Value, Reference, Difference>& r) noexcept
{
        return r.end();
}

}       // namespace util
}       // namespace dctl
