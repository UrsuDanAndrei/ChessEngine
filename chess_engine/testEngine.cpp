#include "chess_map.h"
#include "move_chooser.h"
#include "piece.h"
#include "testEngine.h"

#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>

int process_white_or_black(colours colour) {
    std::string line;
    // citesc time, otim si culoarea

    getline(std::cin, line);
    if (line[0] == 'g' && line[1] == 'o') {
        return -1;
    }

    getline(std::cin, line);
    if (line[0] == 'g' && line[1] == 'o') {
        return -1;
    }

    getline(std::cin, line);
    if (line[0] == 'g' && line[1] == 'o') {
        return -1;
    }

    chess_map::get_instance().set_colour((colours)(1 - colour));
    return 0;
}

void process_edit_position() {
    std::string command;
    // citeste #
    getline(std::cin, command);
    bool colour = false;
    while (true) {
        getline(std::cin, command);
        if (command.compare(".") == 0) {
            // s-a terminat edit position
            break;
        }

        if (command.compare("c") == 0) {
            colour = true;
            continue;
        }

        // primeste pozitia pieselor de culoarea color, de exemplu command: Pa2
        // TODO MyMap.process_special_move(command, color)
    }
}

void process_analyze() {
    std::string command;

    // citeste comanda .
    std::getline(std::cin, command);
    while (true) {
        getline(std::cin, command);
        if (command.compare("exit") == 0) {
            break;
        }

        // incepe sa primeasca comenzi de la xboard pe care sa le faca pe tabla locala
        process_received_move(command);
    }
}

/* intoarce 0 daca totul e ok, -1 daca s-a dat comanda quit, 
-2 daca s-a dat comanda new */
int process_force() {
    std::string command;

    while (true) {
        getline(std::cin, command);

        if (command[0] == 't' && command[1] == 'i' && command[2] == 'm') {
            continue;
        }
        if (command[0] == 'o' && command[1] == 't' && command[2] == 'i') {
            continue;
        }
        if (isIgnoredCommand(command)) {
            continue;
        }
        if (command.compare("force") == 0) {
            continue;
        }
        if (command.compare("go") == 0) {
            return 0;
        }
        if (command.compare("quit") == 0) {
            return -1;
        }
        if (command.compare("new") == 0) {
            return -2;
        }

        if (command.compare("edit") == 0) {
            process_edit_position();
        } else if (command.compare("analyze") == 0) {
            process_analyze();
        } else if (command.compare("white") == 0) {
            if (process_white_or_black(WHITE) == -1) {
                return 0;
            }
        } else if (command.compare("black") == 0) {
            if (process_white_or_black(BLACK) == -1) {
                return 0;
            }
        } else {
            // incepe sa primeasca comenzi de la xboard pe care sa le faca pe tabla locala
            process_received_move(command);
        }
    }

    return 0;
}

// primesc o mutare de forma a2a3 in move si fac update la xboard-ul local
void process_received_move(std::string strMove) {
    chess_map& board = chess_map::get_instance();

    if (board.colour == BLACK) {
        flip_string_move(strMove);
    }

    move* mv = new move(strMove);
    board.execute_move(*mv);
    board.print_map();
}

int main() {
	signal(SIGINT, SIG_IGN); // nu merge fara
	std::cout << "\n"; // nu merge fara

    std::string line;

	while (true) {
        // se printeaza inainte sa se umple buffer-ul
        std::flush(std::cout);

        // se asteapta o comanda de la xboard
        std::getline(std::cin, line);

		if (line.size() == 0){
			return 0;
		}

		if (line[0] == '\n') {
			continue;
		}

        // separa cuvintele din linie
        std::istringstream words(line);

        // salvez primul cuvant
        std::string command1;
        words >> command1;

        if (command1.compare("undo") == 0) {
        }

        // verific ce comanda am primit si o execut
        if (isIgnoredCommand(command1)) {
            continue;
        } else if (command1.compare("new") == 0) {
            // create map and initialize
            // TODO MyMap.initialize();
            // TODO restart game
            // TODO sterge urmatoarele linii sunt doar pentru exemplu

            chess_map& board = chess_map::get_instance();

            board.init_default_map();
            board.print_map();

			continue;
		} else if (command1.compare("protover") == 0) {
            std::cout << "feature done=0\n";
            std::cout << "feature san=0\n";
            std::cout << "feature myname=\"BUM\"\n";
            std::cout << "feature done=1\n";

			continue;
		} else if (command1.compare("quit") == 0 || command1.compare("resign") == 0) {
			return 0;
		} else if (command1.compare("result") == 0) {
            words >> line;
            if (line.compare("*") == 0) {
                // s-a dat un fel de quit
                return 0;
            }
            continue;
        } else if (command1.compare("white") == 0) {
            // daca primesti white pentru prima data aparent esti black :))
            process_white_or_black(WHITE);
            continue;
        } else if (command1.compare("black") == 0) {
            // daca primesti black pentru prima data aparent esti white :))
            process_white_or_black(BLACK);
            continue;
        } else if (command1.compare("force") == 0) {
            int result = process_force();
            if (result == 0) {
                // am primit deja comanda go si trebuie sa fac o mutare
                result = choose_move(chess_map::get_instance().colour);
                if (result < 0) {
                    // daca nu am ce mutare sa fac dau resign la etapa 1
                    std::cout << "resign" << std::endl;
                    continue;
                }
            } else if (result == -1) {
                // s-a dat quit in force
                return 0;
            } else if (result == -2) {
                // s-a dat new in force
                chess_map& board = chess_map::get_instance();
                board.init_default_map();
            }
            continue;
        } else if (command1.compare("go") == 0) {
            // se reia jocul, xboard asteapta o mutare de la noi
            int result = choose_move(chess_map::get_instance().colour);
            if (result < 0) {
                // daca nu am ce mutare sa fac dau resign la etapa 1
                std::cout << "resign" << std::endl;
            }
            continue;
        } else if (command1.compare("otim") == 0) {
            continue;
        }else {
            // primesc o mutare de forma a2a3 in command si fac update la xboard-ul local
            process_received_move(command1);
            // fac si eu o mutare

            int result = choose_move(chess_map::get_instance().colour);
            if (result < 0) {
                // daca nu am ce mutare sa fac dau resign la etapa 1
                std::cout << "resign" << std::endl;
                continue;
            }
        }
	}
	return 0;
}
