#include "move.h"

#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <sstream>

bool isIgnoredCommand(std::string command) {
    static const std::string ignoredCommands[] = {"xboard", "accepted",
    "random", "level", "post", "hard", "time", "otim", "computer"};
    // separa cuvintele din linie
    std::istringstream words(command);

    // salvez primul cuvant
    std::string command1;
    words >> command1;
    for (auto& str : ignoredCommands) {
        if (command1.compare(str) == 0) {
            return true;
        }
    }
    return false;
}

bool isOnMap(int x, int y) {
    return x >= 1 && y >= 1 && x <= 8 && y <= 8;
}

std::tuple<int, int, int, int> get_coordinates_from_string(std::string stringMove) {
    int x1, y1, x2, y2;
    x1 = stringMove[0] - 'a' + 1;
    x2 = stringMove[2] - 'a' + 1;
    y1 = stringMove[1] - '0';
    y2 = stringMove[3] - '0';
    return {x1, y1, x2, y2};
}

std::string get_string_from_coordinates(int x1, int y1, int x2, int y2) {
    char* ret = new char[5];
    ret[0] = x1 + 'a' - 1;
    ret[2] = x2 + 'a' - 1;
    ret[1] = y1 + '0';
    ret[3] = y2 + '0';
    ret[4] = 0;
    return ret;
}

void flip_string_move(std::string& s) {
    s[1] = '8' - s[1] + '1';
    s[3] = '8' - s[3] + '1';
}

move flip_move(move& mv) {
    return move(get_string_from_coordinates(mv.x1, 9 - mv.y1, mv.x2, 9 - mv.y2));
}
