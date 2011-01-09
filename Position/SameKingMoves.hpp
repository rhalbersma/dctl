template<PlyCount N>
bool SameKingMoves::is_restricted(void) const
{
        return moves() >= N;
}
