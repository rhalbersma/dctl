#pragma once
#include "RulesTraits.h"

// http://fpdamas.home.sapo.pt/regrasclass.htm
struct SpanishRules {};         

template<> struct ManCaptureDirections<SpanishRules>            { enum { VALUE = DIRS_2 }; };
template<> struct is_MajorityPrecedence<SpanishRules>           { enum { VALUE = true   }; };
