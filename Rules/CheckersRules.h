#pragma once
#include "RulesTraits.h"

struct CheckersRules {};         // http://www.usacheckers.com/rulesofcheckers.php

// aliases for the rules of checkers
typedef CheckersRules EnglishRules;
typedef CheckersRules AmericanRules;

template<> struct ManCaptureDirections<CheckersRules>           { static const size_t VALUE = DIRS_2; };
template<> struct is_LongKingRange<CheckersRules>               { enum { VALUE = RANGE_1 }; };
