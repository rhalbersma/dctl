#pragma once
#include <cassert>                              // assert
#include <limits>                               // digits
#include <dctl/bit/detail/base_set_fwd.hpp>     // BaseSet
#include <dctl/bit/detail/storage.hpp>          // storage
#include <dctl/bit/detail/intrinsic.hpp>        // popcount

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

        // structors

        constexpr BaseSet() noexcept = default;

        // element access

        constexpr auto block_ptr(T const& n)
        {
                assert(0 <= n <= N);
                return &data_[0] + Storage<Block>::block_idx(n);
        }

        constexpr auto block_ptr(T const& n) const
        {
                assert(0 <= n <= N);
                return &data_[0] + Storage<Block>::block_idx(n);
        }

        // bitwise operations

        constexpr void do_reset() noexcept
        {
                // std::fill_n(std::begin(data_), Nb, Block{0});
                for (auto i = 0; i < Nb; ++i)
                        data_[i] = Block{0};
        }

        constexpr void do_set() noexcept
        {
                // std::fill_n(std::begin(data_), Nb, ~Block{0});
                for (auto i = 0; i < Nb; ++i)
                        data_[i] = ~Block{0};
        }

        constexpr void do_flip() noexcept
        {
                // std::transform(
                //         std::begin(data_), std::end(data_),
                //         std::begin(data_), [](auto e) {
                //         return ~e;
                // });
                for (auto i = 0; i < Nb; ++i)
                        data_[i] = ~data_[i];
        }

        constexpr void do_and(BaseSet const& other) noexcept
        {
                // std::transform(
                //         std::begin(data_), std::end(data_),
                //         std::begin(other.data_), std::begin(data_),
                //         [](auto L, auto R) {
                //         return L & R;
                // });
                for (auto i = 0; i < Nb; ++i)
                        data_[i] &= other.data_[i];
        }

        constexpr void do_or(BaseSet const& other) noexcept
        {
                // std::transform(
                //         std::begin(data_), std::end(data_),
                //         std::begin(other.data_), std::begin(data_),
                //         [](auto L, auto R) {
                //         return L | R;
                // });
                for (auto i = 0; i < Nb; ++i)
                        data_[i] |= other.data_[i];
        }

        constexpr void do_xor(BaseSet const& other) noexcept
        {
                // std::transform(
                //         std::begin(data_), std::end(data_),
                //         std::begin(other.data_), std::begin(data_),
                //         [](auto L, auto R) {
                //         return L ^ R;
                // });
                for (auto i = 0; i < Nb; ++i)
                        data_[i] ^= other.data_[i];
        }

        constexpr void do_left_shift(int n)
        {
                assert(0 <= n && n < N);
                if (n == 0)
                        return;

                auto const n_block = Storage<Block>::block_idx(n);
                auto const L_shift = Storage<Block>::shift_idx(n);

                if (L_shift == 0) {
                        // std::copy_backward(std::begin(data_), std::end(data_) - n_block, std::end(data_));
                        for (auto i = Nb - 1; i >= n_block; --i)
                                data_[i] = data_[i - n_block];
                } else {
                        auto const R_shift = digits - L_shift;

                        // std::transform(
                        //         std::rbegin(data_) + n_block + 1, std::rend(data_) + ,
                        //         std::rbegin(data_) + n_block, std::rbegin(data_),
                        //         [=](auto e1, auto e2) {
                        //         return e1 >> R_shift | e2 << L_shift;
                        // });
                        for (auto i = Nb - 1; i > n_block; --i)
                                data_[i] =
                                        data_[i - n_block    ] << L_shift |
                                        data_[i - n_block - 1] >> R_shift
                                ;

                        // std::transform(
                        //         std::begin(data_), std::begin(data_) + 1,
                        //         std::begin(data_) + n_block,
                        //         [=](auto e) {
                        //         return e << L_shift;
                        // });
                        data_[n_block] = data_[0] << L_shift;
                }

                // std::fill_n(std::begin(data_), n_block, Block{0});
                for (auto i = n_block - 1; i >= 0; --i)
                        data_[i] = Block{0};
        }

        constexpr void do_right_shift(int n)
        {
                assert(0 <= n && n < N);
                if (n == 0)
                        return;

                auto const n_block = Storage<Block>::block_idx(n);
                auto const R_shift = Storage<Block>::shift_idx(n);

                if (R_shift == 0) {
                        for (auto i = 0; i < Nb - n_block; ++i)
                                // std::copy(std::begin(data_) + n_block, std::end(data_), std::begin(data_));
                                data_[i] = data_[i + n_block];
                } else {
                        auto const L_shift = digits - R_shift;

                        // std::transform(
                        //         std::begin(data_) + n_block + 1, std::end(data_),
                        //         std::begin(data_) + n_block, std::begin(data_),
                        //         [=](auto e1, auto e2) {
                        //         return e1 << L_shift | e2 >> R_shift;
                        // });
                        for (auto i = 0; i < Nb - n_block - 1; ++i)
                                data_[i] =
                                        data_[i + n_block    ] >> R_shift |
                                        data_[i + n_block + 1] << L_shift
                                ;

                        // std::transform(
                        //         std::rbegin(data_), std::rbegin(data_) + 1,
                        //         std::rbegin(data_) + n_block,
                        //         [=](auto e) {
                        //         return e >> R_shift;
                        // });
                        data_[Nb - n_block - 1] = data_[Nb - 1] >> R_shift;
                }

                // std::fill_n(std::rbegin(data_), n_block, Block{0});
                for (auto i = Nb - n_block; i < Nb; ++i)
                        data_[i] = Block{0};
        }

        // bitwise algorithms

        static constexpr auto do_equal(BaseSet const& lhs, BaseSet const& rhs) noexcept
        {
                // std::equal(std::begin(lhs.data_), std::end(lhs.data_), std::begin(rhs.data_));
                for (auto i = 0; i < Nb; ++i)
                        if (lhs.data_[i] != rhs.data_[i])
                                return false;
                return true;
        }

        static constexpr auto do_lexicographical_compare(BaseSet const& lhs, BaseSet const& rhs) noexcept
        {
                // std::lexicographical_compare(std::begin(lhs.data_), std::end(lhs.data_), std::begin(rhs.data_), std::end(rhs.data_));
                for (auto i = 0; i < Nb; ++i) {
                        if (lhs.data_[i] < rhs.data_[i]) return true;
                        if (rhs.data_[i] < lhs.data_[i]) return false;
                }
                return false;
        }

        constexpr auto do_includes(BaseSet const& other) const noexcept
        {
                // std::none_of(
                //        boost::make_zip_iterator(boost::make_tuple(std::begin(data_), std::begin(other.data_))),
                //        boost::make_zip_iterator(boost::make_tuple(std::end(data_), std::end(other.data_))),
                //        [](auto const& t) {
                //        return ~t.get<0>() & t.get<1>() != Block{0};
                // });
                for (auto i = 0; i < Nb; ++i)
                        if ((~data_[i] & other.data_[i]) != Block{0})
                                return false;
                return true;
        }

        constexpr auto do_intersects(BaseSet const& other) const noexcept
        {
                // std::any_of(
                //        boost::make_zip_iterator(boost::make_tuple(std::begin(data_), std::begin(other.data_))),
                //        boost::make_zip_iterator(boost::make_tuple(std::end(data_), std::end(other.data_))),
                //        [](auto const& t) {
                //        return t.get<0>() & t.get<1>() != Block{0};
                // });
                for (auto i = 0; i < Nb; ++i)
                        if ((data_[i] & other.data_[i]) != Block{0})
                                return true;
                return false;
        }

        constexpr auto do_none() const noexcept
        {
                // std::none_of(std::begin(data_), std::end(data_), [](auto e) {
                //         return e != Block{0};
                // });
                for (auto i = 0; i < Nb; ++i)
                        if (data_[i] != Block{0})
                                return false;
                return true;
        }

        constexpr auto do_any() const noexcept
        {
                // std::any_of(std::begin(data_), std::end(data_), [](auto e) {
                //         return e != Block{0};
                // });
                for (auto i = 0; i < Nb; ++i)
                        if (data_[i] != Block{0})
                                return true;
                return false;
        }

        constexpr auto do_all() const noexcept
        {
                // std::all_of(std::begin(data_), std::end(data_), [](auto e) {
                //         return e == ~Block{0};
                // });
                for (auto i = 0; i < Nb; ++i)
                        if (data_[i] != ~Block{0})
                                return false;
                return true;
        }

        constexpr auto do_count() const noexcept
        {
                // std::accumulate(std::begin(data_), std::end(data_), 0,
                //         [](auto sum, auto e) {
                //         return sum + bit::popcount(e);
                // });
                auto sum = 0;
                for (auto i = 0; i < Nb; ++i)
                        sum += bit::intrinsic::popcount(data_[i]);
                return sum;
        }

        template<class Pred>
        constexpr auto do_count_until(Pred pred) const noexcept
        {
                auto sum = 0;
                for (auto i = 0; i < Nb; ++i) {
                        sum += bit::intrinsic::popcount(data_[i]);
                        if (pred(sum))
                                break;
                }
                return sum;
        }

        // representation

        Block data_[Nb] {};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
