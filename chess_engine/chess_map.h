#ifndef __CHESS_MAP_H__
#define __CHESS_MAP_H__
#define MAP_SIZE 9

#include "piece.h"
#include "move.h"
#include "helper.h"
#include <iostream>

class piece;

class chess_map {
    private:
        // TODO dynamic allocation
        piece* representation[MAP_SIZE + 1][MAP_SIZE + 1];
        // TODO move history

    public:
        colours colour = BLACK;

        static chess_map& get_instance();

        void init_default_map();

        std::pair<int, int> white_king;
        std::pair<int, int> black_king;

        // for debugging
        void print_map();

        void execute_move(move& mv);

        piece* get_piece_at(int x, int y);

        void set_colour(colours colour);

        void undo_move(move& mv);

        void flip();
};

#endif // __CHESS_MAP_H__