#pragma once
#include <memory>       // std::unique_ptr
#include <string>
#include <vector>
#include "MessageInterface.h"

namespace damexchange {
namespace layer2 {

class Move: public MessageInterface
{
public:
        // views
        int seconds(void) const;
        int from_sq(void) const;
        int dest_sq(void) const;
        int num_captured(void) const;
        const std::vector<int>& captured_pieces(void) const;
        static std::string str(int, int, int, int, const std::vector<int>&);

private:
        // private constructor to enforce factory creation
        explicit Move(const std::string&);

        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;
        static std::string body(int, int, int, int, const std::vector<int>&);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        int seconds_;
        int from_sq_;
        int dest_sq_;
        int num_captured_;
        std::vector<int> captured_pieces_;       
};

}       // namespace layer2
}       // namespace damexchange
