
namespace dctl {
namespace capture {

template<typename Rules>
bool ValueInterface<Rules>::is_large(BitBoard captured_pieces) const
{
        return do_is_large(captured_pieces);
}

template<typename Rules>
bool ValueInterface<Rules>::is_promotion() const
{
        return do_is_promotion();
}

template<typename Rules>
void ValueInterface<Rules>::increment(BitBoard target_sq, BitBoard king_targets)
{
        do_increment(target_sq, king_targets);
}

template<typename Rules>
void ValueInterface<Rules>::decrement(BitBoard target_sq, BitBoard king_targets)
{
        do_decrement(target_sq, king_targets);
}

template<typename Rules>
void ValueInterface<Rules>::toggle_with_king()
{
        do_toggle_with_king();
}

template<typename Rules>
void ValueInterface<Rules>::toggle_promotion()
{
        do_toggle_promotion();
}

}       // namespace capture
}       // namespace dctl
