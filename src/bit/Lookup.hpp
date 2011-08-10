#include <cassert>
#include <iomanip>
#include <iostream>
#include <typeinfo>

namespace dctl {
namespace bit {

template<typename B> template<typename T>
int Lookup<B>::index(T t)
{
        assert(is_single(t));
        const auto NUM_BLOCKS = sizeof(T) / sizeof(B);
        for (auto i = 0; i < NUM_BLOCKS; ++i)               
                if (B b = block(t, i))
                        return (i * BITS_PER_BLOCK) + INDEX[b];
        assert(false);
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
        const int NUM_BLOCKS = sizeof(T) / sizeof(B);	// auto would give unsigned type!
        assert(i >= 0 && i < NUM_BLOCKS);
        return static_cast<B>(t >> (i * BITS_PER_BLOCK));
}

template<typename T> T get_first(T);		// least significant 1-bit
template<typename T> int index_DeBruijn(T);     // index of a set 1-bit
template<typename T> int count_Kernighan(T);    // number of set 1-bits

template<typename B>
void Lookup<B>::generate_index()
{
        std::cout << "template<>" << std::endl;
        std::cout << "const int Lookup<" << typeid(B).name() << ">::INDEX[] = {" << std::endl;
        B i = 0; 
        do {
                if (i % 16 == 0)
                        for (auto j = 0; j < 8; ++j)
                                std::cout << " ";
                std::cout << std::dec << std::setw(2) << std::setfill(' ') << index_DeBruijn(get_first(i));
                if (i != NUM_ENTRIES - 1)
                        std::cout << ",";
                if (i % 16 == 15 || i == NUM_ENTRIES - 1)
                        std::cout << std::endl;
                else 
                        std::cout << " ";
        } while (++i);
        std::cout << "};" << std::endl << std::endl;
}

template<typename B>
void Lookup<B>::generate_count()
{
        std::cout << "template<>" << std::endl;
        std::cout << "const int Lookup<" << typeid(B).name() << ">::COUNT[] = {" << std::endl;
        B i = 0; 
        do {
                if (i % 16 == 0)
                        for (auto j = 0; j < 8; ++j)
                                std::cout << " ";
                std::cout << std::dec << std::setw(2) << std::setfill(' ') << count_Kernighan(i);
                if (i != NUM_ENTRIES - 1)
                        std::cout << ",";
                if (i % 16 == 15 || i == NUM_ENTRIES - 1)
                        std::cout << std::endl;
                else 
                        std::cout << " ";
        } while (++i);
        std::cout << "};" << std::endl << std::endl;
}

}       // namespace bit
}       // namespace dctl
