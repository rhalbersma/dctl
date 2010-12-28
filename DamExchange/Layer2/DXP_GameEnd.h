#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_GameEnd: public DXP_AbstractMessage
{
public:
        // typedefs
        enum StopCode { STOP_THIS = 0, STOP_ALL = 1 };

        // constructors
        DXP_GameEnd(const std::string&);
        DXP_GameEnd(size_t, StopCode);

        // views
        char header(void) const;
        std::string message(void) const;
        size_t reason(void) const;
        StopCode stop_code(void) const;

        // implementation
        static const char HEADER = 'E';

private:
        // representation
        size_t d_reason;
        StopCode d_stop_code;
};
