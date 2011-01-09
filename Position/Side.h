#pragma once

class Side
{
public:
        // typedefs 
	enum { BLACK, WHITE };

        // constructors
        Side(void);                     // initialize with black to move
        explicit Side(bool);            // initialize with a side to move

        // views
        bool to_move(void) const;       // the side to move

        // modifiers
        void pass(void);                // toggle the side to move      

private:
        // implementation
        static const bool PASS = true;  // toggle the side to move

        // representation
        bool side_;                     // the side to move
};
