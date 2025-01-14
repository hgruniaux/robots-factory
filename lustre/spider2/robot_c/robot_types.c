/* --- Generated the 14/1/2025 at 17:9 --- */
/* --- heptagon compiler, version 1.05.00 (compiled mon. sep. 23 14:27:43 CET 2024) --- */
/* --- Command line: /home/vincent/.opam/heptagon/bin/heptc -target c robot.ept --- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "robot_types.h"

Robot__st_3 Robot__st_3_of_string(char* s) {
  if ((strcmp(s, "St_3_Hand_Open")==0)) {
    return Robot__St_3_Hand_Open;
  };
  if ((strcmp(s, "St_3_Hand_Close")==0)) {
    return Robot__St_3_Hand_Close;
  };
}

char* string_of_Robot__st_3(Robot__st_3 x, char* buf) {
  switch (x) {
    case Robot__St_3_Hand_Open:
      strcpy(buf, "St_3_Hand_Open");
      break;
    case Robot__St_3_Hand_Close:
      strcpy(buf, "St_3_Hand_Close");
      break;
    default:
      break;
  };
  return buf;
}

Robot__st_2 Robot__st_2_of_string(char* s) {
  if ((strcmp(s, "St_2_Upside_Down_Right")==0)) {
    return Robot__St_2_Upside_Down_Right;
  };
  if ((strcmp(s, "St_2_Upside_Down_Left")==0)) {
    return Robot__St_2_Upside_Down_Left;
  };
  if ((strcmp(s, "St_2_Upside_Down_Init")==0)) {
    return Robot__St_2_Upside_Down_Init;
  };
  if ((strcmp(s, "St_2_Standard")==0)) {
    return Robot__St_2_Standard;
  };
  if ((strcmp(s, "St_2_Jump")==0)) {
    return Robot__St_2_Jump;
  };
  if ((strcmp(s, "St_2_Falling")==0)) {
    return Robot__St_2_Falling;
  };
}

char* string_of_Robot__st_2(Robot__st_2 x, char* buf) {
  switch (x) {
    case Robot__St_2_Upside_Down_Right:
      strcpy(buf, "St_2_Upside_Down_Right");
      break;
    case Robot__St_2_Upside_Down_Left:
      strcpy(buf, "St_2_Upside_Down_Left");
      break;
    case Robot__St_2_Upside_Down_Init:
      strcpy(buf, "St_2_Upside_Down_Init");
      break;
    case Robot__St_2_Standard:
      strcpy(buf, "St_2_Standard");
      break;
    case Robot__St_2_Jump:
      strcpy(buf, "St_2_Jump");
      break;
    case Robot__St_2_Falling:
      strcpy(buf, "St_2_Falling");
      break;
    default:
      break;
  };
  return buf;
}

Robot__st_1 Robot__st_1_of_string(char* s) {
  if ((strcmp(s, "St_1_Go_Off_Center_Up")==0)) {
    return Robot__St_1_Go_Off_Center_Up;
  };
  if ((strcmp(s, "St_1_Go_Off_Center_Down")==0)) {
    return Robot__St_1_Go_Off_Center_Down;
  };
  if ((strcmp(s, "St_1_Go_Down")==0)) {
    return Robot__St_1_Go_Down;
  };
  if ((strcmp(s, "St_1_Center")==0)) {
    return Robot__St_1_Center;
  };
}

char* string_of_Robot__st_1(Robot__st_1 x, char* buf) {
  switch (x) {
    case Robot__St_1_Go_Off_Center_Up:
      strcpy(buf, "St_1_Go_Off_Center_Up");
      break;
    case Robot__St_1_Go_Off_Center_Down:
      strcpy(buf, "St_1_Go_Off_Center_Down");
      break;
    case Robot__St_1_Go_Down:
      strcpy(buf, "St_1_Go_Down");
      break;
    case Robot__St_1_Center:
      strcpy(buf, "St_1_Center");
      break;
    default:
      break;
  };
  return buf;
}

Robot__st Robot__st_of_string(char* s) {
  if ((strcmp(s, "St_Normal")==0)) {
    return Robot__St_Normal;
  };
  if ((strcmp(s, "St_Move")==0)) {
    return Robot__St_Move;
  };
}

char* string_of_Robot__st(Robot__st x, char* buf) {
  switch (x) {
    case Robot__St_Normal:
      strcpy(buf, "St_Normal");
      break;
    case Robot__St_Move:
      strcpy(buf, "St_Move");
      break;
    default:
      break;
  };
  return buf;
}

