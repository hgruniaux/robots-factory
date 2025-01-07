/* --- Generated the 7/1/2025 at 17:25 --- */
/* --- heptagon compiler, version 1.05.00 (compiled mon. sep. 23 14:27:43 CET 2024) --- */
/* --- Command line: /home/vincent/.opam/heptagon/bin/heptc -target c robot.ept --- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "robot.h"

void Robot__abs_step(float x, Robot__abs_out* _out) {
  
  float v_1;
  int v;
  v_1 = -(x);
  v = (x>=0.000000);
  if (v) {
    _out->r = x;
  } else {
    _out->r = v_1;
  };;
}

void Robot__min_step(float x, float y, Robot__min_out* _out) {
  
  int v;
  v = (x<=y);
  if (v) {
    _out->r = x;
  } else {
    _out->r = y;
  };;
}

void Robot__modulo_step(float x, float y, Robot__modulo_out* _out) {
  
  float v_10;
  float v_9;
  int v_8;
  float v_7;
  int v_6;
  float v_5;
  float v_4;
  int v_3;
  float v_2;
  int v;
  float r1;
  v_4 = (x-y);
  v_3 = (x>=y);
  if (v_3) {
    v_5 = v_4;
  } else {
    v_5 = x;
  };
  v_2 = (x+y);
  v = (x<0.000000);
  if (v) {
    r1 = v_2;
  } else {
    r1 = v_5;
  };
  v_9 = (r1-y);
  v_8 = (r1>=y);
  if (v_8) {
    v_10 = v_9;
  } else {
    v_10 = r1;
  };
  v_7 = (r1+y);
  v_6 = (r1<0.000000);
  if (v_6) {
    _out->r = v_7;
  } else {
    _out->r = v_10;
  };;
}

void Robot__setMotor_step(float current, float goal, float maxi,
                          Robot__setMotor_out* _out) {
  Robot__abs_out Robot__abs_out_st;
  Robot__min_out Robot__min_out_st;
  Robot__modulo_out Robot__modulo_out_st;
  
  float v_16;
  float v_15;
  float v_14;
  float v_13;
  float v_12;
  int v_11;
  float v;
  float delta;
  v = (goal-current);
  Robot__modulo_step(v, 360.000000, &Robot__modulo_out_st);
  delta = Robot__modulo_out_st.r;
  Robot__abs_step(delta, &Robot__abs_out_st);
  v_14 = Robot__abs_out_st.r;
  Robot__min_step(v_14, maxi, &Robot__min_out_st);
  v_15 = Robot__min_out_st.r;
  v_16 = -(v_15);
  Robot__abs_step(delta, &Robot__abs_out_st);
  v_12 = Robot__abs_out_st.r;
  Robot__min_step(v_12, maxi, &Robot__min_out_st);
  v_13 = Robot__min_out_st.r;
  v_11 = (delta<180.000000);
  if (v_11) {
    _out->power = v_13;
  } else {
    _out->power = v_16;
  };;
}

void Robot__setMotorFast_step(float current, float goal, float maxi,
                              Robot__setMotorFast_out* _out) {
  Robot__modulo_out Robot__modulo_out_st;
  
  float v_18;
  int v_17;
  float v;
  float delta;
  v_18 = -(maxi);
  v = (goal-current);
  Robot__modulo_step(v, 360.000000, &Robot__modulo_out_st);
  delta = Robot__modulo_out_st.r;
  v_17 = (delta<180.000000);
  if (v_17) {
    _out->power = maxi;
  } else {
    _out->power = v_18;
  };;
}

void Robot__robot_reset(Robot__robot_mem* self) {
  self->pnr = false;
  self->ck = Robot__St_In_the_sky;
}

void Robot__robot_step(float dt, float hipLangle, float hipRangle,
                       float kneeLangle, float kneeRangle, float gyroscope,
                       float calfLdistance, float calfRdistance,
                       int footLcontact, int footRcontact,
                       Robot__robot_out* _out, Robot__robot_mem* self) {
  Robot__setMotorFast_out Robot__setMotorFast_out_st;
  Robot__setMotor_out Robot__setMotor_out_st;
  
  float v_24;
  float v_23;
  float v_22;
  float v_21;
  int v_20;
  int v_19;
  int v;
  int v_31;
  int v_30;
  int v_29;
  int v_28;
  int v_27;
  int v_26;
  int v_25;
  int v_34;
  int v_33;
  int v_32;
  int nr_St_Jump;
  Robot__st ns_St_Jump;
  float kneeRspeed_St_Jump;
  float kneeLspeed_St_Jump;
  float hipRspeed_St_Jump;
  float hipLspeed_St_Jump;
  int nr_St_On_the_ground;
  Robot__st ns_St_On_the_ground;
  float kneeRspeed_St_On_the_ground;
  float kneeLspeed_St_On_the_ground;
  float hipRspeed_St_On_the_ground;
  float hipLspeed_St_On_the_ground;
  int nr_St_In_the_sky;
  Robot__st ns_St_In_the_sky;
  float kneeRspeed_St_In_the_sky;
  float kneeLspeed_St_In_the_sky;
  float hipRspeed_St_In_the_sky;
  float hipLspeed_St_In_the_sky;
  Robot__st ns;
  int r;
  int nr;
  r = self->pnr;
  switch (self->ck) {
    case Robot__St_In_the_sky:
      Robot__setMotorFast_step(kneeRangle, -80.000000, 100.000000,
                               &Robot__setMotorFast_out_st);
      kneeRspeed_St_In_the_sky = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(kneeLangle, 80.000000, 100.000000,
                               &Robot__setMotorFast_out_st);
      kneeLspeed_St_In_the_sky = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipRangle, 30.000000, 100.000000,
                               &Robot__setMotorFast_out_st);
      hipRspeed_St_In_the_sky = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipLangle, -30.000000, 100.000000,
                               &Robot__setMotorFast_out_st);
      hipLspeed_St_In_the_sky = Robot__setMotorFast_out_st.power;
      v_33 = (calfRdistance<=0.040000);
      v_32 = (calfLdistance<=0.040000);
      v_34 = (v_32&&v_33);
      if (v_34) {
        nr_St_In_the_sky = true;
        ns_St_In_the_sky = Robot__St_On_the_ground;
      } else {
        nr_St_In_the_sky = false;
        ns_St_In_the_sky = Robot__St_In_the_sky;
      };
      _out->hipLspeed = hipLspeed_St_In_the_sky;
      _out->hipRspeed = hipRspeed_St_In_the_sky;
      _out->kneeLspeed = kneeLspeed_St_In_the_sky;
      _out->kneeRspeed = kneeRspeed_St_In_the_sky;
      ns = ns_St_In_the_sky;
      nr = nr_St_In_the_sky;
      break;
    case Robot__St_On_the_ground:
      Robot__setMotor_step(kneeRangle, -100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeRspeed_St_On_the_ground = Robot__setMotor_out_st.power;
      Robot__setMotor_step(kneeLangle, 100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeLspeed_St_On_the_ground = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipRangle, 40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipRspeed_St_On_the_ground = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipLangle, -40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipLspeed_St_On_the_ground = Robot__setMotor_out_st.power;
      _out->hipLspeed = hipLspeed_St_On_the_ground;
      v_25 = (_out->hipLspeed<=1.000000);
      _out->hipRspeed = hipRspeed_St_On_the_ground;
      v_26 = (_out->hipRspeed<=1.000000);
      v_27 = (v_25&&v_26);
      _out->kneeLspeed = kneeLspeed_St_On_the_ground;
      v_28 = (_out->kneeLspeed<=1.000000);
      v_29 = (v_27&&v_28);
      _out->kneeRspeed = kneeRspeed_St_On_the_ground;
      v_30 = (_out->kneeRspeed<=1.000000);
      v_31 = (v_29&&v_30);
      if (v_31) {
        nr_St_On_the_ground = true;
        ns_St_On_the_ground = Robot__St_Jump;
      } else {
        nr_St_On_the_ground = false;
        ns_St_On_the_ground = Robot__St_On_the_ground;
      };
      ns = ns_St_On_the_ground;
      nr = nr_St_On_the_ground;
      break;
    case Robot__St_Jump:
      v_23 = (kneeRangle+hipRangle);
      v_24 = (v_23+gyroscope);
      Robot__setMotorFast_step(v_24, -90.000000, 1000.000000,
                               &Robot__setMotorFast_out_st);
      kneeRspeed_St_Jump = Robot__setMotorFast_out_st.power;
      v_21 = (kneeLangle+hipLangle);
      v_22 = (v_21+gyroscope);
      Robot__setMotorFast_step(v_22, 90.000000, 1000.000000,
                               &Robot__setMotorFast_out_st);
      kneeLspeed_St_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipRangle, -90.000000, 1000.000000,
                               &Robot__setMotorFast_out_st);
      hipRspeed_St_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipLangle, 90.000000, 1000.000000,
                               &Robot__setMotorFast_out_st);
      hipLspeed_St_Jump = Robot__setMotorFast_out_st.power;
      v_19 = (calfRdistance>=0.040000);
      v = (calfLdistance>=0.040000);
      v_20 = (v&&v_19);
      if (v_20) {
        nr_St_Jump = true;
        ns_St_Jump = Robot__St_In_the_sky;
      } else {
        nr_St_Jump = false;
        ns_St_Jump = Robot__St_Jump;
      };
      _out->hipLspeed = hipLspeed_St_Jump;
      _out->hipRspeed = hipRspeed_St_Jump;
      _out->kneeLspeed = kneeLspeed_St_Jump;
      _out->kneeRspeed = kneeRspeed_St_Jump;
      ns = ns_St_Jump;
      nr = nr_St_Jump;
      break;
    default:
      break;
  };
  self->pnr = nr;
  self->ck = ns;;
}

