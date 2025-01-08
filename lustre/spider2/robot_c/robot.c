/* --- Generated the 8/1/2025 at 13:13 --- */
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
  v_9 = (kneeRangle+hipRgoal);
  Robot__setMotor_step(v_9, kneeRgoal, 300.000000, &Robot__setMotor_out_st);
  _out->kneeRspeed = Robot__setMotor_out_st.power;
  Robot__setMotor_step(hipRangle, hipRgoal, 60.000000,
                       &Robot__setMotor_out_st);
  _out->hipRspeed = Robot__setMotor_out_st.power;
  Robot__mux_step(goLeft, -0.000000, -80.000000, &Robot__mux_out_st);
  hipLgoal = Robot__mux_out_st.r;
  v = (kneeLangle+hipLgoal);
  Robot__setMotor_step(v, kneeLgoal, 300.000000, &Robot__setMotor_out_st);
  _out->kneeLspeed = Robot__setMotor_out_st.power;
  Robot__setMotor_step(hipLangle, hipLgoal, 60.000000,
                       &Robot__setMotor_out_st);
  _out->hipLspeed = Robot__setMotor_out_st.power;;
}

void Robot__robot_reset(Robot__robot_mem* self) {
  self->pnr = false;
  self->ck_1 = Robot__St_Init;
  self->pnr_1 = false;
  self->ck = Robot__St_1_In_the_sky;
}

void Robot__robot_step(float dt, float hipLangle, float hipRangle,
                       float kneeLangle, float kneeRangle, float arm1angle,
                       float arm2angle, float arm3angle, float arm4angle,
                       float gyroscope, float calfLdistance,
                       float calfRdistance, int footLcontact,
                       int footRcontact, int bodyContact,
                       Robot__robot_out* _out, Robot__robot_mem* self) {
  Robot__step_out Robot__step_out_st;
  Lib__arm_ik_out Lib__arm_ik_out_st;
  Robot__setMotor_out Robot__setMotor_out_st;
  Lib__abs_out Lib__abs_out_st;
  Robot__setMotorFast_out Robot__setMotorFast_out_st;
  
  int v_14;
  Robot__st_1 v_13;
  int v_12;
  int v_11;
  int v_10;
  int v;
  int v_45;
  int v_44;
  int v_43;
  int v_42;
  int v_41;
  int v_40;
  int v_39;
  int v_38;
  int v_37;
  int v_36;
  int v_35;
  int v_34;
  int v_33;
  float v_32;
  float v_31;
  int v_29;
  Robot__st_1 v_28;
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
  int v_16;
  int v_15;
  float speed;
  int okL;
  int okR;
  float v_52;
  float v_51;
  float v_50;
  float v_49;
  int v_48;
  int v_47;
  int v_46;
  int v_65;
  Robot__st_1 v_64;
  int v_63;
  int v_62;
  int v_61;
  int v_60;
  int v_59;
  int v_58;
  int v_57;
  int v_56;
  int v_55;
  int v_54;
  int v_53;
  int v_66;
  int nr_1_St_1_Go_left;
  Robot__st_1 ns_1_St_1_Go_left;
  float kneeRspeed_St_1_Go_left;
  float kneeLspeed_St_1_Go_left;
  float hipRspeed_St_1_Go_left;
  float hipLspeed_St_1_Go_left;
  int nr_1_St_1_Ground;
  Robot__st_1 ns_1_St_1_Ground;
  float kneeRspeed_St_1_Ground;
  float kneeLspeed_St_1_Ground;
  float hipRspeed_St_1_Ground;
  float hipLspeed_St_1_Ground;
  int nr_1_St_1_Jump;
  Robot__st_1 ns_1_St_1_Jump;
  float kneeRspeed_St_1_Jump;
  float kneeLspeed_St_1_Jump;
  float hipRspeed_St_1_Jump;
  float hipLspeed_St_1_Jump;
  int nr_1_St_1_On_the_ground;
  Robot__st_1 ns_1_St_1_On_the_ground;
  float kneeRspeed_St_1_On_the_ground;
  float kneeLspeed_St_1_On_the_ground;
  float hipRspeed_St_1_On_the_ground;
  float hipLspeed_St_1_On_the_ground;
  int nr_1_St_1_In_the_sky;
  Robot__st_1 ns_1_St_1_In_the_sky;
  float kneeRspeed_St_1_In_the_sky;
  float kneeLspeed_St_1_In_the_sky;
  float hipRspeed_St_1_In_the_sky;
  float hipLspeed_St_1_In_the_sky;
  float theta1;
  float theta2;
  float theta3;
  float theta4;
  int nr_St_Catch;
  Robot__st ns_St_Catch;
  float arm4speed_St_Catch;
  float arm3speed_St_Catch;
  float arm2speed_St_Catch;
  float arm1speed_St_Catch;
  int nr_St_Init;
  Robot__st ns_St_Init;
  float arm4speed_St_Init;
  float arm3speed_St_Init;
  float arm2speed_St_Init;
  float arm1speed_St_Init;
  Robot__st_1 ns_1;
  int r_4;
  int nr_1;
  Robot__st ns;
  int r;
  int nr;
  r = self->pnr;
  r_4 = self->pnr_1;
  switch (self->ck_1) {
    case Robot__St_Init:
      arm4speed_St_Init = 0.000000;
      arm3speed_St_Init = 0.000000;
      arm2speed_St_Init = 0.000000;
      arm1speed_St_Init = 0.000000;
      if (true) {
        nr_St_Init = true;
      } else {
        nr_St_Init = false;
      };
      if (true) {
        ns_St_Init = Robot__St_Catch;
      } else {
        ns_St_Init = Robot__St_Init;
      };
      _out->arm1speed = arm1speed_St_Init;
      _out->arm2speed = arm2speed_St_Init;
      _out->arm3speed = arm3speed_St_Init;
      _out->arm4speed = arm4speed_St_Init;
      ns = ns_St_Init;
      nr = nr_St_Init;
      break;
    case Robot__St_Catch:
      Lib__arm_ik_step(0.600000, 0.600000, 0.000000, arm1angle, arm2angle,
                       arm3angle, arm4angle, 0.260000, 0.260000, 0.260000,
                       0.260000, &Lib__arm_ik_out_st);
      theta1 = Lib__arm_ik_out_st.theta1;
      theta2 = Lib__arm_ik_out_st.theta2;
      theta3 = Lib__arm_ik_out_st.theta3;
      theta4 = Lib__arm_ik_out_st.theta4;
      Robot__setMotor_step(arm4angle, theta4, 60.000000,
                           &Robot__setMotor_out_st);
      arm4speed_St_Catch = Robot__setMotor_out_st.power;
      Robot__setMotor_step(arm3angle, theta3, 60.000000,
                           &Robot__setMotor_out_st);
      arm3speed_St_Catch = Robot__setMotor_out_st.power;
      Robot__setMotor_step(arm2angle, theta2, 60.000000,
                           &Robot__setMotor_out_st);
      arm2speed_St_Catch = Robot__setMotor_out_st.power;
      Robot__setMotor_step(arm1angle, theta1, 60.000000,
                           &Robot__setMotor_out_st);
      arm1speed_St_Catch = Robot__setMotor_out_st.power;
      nr_St_Catch = false;
      ns_St_Catch = Robot__St_Catch;
      _out->arm1speed = arm1speed_St_Catch;
      _out->arm2speed = arm2speed_St_Catch;
      _out->arm3speed = arm3speed_St_Catch;
      _out->arm4speed = arm4speed_St_Catch;
      ns = ns_St_Catch;
      nr = nr_St_Catch;
      break;
    default:
      break;
  };
  switch (self->ck) {
    case Robot__St_1_In_the_sky:
      Robot__setMotor_step(kneeRangle, -100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeRspeed_St_1_In_the_sky = Robot__setMotor_out_st.power;
      Robot__setMotor_step(kneeLangle, 100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeLspeed_St_1_In_the_sky = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipRangle, 40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipRspeed_St_1_In_the_sky = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipLangle, -40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipLspeed_St_1_In_the_sky = Robot__setMotor_out_st.power;
      v_66 = (footLcontact&&footRcontact);
      if (v_66) {
        nr_1_St_1_In_the_sky = true;
        ns_1_St_1_In_the_sky = Robot__St_1_On_the_ground;
      } else {
        nr_1_St_1_In_the_sky = false;
        ns_1_St_1_In_the_sky = Robot__St_1_In_the_sky;
      };
      _out->hipLspeed = hipLspeed_St_1_In_the_sky;
      _out->hipRspeed = hipRspeed_St_1_In_the_sky;
      _out->kneeLspeed = kneeLspeed_St_1_In_the_sky;
      _out->kneeRspeed = kneeRspeed_St_1_In_the_sky;
      ns_1 = ns_1_St_1_In_the_sky;
      nr_1 = nr_1_St_1_In_the_sky;
      break;
    case Robot__St_1_On_the_ground:
      Robot__setMotor_step(kneeRangle, -100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeRspeed_St_1_On_the_ground = Robot__setMotor_out_st.power;
      Robot__setMotor_step(kneeLangle, 100.000000, 60.000000,
                           &Robot__setMotor_out_st);
      kneeLspeed_St_1_On_the_ground = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipRangle, 40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipRspeed_St_1_On_the_ground = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipLangle, -40.000000, 60.000000,
                           &Robot__setMotor_out_st);
      hipLspeed_St_1_On_the_ground = Robot__setMotor_out_st.power;
      v_61 = (footLcontact||footRcontact);
      v_62 = (v_61||bodyContact);
      v_63 = !(v_62);
      if (v_63) {
        v_65 = true;
        v_64 = Robot__St_1_In_the_sky;
      } else {
        v_65 = false;
        v_64 = Robot__St_1_On_the_ground;
      };
      _out->hipLspeed = hipLspeed_St_1_On_the_ground;
      v_53 = (_out->hipLspeed<=1.000000);
      v_54 = (false&&v_53);
      _out->hipRspeed = hipRspeed_St_1_On_the_ground;
      v_55 = (_out->hipRspeed<=1.000000);
      v_56 = (v_54&&v_55);
      _out->kneeLspeed = kneeLspeed_St_1_On_the_ground;
      v_57 = (_out->kneeLspeed<=1.000000);
      v_58 = (v_56&&v_57);
      _out->kneeRspeed = kneeRspeed_St_1_On_the_ground;
      v_59 = (_out->kneeRspeed<=1.000000);
      v_60 = (v_58&&v_59);
      if (v_60) {
        nr_1_St_1_On_the_ground = true;
        ns_1_St_1_On_the_ground = Robot__St_1_Ground;
      } else {
        nr_1_St_1_On_the_ground = v_65;
        ns_1_St_1_On_the_ground = v_64;
      };
      ns_1 = ns_1_St_1_On_the_ground;
      nr_1 = nr_1_St_1_On_the_ground;
      break;
    case Robot__St_1_Jump:
      v_51 = (kneeRangle+hipRangle);
      v_52 = (v_51+gyroscope);
      Robot__setMotorFast_step(v_52, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeRspeed_St_1_Jump = Robot__setMotorFast_out_st.power;
      v_49 = (kneeLangle+hipLangle);
      v_50 = (v_49+gyroscope);
      Robot__setMotorFast_step(v_50, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeLspeed_St_1_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipRangle, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipRspeed_St_1_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipLangle, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipLspeed_St_1_Jump = Robot__setMotorFast_out_st.power;
      v_47 = !(footRcontact);
      v_46 = !(footLcontact);
      v_48 = (v_46&&v_47);
      if (v_48) {
        nr_1_St_1_Jump = true;
        ns_1_St_1_Jump = Robot__St_1_In_the_sky;
      } else {
        nr_1_St_1_Jump = false;
        ns_1_St_1_Jump = Robot__St_1_Jump;
      };
      _out->hipLspeed = hipLspeed_St_1_Jump;
      _out->hipRspeed = hipRspeed_St_1_Jump;
      _out->kneeLspeed = kneeLspeed_St_1_Jump;
      _out->kneeRspeed = kneeRspeed_St_1_Jump;
      ns_1 = ns_1_St_1_Jump;
      nr_1 = nr_1_St_1_Jump;
      break;
    case Robot__St_1_Ground:
      v_45 = (calfLdistance>=9.000000);
      v_44 = (kneeLangle>=-1.000000);
      okR = (v_44||v_45);
      v_43 = (calfLdistance>=9.000000);
      v_42 = (kneeLangle<=1.000000);
      okL = (v_42||v_43);
      v_40 = (calfRdistance<=10.000000);
      v_39 = (kneeRangle<=0.000000);
      v_41 = (v_39&&v_40);
      if (v_41) {
        kneeRspeed_St_1_Ground = 10.000000;
      } else {
        kneeRspeed_St_1_Ground = 0.000000;
      };
      v_37 = (calfLdistance<=10.000000);
      v_36 = (kneeLangle>=0.000000);
      v_38 = (v_36&&v_37);
      if (v_38) {
        kneeLspeed_St_1_Ground = -10.000000;
      } else {
        kneeLspeed_St_1_Ground = 0.000000;
      };
      v_31 = (gyroscope-self->v_30);
      Lib__abs_step(v_31, &Lib__abs_out_st);
      v_32 = Lib__abs_out_st.x;
      v_33 = (v_32<0.100000);
      if (r_4) {
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
      hipRspeed_St_1_Ground = Robot__setMotor_out_st.power;
      Robot__setMotor_step(hipLangle, -70.000000, speed,
                           &Robot__setMotor_out_st);
      hipLspeed_St_1_Ground = Robot__setMotor_out_st.power;
      v_25 = (footLcontact||footRcontact);
      v_26 = (v_25||bodyContact);
      v_27 = !(v_26);
      if (v_27) {
        v_29 = true;
        v_28 = Robot__St_1_In_the_sky;
      } else {
        v_29 = false;
        v_28 = Robot__St_1_Ground;
      };
      v_15 = (okL&&okR);
      _out->hipLspeed = hipLspeed_St_1_Ground;
      v_16 = (_out->hipLspeed<=1.000000);
      v_17 = (v_15&&v_16);
      _out->hipRspeed = hipRspeed_St_1_Ground;
      v_18 = (_out->hipRspeed<=1.000000);
      v_19 = (v_17&&v_18);
      _out->kneeLspeed = kneeLspeed_St_1_Ground;
      v_20 = (_out->kneeLspeed<=1.000000);
      v_21 = (v_19&&v_20);
      _out->kneeRspeed = kneeRspeed_St_1_Ground;
      v_22 = (_out->kneeRspeed<=1.000000);
      v_23 = (v_21&&v_22);
      v_24 = (v_23&&bodyContact);
      if (v_24) {
        nr_1_St_1_Ground = true;
        ns_1_St_1_Ground = Robot__St_1_Go_left;
      } else {
        nr_1_St_1_Ground = v_29;
        ns_1_St_1_Ground = v_28;
      };
      ns_1 = ns_1_St_1_Ground;
      nr_1 = nr_1_St_1_Ground;
      self->v_30 = gyroscope;
      break;
    case Robot__St_1_Go_left:
      Robot__step_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                       gyroscope, false, &Robot__step_out_st);
      hipLspeed_St_1_Go_left = Robot__step_out_st.hipLspeed;
      hipRspeed_St_1_Go_left = Robot__step_out_st.hipRspeed;
      kneeLspeed_St_1_Go_left = Robot__step_out_st.kneeLspeed;
      kneeRspeed_St_1_Go_left = Robot__step_out_st.kneeRspeed;
      v_10 = (footLcontact||footRcontact);
      v_11 = (v_10||bodyContact);
      v_12 = !(v_11);
      if (v_12) {
        v_14 = true;
        v_13 = Robot__St_1_In_the_sky;
      } else {
        v_14 = false;
        v_13 = Robot__St_1_Go_left;
      };
      v = !(bodyContact);
      if (v) {
        nr_1_St_1_Go_left = true;
        ns_1_St_1_Go_left = Robot__St_1_On_the_ground;
      } else {
        nr_1_St_1_Go_left = v_14;
        ns_1_St_1_Go_left = v_13;
      };
      _out->hipLspeed = hipLspeed_St_1_Go_left;
      _out->hipRspeed = hipRspeed_St_1_Go_left;
      _out->kneeLspeed = kneeLspeed_St_1_Go_left;
      _out->kneeRspeed = kneeRspeed_St_1_Go_left;
      ns_1 = ns_1_St_1_Go_left;
      nr_1 = nr_1_St_1_Go_left;
      break;
    default:
      break;
  };
  self->pnr = nr;
  self->ck_1 = ns;
  self->pnr_1 = nr_1;
  self->ck = ns_1;;
}

