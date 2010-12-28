#pragma once
//#include "windows.h"
#define WINAPI

 #ifdef __cplusplus
 extern "C" {
 #endif

// Full documentation of the CheckerBoard 1.64 API: http://www.fierz.ch/cbdeveloper.php

// used for all other communication between engine and CheckerBoard
int WINAPI enginecommand(
        char command[256],      // the interface sends a command string to the engine
        char reply[1024]        // the engine should print a reply
);

// gets a move from the engine
int WINAPI getmove(
        int board[8][8],        // the current board position 
        int color,              // the side to move
        double maxtime,         // the time your program should think on average for its move 
        char str[1024],         // display search information in the status bar of CheckerBoard
        int *playnow,           // immediately return if the user has chosen "Play" in the CheckerBoard menu 
        int info,               // a bunch of flags which are used for some advanced stuff
        int moreinfo,           // currently unused and also meant for further extensions
        struct CBmove *move     // move trajectory
);

// tells CheckerBoard about the rules of your version of checkers
int WINAPI islegal(
        int board[8][8],        // the current board position
        int color,              // the side to move
        int from,               // origin square
        int to,                 // destination square
        struct CBmove *move     // move trajectory
);

// coordinates of a board square
struct coor
{
        int x;                  // row
        int y;                  // column
};

// animate the move properly
struct CBmove
{
        int jumps;              // number of jumps in this move
        int newpiece;           // moving piece after jump
        int oldpiece;           // moving piece before jump
        struct coor from,to;    // from,to squares of moving piece
        struct coor path[12];	// intermediate squares to jump to
        struct coor del[12];    // squares where men are removed
        int delpiece[12];       // piece type which is removed
};

// The board position and the side to move use the conventions:
#define WHITE 1
#define BLACK 2
#define MAN 4
#define KING 8
#define FREE 0

// Getmove should return a value between 0 and 3, defined as follows
#define DRAW 0
#define WIN 1
#define LOSS 2
#define UNKNOWN 3

 #ifdef __cplusplus
 }
 #endif
