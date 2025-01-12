/* --- Generated the 12/1/2025 at 18:57 --- */
/* --- heptagon compiler, version 1.05.00 (compiled mon. sep. 23 14:27:43 CET 2024) --- */
/* --- Command line: /home/vincent/.opam/heptagon/bin/heptc -target c robot.ept --- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "robot.h"

void Robot__mux_step(int x, float y, float z, Robot__mux_out* _out) {
  if (x) {
    _out->r = y;
  } else {
    _out->r = z;
  };
}

void Robot__sgn_step(float x, Robot__sgn_out* _out) {
  
  float v_2;
  int v_1;
  int v;
  v_1 = (x<0.000000);
  if (v_1) {
    v_2 = -1.000000;
  } else {
    v_2 = 0.000000;
  };
  v = (x>0.000000);
  if (v) {
    _out->r = 1.000000;
  } else {
    _out->r = v_2;
  };;
}

void Robot__setMotor_step(float current, float goal, float maxi,
                          Robot__setMotor_out* _out) {
  Lib__fmod_out Lib__fmod_out_st;
  Lib__min_out Lib__min_out_st;
  Lib__abs_out Lib__abs_out_st;
  
  float v_8;
  float v_7;
  float v_6;
  float v_5;
  float v_4;
  int v_3;
  float v;
  float delta;
  v = (goal-current);
  Lib__fmod_step(v, 360.000000, &Lib__fmod_out_st);
  delta = Lib__fmod_out_st.x;
  Lib__abs_step(delta, &Lib__abs_out_st);
  v_6 = Lib__abs_out_st.x;
  Lib__min_step(v_6, maxi, &Lib__min_out_st);
  v_7 = Lib__min_out_st.x;
  v_8 = -(v_7);
  Lib__abs_step(delta, &Lib__abs_out_st);
  v_4 = Lib__abs_out_st.x;
  Lib__min_step(v_4, maxi, &Lib__min_out_st);
  v_5 = Lib__min_out_st.x;
  v_3 = (delta<180.000000);
  if (v_3) {
    _out->power = v_5;
  } else {
    _out->power = v_8;
  };;
}

void Robot__setMotorFast_step(float current, float goal, float maxi,
                              Robot__setMotorFast_out* _out) {
  Lib__fmod_out Lib__fmod_out_st;
  
  float v_10;
  int v_9;
  float v;
  float delta;
  v_10 = -(maxi);
  v = (goal-current);
  Lib__fmod_step(v, 360.000000, &Lib__fmod_out_st);
  delta = Lib__fmod_out_st.x;
  v_9 = (delta<180.000000);
  if (v_9) {
    _out->power = maxi;
  } else {
    _out->power = v_10;
  };;
}

void Robot__setMotorPID_reset(Robot__setMotorPID_mem* self) {
  self->v_13 = true;
  self->v = true;
}

void Robot__setMotorPID_step(float currentAngle, float goalAngle,
                             float maxSpeed, float kp, float ki, float kd,
                             Robot__setMotorPID_out* _out,
                             Robot__setMotorPID_mem* self) {
  
  float v_24;
  float v_23;
  int v_22;
  float v_21;
  int v_20;
  float v_19;
  float v_18;
  float v_17;
  float v_16;
  float v_15;
  float v_12;
  float error;
  float integral;
  float derivative;
  float lastError;
  float rawSpeed;
  v_23 = -(maxSpeed);
  v_21 = -(maxSpeed);
  error = (goalAngle-currentAngle);
  lastError = error;
  v_16 = (kp*error);
  v_15 = (error-self->v_14);
  if (self->v_13) {
    derivative = 0.000000;
  } else {
    derivative = v_15;
  };
  v_19 = (kd*derivative);
  v_12 = (self->v_11+error);
  if (self->v) {
    integral = 0.000000;
  } else {
    integral = v_12;
  };
  v_17 = (ki*integral);
  v_18 = (v_16+v_17);
  rawSpeed = (v_18+v_19);
  v_22 = (rawSpeed<v_21);
  if (v_22) {
    v_24 = v_23;
  } else {
    v_24 = rawSpeed;
  };
  v_20 = (rawSpeed>maxSpeed);
  if (v_20) {
    _out->motorSpeed = maxSpeed;
  } else {
    _out->motorSpeed = v_24;
  };
  self->v_14 = lastError;
  self->v_13 = false;
  self->v_11 = integral;
  self->v = false;;
}

void Robot__do_not_move_step(float limit, float speedA, float speedB,
                             float speedC, float speedD,
                             Robot__do_not_move_out* _out) {
  
  int v_29;
  int v_28;
  int v_27;
  int v_26;
  int v_25;
  int v;
  v_29 = (speedD<=limit);
  v_27 = (speedC<=limit);
  v_25 = (speedB<=limit);
  v = (speedA<=limit);
  v_26 = (v&&v_25);
  v_28 = (v_26&&v_27);
  _out->ok = (v_28&&v_29);;
}

void Robot__robot_reset(Robot__robot_mem* self) {
  Robot__setMotorPID_reset(&self->setMotorPID);
  Robot__setMotorPID_reset(&self->setMotorPID_1);
  Robot__setMotorPID_reset(&self->setMotorPID_2);
  Robot__setMotorPID_reset(&self->setMotorPID_3);
  self->pnr = false;
  self->ck = Robot__St_Falling;
}

void Robot__robot_step(float dt, float hipLangle, float hipRangle,
                       float kneeLangle, float kneeRangle, float arm1angle,
                       float arm2angle, float arm3angle, float arm4angle,
                       float gyroscope, float calfLdistance,
                       float calfRdistance, int footLcontact,
                       int footRcontact, int bodyContact,
                       Robot__robot_out* _out, Robot__robot_mem* self) {
  Robot__setMotorPID_out Robot__setMotorPID_out_st;
  Robot__setMotor_out Robot__setMotor_out_st;
  
  int r_4;
  int r_3;
  int r_2;
  int r_1;
  int nr_St_Standard;
  Robot__st ns_St_Standard;
  float arm4speed_St_Standard;
  float arm3speed_St_Standard;
  float arm2speed_St_Standard;
  float arm1speed_St_Standard;
  float kneeRspeed_St_Standard;
  float kneeLspeed_St_Standard;
  float hipRspeed_St_Standard;
  float hipLspeed_St_Standard;
  int nr_St_Upside_Down_Init;
  Robot__st ns_St_Upside_Down_Init;
  float arm4speed_St_Upside_Down_Init;
  float arm3speed_St_Upside_Down_Init;
  float arm2speed_St_Upside_Down_Init;
  float arm1speed_St_Upside_Down_Init;
  float kneeRspeed_St_Upside_Down_Init;
  float kneeLspeed_St_Upside_Down_Init;
  float hipRspeed_St_Upside_Down_Init;
  float hipLspeed_St_Upside_Down_Init;
  int nr_St_Falling;
  Robot__st ns_St_Falling;
  float arm4speed_St_Falling;
  float arm3speed_St_Falling;
  float arm2speed_St_Falling;
  float arm1speed_St_Falling;
  float kneeRspeed_St_Falling;
  float kneeLspeed_St_Falling;
  float hipRspeed_St_Falling;
  float hipLspeed_St_Falling;
  Robot__st ns;
  int r;
  int nr;
  r = self->pnr;
  switch (self->ck) {
    case Robot__St_Falling:
      Robot__setMotor_step(kneeRangle, -100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeRspeed_St_Falling = Robot__setMotor_out_st.power;
      Robot__setMotor_step(kneeLangle, 100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeLspeed_St_Falling = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipRangle, 40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipRspeed_St_Falling = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipLangle, -40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipLspeed_St_Falling = Robot__setMotor_out_st.power;
      if (false) {
        nr_St_Falling = true;
      } else {
        nr_St_Falling = false;
      };
      if (false) {
        ns_St_Falling = Robot__St_Standard;
      } else {
        ns_St_Falling = Robot__St_Falling;
      };
      r_1 = r;
      if (r_1) {
        Robot__setMotorPID_reset(&self->setMotorPID);
      };
      Robot__setMotorPID_step(arm1angle, -40.000000, 20.000000, 0.500000,
                              0.000000, 0.200000, &Robot__setMotorPID_out_st,
                              &self->setMotorPID);
      arm1speed_St_Falling = Robot__setMotorPID_out_st.motorSpeed;
      r_2 = r;
      if (r_2) {
        Robot__setMotorPID_reset(&self->setMotorPID_1);
      };
      Robot__setMotorPID_step(arm2angle, 30.000000, 20.000000, 0.700000,
                              0.000000, 0.300000, &Robot__setMotorPID_out_st,
                              &self->setMotorPID_1);
      arm2speed_St_Falling = Robot__setMotorPID_out_st.motorSpeed;
      r_3 = r;
      if (r_3) {
        Robot__setMotorPID_reset(&self->setMotorPID_2);
      };
      Robot__setMotorPID_step(arm3angle, 80.000000, 20.000000, 0.900000,
                              0.000000, 0.400000, &Robot__setMotorPID_out_st,
                              &self->setMotorPID_2);
      arm3speed_St_Falling = Robot__setMotorPID_out_st.motorSpeed;
      r_4 = r;
      if (r_4) {
        Robot__setMotorPID_reset(&self->setMotorPID_3);
      };
      Robot__setMotorPID_step(arm4angle, 60.000000, 20.000000, 1.000000,
                              0.000000, 0.500000, &Robot__setMotorPID_out_st,
                              &self->setMotorPID_3);
      arm4speed_St_Falling = Robot__setMotorPID_out_st.motorSpeed;
      _out->hipLspeed = hipLspeed_St_Falling;
      _out->hipRspeed = hipRspeed_St_Falling;
      _out->kneeLspeed = kneeLspeed_St_Falling;
      _out->kneeRspeed = kneeRspeed_St_Falling;
      _out->arm1speed = arm1speed_St_Falling;
      _out->arm2speed = arm2speed_St_Falling;
      _out->arm3speed = arm3speed_St_Falling;
      _out->arm4speed = arm4speed_St_Falling;
      ns = ns_St_Falling;
      nr = nr_St_Falling;
      break;
    case Robot__St_Upside_Down_Init:
      Robot__setMotor_step(arm4angle, 0.000000, 60.000000,
                           &Robot__setMotor_out_st);
      arm4speed_St_Upside_Down_Init = Robot__setMotor_out_st.power;
      Robot__setMotor_step(arm3angle, 0.000000, 60.000000,
                           &Robot__setMotor_out_st);
      arm3speed_St_Upside_Down_Init = Robot__setMotor_out_st.power;
      Robot__setMotor_step(arm2angle, 0.000000, 60.000000,
                           &Robot__setMotor_out_st);
      arm2speed_St_Upside_Down_Init = Robot__setMotor_out_st.power;
      Robot__setMotor_step(arm1angle, 0.000000, 60.000000,
                           &Robot__setMotor_out_st);
      arm1speed_St_Upside_Down_Init = Robot__setMotor_out_st.power;
      Robot__setMotor_step(kneeRangle, -100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeRspeed_St_Upside_Down_Init = Robot__setMotor_out_st.power;
      Robot__setMotor_step(kneeLangle, 100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeLspeed_St_Upside_Down_Init = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipRangle, 40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipRspeed_St_Upside_Down_Init = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipLangle, -40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipLspeed_St_Upside_Down_Init = Robot__setMotor_out_st.power;
      nr_St_Upside_Down_Init = false;
      ns_St_Upside_Down_Init = Robot__St_Upside_Down_Init;
      _out->hipLspeed = hipLspeed_St_Upside_Down_Init;
      _out->hipRspeed = hipRspeed_St_Upside_Down_Init;
      _out->kneeLspeed = kneeLspeed_St_Upside_Down_Init;
      _out->kneeRspeed = kneeRspeed_St_Upside_Down_Init;
      _out->arm1speed = arm1speed_St_Upside_Down_Init;
      _out->arm2speed = arm2speed_St_Upside_Down_Init;
      _out->arm3speed = arm3speed_St_Upside_Down_Init;
      _out->arm4speed = arm4speed_St_Upside_Down_Init;
      ns = ns_St_Upside_Down_Init;
      nr = nr_St_Upside_Down_Init;
      break;
    case Robot__St_Standard:
      Robot__setMotor_step(arm4angle, 0.000000, 60.000000,
                           &Robot__setMotor_out_st);
      arm4speed_St_Standard = Robot__setMotor_out_st.power;
      Robot__setMotor_step(arm3angle, 0.000000, 60.000000,
                           &Robot__setMotor_out_st);
      arm3speed_St_Standard = Robot__setMotor_out_st.power;
      Robot__setMotor_step(arm2angle, 0.000000, 60.000000,
                           &Robot__setMotor_out_st);
      arm2speed_St_Standard = Robot__setMotor_out_st.power;
      Robot__setMotor_step(arm1angle, 0.000000, 60.000000,
                           &Robot__setMotor_out_st);
      arm1speed_St_Standard = Robot__setMotor_out_st.power;
      Robot__setMotor_step(kneeRangle, -100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeRspeed_St_Standard = Robot__setMotor_out_st.power;
      Robot__setMotor_step(kneeLangle, 100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeLspeed_St_Standard = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipRangle, 40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipRspeed_St_Standard = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipLangle, -40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipLspeed_St_Standard = Robot__setMotor_out_st.power;
      nr_St_Standard = false;
      ns_St_Standard = Robot__St_Standard;
      _out->hipLspeed = hipLspeed_St_Standard;
      _out->hipRspeed = hipRspeed_St_Standard;
      _out->kneeLspeed = kneeLspeed_St_Standard;
      _out->kneeRspeed = kneeRspeed_St_Standard;
      _out->arm1speed = arm1speed_St_Standard;
      _out->arm2speed = arm2speed_St_Standard;
      _out->arm3speed = arm3speed_St_Standard;
      _out->arm4speed = arm4speed_St_Standard;
      ns = ns_St_Standard;
      nr = nr_St_Standard;
      break;
    default:
      break;
  };
  self->pnr = nr;
  self->ck = ns;;
}

