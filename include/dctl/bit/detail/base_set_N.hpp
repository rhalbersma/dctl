#pragma once
#include <dctl/bit/detail/base_set_fwd.hpp>     // BaseSet
#include <dctl/bit/detail/intrinsic.hpp>        // popcount
#include <dctl/bit/detail/storage.hpp>          // storage
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <limits>                               // digits

namespace dctl {
namespace bit {
namespace detail {

template<class UnsignedInteger, std::size_t Nb>
class BaseSet
{
private:
        static_assert(
                !std::numeric_limits<UnsignedInteger>::is_signed &&
                 std::numeric_limits<UnsignedInteger>::is_integer,
                "Template parameter 'T' in 'BaseSet<T, N>' shall be of unsigned integer type."
        );

        static constexpr auto digits = std::numeric_limits<UnsignedInteger>::digits;
        static constexpr auto N = Nb * digits;

public:
        // constructors

        constexpr BaseSet() = default;

protected:
        // destructor

        ~BaseSet() = default;

public:
        // copying and assignment

        BaseSet(BaseSet const&) = default;
        BaseSet& operator=(BaseSet const&) = default;

        // element access

        constexpr auto* block_ptr(std::size_t n)
        {
                assert(n <= N);
                return &data_[Storage<UnsignedInteger>::block_index(n)];
        }

        constexpr auto const* block_ptr(std::size_t n) const
        {
                assert(n <= N);
                return &data_[Storage<UnsignedInteger>::block_index(n)];
        }

        // data access

        constexpr auto* data()
        {
                return &data_;
        }

        constexpr auto const* data() const
        {
                return &data_;
        }

        // comparators

        constexpr auto do_equal(BaseSet const& other) const noexcept
        {
                for (std::size_t i = 0; i < Nb; ++i)
                        if (data_[i] != other.data_[i])
                                return false;
                return true;
        }

        constexpr auto do_colexicographical_compare(BaseSet const& other) const noexcept
        {
                for (auto i = Nb; i < Nb; --i) {
                        if (data_[i] < other.data_[i])
                                return true;
                        if (data_[i] > other.data_[i])
                                return false;
                }
                return false;
        }

        constexpr auto do_is_proper_subset_of(BaseSet const& other) const noexcept
        {
                auto proper = false;
                for (std::size_t i = 0; i < Nb; ++i) {
                        if ( data_[i] & ~other.data_[i])
                                return false;
                        if (~data_[i] &  other.data_[i])
                                proper = true;
                }
                return proper;
        }

        constexpr auto do_is_subset_of(BaseSet const& other) const noexcept
        {
                for (std::size_t i = 0; i < Nb; ++i)
                        if (data_[i] & ~other.data_[i])
                                return false;
                return true;
        }

        constexpr auto do_intersects(BaseSet const& other) const noexcept
        {
                for (std::size_t i = 0; i < Nb; ++i)
                        if (data_[i] & other.data_[i])
                                return true;
                return false;
        }

        // modifiers

        constexpr auto do_set() noexcept
        {
                for (auto&& block : data_)
                        block = ~static_cast<UnsignedInteger>(0);
        }

        constexpr auto do_reset() noexcept
        {
                for (auto&& block : data_)
                        block = static_cast<UnsignedInteger>(0);
        }

        constexpr auto do_flip() noexcept
        {
                for (auto&& block : data_)
                        block = ~block;
        }

        constexpr auto do_and(BaseSet const& other) noexcept
        {
                for (std::size_t i = 0; i < Nb; ++i)
                        data_[i] &= other.data_[i];
        }

        constexpr auto do_or(BaseSet const& other) noexcept
        {
                for (std::size_t i = 0; i < Nb; ++i)
                        data_[i] |= other.data_[i];
        }

        constexpr auto do_xor(BaseSet const& other) noexcept
        {
                for (std::size_t i = 0; i < Nb; ++i)
                        data_[i] ^= other.data_[i];
        }

        constexpr auto do_minus(BaseSet const& other) noexcept
        {
                for (std::size_t i = 0; i < Nb; ++i)
                        data_[i] &= ~other.data_[i];
        }

        constexpr auto do_left_shift(std::size_t n)
        {
                assert(n < N);
                if (n == 0) return;

                auto const n_block = Storage<UnsignedInteger>::block_index(n);
                auto const L_shift = Storage<UnsignedInteger>::bit_index(n);

                if (L_shift == 0) {
                        for (auto i = Nb - 1; i >= n_block; --i)
                                data_[i] = data_[i - n_block];
                } else {
                        auto const R_shift = digits - L_shift;

                        for (auto i = Nb - 1; i > n_block; --i)
                                data_[i] =
                                        (data_[i - n_block    ] << L_shift) |
                                        (data_[i - n_block - 1] >> R_shift)
                                ;
                        data_[n_block] = data_[0] << L_shift;
                }
                for (std::size_t i = 0; i < n_block; ++i)
                        data_[i] = static_cast<UnsignedInteger>(0);
        }

        constexpr auto do_right_shift(std::size_t n)
        {
                assert(n < N);
                if (n == 0) return;

                auto const n_block = Storage<UnsignedInteger>::block_index(n);
                auto const R_shift = Storage<UnsignedInteger>::bit_index(n);

                if (R_shift == 0) {
                       for (std::size_t i  = 0; i <= Nb - 1 - n_block; ++i)
                               data_[i] = data_[i + n_block];
                } else {
                        auto const L_shift = digits - R_shift;

                        for (std::size_t i = 0; i < Nb - 1 - n_block; ++i)
                                data_[i] =
                                        (data_[i + n_block    ] >> R_shift) |
                                        (data_[i + n_block + 1] << L_shift)
                                ;
                        data_[Nb - 1 - n_block] = data_[Nb - 1] >> R_shift;
                }
                for (auto i = Nb - n_block; i < Nb; ++i)
                        data_[i] = static_cast<UnsignedInteger>(0);
        }

        // observers

        constexpr auto do_count() const noexcept
        {
                auto sum = 0;
                for (auto&& block : data_)
                        sum += bit::intrinsic::popcount(block);
                return sum;
        }

        constexpr auto do_all() const noexcept
        {
                for (auto&& block : data_)
                        if (block != ~static_cast<UnsignedInteger>(0))
                                return false;
                return true;
        }

        constexpr auto do_any() const noexcept
        {
                for (auto&& block : data_)
                        if (block)
                                return true;
                return false;
        }

        constexpr auto do_none() const noexcept
        {
                for (auto&& block : data_)
                        if (block)
                                return false;
                return true;
        }

private:
        // representation

        UnsignedInteger data_[Nb]{};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
