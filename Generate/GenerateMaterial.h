#pragma once
#include <cstddef>

template<typename> class Position;
class MoveList;

template<bool Color, typename Rules, typename Board> 
class AbstractGenerateMaterial
{
public:
        // interface
        virtual void generate(const Position<Board>&, MoveList&) const = 0;
        virtual void generate_captures(const Position<Board>&, MoveList&) const = 0;
        virtual void generate_promotions(const Position<Board>&, MoveList&) const = 0;

        virtual size_t count(const Position<Board>&) const = 0;
        virtual size_t count_mobility(const Position<Board>&) const = 0;

        virtual bool detect(const Position<Board>&) const = 0;        
        virtual bool detect_captures(const Position<Board>&) const = 0;
        virtual bool detect_promotions(const Position<Board>&) const = 0;

        // virtual destructor
        virtual ~AbstractGenerateMaterial(void) {};
};

template<bool Color, Pieces::Composition Material, typename Rules, typename Board> 
class GenerateMaterial: public AbstractGenerateMaterial<Color, Rules, Board>
{
public:		
        GenerateMaterial(void) {};

        virtual void generate(const Position<Board>&, MoveList&) const;
        virtual void generate_captures(const Position<Board>&, MoveList&) const;
        virtual void generate_promotions(const Position<Board>&, MoveList&) const;

        virtual size_t count(const Position<Board>&) const;
        virtual size_t count_mobility(const Position<Board>&) const;

        virtual bool detect(const Position<Board>&) const;
        virtual bool detect_captures(const Position<Board>&) const;
        virtual bool detect_promotions(const Position<Board>&) const;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "GenerateMaterial.hpp"
