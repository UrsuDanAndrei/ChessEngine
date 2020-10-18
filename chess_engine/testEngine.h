#ifndef __TEST_ENGINE_H__
#define __TEST_ENGINE_H__

int process_white_or_black(colours colour);
int process_force();
void process_received_move(std::string strMove);
void process_edit_position();
int execute_move();

#endif // __TEST_ENGINE_H__
