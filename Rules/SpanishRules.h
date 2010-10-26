#pragma once
#include "RulesTraits.h"

struct SpanishRules {};         // http://fpdamas.home.sapo.pt/regrasclass.htm

template<> struct ManCaptureDirections<SpanishRules>            { static const size_t VALUE = DIRS_2; };
template<> struct is_MajorityPrecedence<SpanishRules>           { enum { VALUE = true  }; };
