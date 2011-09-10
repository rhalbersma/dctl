#include <boost/assert.hpp>
#include <iomanip>
#include <iostream>
#include <typeinfo>

namespace dctl {
namespace bit {

template<typename B> template<typename T>
int Lookup<B>::index(T t)
{
        BOOST_ASSERT(is_single(t));
        const int NUM_BLOCKS = sizeof(T) / sizeof(B);	// auto would give unsigned type!
        for (auto i = 0; i < NUM_BLOCKS; ++i)               
                if (B b = block(t, i))
                        return (i * BITS_PER_BLOCK) + INDEX[b];
        BOOST_ASSERT(false);
        return 0;
}

template<typename B> template<typename T>
int Lookup<B>::count(T t)
{
        const int NUM_BLOCKS = sizeof(T) / sizeof(B);	// auto would give unsigned type!
        auto count = 0;
        for (auto i = 0; i < NUM_BLOCKS; ++i)
                count += COUNT[block(t, i)];
        return count;
}

template<typename B> template<typename T>
B Lookup<B>::block(T t, int i)
{
        BOOST_ASSERT(i >= 0 && i < (sizeof(T) / sizeof(B)));
        return static_cast<B>(t >> (i * BITS_PER_BLOCK));
}

template<typename T> T get_first(T);		// least significant 1-bit
template<typename T> int index_DeBruijn(T);     // index of a set 1-bit
template<typename T> int count_Kernighan(T);    // number of set 1-bits

template<typename B>
void Lookup<B>::generate_index()
{
        std::cout << "template<>\n";
        std::cout << "const int Lookup<" << typeid(B).name() << ">::INDEX[] = {\n";
        B i = 0; 
        do {
                if (i % 16 == 0)
                        for (auto j = 0; j < 8; ++j)
                                std::cout << " ";
                std::cout << std::dec << std::setw(2) << std::setfill(' ') << index_DeBruijn(get_first(i));
                if (i != NUM_ENTRIES - 1)
                        std::cout << ",";
                if (i % 16 == 15 || i == NUM_ENTRIES - 1)
                        std::cout << "\n";
                else 
                        std::cout << " ";
        } while (++i);
        std::cout << "};" << "\n\n";
}

template<typename B>
void Lookup<B>::generate_count()
{
        std::cout << "template<>\n";
        std::cout << "const int Lookup<" << typeid(B).name() << ">::COUNT[] = {\n";
        B i = 0; 
        do {
                if (i % 16 == 0)
                        for (auto j = 0; j < 8; ++j)
                                std::cout << " ";
                std::cout << std::dec << std::setw(2) << std::setfill(' ') << count_Kernighan(i);
                if (i != NUM_ENTRIES - 1)
                        std::cout << ",";
                if (i % 16 == 15 || i == NUM_ENTRIES - 1)
                        std::cout << "\n";
                else 
                        std::cout << " ";
        } while (++i);
        std::cout << "};" << "\n\n";
}

}       // namespace bit
}       // namespace dctl
