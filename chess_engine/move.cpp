#include "piece.h"
#include "helper.h"
#include "move.h"
#include "chess_map.h"
#include <iostream>

move::move(std::string strMove) {
    auto [x1, y1, x2, y2] = get_coordinates_from_string(strMove);
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    toMove = chess_map::get_instance().get_piece_at(x1, y1);
    toTake = chess_map::get_instance().get_piece_at(x2, y2);
}

move::move(int x1, int y1, int x2, int y2, piece* toMove, piece* toTake) {
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    this->toMove = toMove;
    this->toTake = toTake;
}