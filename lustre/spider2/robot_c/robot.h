/* --- Generated the 14/1/2025 at 17:9 --- */
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

typedef struct Robot__mod2_out {
  float r;
} Robot__mod2_out;

void Robot__mod2_step(float x, float y, Robot__mod2_out* _out);

typedef struct Robot__setMotor_out {
  float power;
} Robot__setMotor_out;

void Robot__setMotor_step(float current2, float goal2, float maxi,
                          Robot__setMotor_out* _out);

typedef struct Robot__setMotorFast_out {
  float power;
} Robot__setMotorFast_out;

void Robot__setMotorFast_step(float current, float goal, float maxi,
                              Robot__setMotorFast_out* _out);

typedef struct Robot__setMotorPID_mem {
  float v_18;
  int v_17;
  float v_15;
  int v_14;
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

typedef struct Robot__funHip_out {
  float y;
} Robot__funHip_out;

void Robot__funHip_step(float x, Robot__funHip_out* _out);

typedef struct Robot__funKnee_out {
  float y;
} Robot__funKnee_out;

void Robot__funKnee_step(float x, Robot__funKnee_out* _out);

typedef struct Robot__legSpeedsWithoutContact_out {
  float hipSpeed;
  float kneeSpeed;
} Robot__legSpeedsWithoutContact_out;

void Robot__legSpeedsWithoutContact_step(float hipAngle, float kneeAngle,
                                         float hipGoal, float kneeGoal,
                                         int footContact, float calfDistance,
                                         Robot__legSpeedsWithoutContact_out* _out);

typedef struct Robot__legSpeedsWithContact_out {
  float hipSpeed;
  float kneeSpeed;
} Robot__legSpeedsWithContact_out;

void Robot__legSpeedsWithContact_step(float hipAngle, float kneeAngle,
                                      float hipGoal, float kneeGoal,
                                      int footContact, float calfDistance,
                                      Robot__legSpeedsWithContact_out* _out);

typedef struct Robot__robot_mem {
  int v_86;
  int v_90;
  Robot__st_3 ck;
  Robot__st_1 v_168;
  Robot__st v_171;
  int v_172;
  int v_169;
  float v_192;
  float v_206;
  Robot__st_2 ck_1;
  float v_78;
  int v_77;
  float v_72;
  int v_71;
  float v_66;
  int v_65;
  float v_60;
  int v_59;
  float v_54;
  int v;
  int pnr_3;
  int pnr_2;
  Robot__setMotorArm_mem setMotorArm;
  Robot__setMotorArm_mem setMotorArm_1;
  Robot__setMotorArm_mem setMotorArm_2;
  Robot__setMotorArm_mem setMotorArm_3;
  Robot__setMotorArm_mem setMotorArm_4;
  Robot__setMotorArm_mem setMotorArm_5;
  Robot__setMotorArm_mem setMotorArm_6;
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
  float fingerL1speed;
  float fingerL2speed;
  float fingerR1speed;
  float fingerR2speed;
  float arm_target_x;
  float arm_target_y;
  float arm_target_angle;
} Robot__robot_out;

void Robot__robot_reset(Robot__robot_mem* self);

void Robot__robot_step(float dt, float hipLangle, float hipRangle,
                       float kneeLangle, float kneeRangle, float arm1angle,
                       float arm2angle, float arm3angle, float arm4angle,
                       float fingerL1angle, float fingerL2angle,
                       float fingerR1angle, float fingerR2angle,
                       float gyroscope, float calfLdistance,
                       float calfRdistance, float bodyDistance,
                       int footLcontact, int footRcontact, int bodyContact,
                       float main_x, float main_y, float secondary_x,
                       float secondary_y, int action_a, int action_x,
                       int action_b, int action_lb, int action_rb,
                       Robot__robot_out* _out, Robot__robot_mem* self);

#endif // ROBOT_H
