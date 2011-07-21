#include <type_traits>  // std::is_integral

namespace dtl {
namespace hash {

template<typename Index, typename Key>
Key ShiftSign<Index, Key>::operator()(Index index) const
{
        static_assert(std::is_integral<Index>::value == true, "Bitwise shift only applicable to integer types.");
        static_assert(std::is_integral<Key>::value == true, "Bitwise shift only applicable to integer types.");
        static_assert(sizeof(Index) >= sizeof(Key), "Hash key cannot be of larger type than the hash index.");

        return static_cast<Key>(index >> 8 * (sizeof(Index) - sizeof(Key)));
}

template<typename Item, typename Key>
const Key& FindSign<Item, Key>::operator()(const Item& item) const
{
        static_assert(std::is_integral<Item>::value == false, "Only bitwise shift applicable to integer types.");

        return item.key();
}

}       // namespace hash
}       // namespace dtl
