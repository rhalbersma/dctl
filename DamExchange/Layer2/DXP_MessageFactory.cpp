#include "DXP_MessageFactory.h"
#include "DXP_GameRequest.h"
#include "DXP_GameAcknowledge.h"
#include "DXP_Move.h"
#include "DXP_GameEnd.h"
#include "DXP_Chat.h"
#include "DXP_BackRequest.h"
#include "DXP_BackAcknowledge.h"
#include <cassert>

const DXP_AbstractMessage* DXP_MessageFactory::create(const std::string& dxp_message)
{
        const char dxp_header = *dxp_message.begin();

        switch(dxp_header) {
        case DXP_GameRequest::HEADER: 
                return new DXP_GameRequest(dxp_message);
        case DXP_GameAcknowledge::HEADER: 
                return new DXP_GameAcknowledge(dxp_message);
        case DXP_Move::HEADER: 
                return new DXP_Move(dxp_message);
        case DXP_GameEnd::HEADER: 
                return new DXP_GameEnd(dxp_message);
        case DXP_Chat::HEADER:
                return new DXP_Chat(dxp_message);
        case DXP_BackRequest::HEADER: 
                return new DXP_BackRequest(dxp_message);
        case DXP_BackAcknowledge::HEADER: 
                return new DXP_BackAcknowledge(dxp_message);
        default:
                assert(false);
                return 0;
        }
}
