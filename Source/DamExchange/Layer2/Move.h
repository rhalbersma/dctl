#pragma once
#include <memory>
#include <string>
#include <vector>
#include "MessageInterface.h"

namespace damexchange {
namespace layer2 {

class Move: public MessageInterface
{
public:
        // constructors
        explicit Move(const std::string&);
        Move(int, int, int, int, const std::vector<int>&);

        // views
        int seconds(void) const;
        int from_sq(void) const;
        int dest_sq(void) const;
        int num_captured(void) const;
        const std::vector<int>& captured_pieces(void) const;

private:
        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;

        static std::shared_ptr<MessageInterface> create(const std::string&);
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
