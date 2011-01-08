#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename> class Position;
template<typename, typename> class Propagate;

template<bool, typename Rules, typename Board> 
class AbstractGeneratePiece
{
public:
        // interface
        virtual void generate(const Position<Board>&, Propagate<Rules, Board>&) const = 0;
        virtual void generate_captures(const Position<Board>&, Propagate<Rules, Board>&) const = 0;
        virtual void generate_promotions(const Position<Board>&, Propagate<Rules, Board>&) const = 0;

        virtual size_t count(const Position<Board>&) const = 0;
        virtual size_t count_moves(const Position<Board>&) const = 0;

        virtual bool detect(const Position<Board>&) const = 0;        
        virtual bool detect_captures(const Position<Board>&) const = 0;
        virtual bool detect_promotions(const Position<Board>&) const = 0;

        // virtual destructor
        virtual ~AbstractGeneratePiece(void) {};
};

template<bool Color, Pieces::Composition, typename Rules, typename Board> 
class GeneratePiece: public AbstractGeneratePiece<Color, Rules, Board>
{
public:		
        GeneratePiece(void) {};

        virtual void generate(const Position<Board>&, Propagate<Rules, Board>&) const;
        virtual void generate_captures(const Position<Board>&, Propagate<Rules, Board>&) const;
        virtual void generate_promotions(const Position<Board>&, Propagate<Rules, Board>&) const;

        virtual size_t count(const Position<Board>&) const;
        virtual size_t count_moves(const Position<Board>&) const;

        virtual bool detect(const Position<Board>&) const;
        virtual bool detect_captures(const Position<Board>&) const;
        virtual bool detect_promotions(const Position<Board>&) const;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "GeneratePiece.hpp"
