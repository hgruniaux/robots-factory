/* --- Generated the 8/1/2025 at 12:26 --- */
/* --- heptagon compiler, version 1.05.00 (compiled mon. sep. 23 14:27:43 CET 2024) --- */
/* --- Command line: /home/vincent/.opam/heptagon/bin/heptc -target c robot.ept --- */

#ifndef ROBOT_H
#define ROBOT_H

#include "robot_types.h"
#include "lib.h"
typedef struct Robot__mux_out {
  float r;
} Robot__mux_out;

void Robot__mux_step(int x, float y, float z, Robot__mux_out* _out);

typedef struct Robot__setMotor_out {
  float power;
} Robot__setMotor_out;

void Robot__setMotor_step(float current, float goal, float maxi,
                          Robot__setMotor_out* _out);

typedef struct Robot__setMotorFast_out {
  float power;
} Robot__setMotorFast_out;

void Robot__setMotorFast_step(float current, float goal, float maxi,
                              Robot__setMotorFast_out* _out);

typedef struct Robot__step_out {
  float hipLspeed;
  float hipRspeed;
  float kneeLspeed;
  float kneeRspeed;
} Robot__step_out;

void Robot__step_step(float hipLangle, float hipRangle, float kneeLangle,
                      float kneeRangle, float gyroscope, int goLeft,
                      Robot__step_out* _out);

typedef struct Robot__robot_mem {
  float v_30;
  Robot__st ck;
  int pnr;
} Robot__robot_mem;

typedef struct Robot__robot_out {
  float hipLspeed;
  float hipRspeed;
  float kneeLspeed;
  float kneeRspeed;
  float arm1speed;
  float arm2speed;
  float arm3speed;
  float arm4speed;
} Robot__robot_out;

void Robot__robot_reset(Robot__robot_mem* self);

void Robot__robot_step(float dt, float hipLangle, float hipRangle,
                       float kneeLangle, float kneeRangle, float arm1angle,
                       float arm2angle, float arm3angle, float arm4angle,
                       float gyroscope, float calfLdistance,
                       float calfRdistance, int footLcontact,
                       int footRcontact, int bodyContact,
                       Robot__robot_out* _out, Robot__robot_mem* self);

#endif // ROBOT_H
