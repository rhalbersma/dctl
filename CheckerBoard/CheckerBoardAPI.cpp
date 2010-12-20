#include "CheckerBoardAPI.h"

// used for all other communication between engine and CheckerBoard
int WINAPI enginecommand( /*
        char command[256],      // the interface sends a command string to the engine
        char reply[1024]        // the engine should print a reply */
) 
{
        // 
}

// gets a move from the engine
int WINAPI getmove( /*
        int board[8][8],        // the current board position 
        int color,              // the side to move
        double maxtime,         // the time your program should think on average for its move 
        char str[1024],         // display search information in the status bar of CheckerBoard
        int *playnow,           // immediately return if the user has chosen "Play" in the CheckerBoard menu 
        int info,               // a bunch of flags which are used for some advanced stuff
        int moreinfo,           // currently unused and also meant for further extensions
        struct CBmove *move     // move trajectory */
)
{
        // 
}

// tells CheckerBoard about the rules of your version of checkers
int WINAPI islegal( /*
        int board[8][8],        // the current board position
        int color,              // the side to move
        int from,               // origin square
        int to,                 // destination square
        struct CBmove *move     // move trajectory */
) 
{
        //
}
