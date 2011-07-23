namespace dctl {

// initialize with a set of bitboards
template<bool Color>
void Move::init(BitBoard active_pieces, BitBoard opponent_pieces, BitBoard kings)
{
        material_.init<Color>(active_pieces, opponent_pieces, kings);
}

}       // namespace dctl
