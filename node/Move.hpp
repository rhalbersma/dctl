namespace dctl {

// create a new object from a set of bitboards
template<bool Color>
Move Move::create(BitBoard active_pieces, BitBoard opponent_pieces, BitBoard kings)
{
        return Move(Material::create<Color>(active_pieces, opponent_pieces, kings));
}

}       // namespace dctl
