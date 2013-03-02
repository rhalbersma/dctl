#pragma once

namespace dctl {
namespace bit {

class reference
{
public:
        operator bool() const
        {
                return static_cast<bool>(*pseg_ & mask_);
        }

        iterator operator&() const
        {
                return iterator(pseg_, mask_);
        }

private:
        storage* pseg_;
        storage mask_;
};

class iterator
{
public:
        iterator(storage* p, unsigned n)
        :
                pseg_(p),
                ctz_(n)
        {}

        reference operator*() const
        {
                return reference(pseg_, bit::singlet<storage>(ctz_));
        }

        iterator operator++()
        {
                return iterator(pseg_, bit::ctz(*pseg_ & (~storage(0) << ctz_)));
        }

private:
        storage* pseg_;
        unsigned ctz_;
};

class container
{
public:
        explicit container(storage b)
        :
                data_(b)
        {}

        iterator begin() const
        {
                return iterator(&data_, ctz(data_));
        }

        iterator end() const
        {
                return iterator(&data_, num_bits<storage>::value - clz(data_));
        }

        reference front() const
        {
                return *begin();
        }

        reference back() const
        {
                return *end();
        }

        std::size_t size() const
        {
                return count(data_);
        }

private:
        storage data_;
};

}       // namespace bit
}       // namespace dctl
