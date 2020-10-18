#ifndef __PIECE_H__
#define __PIECE_H__

#include "move.h"
#include "helper.h"
#include <vector>
#include <iostream>
#include <fstream>

// enums for readability
enum piece_type {
    KING, QUEEN, KNIGHT, ROOK, BISHOP, PAWN, EMPTY
};

enum colours {
    WHITE, BLACK
};

class move;
// class chess_map;

class piece {
    public:
        // coordinates; x is column, y is line
        int x, y;
        // false = white, true = black
        colours colour;
        piece_type type = EMPTY;

        piece();

        piece(int x, int y, colours colour, piece_type type);

        bool is_same_colour(piece& otherPiece);

        char get_type_char();
};

piece* get_empty_piece();

void print_piece(piece& p);

#endif // __PIECE_H__
