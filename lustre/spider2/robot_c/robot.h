/* --- Generated the 7/1/2025 at 13:1 --- */
/* --- heptagon compiler, version 1.05.00 (compiled mon. sep. 23 14:27:43 CET 2024) --- */
/* --- Command line: /home/vincent/.opam/heptagon/bin/heptc -target c robot.ept --- */

#ifndef ROBOT_H
#define ROBOT_H

#include "robot_types.h"
typedef struct Robot__abs_out {
  float r;
} Robot__abs_out;

void Robot__abs_step(float x, Robot__abs_out* _out);

typedef struct Robot__robot_mem {
  Robot__st ck;
  int pnr;
} Robot__robot_mem;

typedef struct Robot__robot_out {
  float hipLspeed;
  float hipRspeed;
  float kneeLspeed;
  float kneeRspeed;
} Robot__robot_out;

void Robot__robot_reset(Robot__robot_mem* self);

void Robot__robot_step(float dt, float hipLangle, float hipRangle,
                       float kneeLangle, float kneeRangle, float gyroscope,
                       float calfLdistance, float calfRdistance,
                       Robot__robot_out* _out, Robot__robot_mem* self);

#endif // ROBOT_H
