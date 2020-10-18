#include "chess_map.h"
#include "piece.h"
#include "move.h"
#include "helper.h"
#include <vector>
#include <iostream>

piece::piece() {
    x = y = 0;
    colour = WHITE;
    type = EMPTY;
}

piece::piece(int x, int y, colours colour, piece_type type) {
    this->x = x;
    this->y = y;
    this->colour = colour;
    this->type = type;
}

bool piece::is_same_colour(piece& otherPiece) {
    return otherPiece.colour == colour;
}

char piece::get_type_char() {
    static char letter[] = {'k', 'q', 'c', 'r', 'b', 'p', '0'};
    return letter[type];
}

piece* get_empty_piece() {
    return (new piece());
}

void print_piece(piece& p) {
    std::cout << "piece: " << p.x << ' ' << p.y << '\n';
}