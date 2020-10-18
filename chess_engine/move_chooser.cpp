#include "chess_map.h"
#include "move.h"
#include "move_chooser.h"
#include "piece.h"
#include "helper.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

std::vector<std::vector<std::pair<int, int>>> move_increments = {
    {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}}, // king
    {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}}, // queen
    {{1, 2}, {1, -2}, {2, 1}, {2, -1}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}}, // knight
    {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}, // rook
    {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}}, // bishop
};
int move_index;
bool multipleMoves[] = {0, 1, 0, 1, 1};
int value[] = {2000, 90, 30, 50, 30, 10, 0};
std::vector<std::vector<int>> pawnBonus = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 5, 5, 5, 5, 5, 5, 5, 5},
{0, 1, 1, 2, 3, 3, 2, 1, 1},
{0, 0, 0, 1, 2, 2, 1, 0, 0},
{0, 0, 0, 0, 2, 2, 0, 0, 0},
{0, 0, 0,-1, 0, 0,-1, 0, 0},
{0, 0, 1, 1,-2,-2, 1, 1, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0}};
std::vector<std::vector<int>> knightBonus = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, -5,-4,-3,-3,-3,-3,-4,-5},
{0, -4,-2, 0, 0, 0, 0,-2,-4},
{0, -3, 0, 1, 1, 1, 1, 0,-3},
{0, -3, 0, 1, 2, 2, 1, 0,-3},
{0, -3, 0, 1, 2, 2, 1, 0,-3},
{0, -3, 0, 1, 1, 1, 1, 0,-3},
{0, -4,-2, 0, 0, 0, 0,-2,-4},
{0, -5,-4,-3,-3,-3,-3,-4,-5}};
std::vector<std::vector<int>> bishopBonus = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, -2,-1,-1,-1,-1,-1,-1,-2},
{0, -1, 0, 0, 0, 0, 0, 0,-1},
{0, -1, 0, 0, 1, 1, 0, 0,-1},
{0, -1, 0, 0, 1, 1, 0, 0,-1},
{0, -1, 0, 1, 1, 1, 1, 0,-1},
{0, -1, 1, 1, 1, 1, 1, 1,-1},
{0, -1, 0, 0, 0, 0, 0, 0,-1},
{0, -2,-1,-1,-1,-1,-1,-1,-2}};
std::vector<std::vector<int>> rookBonus = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 1, 1, 1, 1, 1, 1, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0}};
std::vector<std::vector<int>> queenBonus = {{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0,-2,-1,-1, 0, 0,-1,-1,-2},
{0,-1, 0, 0, 0, 0, 0, 0,-1},
{0,-1, 0, 0, 0, 0, 0, 0,-1},
{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0},
{0,-1, 0, 0, 0, 0, 0, 0,-1},
{0,-1, 0, 0, 0, 0, 0, 0,-1},
{0,-2,-1,-1, 0, 0,-1,-1,-2}};
std::vector<std::vector<int>>* pieceBonuses[] = {0, &queenBonus, &knightBonus, &rookBonus, &bishopBonus, &pawnBonus, 0};

bool is_checked(move& mv) {
    chess_map& board = chess_map::get_instance();
    board.execute_move(mv);

    // king position
    auto [xK, yK] = board.white_king;

    // check pawns
    if (isOnMap(xK - 1, yK + 1)) {
        piece* topLeft = board.get_piece_at(xK - 1, yK + 1);
        if (topLeft->type == PAWN && topLeft->colour == BLACK) {
            board.undo_move(mv);
            return true;
        }
    }
    if (isOnMap(xK + 1, yK + 1)) {
        piece* topRight = board.get_piece_at(xK + 1, yK + 1);
        if (topRight->type == PAWN && topRight->colour == BLACK) {
            board.undo_move(mv);
            return true;
        }
    }

    // piece.type
    for (int i = 0; i < 5; ++i) {
        // single move pieces
        if (!multipleMoves[i]) {
            // for each direction of movement
            for (auto [incX, incY] : move_increments[i]) {
                if (!isOnMap(xK + incX, yK + incY)) {
                    continue;
                }
                piece* current = board.get_piece_at(xK + incX, yK + incY);
                if (current->type == i && current->colour == BLACK) {
                    board.undo_move(mv);
                    return true;
                }
            }
        } else {
            // for each direction of movement
            for (auto [incX, incY] : move_increments[i]) {
                int ics = xK + incX, igrec = yK + incY;
                while (isOnMap(ics, igrec)) {
                    piece* current = board.get_piece_at(ics, igrec);
                    if (current->type != EMPTY && current->type != i) {
                        break;
                    }
                    if (current->type == i && current->colour == BLACK) {
                        board.undo_move(mv);
                        return true;
                    }
                    ics += incX;
                    igrec += incY;
                }
            }
        }
    }

    board.undo_move(mv);
    return false;
}

std::vector<move>& get_pawn_moves(piece* p) {
    int x = p->x, y = p->y;
    chess_map& board = chess_map::get_instance();
    std::vector<move> *ret_pointer = new std::vector<move>();
    std::vector<move>& ret = *ret_pointer;

    // take diagonally
    if (x != 1) {
        piece* diagonalLeft = board.get_piece_at(x - 1, y + 1);
        if (diagonalLeft->type != EMPTY && diagonalLeft->colour == BLACK) {
            ret.push_back(move(x, y, x - 1, y + 1, p, diagonalLeft));
        }
    }
    if (x != 8) {
        piece* diagonalRight = board.get_piece_at(x + 1, y + 1);
        if (diagonalRight->type != EMPTY && diagonalRight->colour == BLACK) {
            ret.push_back(move(x, y, x + 1, y + 1, p, diagonalRight));
        }
    }

    // take in front
    piece* pieceInFront = board.get_piece_at(x, y + 1);
    if (pieceInFront->type != EMPTY) {
        return ret;
    }
    ret.push_back(move(x, y, x, y + 1, p, get_empty_piece()));
    if (y!= 2 || !isOnMap(x, y + 2)) {
        return ret;
    }
    piece* twoInFront = board.get_piece_at(x, y + 2);
    if (twoInFront->type == EMPTY) {
        ret.push_back(move(x, y, x, y + 2, p, get_empty_piece()));
    }
    return ret;
}

// returns vector of theoretically possible moves for white piece p
std::vector<move>& get_piece_moves(piece* p) {
    chess_map& board = chess_map::get_instance();
    std::vector<move> *ret_pointer = new std::vector<move>();
    std::vector<move>& ret = *ret_pointer;

    std::vector<std::pair<int, int>>& inc = move_increments[p->type];
    if (p->type == EMPTY) {
        return ret;
    }

    // TODO
    if (p->type == PAWN) {
        return get_pawn_moves(p);;
    }

    // piece moves one square
    if (!multipleMoves[p->type]) {
        // for each direction
        for (auto [xInc, yInc] : inc) {
            if (!isOnMap(p->x + xInc, p->y + yInc)) {
                continue;
            }
            piece* toTake = board.get_piece_at(p->x + xInc, p->y + yInc);
            if (toTake->type == EMPTY || toTake->colour == BLACK) {
                ret.push_back(move(p->x, p->y, p->x + xInc, p->y + yInc, p, toTake));
            }
        }
        return ret;
    }
    // piece can move multiple squares
    // for each direction
    for (auto [xInc, yInc] : inc) {
        int xC = p->x + xInc, yC = p->y + yInc;
        while (isOnMap(xC, yC)) {
            piece* toTake = board.get_piece_at(xC, yC);
            if (toTake->type != EMPTY) {
                if (toTake->colour == BLACK) {
                    ret.push_back(move(p->x, p->y, xC, yC, p, toTake));
                }
                break;
            } else {
                ret.push_back(move(p->x, p->y, xC, yC, p, toTake));
            }
            xC += xInc;
            yC += yInc;
        }
    }
    return ret;
}

std::vector<move>& get_all_valid_moves() {
    chess_map& board = chess_map::get_instance();
    std::vector<move> *allPossible_pointer = new std::vector<move>();
    std::vector<move>& allPossible = *allPossible_pointer;
    // std::ofstream fout;
    for (int col = 1; col < MAP_SIZE; ++col) {
        for (int line = 1; line < MAP_SIZE; ++line) {
            piece* currentPiece = board.get_piece_at(col, line);
            if (currentPiece->colour == BLACK || currentPiece->type == EMPTY) {
                continue;
            }
            std::vector<move>& possible = get_piece_moves(currentPiece);
            for (move& m : possible) {
                if (!is_checked(m)) {
                    allPossible.push_back(m);
                }
            }
        }
    }
    return allPossible;
}

int eval(chess_map& board) {
    int sum = 0;
    for (int i = 1; i < MAP_SIZE; ++i) {
        for (int j = 1; j < MAP_SIZE; ++j) {
            piece* currentPiece = board.get_piece_at(i, j);
            // piece value
            sum += value[currentPiece->type] * ((currentPiece->colour == WHITE) ? 1 : -1);
            // position bonuses
            std::vector<std::vector<int>>* currentBonus = pieceBonuses[currentPiece->type];
            if (!currentBonus || currentPiece->colour == BLACK) {
                continue;
            }
            sum += currentBonus->at(currentPiece->x)[currentPiece->y] * ((currentPiece->colour == WHITE) ? 1 : -1);
        }
    }
    return sum;
}

std::pair<int, move> minimax(int depth, int alpha, int beta) {
    chess_map& board = chess_map::get_instance();
    if (depth == 0) {
        return {eval(board), move("a1a1")}; // fa ti un get empty move
    }
    std::vector<move>& allMoves = get_all_valid_moves();
    if (allMoves.size() == 0) {
        return {-9999, move("a1a1")};
    }
    board.flip();
    int maxScore = -10000;
    std::tuple<int, int, int, int> maxMove;

    for (move& mv : allMoves) {
        move temp = flip_move(mv);
        board.execute_move(temp);
        auto [score, trash] = minimax(depth - 1, - beta, - alpha);
        board.undo_move(temp);

        if (-score > maxScore) {
            maxScore = -score;
            maxMove = {mv.x1, mv.y1, mv.x2, mv.y2};
        } else if (-score == maxScore) {
            if (rand() % 8 != 0) {
                continue;
            }
            maxScore = -score;
            maxMove = {mv.x1, mv.y1, mv.x2, mv.y2};
        }
        if (- score > alpha) {
            alpha = - score;
        }
        if (alpha >= beta) {
            break;
        }
    }
    board.flip();
    auto& [a, b, c, d] = maxMove;
    delete &allMoves;
    return {maxScore, move(get_string_from_coordinates(a, b, c, d))};
}

int choose_move(colours myColour) {
    move_index++;
    chess_map& board = chess_map::get_instance();
    move* mv;
    if (move_index > 2) {
        std::pair<int, move> mymv = minimax(3, -10000, 10000);
        int score = mymv.first;
        mv = &mymv.second;
        if (mv->x1 == 1 && mv->y1 == 1 && mv->x2 == 1 && mv->y2 == 1) {
            return -1; // nu am gasit miscari, resign
        }
    }
    if (move_index == 1) {
        move_index++;
        mv = new move(5, 2, 5, 4, board.get_piece_at(5, 2), board.get_piece_at(5, 4));
    } else if (move_index == 2) {
        if ((*board.get_piece_at(3, 5)).type != EMPTY) {
            mv = new move(7, 1, 6, 3, board.get_piece_at(7, 1), board.get_piece_at(6, 3));
        } else if ((*board.get_piece_at(5, 5)).type != EMPTY) {
            mv = new move(7, 1, 6, 3, board.get_piece_at(7, 1), board.get_piece_at(6, 3));
        } else if ((*board.get_piece_at(5, 6)).type != EMPTY) {
            mv = new move(4, 2, 4, 4, board.get_piece_at(4, 2), board.get_piece_at(4, 4));
        } else if ((*board.get_piece_at(3, 6)).type != EMPTY) {
            mv = new move(4, 2, 4, 4, board.get_piece_at(4, 2), board.get_piece_at(4, 4));
        } else if ((*board.get_piece_at(4, 6)).type != EMPTY) {
            mv = new move(4, 2, 4, 4, board.get_piece_at(4, 2), board.get_piece_at(4, 4));
        } else if ((*board.get_piece_at(4, 5)).type != EMPTY) {
            mv = new move(5, 4, 4, 5, board.get_piece_at(5, 4), board.get_piece_at(4, 5));
        } else if ((*board.get_piece_at(2, 6)).type != EMPTY) {
            mv = new move(4, 2, 4, 4, board.get_piece_at(4, 2), board.get_piece_at(4, 4));
        } else if ((*board.get_piece_at(1, 6)).type != EMPTY) {
            mv = new move(4, 2, 4, 4, board.get_piece_at(4, 2), board.get_piece_at(4, 4));
        } else if ((*board.get_piece_at(7, 5)).type != EMPTY) {
            mv = new move(4, 2, 4, 4, board.get_piece_at(4, 2), board.get_piece_at(4, 4));
        } else if ((*board.get_piece_at(8, 5)).type != EMPTY) {
            mv = new move(4, 2, 4, 4, board.get_piece_at(4, 2), board.get_piece_at(4, 4));
        } else if ((*board.get_piece_at(1, 5)).type != EMPTY) {
            mv = new move(4, 2, 4, 4, board.get_piece_at(4, 2), board.get_piece_at(4, 4));
        } else if ((*board.get_piece_at(6, 5)).type != EMPTY) {
            mv = new move(5, 4, 6, 5, board.get_piece_at(5, 4), board.get_piece_at(6, 5));
        } else if ((*board.get_piece_at(2, 5)).type != EMPTY) {
            mv = new move(4, 2, 4, 4, board.get_piece_at(4, 2), board.get_piece_at(4, 4));
        } else {
            mv = new move(4, 2, 4, 3, board.get_piece_at(4, 2), board.get_piece_at(4, 3));
        }
    }
    board.execute_move(*mv);
    std::string s = get_string_from_coordinates(mv->x1, mv->y1, mv->x2, mv->y2);
    if (mv->toMove->type == PAWN && mv->y2 == 8) {
    s = s + "q";
    }
    
    if (myColour == BLACK) {
    flip_string_move(s);
    }
    
    std::cout << "move " << s << '\n';
    return 0;
}