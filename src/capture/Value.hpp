namespace dctl {
namespace capture {

template<typename Rules>
bool operator<(const Value<Rules>&, const Value<Rules>&)
{
        // MUST be overriden for Rules instantiations with capture precedence semantics
        return false;
}

template<typename Rules>
bool operator>(const Value<Rules>& left, const Value<Rules>& right)
{
        // false by default
        return right < left;
}

template<typename Rules>
bool operator>=(const Value<Rules>& left, const Value<Rules>& right)
{
        // true by default
        return !(left < right);
}

template<typename Rules>
bool operator<=(const Value<Rules>& left, const Value<Rules>& right)
{
        // true by default
        return !(right < left);
}

template<typename Rules>
bool operator==(const Value<Rules>& left, const Value<Rules>& right)
{
        // true by default
        return !(left < right) && !(right < left);
}

template<typename Rules>
bool operator!=(const Value<Rules>& left, const Value<Rules>& right)
{
        // false by default
        return !(left == right);
}

}       // namespace capture
}       // namespace dctl
