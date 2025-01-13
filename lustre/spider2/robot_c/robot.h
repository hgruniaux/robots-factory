/* --- Generated the 13/1/2025 at 11:26 --- */
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

typedef struct Robot__sgn_out {
  float r;
} Robot__sgn_out;

void Robot__sgn_step(float x, Robot__sgn_out* _out);

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

typedef struct Robot__setMotorPID_mem {
  float v_14;
  int v_13;
  float v_11;
  int v;
} Robot__setMotorPID_mem;

typedef struct Robot__setMotorPID_out {
  float motorSpeed;
} Robot__setMotorPID_out;

void Robot__setMotorPID_reset(Robot__setMotorPID_mem* self);

void Robot__setMotorPID_step(float currentAngle, float goalAngle,
                             float maxSpeed, float kp, float ki, float kd,
                             Robot__setMotorPID_out* _out,
                             Robot__setMotorPID_mem* self);

typedef struct Robot__doNotMove_out {
  int ok;
} Robot__doNotMove_out;

void Robot__doNotMove_step(float limit, float speedA, float speedB,
                           float speedC, float speedD,
                           Robot__doNotMove_out* _out);

typedef struct Robot__setMotorArm_mem {
  Robot__setMotorPID_mem setMotorPID;
  Robot__setMotorPID_mem setMotorPID_1;
  Robot__setMotorPID_mem setMotorPID_2;
  Robot__setMotorPID_mem setMotorPID_3;
} Robot__setMotorArm_mem;

typedef struct Robot__setMotorArm_out {
  float arm1speed;
  float arm2speed;
  float arm3speed;
  float arm4speed;
} Robot__setMotorArm_out;

void Robot__setMotorArm_reset(Robot__setMotorArm_mem* self);

void Robot__setMotorArm_step(float arm1angle, float arm2angle,
                             float arm3angle, float arm4angle,
                             float arm1goal, float arm2goal, float arm3goal,
                             float arm4goal, Robot__setMotorArm_out* _out,
                             Robot__setMotorArm_mem* self);

typedef struct Robot__setMotorLegs_out {
  float hipLspeed;
  float hipRspeed;
  float kneeLspeed;
  float kneeRspeed;
} Robot__setMotorLegs_out;

void Robot__setMotorLegs_step(float hipLangle, float hipRangle,
                              float kneeLangle, float kneeRangle,
                              float hipLgoal, float hipRgoal,
                              float kneeLgoal, float kneeRgoal,
                              Robot__setMotorLegs_out* _out);

typedef struct Robot__robot_mem {
  Robot__st ck;
  float v_30;
  int v;
  int pnr;
  Robot__setMotorArm_mem setMotorArm;
  Robot__setMotorArm_mem setMotorArm_1;
  Robot__setMotorArm_mem setMotorArm_2;
  Robot__setMotorArm_mem setMotorArm_3;
  Robot__setMotorArm_mem setMotorArm_4;
  Robot__setMotorArm_mem setMotorArm_5;
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
                       float gyroscopeTot, float calfLdistance,
                       float calfRdistance, int footLcontact,
                       int footRcontact, int bodyContact, float main_x,
                       float main_y, float secondary_x, float secondary_y,
                       int action_a, Robot__robot_out* _out,
                       Robot__robot_mem* self);

#endif // ROBOT_H
