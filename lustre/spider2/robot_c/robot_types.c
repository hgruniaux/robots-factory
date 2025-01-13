/* --- Generated the 13/1/2025 at 16:11 --- */
/* --- heptagon compiler, version 1.05.00 (compiled mon. sep. 23 14:27:43 CET 2024) --- */
/* --- Command line: /home/vincent/.opam/heptagon/bin/heptc -target c robot.ept --- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "robot_types.h"

Robot__st_1 Robot__st_1_of_string(char* s) {
  if ((strcmp(s, "St_1_Upside_Down_Right")==0)) {
    return Robot__St_1_Upside_Down_Right;
  };
  if ((strcmp(s, "St_1_Upside_Down_Left")==0)) {
    return Robot__St_1_Upside_Down_Left;
  };
  if ((strcmp(s, "St_1_Upside_Down_Init")==0)) {
    return Robot__St_1_Upside_Down_Init;
  };
  if ((strcmp(s, "St_1_Standard")==0)) {
    return Robot__St_1_Standard;
  };
  if ((strcmp(s, "St_1_Jump")==0)) {
    return Robot__St_1_Jump;
  };
  if ((strcmp(s, "St_1_Falling")==0)) {
    return Robot__St_1_Falling;
  };
}

char* string_of_Robot__st_1(Robot__st_1 x, char* buf) {
  switch (x) {
    case Robot__St_1_Upside_Down_Right:
      strcpy(buf, "St_1_Upside_Down_Right");
      break;
    case Robot__St_1_Upside_Down_Left:
      strcpy(buf, "St_1_Upside_Down_Left");
      break;
    case Robot__St_1_Upside_Down_Init:
      strcpy(buf, "St_1_Upside_Down_Init");
      break;
    case Robot__St_1_Standard:
      strcpy(buf, "St_1_Standard");
      break;
    case Robot__St_1_Jump:
      strcpy(buf, "St_1_Jump");
      break;
    case Robot__St_1_Falling:
      strcpy(buf, "St_1_Falling");
      break;
    default:
      break;
  };
  return buf;
}

Robot__st Robot__st_of_string(char* s) {
  if ((strcmp(s, "St_Center")==0)) {
    return Robot__St_Center;
  };
  if ((strcmp(s, "St_AnOtherState")==0)) {
    return Robot__St_AnOtherState;
  };
}

char* string_of_Robot__st(Robot__st x, char* buf) {
  switch (x) {
    case Robot__St_Center:
      strcpy(buf, "St_Center");
      break;
    case Robot__St_AnOtherState:
      strcpy(buf, "St_AnOtherState");
      break;
    default:
      break;
  };
  return buf;
}

