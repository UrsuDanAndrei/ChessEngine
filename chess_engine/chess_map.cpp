#include "chess_map.h"
#include "piece.h"
#include "move.h"
#include "helper.h"
#include <iostream>
#include <fstream>
#include <string.h>

chess_map& chess_map::get_instance() {
    static chess_map instance;
    return instance;
}

void chess_map::init_default_map() {
    // default suntem black
    this->set_colour(BLACK);

    // pawns
    for (int col = 1; col < MAP_SIZE; ++col) {
        representation[col][2] = new piece(col, 2, WHITE, PAWN);
        representation[col][7] = new piece(col, 7, BLACK, PAWN);
    }

    // rooks
    representation[1][1] = new piece(1, 1, WHITE, ROOK);
    representation[8][1] = new piece(8, 1, WHITE, ROOK);
    representation[1][8] = new piece(1, 8, BLACK, ROOK);
    representation[8][8] = new piece(8, 8, BLACK, ROOK);

    // bishops
    representation[3][1] = new piece(3, 1, WHITE, BISHOP);
    representation[6][1] = new piece(6, 1, WHITE, BISHOP);
    representation[3][8] = new piece(3, 8, BLACK, BISHOP);
    representation[6][8] = new piece(6, 8, BLACK, BISHOP);

    // knights
    representation[2][1] = new piece(2, 1, WHITE, KNIGHT);
    representation[7][1] = new piece(7, 1, WHITE, KNIGHT);
    representation[2][8] = new piece(2, 8, BLACK, KNIGHT);
    representation[7][8] = new piece(7, 8, BLACK, KNIGHT);

    // kings
    representation[5][1] = new piece(5, 1, WHITE, KING);
    representation[5][8] = new piece(5, 8, BLACK, KING);
    white_king = {5, 1};
    black_king = {5, 8};

    // queens
    representation[4][1] = new piece(4, 1, WHITE, QUEEN);
    representation[4][8] = new piece(4, 8, BLACK, QUEEN);

    // empty places
    for (int col = 1; col < MAP_SIZE; ++col) {
        for (int line = 3; line < 7; ++line) {
            representation[col][line] = get_empty_piece();
        }
    }
}

void chess_map::print_map() {
    for (int line = 8; line > 0; --line) {
        for (int col = 1; col < MAP_SIZE; ++col) {
            piece* currentPiece = representation[col][line];
        }
    }
}

void chess_map::undo_move(move& mv) {
    representation[mv.x1][mv.y1] = mv.toMove;
    representation[mv.x2][mv.y2] = mv.toTake;
    mv.toMove->x = mv.x1;
    mv.toMove->y = mv.y1;
    if (mv.toMove->type == KING) {
        if (mv.toMove->colour == WHITE) {
            white_king = {mv.x1, mv.y1};
        } else {
            black_king = {mv.x1, mv.y1};
        }
    }
}

void chess_map::execute_move(move& mv) {
    // castling
    if (mv.toMove->type == KING
        && abs(mv.x2 - mv.x1) == 2) {
            if (mv.toMove->colour == BLACK) {
                if (mv.x2 == 7) {
                    piece* tura = get_piece_at(8, 8);
                    tura->x = 6;
                    representation[6][8] = tura;
                    representation[8][8] = get_empty_piece();
                } else {
                    piece* tura = get_piece_at(1, 8);
                    tura->x = 4;
                    representation[4][8] = tura;
                    representation[1][8] = get_empty_piece();
                }
        }
    }
    if (mv.toMove->type == PAWN) {
        if (mv.x1 != mv.x2) {
            piece* p = get_piece_at(mv.x2, mv.y2);
            if (p->type == EMPTY) {
                // s-a facut un en passent

                if (mv.toMove->colour == BLACK) {
                    representation[mv.x2][mv.y2 + 1] = get_empty_piece();
                } else {
                    representation[mv.x2][mv.y2 - 1] = get_empty_piece();
                }
            }
        }
    }
    if (mv.toMove->type == KING) {
        if (mv.toMove->colour == WHITE) {
            white_king = {mv.x2, mv.y2};
        } else {
            black_king = {mv.x2, mv.y2};
        }
    }

    representation[mv.x2][mv.y2] = mv.toMove;
    mv.toMove->x = mv.x2;
    mv.toMove->y = mv.y2;
    representation[mv.x1][mv.y1] = get_empty_piece();

    if (mv.toMove->type == PAWN && (mv.y2 == 8 || mv.y2 == 1)) {
        representation[mv.x2][mv.y2] = new piece(mv.x2, mv.y2, mv.toMove->colour, QUEEN);
    }
}

piece* chess_map::get_piece_at(int x, int y) {
    return representation[x][y];
}

void chess_map::set_colour(colours colour) {
    this->colour = colour;
}

void chess_map::flip() {
    for (int col = 1; col < MAP_SIZE; ++col) {
        for (int lin = 1; lin < 5; ++lin) {
            piece* pc = get_piece_at(col, lin);
            piece* pc2 = get_piece_at(col, 9 - lin);
            if (pc->type == EMPTY && pc2->type == EMPTY) {
                continue;
            }
            colours clr = pc->colour == WHITE ? BLACK : WHITE;
            colours clr2 = pc2->colour == WHITE ? BLACK : WHITE;

            pc->colour = clr;
            pc->y = 9 - pc->y;

            pc2->colour = clr2;
            pc2->y = 9 - pc2->y;

            representation[col][9 - lin] = pc;
            representation[col][lin] = pc2;
        }
    }

    int tempX = white_king.first;
    int tempY = white_king.second;
    white_king = {black_king.first, 9 - black_king.second};
    black_king = {tempX, 9 - tempY};
    set_colour((colours) (1 - colour));
}
