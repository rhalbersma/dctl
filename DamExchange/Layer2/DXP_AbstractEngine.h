#pragma once

class DXP_AbstractEngine
{
public:
        // interface
        virtual void handle(const DXP_GameRequest&) = 0;
        virtual void handle(const DXP_GameAcknowledge&) = 0;
        virtual void handle(const DXP_Move&) = 0;
        virtual void handle(const DXP_GameEnd&) = 0;
        virtual void handle(const DXP_Chat&) = 0;
        virtual void handle(const DXP_BackRequest&) = 0;
        virtual void handle(const DXP_BackAcknowledge&) = 0;

        // virtual destructor
        virtual DXP_AbstractEngine(void) {};
};
