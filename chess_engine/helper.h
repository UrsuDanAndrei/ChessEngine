#ifndef __HELPER_H__
#define __HELPER_H__

#include "move.h"

#include <string>
#include <tuple>
#include <vector>

class move;

bool isIgnoredCommand(std::string command);

bool isOnMap(int x, int y);

std::tuple<int, int, int, int> get_coordinates_from_string(std::string stringMove);

std::string get_string_from_coordinates(int x1, int y1, int x2, int y2);

void flip_string_move(std::string& s);

move flip_move(move& mv);

#endif // __HELPER_H__
