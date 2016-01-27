#ifndef __HAL_H__
#define __HAL_H__

#include <string>

void move_forward_to_red_block();
void move_forward_to_green_block();
void move_forward_to_block_stack();
void turn_right();
void turn_left();
void move_forward();
void back_up_rotate();

void echo(const std::string &s);

#endif // __HAL_H__
