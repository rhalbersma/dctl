namespace dctl {

template<bool Color, typename Board>
BitBoard promotion_sq(BitBoard dest_sq)
{
        return dest_sq & Board::PROMOTION[Color][0];
}

template<bool Color, typename Board>
BitBoard promoting_men(BitBoard active_men)
{
        return active_men & Board::PROMOTION[Color][1];
}

}       // namespace dctl
