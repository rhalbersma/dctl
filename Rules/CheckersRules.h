#pragma once
#include "RulesTraits.h"

// http://www.usacheckers.com/rulesofcheckers.php
struct CheckersRules {};

// aliases for the rules of checkers
typedef CheckersRules EnglishRules;
typedef CheckersRules AmericanRules;

template<> struct ManCaptureDirections<CheckersRules>           { enum { VALUE = DIRS_2  }; };
template<> struct is_LongKingRange<CheckersRules>               { enum { VALUE = RANGE_1 }; };
