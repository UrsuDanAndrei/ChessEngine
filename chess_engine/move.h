#ifndef __MOVE_H__
#define __MOVE_H__

#include "piece.h"
#include "helper.h"

class piece;
piece* get_empty_piece();

class move {
    public:
        // original and new positions
        int x1, x2, y1, y2;
        piece* toMove = get_empty_piece();
        piece* toTake = get_empty_piece();

        // constructor for move from a received move
        move(std::string strMove);

        // the basic move constructor
        move(int x1, int y1, int x2, int y2, piece* toMove, piece* toTake);

        // copy constructor
        move(const move& other) {
            this->x1 = other.x1;
            this->y1 = other.y1;
            this->x2 = other.x2;
            this->y2 = other.y2;
            this->toMove = other.toMove;
            this->toTake = other.toTake;
        }

        // copy assigment
        move& operator=(const move& other) {
            this->x1 = other.x1;
            this->y1 = other.y1;
            this->x2 = other.x2;
            this->y2 = other.y2;
            this->toMove = other.toMove;
            this->toTake = other.toTake;

            return *this;
        }

        ~move() {
            
        }
};

#endif // __MOVE_H__
