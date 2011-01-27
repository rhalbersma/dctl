#pragma once
#include "../Utilities/IntegerTypes.h"

namespace Variant {

//+----------------------------------------------------------------------------+
//|      Move and capture mechanics                                            |
//+----------------------------------------------------------------------------+

// man capture directions
enum {DIRS_2, DIRS_4, DIRS_8};
template<typename> struct ManCaptureDirections                  { enum { VALUE = DIRS_4 }; };

template<typename Rules>
struct is_ManCaptureBackwards
{
        enum { VALUE = ManCaptureDirections<Rules>::VALUE != DIRS_2 };
};

// king capture directions
template<typename> struct KingCaptureDirections                 { enum { VALUE = DIRS_4 }; };

// king range
enum {RANGE_1, RANGE_N};
template<typename> struct is_LongKingRange                      { enum { VALUE = RANGE_N }; };

// king halt after final capture
enum {HALT_1, HALT_N, HALT_K};
template<typename Rules> struct KingCaptureHalt                 { enum { VALUE = is_LongKingRange<Rules>::VALUE }; };

// men can capture kings
template<typename> struct is_MenCaptureKings                    { enum { VALUE = true  }; };

// promotion en-passant
enum {PROMOTE_BR, PROMOTE_EP};
template<typename> struct PromotionCondition                    { enum { VALUE = PROMOTE_BR }; };

// capture removal
enum {REMOVE_1, REMOVE_N};
template<typename> struct CaptureRemoval                        { enum { VALUE = REMOVE_N }; };

// capture direction reversal
template<typename> struct is_DirectionReversal                  { enum { VALUE = false }; };

// smallest value for which non-unique capture sequences can occur
template<typename> struct LargeCaptureValue                     { enum { VALUE = 4 }; };

template<typename Rules>
struct is_AmbiguousManCapture
{
        enum { 
                VALUE = is_ManCaptureBackwards<Rules>::VALUE || PromotionCondition<Rules>::VALUE 
        };
};

// restricted consecutive moves with the same king
template<typename> struct is_RestrictedSameKingMoves            { enum { VALUE = false }; };

// maximum allowed consecutive moves with the same king
template<typename> struct MaxSameKingMoves                      { enum { VALUE = 6 }; };

//+----------------------------------------------------------------------------+
//|      Capture precedence                                                    |
//+----------------------------------------------------------------------------+

// absolute king capture precedence
template<typename> struct is_AbsoluteKingPrecedence             { enum { VALUE = false }; };

// relative king capture precedence
template<typename> struct is_RelativeKingPrecedence             { enum { VALUE = false }; };

// majority capture precedence
template<typename> struct is_MajorityPrecedence                 { enum { VALUE = false }; };

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

//+----------------------------------------------------------------------------+
//|      Implementation of move generation                                     |
//+----------------------------------------------------------------------------+

template<typename> struct is_CheckCaptureUniqueness             { enum { VALUE = true }; };

}       // namespace Variant
