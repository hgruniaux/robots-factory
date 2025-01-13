/* --- Generated the 13/1/2025 at 13:24 --- */
/* --- heptagon compiler, version 1.05.00 (compiled mon. sep. 23 14:27:43 CET 2024) --- */
/* --- Command line: /home/vincent/.opam/heptagon/bin/heptc -target c robot.ept --- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "robot_types.h"

Robot__st Robot__st_of_string(char* s) {
  if ((strcmp(s, "St_Upside_Down_Right")==0)) {
    return Robot__St_Upside_Down_Right;
  };
  if ((strcmp(s, "St_Upside_Down_Left")==0)) {
    return Robot__St_Upside_Down_Left;
  };
  if ((strcmp(s, "St_Upside_Down_Init")==0)) {
    return Robot__St_Upside_Down_Init;
  };
  if ((strcmp(s, "St_Standard")==0)) {
    return Robot__St_Standard;
  };
  if ((strcmp(s, "St_Jump")==0)) {
    return Robot__St_Jump;
  };
  if ((strcmp(s, "St_Falling")==0)) {
    return Robot__St_Falling;
  };
}

char* string_of_Robot__st(Robot__st x, char* buf) {
  switch (x) {
    case Robot__St_Upside_Down_Right:
      strcpy(buf, "St_Upside_Down_Right");
      break;
    case Robot__St_Upside_Down_Left:
      strcpy(buf, "St_Upside_Down_Left");
      break;
    case Robot__St_Upside_Down_Init:
      strcpy(buf, "St_Upside_Down_Init");
      break;
    case Robot__St_Standard:
      strcpy(buf, "St_Standard");
      break;
    case Robot__St_Jump:
      strcpy(buf, "St_Jump");
      break;
    case Robot__St_Falling:
      strcpy(buf, "St_Falling");
      break;
    default:
      break;
  };
  return buf;
}

