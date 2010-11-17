#pragma once
#include "RulesTraits.h"

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc
struct InternationalRules {};   

// aliases for the international rules on 8x8, 10x10 and 12x12 boards
typedef InternationalRules BrazilianRules;
typedef InternationalRules PolishRules;
typedef InternationalRules CanadianRules;

template<> struct is_MajorityPrecedence<InternationalRules>     { enum { VALUE = true  }; };
