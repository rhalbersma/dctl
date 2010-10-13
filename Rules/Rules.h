#pragma once
#include "../Utilities/IntegerTypes.h"
#include <climits>

//+----------------------------------------------------------------------------+
//|      Rule tags for which type traits are defined                           |
//+----------------------------------------------------------------------------+

// draughts variants with official rules
struct InternationalRules {};   // http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc
struct KillerRules {};          // http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules
struct FrisianRules {};         // http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct PoolRules {};            // http://americanpoolcheckers.us/americanpoolcheckers/index.php?option=com_content&view=article&id=48:the-apca-playing-rules&catid=37:documents&Itemid=56
struct RussianRules {};         // http://www.shashist.ru/kodeks/kodeks2004.doc
struct SpanishRules {};         // http://fpdamas.home.sapo.pt/regrasclass.htm
struct CzechRules {};           // http://www.damweb.cz/pravidla/cdfull.html
struct ThaiRules {};
struct ItalianRules {};         // http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
struct EnglishRules {};         // http://www.usacheckers.com/rulesofcheckers.php

// aliases for the international rules on 8x8, 10x10 and 12x12 boards
typedef InternationalRules BrazilianRules;
typedef InternationalRules PolishRules;
typedef InternationalRules CanadianRules;

//+----------------------------------------------------------------------------+
//|      Move and capture mechanics                                            |
//+----------------------------------------------------------------------------+

// man capture directions
enum {DIRS_2, DIRS_4, DIRS_8};
template<typename> struct ManCaptureDirections                  { static const size_t VALUE = DIRS_4; };
template<> struct ManCaptureDirections<SpanishRules>            { static const size_t VALUE = DIRS_2; };
template<> struct ManCaptureDirections<CzechRules>              { static const size_t VALUE = DIRS_2; };
template<> struct ManCaptureDirections<ThaiRules>               { static const size_t VALUE = DIRS_2; };
template<> struct ManCaptureDirections<ItalianRules>            { static const size_t VALUE = DIRS_2; };
template<> struct ManCaptureDirections<EnglishRules>            { static const size_t VALUE = DIRS_2; };
template<> struct ManCaptureDirections<FrisianRules>            { static const size_t VALUE = DIRS_8; };

template<typename Rules>
struct is_ManCaptureBackwards
{
        static const bool VALUE = ManCaptureDirections<Rules>::VALUE != DIRS_2;
};

// king capture directions
template<typename> struct KingCaptureDirections                 { enum { VALUE = DIRS_4 }; };
template<> struct KingCaptureDirections<FrisianRules>           { enum { VALUE = DIRS_8 }; };

// king range
enum {RANGE_1, RANGE_N};
template<typename> struct is_LongKingRange                      { enum { VALUE = RANGE_N }; };
template<> struct is_LongKingRange<ItalianRules>                { enum { VALUE = RANGE_1 }; };
template<> struct is_LongKingRange<EnglishRules>                { enum { VALUE = RANGE_1 }; };

// king halt after final capture
enum {HALT_1, HALT_N, HALT_K};
template<typename Rules> struct KingCaptureHalt                 { enum { VALUE = is_LongKingRange<Rules>::VALUE }; };
template<> struct KingCaptureHalt<KillerRules>                  { enum { VALUE = HALT_K }; };
template<> struct KingCaptureHalt<ThaiRules>                    { enum { VALUE = HALT_1 }; };

// men can capture kings
template<typename> struct is_MenCaptureKings                    { enum { VALUE = true  }; };
template<> struct is_MenCaptureKings<ItalianRules>              { enum { VALUE = false }; };

// promotion en-passant
enum {PROMOTE_BR, PROMOTE_EP};
template<typename> struct is_PromotionEnPassant                 { enum { VALUE = PROMOTE_BR }; };
template<> struct is_PromotionEnPassant<RussianRules>           { enum { VALUE = PROMOTE_EP }; };

// capture removal
enum {REMOVE_1, REMOVE_N};
template<typename> struct is_IntegralCaptureRemoval             { enum { VALUE = REMOVE_N }; };
template<> struct is_IntegralCaptureRemoval<ThaiRules>          { enum { VALUE = REMOVE_1 }; };

// capture direction reversal
template<typename> struct is_DirectionReversal                  { enum { VALUE = false }; };
template<> struct is_DirectionReversal<ThaiRules>               { enum { VALUE = true  }; };

// smallest value for which non-unique capture sequences can occur
template<typename> struct LargeCaptureValue                     { enum { VALUE = 4 }; };
template<> struct LargeCaptureValue<FrisianRules>               { enum { VALUE = 3 }; };
template<> struct LargeCaptureValue<ThaiRules>                  { enum { VALUE = 3 }; };

template<typename Rules>
struct is_AmbiguousManCapture
{
        static const bool VALUE = is_ManCaptureBackwards<Rules>::VALUE || is_PromotionEnPassant<Rules>::VALUE;
};

// restricted consecutive moves with the same king
template<typename> struct is_RestrictedSameKingMoves            { enum { VALUE = false }; };
template<> struct is_RestrictedSameKingMoves<FrisianRules>      { enum { VALUE = true  }; };

// maximum allowed consecutive moves with the same king
template<typename> struct MaxSameKingMoves                      { enum { VALUE = 6 }; };

//+----------------------------------------------------------------------------+
//|      Capture precedence                                                    |
//+----------------------------------------------------------------------------+

// absolute king capture precedence
template<typename> struct is_AbsoluteKingPrecedence             { enum { VALUE = false }; };
template<> struct is_AbsoluteKingPrecedence<CzechRules>         { enum { VALUE = true  }; };

// relative king capture precedence
template<typename> struct is_RelativeKingPrecedence             { enum { VALUE = false }; };
template<> struct is_RelativeKingPrecedence<FrisianRules>       { enum { VALUE = true  }; };
template<> struct is_RelativeKingPrecedence<ItalianRules>       { enum { VALUE = true  }; };

// majority capture precedence
template<typename> struct is_MajorityPrecedence                 { enum { VALUE = false }; };
template<> struct is_MajorityPrecedence<InternationalRules>     { enum { VALUE = true  }; };
template<> struct is_MajorityPrecedence<KillerRules>            { enum { VALUE = true  }; };
template<> struct is_MajorityPrecedence<FrisianRules>           { enum { VALUE = true  }; };
template<> struct is_MajorityPrecedence<SpanishRules>           { enum { VALUE = true  }; };
template<> struct is_MajorityPrecedence<ItalianRules>           { enum { VALUE = true  }; };

//+----------------------------------------------------------------------------+
//|      Drawing rules                                                         |
//+----------------------------------------------------------------------------+

// restricted repeated positions
template<typename> struct RestrictedRepetitions                 { enum { VALUE = true }; };

// maximum number of repeated positions
template<typename> struct MaxRepetitions                        { enum { VALUE =  3 }; };       // (FMJD rule 6.1)

// restricted consecutive non-conversion moves
template<typename> struct is_RestrictedNonConversionMoves       { enum { VALUE = true }; };

// maximum allowed consecutive non-conversion moves 
template<typename> struct MaxNonConversionMoves                 { enum { VALUE = 50 }; };       // (FMJD rule 6.2)

// other drawing rules
template<typename> struct num_Max3v1                            { enum { VALUE = 32 }; };       // (FMJD rule 6.3)
template<typename> struct num_Max2v1                            { enum { VALUE = 10 }; };       // (FMJD rule 6.4)
