#pragma once
#include "DXP_AbstractMessage.h"
#include "DXP_String.h"
#include <string>

class DXP_GameEnd: public DXP_AbstractMessage
{
public:
        // typedefs
        enum Reason { FORFEIT = 0, RESIGN = 1, CLAIM_DRAW = 2, CLAIM_WIN = 3 }; 
        enum StopCode { STOP_THIS = 0, STOP_ALL = 1 };

        // constructors
        static DXP_AbstractMessage* create(const DXP_String&);

        DXP_GameEnd(const std::string&);
        DXP_GameEnd(Reason, StopCode);

        // views
        virtual DXP_String message(void) const;

        Reason reason(void) const;
        StopCode stop_code(void) const;

private:
        // implementation
        static const char HEADER = 'E';
        static const bool REGISTERED;

        // representation
        Reason d_reason;
        StopCode d_stop_code;
};
