/* --- Generated the 12/1/2025 at 20:0 --- */
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

void Robot__doNotMove_step(float limit, float speedA, float speedB,
                           float speedC, float speedD,
                           Robot__doNotMove_out* _out) {
  
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

void Robot__setMotorArm_reset(Robot__setMotorArm_mem* self) {
  Robot__setMotorPID_reset(&self->setMotorPID_3);
  Robot__setMotorPID_reset(&self->setMotorPID_2);
  Robot__setMotorPID_reset(&self->setMotorPID_1);
  Robot__setMotorPID_reset(&self->setMotorPID);
}

void Robot__setMotorArm_step(float arm1angle, float arm2angle,
                             float arm3angle, float arm4angle,
                             float arm1goal, float arm2goal, float arm3goal,
                             float arm4goal, Robot__setMotorArm_out* _out,
                             Robot__setMotorArm_mem* self) {
  Robot__setMotorPID_out Robot__setMotorPID_out_st;
  Robot__setMotorPID_step(arm4angle, arm4goal, 20.000000, 1.000000, 0.000000,
                          0.500000, &Robot__setMotorPID_out_st,
                          &self->setMotorPID_3);
  _out->arm4speed = Robot__setMotorPID_out_st.motorSpeed;
  Robot__setMotorPID_step(arm3angle, arm3goal, 20.000000, 0.900000, 0.000000,
                          0.400000, &Robot__setMotorPID_out_st,
                          &self->setMotorPID_2);
  _out->arm3speed = Robot__setMotorPID_out_st.motorSpeed;
  Robot__setMotorPID_step(arm2angle, arm2goal, 20.000000, 0.700000, 0.000000,
                          0.300000, &Robot__setMotorPID_out_st,
                          &self->setMotorPID_1);
  _out->arm2speed = Robot__setMotorPID_out_st.motorSpeed;
  Robot__setMotorPID_step(arm1angle, arm1goal, 20.000000, 0.500000, 0.000000,
                          0.200000, &Robot__setMotorPID_out_st,
                          &self->setMotorPID);
  _out->arm1speed = Robot__setMotorPID_out_st.motorSpeed;
}

void Robot__setMotorLegs_step(float hipLangle, float hipRangle,
                              float kneeLangle, float kneeRangle,
                              float hipLgoal, float hipRgoal,
                              float kneeLgoal, float kneeRgoal,
                              Robot__setMotorLegs_out* _out) {
  Robot__setMotor_out Robot__setMotor_out_st;
  Robot__setMotor_step(kneeRangle, kneeRgoal, 60.000000,
                       &Robot__setMotor_out_st);
  _out->kneeRspeed = Robot__setMotor_out_st.power;
  Robot__setMotor_step(kneeLangle, kneeLgoal, 60.000000,
                       &Robot__setMotor_out_st);
  _out->kneeLspeed = Robot__setMotor_out_st.power;
  Robot__setMotor_step(hipRangle, hipRgoal, 60.000000,
                       &Robot__setMotor_out_st);
  _out->hipRspeed = Robot__setMotor_out_st.power;
  Robot__setMotor_step(hipLangle, hipLgoal, 60.000000,
                       &Robot__setMotor_out_st);
  _out->hipLspeed = Robot__setMotor_out_st.power;
}

void Robot__robot_reset(Robot__robot_mem* self) {
  Robot__setMotorArm_reset(&self->setMotorArm_4);
  Robot__setMotorArm_reset(&self->setMotorArm_3);
  Robot__setMotorArm_reset(&self->setMotorArm_2);
  Robot__setMotorArm_reset(&self->setMotorArm_1);
  Robot__setMotorArm_reset(&self->setMotorArm);
  self->pnr = false;
  self->ck = Robot__St_Falling;
  self->v = true;
}

void Robot__robot_step(float dt, float hipLangle, float hipRangle,
                       float kneeLangle, float kneeRangle, float arm1angle,
                       float arm2angle, float arm3angle, float arm4angle,
                       float gyroscopeTot, float calfLdistance,
                       float calfRdistance, int footLcontact,
                       int footRcontact, int bodyContact,
                       Robot__robot_out* _out, Robot__robot_mem* self) {
  Robot__setMotorLegs_out Robot__setMotorLegs_out_st;
  Lib__fmod_out Lib__fmod_out_st;
  Robot__doNotMove_out Robot__doNotMove_out_st;
  Robot__setMotorArm_out Robot__setMotorArm_out_st;
  Lib__abs_out Lib__abs_out_st;
  
  int v_36;
  int v_35;
  int v_34;
  int r_1;
  int v_46;
  Robot__st v_45;
  int v_44;
  int v_43;
  int v_42;
  int v_41;
  int v_40;
  int v_39;
  int v_38;
  int v_37;
  int r_2;
  int v_56;
  Robot__st v_55;
  int v_54;
  int v_53;
  int v_52;
  int v_51;
  int v_50;
  int v_49;
  int v_48;
  int v_47;
  int r_3;
  int v_64;
  int v_63;
  int v_62;
  Robot__st v_61;
  int v_60;
  int v_59;
  int v_58;
  int v_57;
  int r_4;
  int isStatic;
  int v_66;
  int v_65;
  int r_5;
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
  int nr_St_Upside_Down_Right;
  Robot__st ns_St_Upside_Down_Right;
  float arm4speed_St_Upside_Down_Right;
  float arm3speed_St_Upside_Down_Right;
  float arm2speed_St_Upside_Down_Right;
  float arm1speed_St_Upside_Down_Right;
  float kneeRspeed_St_Upside_Down_Right;
  float kneeLspeed_St_Upside_Down_Right;
  float hipRspeed_St_Upside_Down_Right;
  float hipLspeed_St_Upside_Down_Right;
  int nr_St_Upside_Down_Left;
  Robot__st ns_St_Upside_Down_Left;
  float arm4speed_St_Upside_Down_Left;
  float arm3speed_St_Upside_Down_Left;
  float arm2speed_St_Upside_Down_Left;
  float arm1speed_St_Upside_Down_Left;
  float kneeRspeed_St_Upside_Down_Left;
  float kneeLspeed_St_Upside_Down_Left;
  float hipRspeed_St_Upside_Down_Left;
  float hipLspeed_St_Upside_Down_Left;
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
  float v_33;
  float v_32;
  float v_31;
  Robot__st ns;
  int r;
  int nr;
  float gyroscope;
  float dgyroscope;
  r = self->pnr;
  Lib__fmod_step(gyroscopeTot, 360.000000, &Lib__fmod_out_st);
  gyroscope = Lib__fmod_out_st.x;
  v_31 = (gyroscope-self->v_30);
  Lib__abs_step(v_31, &Lib__abs_out_st);
  v_32 = Lib__abs_out_st.x;
  v_33 = (v_32/dt);
  if (self->v) {
    dgyroscope = 100.000000;
  } else {
    dgyroscope = v_33;
  };
  switch (self->ck) {
    case Robot__St_Falling:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, 40.000000, 100.000000,
                               -100.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_Falling = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_Falling = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_Falling = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_Falling = Robot__setMotorLegs_out_st.kneeRspeed;
      v_65 = (footLcontact||footRcontact);
      v_66 = (v_65||bodyContact);
      if (v_66) {
        nr_St_Falling = true;
        ns_St_Falling = Robot__St_Standard;
      } else {
        nr_St_Falling = false;
        ns_St_Falling = Robot__St_Falling;
      };
      r_5 = r;
      if (r_5) {
        Robot__setMotorArm_reset(&self->setMotorArm_4);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -40.000000, 30.000000, 80.000000, 60.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_4);
      arm1speed_St_Falling = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_Falling = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_Falling = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_Falling = Robot__setMotorArm_out_st.arm4speed;
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
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, 40.000000, 100.000000,
                               -100.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_Upside_Down_Init = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_Upside_Down_Init = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_Upside_Down_Init = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_Upside_Down_Init = Robot__setMotorLegs_out_st.kneeRspeed;
      v_59 = (gyroscope<180.000000);
      v_57 = (gyroscope>=180.000000);
      r_4 = r;
      if (r_4) {
        Robot__setMotorArm_reset(&self->setMotorArm_3);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -0.000000, 0.000000, 0.000000, 0.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_3);
      arm1speed_St_Upside_Down_Init = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_Upside_Down_Init = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_Upside_Down_Init = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_Upside_Down_Init = Robot__setMotorArm_out_st.arm4speed;
      _out->hipLspeed = hipLspeed_St_Upside_Down_Init;
      _out->hipRspeed = hipRspeed_St_Upside_Down_Init;
      _out->kneeLspeed = kneeLspeed_St_Upside_Down_Init;
      _out->kneeRspeed = kneeRspeed_St_Upside_Down_Init;
      Robot__doNotMove_step(1.000000, _out->hipLspeed, _out->hipRspeed,
                            _out->kneeLspeed, _out->kneeRspeed,
                            &Robot__doNotMove_out_st);
      v_63 = Robot__doNotMove_out_st.ok;
      _out->arm1speed = arm1speed_St_Upside_Down_Init;
      _out->arm2speed = arm2speed_St_Upside_Down_Init;
      _out->arm3speed = arm3speed_St_Upside_Down_Init;
      _out->arm4speed = arm4speed_St_Upside_Down_Init;
      Robot__doNotMove_step(1.000000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_64 = Robot__doNotMove_out_st.ok;
      isStatic = (v_63&&v_64);
      v_60 = (isStatic&&v_59);
      if (v_60) {
        v_62 = true;
        v_61 = Robot__St_Upside_Down_Right;
      } else {
        v_62 = false;
        v_61 = Robot__St_Upside_Down_Init;
      };
      v_58 = (isStatic&&v_57);
      if (v_58) {
        nr_St_Upside_Down_Init = true;
        ns_St_Upside_Down_Init = Robot__St_Upside_Down_Left;
      } else {
        nr_St_Upside_Down_Init = v_62;
        ns_St_Upside_Down_Init = v_61;
      };
      ns = ns_St_Upside_Down_Init;
      nr = nr_St_Upside_Down_Init;
      break;
    case Robot__St_Upside_Down_Left:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               60.000000, 40.000000, 40.000000, -100.000000,
                               &Robot__setMotorLegs_out_st);
      hipLspeed_St_Upside_Down_Left = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_Upside_Down_Left = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_Upside_Down_Left = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_Upside_Down_Left = Robot__setMotorLegs_out_st.kneeRspeed;
      v_53 = (dgyroscope<=1.000000);
      v_51 = (arm1angle<=-20.000000);
      v_48 = (gyroscope>=320.000000);
      v_47 = (gyroscope<=40.000000);
      v_49 = (v_47||v_48);
      r_3 = r;
      if (r_3) {
        Robot__setMotorArm_reset(&self->setMotorArm_2);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -50.000000, -10.000000, -10.000000, -30.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_2);
      arm1speed_St_Upside_Down_Left = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_Upside_Down_Left = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_Upside_Down_Left = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_Upside_Down_Left = Robot__setMotorArm_out_st.arm4speed;
      _out->hipLspeed = hipLspeed_St_Upside_Down_Left;
      _out->hipRspeed = hipRspeed_St_Upside_Down_Left;
      _out->kneeLspeed = kneeLspeed_St_Upside_Down_Left;
      _out->kneeRspeed = kneeRspeed_St_Upside_Down_Left;
      _out->arm1speed = arm1speed_St_Upside_Down_Left;
      _out->arm2speed = arm2speed_St_Upside_Down_Left;
      _out->arm3speed = arm3speed_St_Upside_Down_Left;
      _out->arm4speed = arm4speed_St_Upside_Down_Left;
      Robot__doNotMove_step(0.100000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_50 = Robot__doNotMove_out_st.ok;
      v_52 = (v_50&&v_51);
      v_54 = (v_52&&v_53);
      if (v_54) {
        v_56 = true;
      } else {
        v_56 = false;
      };
      if (v_49) {
        nr_St_Upside_Down_Left = true;
      } else {
        nr_St_Upside_Down_Left = v_56;
      };
      if (v_54) {
        v_55 = Robot__St_Upside_Down_Right;
      } else {
        v_55 = Robot__St_Upside_Down_Left;
      };
      if (v_49) {
        ns_St_Upside_Down_Left = Robot__St_Standard;
      } else {
        ns_St_Upside_Down_Left = v_55;
      };
      ns = ns_St_Upside_Down_Left;
      nr = nr_St_Upside_Down_Left;
      break;
    case Robot__St_Upside_Down_Right:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, -60.000000, 100.000000,
                               -40.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_Upside_Down_Right = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_Upside_Down_Right = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_Upside_Down_Right = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_Upside_Down_Right = Robot__setMotorLegs_out_st.kneeRspeed;
      v_43 = (dgyroscope<=1.000000);
      v_41 = (arm1angle>=20.000000);
      v_38 = (gyroscope>=320.000000);
      v_37 = (gyroscope<=40.000000);
      v_39 = (v_37||v_38);
      r_2 = r;
      if (r_2) {
        Robot__setMotorArm_reset(&self->setMotorArm_1);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              50.000000, 10.000000, 10.000000, 30.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_1);
      arm1speed_St_Upside_Down_Right = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_Upside_Down_Right = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_Upside_Down_Right = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_Upside_Down_Right = Robot__setMotorArm_out_st.arm4speed;
      _out->hipLspeed = hipLspeed_St_Upside_Down_Right;
      _out->hipRspeed = hipRspeed_St_Upside_Down_Right;
      _out->kneeLspeed = kneeLspeed_St_Upside_Down_Right;
      _out->kneeRspeed = kneeRspeed_St_Upside_Down_Right;
      _out->arm1speed = arm1speed_St_Upside_Down_Right;
      _out->arm2speed = arm2speed_St_Upside_Down_Right;
      _out->arm3speed = arm3speed_St_Upside_Down_Right;
      _out->arm4speed = arm4speed_St_Upside_Down_Right;
      Robot__doNotMove_step(0.100000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_40 = Robot__doNotMove_out_st.ok;
      v_42 = (v_40&&v_41);
      v_44 = (v_42&&v_43);
      if (v_44) {
        v_46 = true;
      } else {
        v_46 = false;
      };
      if (v_39) {
        nr_St_Upside_Down_Right = true;
      } else {
        nr_St_Upside_Down_Right = v_46;
      };
      if (v_44) {
        v_45 = Robot__St_Upside_Down_Left;
      } else {
        v_45 = Robot__St_Upside_Down_Right;
      };
      if (v_39) {
        ns_St_Upside_Down_Right = Robot__St_Standard;
      } else {
        ns_St_Upside_Down_Right = v_45;
      };
      ns = ns_St_Upside_Down_Right;
      nr = nr_St_Upside_Down_Right;
      break;
    case Robot__St_Standard:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, 40.000000, 100.000000,
                               -100.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_Standard = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_Standard = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_Standard = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_Standard = Robot__setMotorLegs_out_st.kneeRspeed;
      v_35 = (gyroscope<=260.000000);
      v_34 = (gyroscope>=90.000000);
      v_36 = (v_34&&v_35);
      if (v_36) {
        nr_St_Standard = true;
        ns_St_Standard = Robot__St_Upside_Down_Init;
      } else {
        nr_St_Standard = false;
        ns_St_Standard = Robot__St_Standard;
      };
      r_1 = r;
      if (r_1) {
        Robot__setMotorArm_reset(&self->setMotorArm);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -40.000000, 30.000000, 80.000000, 60.000000,
                              &Robot__setMotorArm_out_st, &self->setMotorArm);
      arm1speed_St_Standard = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_Standard = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_Standard = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_Standard = Robot__setMotorArm_out_st.arm4speed;
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
  self->ck = ns;
  self->v_30 = gyroscope;
  self->v = false;;
}

