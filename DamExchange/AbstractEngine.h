#pragma once

namespace DamExchangeProtocol {

class AbstractEngine
{
public:
        // interface
        virtual void handle(const GameRequest&) = 0;
        virtual void handle(const GameAcknowledge&) = 0;
        virtual void handle(const Move&) = 0;
        virtual void handle(const GameEnd&) = 0;
        virtual void handle(const Chat&) = 0;
        virtual void handle(const BackRequest&) = 0;
        virtual void handle(const BackAcknowledge&) = 0;

        // virtual destructor
        virtual AbstractEngine(void) {};
};

}       // namespace DamExchangeProtocol
