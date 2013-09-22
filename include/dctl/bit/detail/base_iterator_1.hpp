#pragma once
#include <cassert>                                      // assert
#include <cstdint>                                      // CHAR_BIT
#include <dctl/bit/detail/base_iterator_fwd.hpp>        // base_iterator
#include <dctl/bit/intrinsic.hpp>                       // clz, ctz

namespace dctl {
namespace bit {
namespace detail {

template<class WordT>
struct base_iterator<WordT, 1>
{
        static constexpr auto Nb = static_cast<int>(CHAR_BIT * sizeof(WordT));

        static constexpr auto do_find(WordT const* s) noexcept
        {
                return (s && *s)? bit::intrinsic::ctz(*s) : Nb;
        }

        constexpr void do_increment() noexcept
        {
                assert(index_ < Nb);
                if (Nb <= ++index_) return;
                auto const mask = *segment_ >> index_;
                index_ = mask? index_ + bit::intrinsic::ctz(mask) : Nb;
                assert(-1 < index_);
        }

        constexpr void do_decrement() noexcept
        {
                assert(-1 < index_);
                if (--index_ <= -1) return;
                auto const mask = *segment_ << (Nb - 1 - index_);
                index_ = mask? index_ - bit::intrinsic::clz(mask) : -1;
                assert(index_ < Nb);
        }

        WordT const* segment_;
        int index_;
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
