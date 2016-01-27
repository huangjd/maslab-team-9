#ifndef __HAL_H__
#define __HAL_H__

#include <string>

void move_forward_to_red_block();
void move_forward_to_green_block();
void move_forward_to_block_stack();
void turn_right();
void turn_left();
void back_up_rotate();


void move_to(double delta_x, double delta_y);
void move_forward(double x);
void turn(double deg);

#define LEFT 0
#define RIGHT 1

void pickup(bool side);
void release(bool side);
void releaseToPlatform(bool side);

void halt();

void echo(const std::string &in, std::string &out);


#endif // __HAL_H__
