#include <cassert>
#include "../Node/Position.h"
#include "../../Geometry/Board.h"
#include "../../Geometry/Direction.h"
#include "../../Geometry/PushPull.h"
#include "../../Utilities/Bit.h"

using namespace geometry::direction;

namespace tree {
namespace generate {

template<bool Color, typename Rules, typename Board>
void Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate(const node::Position<Board>& p, node::Stack* move_stack)
{
        generate_serial(p.template unrestricted_kings<Rules>(Color), p.not_occupied(), move_stack);
}

// tag dispatching for restrictions on consecutive moves with the same king
template<bool Color, typename Rules, typename Board>
void Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate_serial(BitBoard active_kings, BitBoard not_occupied, node::Stack* move_stack)
{
        generate_serial(active_kings, not_occupied, move_stack, Int2Type<variants::is_restricted_same_king_moves<Rules>::value>());
}

// partial specialization for restricted consecutive moves with the same king
template<bool Color, typename Rules, typename Board>
void Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate_serial(BitBoard active_kings, BitBoard not_occupied, node::Stack* move_stack, Int2Type<true>)
{
        // loop could be empty if the single active king detected during Successors<Rules, Board>::select is restricted to move
        while (active_kings) {
                generate_dirs(bit::get_lowest(active_kings), not_occupied, move_stack);
                bit::clear_lowest(active_kings);
        }
}

// partial specialization for unrestricted consecutive moves with the same king
template<bool Color, typename Rules, typename Board>
void Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate_serial(BitBoard active_kings, BitBoard not_occupied, node::Stack* move_stack, Int2Type<false>)
{
        // loop cannot be empty because all active kings detected during Successors<Rules, Board>::select() are unrestricted to move
        assert(!bit::is_zero(active_kings));
        do {
                generate_dirs(bit::get_lowest(active_kings), not_occupied, move_stack);
                bit::clear_lowest(active_kings);
        } while (active_kings);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate_dirs(BitBoard from_sq, BitBoard not_occupied, node::Stack* move_stack)
{
        generate_dir<Indices<Board, Color>::LEFT_DOWN >(from_sq, not_occupied, move_stack);
        generate_dir<Indices<Board, Color>::RIGHT_DOWN>(from_sq, not_occupied, move_stack);
        generate_dir<Indices<Board, Color>::LEFT_UP   >(from_sq, not_occupied, move_stack);
        generate_dir<Indices<Board, Color>::RIGHT_UP  >(from_sq, not_occupied, move_stack);
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index>
void Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate_dir(BitBoard from_sq, BitBoard not_occupied, node::Stack* move_stack)
{
        return generate_dir<Index>(from_sq, not_occupied, move_stack, Int2Type<variants::is_long_king_range<Rules>::value>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
void Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate_dir(BitBoard from_sq, BitBoard not_occupied, node::Stack* move_stack, Int2Type<variants::RANGE_1>)
{
        if (BitBoard dest_sq = Push<Board, Index>()(from_sq) & not_occupied)
                node::push<Color>(from_sq ^ dest_sq, move_stack);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
void Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate_dir(BitBoard from_sq, BitBoard not_occupied, node::Stack* move_stack, Int2Type<variants::RANGE_N>)
{
        for (BitBoard dest_sq = Push<Board, Index>()(from_sq); dest_sq & not_occupied; PushAssign<Board, Index>()(dest_sq))
                node::push<Color>(from_sq ^ dest_sq, move_stack);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate_reverse(const node::Position<Board>& p, node::Stack* move_stack)
{
        generate_serial(p.kings(Color), p.not_occupied(), move_stack);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::generate_promotions(const node::Position<Board>&, node::Stack*) 
{ 
        return;
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::count(const node::Position<Board>& p)
{
        return count_dirs(p.template unrestricted_kings<Rules>(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::count_dirs(BitBoard active_kings, BitBoard not_occupied)
{
        return (
                count_dir<Indices<Board, Color>::LEFT_DOWN >(active_kings, not_occupied) +
                count_dir<Indices<Board, Color>::RIGHT_DOWN>(active_kings, not_occupied) +
                count_dir<Indices<Board, Color>::LEFT_UP   >(active_kings, not_occupied) +
                count_dir<Indices<Board, Color>::RIGHT_UP  >(active_kings, not_occupied)
        );
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index>
size_t Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::count_dir(BitBoard active_kings, BitBoard not_occupied)
{
        return count_dir<Index>(active_kings, not_occupied, Int2Type<variants::is_long_king_range<Rules>::value>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
size_t Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::count_dir(BitBoard active_kings, BitBoard not_occupied, Int2Type<variants::RANGE_1>)
{
        return bit::count(Push<Board, Index>()(active_kings) & not_occupied);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
size_t Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::count_dir(BitBoard active_kings, BitBoard not_occupied, Int2Type<variants::RANGE_N>)
{
        return bit::count(active_kings ^ FloodFill<Board, Index>()(active_kings, not_occupied));
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::count_reverse(const node::Position<Board>& p)
{
        return count(p);
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::count_promotions(const node::Position<Board>&) 
{ 
        return 0; 
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::detect(const node::Position<Board>& p)
{
        return detect_dirs(p.template unrestricted_kings<Rules>(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard not_occupied)
{
        return (
                detect_dir<Indices<Board, Color>::LEFT_DOWN >(active_kings, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_DOWN>(active_kings, not_occupied) ||
                detect_dir<Indices<Board, Color>::LEFT_UP   >(active_kings, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_UP  >(active_kings, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard not_occupied)
{
        return !bit::is_zero(Push<Board, Index>()(active_kings) & not_occupied);
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::detect_reverse(const node::Position<Board>& p)
{
        return detect(p);
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, node::Pieces::KING, node::MOVES, Rules, Board>::detect_promotions(const node::Position<Board>&) 
{ 
        return false; 
}

}       // namespace generate
}       // namespace tree
