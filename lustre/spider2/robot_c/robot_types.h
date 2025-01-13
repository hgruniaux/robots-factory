/* --- Generated the 13/1/2025 at 10:47 --- */
/* --- heptagon compiler, version 1.05.00 (compiled mon. sep. 23 14:27:43 CET 2024) --- */
/* --- Command line: /home/vincent/.opam/heptagon/bin/heptc -target c robot.ept --- */

#ifndef ROBOT_TYPES_H
#define ROBOT_TYPES_H

#include "stdbool.h"
#include "assert.h"
#include "pervasives.h"
#include "lib_types.h"
typedef enum {
  Robot__St_Upside_Down_Right,
  Robot__St_Upside_Down_Left,
  Robot__St_Upside_Down_Init,
  Robot__St_Standard,
  Robot__St_Falling
} Robot__st;

Robot__st Robot__st_of_string(char* s);

char* string_of_Robot__st(Robot__st x, char* buf);

#endif // ROBOT_TYPES_H
