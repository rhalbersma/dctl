#pragma once
#include "Reversible/Pieces.h"

class AbstractComposition 
{
public:
        virtual Pieces::Composition get_type(void) const = 0;
};

class CompositionNone: public AbstractComposition
{
public:
        Pieces::Composition get_type(void) const { return Pieces::NONE; };
};

class CompositionOnlyMen: public AbstractComposition
{          
public:
        Pieces::Composition get_type(void) const { return Pieces::ONLY_MEN; };
};

class CompositionOnlyKings: public AbstractComposition
{
public:
        Pieces::Composition get_type(void) const { return Pieces::ONLY_KINGS; };
};

class CompositionMenAndKings: public AbstractComposition
{
public:
        Pieces::Composition get_type(void) const { return Pieces::MEN_AND_KINGS; };
};

class ConcreteComposition
{
private:
        AbstractComposition* d_type;

public:
        Pieces::Composition get_type(void) const { return d_type->get_type(); };
        void set_type(Pieces::Composition s_type)
        {
                switch(s_type) {
                case Pieces::NONE:
                        d_type = new CompositionNone;
                        break;
                case Pieces::ONLY_MEN:
                        d_type = new CompositionOnlyMen;
                        break;
                case Pieces::ONLY_KINGS:
                        d_type = new CompositionOnlyKings;
                        break;
                case Pieces::MEN_AND_KINGS:
                        d_type = new CompositionMenAndKings;
                        break;
                default:
                        assert(false);
                }
        }
};
