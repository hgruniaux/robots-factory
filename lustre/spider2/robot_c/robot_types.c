/* --- Generated the 7/1/2025 at 13:1 --- */
/* --- heptagon compiler, version 1.05.00 (compiled mon. sep. 23 14:27:43 CET 2024) --- */
/* --- Command line: /home/vincent/.opam/heptagon/bin/heptc -target c robot.ept --- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "robot_types.h"

Robot__st Robot__st_of_string(char* s) {
  if ((strcmp(s, "St_On_the_ground")==0)) {
    return Robot__St_On_the_ground;
  };
  if ((strcmp(s, "St_Jump")==0)) {
    return Robot__St_Jump;
  };
  if ((strcmp(s, "St_In_the_sky")==0)) {
    return Robot__St_In_the_sky;
  };
}

char* string_of_Robot__st(Robot__st x, char* buf) {
  switch (x) {
    case Robot__St_On_the_ground:
      strcpy(buf, "St_On_the_ground");
      break;
    case Robot__St_Jump:
      strcpy(buf, "St_Jump");
      break;
    case Robot__St_In_the_sky:
      strcpy(buf, "St_In_the_sky");
      break;
    default:
      break;
  };
  return buf;
}
