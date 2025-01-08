/* --- Generated the 8/1/2025 at 11:17 --- */
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

void Robot__setMotor_step(float current, float goal, float maxi,
                          Robot__setMotor_out* _out) {
  Lib__fmod_out Lib__fmod_out_st;
  Lib__min_out Lib__min_out_st;
  Lib__abs_out Lib__abs_out_st;
  
  float v_6;
  float v_5;
  float v_4;
  float v_3;
  float v_2;
  int v_1;
  float v;
  float delta;
  v = (goal-current);
  Lib__fmod_step(v, 360.000000, &Lib__fmod_out_st);
  delta = Lib__fmod_out_st.x;
  Lib__abs_step(delta, &Lib__abs_out_st);
  v_4 = Lib__abs_out_st.x;
  Lib__min_step(v_4, maxi, &Lib__min_out_st);
  v_5 = Lib__min_out_st.x;
  v_6 = -(v_5);
  Lib__abs_step(delta, &Lib__abs_out_st);
  v_2 = Lib__abs_out_st.x;
  Lib__min_step(v_2, maxi, &Lib__min_out_st);
  v_3 = Lib__min_out_st.x;
  v_1 = (delta<180.000000);
  if (v_1) {
    _out->power = v_3;
  } else {
    _out->power = v_6;
  };;
}

void Robot__setMotorFast_step(float current, float goal, float maxi,
                              Robot__setMotorFast_out* _out) {
  Lib__fmod_out Lib__fmod_out_st;
  
  float v_8;
  int v_7;
  float v;
  float delta;
  v_8 = -(maxi);
  v = (goal-current);
  Lib__fmod_step(v, 360.000000, &Lib__fmod_out_st);
  delta = Lib__fmod_out_st.x;
  v_7 = (delta<180.000000);
  if (v_7) {
    _out->power = maxi;
  } else {
    _out->power = v_8;
  };;
}

void Robot__step_step(float hipLangle, float hipRangle, float kneeLangle,
                      float kneeRangle, float gyroscope, int goLeft,
                      Robot__step_out* _out) {
  Robot__mux_out Robot__mux_out_st;
  Robot__setMotor_out Robot__setMotor_out_st;
  
  float v_11;
  float v_10;
  float v_9;
  float v;
  float hipLgoal;
  float hipRgoal;
  float kneeLgoal;
  float kneeRgoal;
  Robot__mux_step(goLeft, -80.000000, -20.000000, &Robot__mux_out_st);
  kneeRgoal = Robot__mux_out_st.r;
  Robot__mux_step(goLeft, 20.000000, 80.000000, &Robot__mux_out_st);
  kneeLgoal = Robot__mux_out_st.r;
  Robot__mux_step(goLeft, 80.000000, 0.000000, &Robot__mux_out_st);
  hipRgoal = Robot__mux_out_st.r;
  v_10 = (kneeRangle+hipRgoal);
  v_11 = (v_10+gyroscope);
  Robot__setMotor_step(v_11, kneeRgoal, 300.000000, &Robot__setMotor_out_st);
  _out->kneeRspeed = Robot__setMotor_out_st.power;
  Robot__setMotor_step(hipRangle, hipRgoal, 60.000000,
                       &Robot__setMotor_out_st);
  _out->hipRspeed = Robot__setMotor_out_st.power;
  Robot__mux_step(goLeft, -0.000000, -80.000000, &Robot__mux_out_st);
  hipLgoal = Robot__mux_out_st.r;
  v = (kneeLangle+hipLgoal);
  v_9 = (v+gyroscope);
  Robot__setMotor_step(v_9, kneeLgoal, 300.000000, &Robot__setMotor_out_st);
  _out->kneeLspeed = Robot__setMotor_out_st.power;
  Robot__setMotor_step(hipLangle, hipLgoal, 60.000000,
                       &Robot__setMotor_out_st);
  _out->hipLspeed = Robot__setMotor_out_st.power;;
}

void Robot__robot_reset(Robot__robot_mem* self) {
  self->pnr = false;
  self->ck = Robot__St_In_the_sky;
}

void Robot__robot_step(float dt, float hipLangle, float hipRangle,
                       float kneeLangle, float kneeRangle, float gyroscope,
                       float calfLdistance, float calfRdistance,
                       int footLcontact, int footRcontact, int bodyContact,
                       Robot__robot_out* _out, Robot__robot_mem* self) {
  Robot__step_out Robot__step_out_st;
  Robot__setMotorFast_out Robot__setMotorFast_out_st;
  Robot__setMotor_out Robot__setMotor_out_st;
  Lib__abs_out Lib__abs_out_st;
  
  int v_16;
  Robot__st v_15;
  int v_14;
  int v_13;
  int v_12;
  int v;
  int v_35;
  int v_34;
  int v_33;
  float v_32;
  float v_31;
  int v_29;
  Robot__st v_28;
  int v_27;
  int v_26;
  int v_25;
  int v_24;
  int v_23;
  int v_22;
  int v_21;
  int v_20;
  int v_19;
  int v_18;
  int v_17;
  float speed;
  float v_42;
  float v_41;
  float v_40;
  float v_39;
  int v_38;
  int v_37;
  int v_36;
  int v_54;
  Robot__st v_53;
  int v_52;
  int v_51;
  int v_50;
  int v_49;
  int v_48;
  int v_47;
  int v_46;
  int v_45;
  int v_44;
  int v_43;
  int v_55;
  int nr_St_Go_left;
  Robot__st ns_St_Go_left;
  float kneeRspeed_St_Go_left;
  float kneeLspeed_St_Go_left;
  float hipRspeed_St_Go_left;
  float hipLspeed_St_Go_left;
  int nr_St_Ground;
  Robot__st ns_St_Ground;
  float kneeRspeed_St_Ground;
  float kneeLspeed_St_Ground;
  float hipRspeed_St_Ground;
  float hipLspeed_St_Ground;
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
      Robot__setMotor_step(kneeRangle, -100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeRspeed_St_In_the_sky = Robot__setMotor_out_st.power;
      Robot__setMotor_step(kneeLangle, 100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeLspeed_St_In_the_sky = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipRangle, 40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipRspeed_St_In_the_sky = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipLangle, -40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipLspeed_St_In_the_sky = Robot__setMotor_out_st.power;
      v_55 = (footLcontact&&footRcontact);
      if (v_55) {
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
      v_50 = (footLcontact||footRcontact);
      v_51 = (v_50||bodyContact);
      v_52 = !(v_51);
      if (v_52) {
        v_54 = true;
        v_53 = Robot__St_In_the_sky;
      } else {
        v_54 = false;
        v_53 = Robot__St_On_the_ground;
      };
      _out->hipLspeed = hipLspeed_St_On_the_ground;
      v_43 = (_out->hipLspeed<=1.000000);
      _out->hipRspeed = hipRspeed_St_On_the_ground;
      v_44 = (_out->hipRspeed<=1.000000);
      v_45 = (v_43&&v_44);
      _out->kneeLspeed = kneeLspeed_St_On_the_ground;
      v_46 = (_out->kneeLspeed<=1.000000);
      v_47 = (v_45&&v_46);
      _out->kneeRspeed = kneeRspeed_St_On_the_ground;
      v_48 = (_out->kneeRspeed<=1.000000);
      v_49 = (v_47&&v_48);
      if (v_49) {
        nr_St_On_the_ground = true;
        ns_St_On_the_ground = Robot__St_Ground;
      } else {
        nr_St_On_the_ground = v_54;
        ns_St_On_the_ground = v_53;
      };
      ns = ns_St_On_the_ground;
      nr = nr_St_On_the_ground;
      break;
    case Robot__St_Jump:
      v_41 = (kneeRangle+hipRangle);
      v_42 = (v_41+gyroscope);
      Robot__setMotorFast_step(v_42, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeRspeed_St_Jump = Robot__setMotorFast_out_st.power;
      v_39 = (kneeLangle+hipLangle);
      v_40 = (v_39+gyroscope);
      Robot__setMotorFast_step(v_40, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeLspeed_St_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipRangle, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipRspeed_St_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipLangle, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipLspeed_St_Jump = Robot__setMotorFast_out_st.power;
      v_37 = !(footRcontact);
      v_36 = !(footLcontact);
      v_38 = (v_36&&v_37);
      if (v_38) {
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
    case Robot__St_Ground:
      kneeRspeed_St_Ground = 0.000000;
      kneeLspeed_St_Ground = 0.000000;
      v_31 = (gyroscope-self->v_30);
      Lib__abs_step(v_31, &Lib__abs_out_st);
      v_32 = Lib__abs_out_st.x;
      v_33 = (v_32<0.100000);
      if (r) {
        v_34 = false;
      } else {
        v_34 = v_33;
      };
      v_35 = (bodyContact&&v_34);
      if (v_35) {
        speed = 100.000000;
      } else {
        speed = 10.000000;
      };
      Robot__setMotor_step(hipRangle, 70.000000, speed,
                           &Robot__setMotor_out_st);
      hipRspeed_St_Ground = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipLangle, -70.000000, speed,
                           &Robot__setMotor_out_st);
      hipLspeed_St_Ground = Robot__setMotor_out_st.power;
      v_25 = (footLcontact||footRcontact);
      v_26 = (v_25||bodyContact);
      v_27 = !(v_26);
      if (v_27) {
        v_29 = true;
        v_28 = Robot__St_In_the_sky;
      } else {
        v_29 = false;
        v_28 = Robot__St_Ground;
      };
      _out->hipLspeed = hipLspeed_St_Ground;
      v_17 = (_out->hipLspeed<=1.000000);
      _out->hipRspeed = hipRspeed_St_Ground;
      v_18 = (_out->hipRspeed<=1.000000);
      v_19 = (v_17&&v_18);
      _out->kneeLspeed = kneeLspeed_St_Ground;
      v_20 = (_out->kneeLspeed<=1.000000);
      v_21 = (v_19&&v_20);
      _out->kneeRspeed = kneeRspeed_St_Ground;
      v_22 = (_out->kneeRspeed<=1.000000);
      v_23 = (v_21&&v_22);
      v_24 = (v_23&&bodyContact);
      if (v_24) {
        nr_St_Ground = true;
        ns_St_Ground = Robot__St_Go_left;
      } else {
        nr_St_Ground = v_29;
        ns_St_Ground = v_28;
      };
      ns = ns_St_Ground;
      nr = nr_St_Ground;
      self->v_30 = gyroscope;
      break;
    case Robot__St_Go_left:
      Robot__step_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                       gyroscope, false, &Robot__step_out_st);
      hipLspeed_St_Go_left = Robot__step_out_st.hipLspeed;
      hipRspeed_St_Go_left = Robot__step_out_st.hipRspeed;
      kneeLspeed_St_Go_left = Robot__step_out_st.kneeLspeed;
      kneeRspeed_St_Go_left = Robot__step_out_st.kneeRspeed;
      v_12 = (footLcontact||footRcontact);
      v_13 = (v_12||bodyContact);
      v_14 = !(v_13);
      if (v_14) {
        v_16 = true;
        v_15 = Robot__St_In_the_sky;
      } else {
        v_16 = false;
        v_15 = Robot__St_Go_left;
      };
      v = !(bodyContact);
      if (v) {
        nr_St_Go_left = true;
        ns_St_Go_left = Robot__St_On_the_ground;
      } else {
        nr_St_Go_left = v_16;
        ns_St_Go_left = v_15;
      };
      _out->hipLspeed = hipLspeed_St_Go_left;
      _out->hipRspeed = hipRspeed_St_Go_left;
      _out->kneeLspeed = kneeLspeed_St_Go_left;
      _out->kneeRspeed = kneeRspeed_St_Go_left;
      ns = ns_St_Go_left;
      nr = nr_St_Go_left;
      break;
    default:
      break;
  };
  self->pnr = nr;
  self->ck = ns;;
}

