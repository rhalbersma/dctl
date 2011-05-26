namespace Hash {

template<typename Entry, typename Key>
bool key_equal_to<Entry, Key>::operator()(const Entry& entry, const Key& key) const
{
        return entry.first == key;
}

template<typename Entry>
bool depth_compare<Entry>::operator()(const Entry& left, const Entry& right) const
{
        return left.second.depth() < right.second.depth();
}

template<typename Entry>
bool leafs_compare<Entry>::operator()(const Entry& left, const Entry& right) const
{
        return left.second.leafs() < right.second.leafs();
}

}       // namespace Hash
