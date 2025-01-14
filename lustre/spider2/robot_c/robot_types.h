/* --- Generated the 14/1/2025 at 13:45 --- */
/* --- heptagon compiler, version 1.05.00 (compiled mon. sep. 23 14:27:43 CET 2024) --- */
/* --- Command line: /home/vincent/.opam/heptagon/bin/heptc -target c robot.ept --- */

#ifndef ROBOT_TYPES_H
#define ROBOT_TYPES_H

#include "stdbool.h"
#include "assert.h"
#include "pervasives.h"
#include "lib_types.h"
typedef enum {
  Robot__St_2_Upside_Down_Right,
  Robot__St_2_Upside_Down_Left,
  Robot__St_2_Upside_Down_Init,
  Robot__St_2_Standard,
  Robot__St_2_Jump,
  Robot__St_2_Falling
} Robot__st_2;

Robot__st_2 Robot__st_2_of_string(char* s);

char* string_of_Robot__st_2(Robot__st_2 x, char* buf);

typedef enum {
  Robot__St_1_Center
} Robot__st_1;

Robot__st_1 Robot__st_1_of_string(char* s);

char* string_of_Robot__st_1(Robot__st_1 x, char* buf);

typedef enum {
  Robot__St_Normal,
  Robot__St_Move
} Robot__st;

Robot__st Robot__st_of_string(char* s);

char* string_of_Robot__st(Robot__st x, char* buf);

#endif // ROBOT_TYPES_H
