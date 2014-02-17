#pragma once
#include <cassert>                              // assert
#include <limits>                               // digits
#include <boost/iterator/zip_iterator.hpp>      // zip_iterator
#include <boost/tuple/tuple.hpp>                // make_tuple
#include <dctl/bit/detail/base_set_fwd.hpp>     // BaseSet
#include <dctl/bit/detail/storage.hpp>          // storage
#include <dctl/bit/detail/intrinsic.hpp>        // popcount
#include <dctl/cpp14/reverse_iterator.hpp>      // rbegin, rend
#include <dctl/utility/algorithm.hpp>           // accumulate_until

namespace dctl {
namespace bit {
namespace detail {

template<class T, class Block, int Nb>
struct BaseSet
{
        static_assert(
                !std::numeric_limits<Block>::is_signed &&
                 std::numeric_limits<Block>::is_integer,
                "Block has to be of unsigned integer type."
        );

        enum { digits = std::numeric_limits<Block>::digits };
        enum { N = Nb * digits };

        Block& data()
        {
                return data_[0];
        }

        // structors

        constexpr BaseSet() = default;

        // element access

        constexpr auto block_ptr(T const& n)
        {
                assert(0 <= n && n <= N);
                return &data_[0] + Storage<Block>::block_idx(n);
        }

        constexpr auto block_ptr(T const& n) const
        {
                assert(0 <= n && n <= N);
                return &data_[0] + Storage<Block>::block_idx(n);
        }

        // bitwise operations

        void do_reset() noexcept
        {
                std::fill_n(std::begin(data_), Nb, Block{0});
        }

        void do_set() noexcept
        {
                std::fill_n(std::begin(data_), Nb, ~Block{0});
        }

        void do_flip() noexcept
        {
                std::transform(
                        std::begin(data_), std::end(data_),
                        std::begin(data_), [](auto const& e) {
                        return ~e;
                });
        }

        void do_and(BaseSet const& other) noexcept
        {
                std::transform(
                        std::begin(data_), std::end(data_),
                        std::begin(other.data_), std::begin(data_),
                        [](auto const& L, auto const& R) {
                        return L & R;
                });
        }

        void do_or(BaseSet const& other) noexcept
        {
                std::transform(
                        std::begin(data_), std::end(data_),
                        std::begin(other.data_), std::begin(data_),
                        [](auto const& L, auto const& R) {
                        return L | R;
                });
        }

        void do_xor(BaseSet const& other) noexcept
        {
                std::transform(
                        std::begin(data_), std::end(data_),
                        std::begin(other.data_), std::begin(data_),
                        [](auto const& L, auto const& R) {
                        return L ^ R;
                });
        }

        void do_left_shift(int n)
        {
                assert(0 <= n && n < N);
                if (n == 0)
                        return;

                auto const n_block = Storage<Block>::block_idx(n);
                auto const L_shift = Storage<Block>::shift_idx(n);

                if (L_shift == 0) {
                        std::copy_backward(
                                std::begin(data_), std::end(data_) - n_block,
                                std::end(data_)
                        );
                } else {
                        auto const R_shift = digits - L_shift;

                        std::transform(
                                cpp14::rbegin(data_) + n_block + 1, cpp14::rend(data_),
                                cpp14::rbegin(data_) + n_block    ,
                                cpp14::rbegin(data_),
                                [=](auto const& e1, auto const& e2) {
                                return e1 >> R_shift | e2 << L_shift;
                        });
                        std::transform(
                                std::begin(data_), std::begin(data_) + 1,
                                std::begin(data_) + n_block,
                                [=](auto const& e) {
                                return e << L_shift;
                        });
                }
                std::fill_n(std::begin(data_), n_block, Block{0});
        }

        void do_right_shift(int n)
        {
                assert(0 <= n && n < N);
                if (n == 0)
                        return;

                auto const n_block = Storage<Block>::block_idx(n);
                auto const R_shift = Storage<Block>::shift_idx(n);

                if (R_shift == 0) {
                       std::copy(
                                std::begin(data_) + n_block, std::end(data_),
                                std::begin(data_)
                       );
                } else {
                        auto const L_shift = digits - R_shift;

                        std::transform(
                               std::begin(data_) + n_block + 1, std::end(data_),
                               std::begin(data_) + n_block    ,
                               std::begin(data_),
                               [=](auto const& e1, auto const& e2) {
                               return e1 << L_shift | e2 >> R_shift;
                        });
                        std::transform(
                                cpp14::rbegin(data_), cpp14::rbegin(data_) + 1,
                                cpp14::rbegin(data_) + n_block,
                                [=](auto const& e) {
                                return e >> R_shift;
                        });
                }
                std::fill_n(cpp14::rbegin(data_), n_block, Block{0});
        }

        // bitwise algorithms

        static auto do_equal(BaseSet const& lhs, BaseSet const& rhs) noexcept
        {
                return std::equal(
                        std::begin(lhs.data_), std::end(lhs.data_),
                        std::begin(rhs.data_)
                );
        }

        static auto do_lexicographical_compare(BaseSet const& lhs, BaseSet const& rhs) noexcept
        {
                return std::lexicographical_compare(
                        std::begin(lhs.data_), std::end(lhs.data_),
                        std::begin(rhs.data_), std::end(rhs.data_)
                );
        }

        auto do_includes(BaseSet const& other) const noexcept
        {
                return std::none_of(
                       boost::make_zip_iterator(boost::make_tuple(std::begin(data_), std::begin(other.data_))),
                       boost::make_zip_iterator(boost::make_tuple(std::end(data_), std::end(other.data_))),
                       [](auto const& t) {
                       return (~t.template get<0>() & t.template get<1>()) != Block{0};
                });
        }

        auto do_intersects(BaseSet const& other) const noexcept
        {
                return std::any_of(
                       boost::make_zip_iterator(boost::make_tuple(std::begin(data_), std::begin(other.data_))),
                       boost::make_zip_iterator(boost::make_tuple(std::end(data_), std::end(other.data_))),
                       [](auto const& t) {
                       return (t.template get<0>() & t.template get<1>()) != Block{0};
                });
        }

        auto do_none() const noexcept
        {
                return std::none_of(
                        std::begin(data_), std::end(data_),
                        [](auto const& e) {
                        return e != Block{0};
                });
        }

        auto do_any() const noexcept
        {
                return std::any_of(
                        std::begin(data_), std::end(data_),
                        [](auto const& e) {
                        return e != Block{0};
                });
        }

        auto do_all() const noexcept
        {
                return std::all_of(
                         std::begin(data_), std::end(data_),
                         [](auto const& e) {
                         return e == ~Block{0};
                });
        }

        auto do_count() const noexcept
        {
                return std::accumulate(
                        std::begin(data_), std::end(data_), 0,
                        [](auto const& sum, auto const& e) {
                        return sum + bit::intrinsic::popcount(e);
                });
        }

        template<class UnaryPredicate>
        auto do_count_until(UnaryPredicate pred) const
        {
                return util::accumulate_until(
                        std::begin(data_), std::end(data_), 0,
                        [](auto const& sum, auto const& e) {
                                return sum + bit::intrinsic::popcount(e);
                        },
                        pred
                ).second;
        }

        // representation

        Block data_[Nb] {};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
