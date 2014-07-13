#pragma once
#include <dctl/bit/detail/base_set_fwd.hpp>     // BaseSet
#include <dctl/bit/detail/storage.hpp>          // storage
#include <dctl/bit/detail/intrinsic.hpp>        // popcount
#include <dctl/cpp14/iterator.hpp>              // rbegin, rend
#include <dctl/utility/algorithm.hpp>           // accumulate_until
#include <boost/iterator/zip_iterator.hpp>      // zip_iterator
#include <boost/tuple/tuple.hpp>                // make_tuple
#include <cassert>                              // assert
#include <limits>                               // digits

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

        Block const& data() const
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
                using std::begin;
                std::fill_n(begin(data_), Nb, Block{0});
        }

        void do_set() noexcept
        {
                using std::begin;
                std::fill_n(begin(data_), Nb, ~Block{0});
        }

        void do_flip() noexcept
        {
                using std::begin; using std::end;
                std::transform(
                        begin(data_), end(data_),
                        begin(data_), [](auto const& e) {
                        return ~e;
                });
        }

        void do_and(BaseSet const& other) noexcept
        {
                using std::begin; using std::end;
                std::transform(
                        begin(data_), end(data_),
                        begin(other.data_), begin(data_),
                        [](auto const& L, auto const& R) {
                        return L & R;
                });
        }

        void do_or(BaseSet const& other) noexcept
        {
                using std::begin; using std::end;
                std::transform(
                        begin(data_), end(data_),
                        begin(other.data_), begin(data_),
                        [](auto const& L, auto const& R) {
                        return L | R;
                });
        }

        void do_xor(BaseSet const& other) noexcept
        {
                using std::begin; using std::end;
                std::transform(
                        begin(data_), end(data_),
                        begin(other.data_), begin(data_),
                        [](auto const& L, auto const& R) {
                        return L ^ R;
                });
        }

        void do_left_shift(int n)
        {
                using std::begin; using std::end;
                using namespace cpp14;

                assert(0 <= n && n < N);
                if (n == 0)
                        return;

                auto const n_block = Storage<Block>::block_idx(n);
                auto const L_shift = Storage<Block>::shift_idx(n);

                if (L_shift == 0) {
                        std::copy_backward(
                                begin(data_), end(data_) - n_block,
                                end(data_)
                        );
                } else {
                        auto const R_shift = digits - L_shift;

                        std::transform(
                                rbegin(data_) + n_block + 1, rend(data_),
                                rbegin(data_) + n_block    ,
                                rbegin(data_),
                                [=](auto const& e1, auto const& e2) {
                                return e1 >> R_shift | e2 << L_shift;
                        });
                        std::transform(
                                begin(data_), begin(data_) + 1,
                                begin(data_) + n_block,
                                [=](auto const& e) {
                                return e << L_shift;
                        });
                }
                std::fill_n(begin(data_), n_block, Block{0});
        }

        void do_right_shift(int n)
        {
                using std::begin; using std::end;
                using namespace cpp14;

                assert(0 <= n && n < N);
                if (n == 0)
                        return;

                auto const n_block = Storage<Block>::block_idx(n);
                auto const R_shift = Storage<Block>::shift_idx(n);

                if (R_shift == 0) {
                       std::copy(
                                begin(data_) + n_block, end(data_),
                                begin(data_)
                       );
                } else {
                        auto const L_shift = digits - R_shift;

                        std::transform(
                               begin(data_) + n_block + 1, end(data_),
                               begin(data_) + n_block    ,
                               begin(data_),
                               [=](auto const& e1, auto const& e2) {
                               return e1 << L_shift | e2 >> R_shift;
                        });
                        std::transform(
                                rbegin(data_), rbegin(data_) + 1,
                                rbegin(data_) + n_block,
                                [=](auto const& e) {
                                return e >> R_shift;
                        });
                }
                std::fill_n(rbegin(data_), n_block, Block{0});
        }

        // bitwise algorithms

        static auto do_equal(BaseSet const& lhs, BaseSet const& rhs) noexcept
        {
                using namespace cpp14;
                return std::equal(
                        cbegin(lhs.data_), cend(lhs.data_),
                        cbegin(rhs.data_)
                );
        }

        static auto do_lexicographical_compare(BaseSet const& lhs, BaseSet const& rhs) noexcept
        {
                using namespace cpp14;
                return std::lexicographical_compare(
                        cbegin(lhs.data_), cend(lhs.data_),
                        cbegin(rhs.data_), cend(rhs.data_)
                );
        }

        auto do_includes(BaseSet const& other) const noexcept
        {
                using namespace cpp14;
                return std::all_of(
                       boost::make_zip_iterator(boost::make_tuple(cbegin(data_), cbegin(other.data_))),
                       boost::make_zip_iterator(boost::make_tuple(cend(data_), cend(other.data_))),
                       [](auto const& t) {
                       return (~t.template get<0>() & t.template get<1>()) == Block{0};
                });
        }

        auto do_intersects(BaseSet const& other) const noexcept
        {
                using namespace cpp14;
                return std::any_of(
                       boost::make_zip_iterator(boost::make_tuple(cbegin(data_), cbegin(other.data_))),
                       boost::make_zip_iterator(boost::make_tuple(cend(data_), cend(other.data_))),
                       [](auto const& t) {
                       return (t.template get<0>() & t.template get<1>()) != Block{0};
                });
        }

        auto do_none() const noexcept
        {
                using namespace cpp14;
                return std::none_of(
                        cbegin(data_), cend(data_),
                        [](auto const& e) {
                        return e != Block{0};
                });
        }

        auto do_any() const noexcept
        {
                using namespace cpp14;
                return std::any_of(
                        cbegin(data_), cend(data_),
                        [](auto const& e) {
                        return e != Block{0};
                });
        }

        auto do_all() const noexcept
        {
                using namespace cpp14;
                return std::all_of(
                         cbegin(data_), cend(data_),
                         [](auto const& e) {
                         return e == ~Block{0};
                });
        }

        auto do_count() const noexcept
        {
                using namespace cpp14;
                return std::accumulate(
                        cbegin(data_), cend(data_), 0,
                        [](auto const& sum, auto const& e) {
                        return sum + bit::intrinsic::popcount(e);
                });
        }

        template<class UnaryPredicate>
        auto do_count_until(UnaryPredicate pred) const
        {
                using namespace cpp14;
                return util::accumulate_until(
                        cbegin(data_), cend(data_), 0,
                        [](auto const& sum, auto const& e) {
                                return sum + bit::intrinsic::popcount(e);
                        },
                        pred
                ).second;
        }

        // representation

        Block data_[Nb]{};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
