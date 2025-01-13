/* --- Generated the 13/1/2025 at 12:0 --- */
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
  Robot__setMotorArm_reset(&self->setMotorArm_5);
  Robot__setMotorArm_reset(&self->setMotorArm_4);
  Robot__setMotorArm_reset(&self->setMotorArm_3);
  Robot__setMotorArm_reset(&self->setMotorArm_2);
  Robot__setMotorArm_reset(&self->setMotorArm_1);
  Robot__setMotorArm_reset(&self->setMotorArm);
  self->pnr = false;
  self->ck = Robot__St_Falling;
  self->v_41 = true;
  self->v_34 = true;
  self->v = true;
}

void Robot__robot_step(float dt, float hipLangle, float hipRangle,
                       float kneeLangle, float kneeRangle, float arm1angle,
                       float arm2angle, float arm3angle, float arm4angle,
                       float gyroscopeTot, float calfLdistance,
                       float calfRdistance, int footLcontact,
                       int footRcontact, int bodyContact, float main_x,
                       float main_y, float secondary_x, float secondary_y,
                       int action_a, Robot__robot_out* _out,
                       Robot__robot_mem* self) {
  Lib__arm_ik_out Lib__arm_ik_out_st;
  Lib__abs_out Lib__abs_out_st;
  Robot__setMotorLegs_out Robot__setMotorLegs_out_st;
  Lib__fmod_out Lib__fmod_out_st;
  Lib__clamp_out Lib__clamp_out_st;
  Robot__setMotorFast_out Robot__setMotorFast_out_st;
  Robot__doNotMove_out Robot__doNotMove_out_st;
  Robot__setMotorArm_out Robot__setMotorArm_out_st;
  
  int v_51;
  Robot__st v_50;
  int v_49;
  int v_48;
  int v_47;
  int r_1;
  float theta1;
  float theta2;
  float theta3;
  float theta4;
  float v_58;
  float v_57;
  float v_56;
  float v_55;
  int v_54;
  int v_53;
  int v_52;
  int r_2;
  int v_68;
  Robot__st v_67;
  int v_66;
  int v_65;
  int v_64;
  int v_63;
  int v_62;
  int v_61;
  int v_60;
  int v_59;
  int r_3;
  int v_78;
  Robot__st v_77;
  int v_76;
  int v_75;
  int v_74;
  int v_73;
  int v_72;
  int v_71;
  int v_70;
  int v_69;
  int r_4;
  int v_86;
  int v_85;
  int v_84;
  Robot__st v_83;
  int v_82;
  int v_81;
  int v_80;
  int v_79;
  int r_5;
  int isStatic;
  int v_88;
  int v_87;
  int r_6;
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
  int nr_St_Jump;
  Robot__st ns_St_Jump;
  float arm4speed_St_Jump;
  float arm3speed_St_Jump;
  float arm2speed_St_Jump;
  float arm1speed_St_Jump;
  float kneeRspeed_St_Jump;
  float kneeLspeed_St_Jump;
  float hipRspeed_St_Jump;
  float hipLspeed_St_Jump;
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
  float v_46;
  float v_45;
  float v_44;
  float v_43;
  float v_40;
  float v_39;
  float v_38;
  float v_37;
  float v_36;
  float v_33;
  float v_32;
  float v_31;
  Robot__st ns;
  int r;
  int nr;
  float gyroscope;
  float dgyroscope;
  float secondary_sum_x;
  float secondary_sum_y;
  r = self->pnr;
  v_43 = (secondary_y*dt);
  v_44 = (v_43*0.500000);
  v_45 = (self->v_42+v_44);
  if (self->v_41) {
    v_46 = 0.200000;
  } else {
    v_46 = v_45;
  };
  Lib__clamp_step(v_46, -1.000000, 1.000000, &Lib__clamp_out_st);
  secondary_sum_y = Lib__clamp_out_st.x;
  v_36 = (secondary_x*dt);
  v_37 = (v_36*0.500000);
  v_38 = (self->v_35+v_37);
  if (self->v_34) {
    v_39 = 0.200000;
  } else {
    v_39 = v_38;
  };
  v_40 = -(v_39);
  Lib__clamp_step(v_40, -1.000000, 1.000000, &Lib__clamp_out_st);
  secondary_sum_x = Lib__clamp_out_st.x;
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
      v_87 = (footLcontact||footRcontact);
      v_88 = (v_87||bodyContact);
      if (v_88) {
        nr_St_Falling = true;
        ns_St_Falling = Robot__St_Standard;
      } else {
        nr_St_Falling = false;
        ns_St_Falling = Robot__St_Falling;
      };
      r_6 = r;
      if (r_6) {
        Robot__setMotorArm_reset(&self->setMotorArm_5);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -40.000000, 30.000000, 80.000000, 60.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_5);
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
      v_81 = (gyroscope<180.000000);
      v_79 = (gyroscope>=180.000000);
      r_5 = r;
      if (r_5) {
        Robot__setMotorArm_reset(&self->setMotorArm_4);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -0.000000, 0.000000, 0.000000, 0.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_4);
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
      v_85 = Robot__doNotMove_out_st.ok;
      _out->arm1speed = arm1speed_St_Upside_Down_Init;
      _out->arm2speed = arm2speed_St_Upside_Down_Init;
      _out->arm3speed = arm3speed_St_Upside_Down_Init;
      _out->arm4speed = arm4speed_St_Upside_Down_Init;
      Robot__doNotMove_step(1.000000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_86 = Robot__doNotMove_out_st.ok;
      isStatic = (v_85&&v_86);
      v_82 = (isStatic&&v_81);
      if (v_82) {
        v_84 = true;
        v_83 = Robot__St_Upside_Down_Right;
      } else {
        v_84 = false;
        v_83 = Robot__St_Upside_Down_Init;
      };
      v_80 = (isStatic&&v_79);
      if (v_80) {
        nr_St_Upside_Down_Init = true;
        ns_St_Upside_Down_Init = Robot__St_Upside_Down_Left;
      } else {
        nr_St_Upside_Down_Init = v_84;
        ns_St_Upside_Down_Init = v_83;
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
      v_75 = (dgyroscope<=1.000000);
      v_73 = (arm1angle<=-20.000000);
      v_70 = (gyroscope>=320.000000);
      v_69 = (gyroscope<=40.000000);
      v_71 = (v_69||v_70);
      r_4 = r;
      if (r_4) {
        Robot__setMotorArm_reset(&self->setMotorArm_3);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -65.000000, -0.000000, -10.000000, -30.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_3);
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
      v_72 = Robot__doNotMove_out_st.ok;
      v_74 = (v_72&&v_73);
      v_76 = (v_74&&v_75);
      if (v_76) {
        v_78 = true;
      } else {
        v_78 = false;
      };
      if (v_71) {
        nr_St_Upside_Down_Left = true;
      } else {
        nr_St_Upside_Down_Left = v_78;
      };
      if (v_76) {
        v_77 = Robot__St_Upside_Down_Right;
      } else {
        v_77 = Robot__St_Upside_Down_Left;
      };
      if (v_71) {
        ns_St_Upside_Down_Left = Robot__St_Standard;
      } else {
        ns_St_Upside_Down_Left = v_77;
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
      v_65 = (dgyroscope<=1.000000);
      v_63 = (arm1angle>=20.000000);
      v_60 = (gyroscope>=320.000000);
      v_59 = (gyroscope<=40.000000);
      v_61 = (v_59||v_60);
      r_3 = r;
      if (r_3) {
        Robot__setMotorArm_reset(&self->setMotorArm_2);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              65.000000, 0.000000, 10.000000, 30.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_2);
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
      v_62 = Robot__doNotMove_out_st.ok;
      v_64 = (v_62&&v_63);
      v_66 = (v_64&&v_65);
      if (v_66) {
        v_68 = true;
      } else {
        v_68 = false;
      };
      if (v_61) {
        nr_St_Upside_Down_Right = true;
      } else {
        nr_St_Upside_Down_Right = v_68;
      };
      if (v_66) {
        v_67 = Robot__St_Upside_Down_Left;
      } else {
        v_67 = Robot__St_Upside_Down_Right;
      };
      if (v_61) {
        ns_St_Upside_Down_Right = Robot__St_Standard;
      } else {
        ns_St_Upside_Down_Right = v_67;
      };
      ns = ns_St_Upside_Down_Right;
      nr = nr_St_Upside_Down_Right;
      break;
    case Robot__St_Jump:
      v_57 = (kneeRangle+hipRangle);
      v_58 = (v_57+gyroscope);
      Robot__setMotorFast_step(v_58, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeRspeed_St_Jump = Robot__setMotorFast_out_st.power;
      v_55 = (kneeLangle+hipLangle);
      v_56 = (v_55+gyroscope);
      Robot__setMotorFast_step(v_56, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeLspeed_St_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipRangle, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipRspeed_St_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipLangle, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipLspeed_St_Jump = Robot__setMotorFast_out_st.power;
      v_53 = !(footRcontact);
      v_52 = !(footLcontact);
      v_54 = (v_52&&v_53);
      if (v_54) {
        nr_St_Jump = true;
        ns_St_Jump = Robot__St_Falling;
      } else {
        nr_St_Jump = false;
        ns_St_Jump = Robot__St_Jump;
      };
      r_2 = r;
      if (r_2) {
        Robot__setMotorArm_reset(&self->setMotorArm_1);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -10.000000, 10.000000, -10.000000, 10.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_1);
      arm1speed_St_Jump = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_Jump = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_Jump = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_Jump = Robot__setMotorArm_out_st.arm4speed;
      _out->hipLspeed = hipLspeed_St_Jump;
      _out->hipRspeed = hipRspeed_St_Jump;
      _out->kneeLspeed = kneeLspeed_St_Jump;
      _out->kneeRspeed = kneeRspeed_St_Jump;
      _out->arm1speed = arm1speed_St_Jump;
      _out->arm2speed = arm2speed_St_Jump;
      _out->arm3speed = arm3speed_St_Jump;
      _out->arm4speed = arm4speed_St_Jump;
      ns = ns_St_Jump;
      nr = nr_St_Jump;
      break;
    case Robot__St_Standard:
      Lib__arm_ik_step(secondary_sum_x, secondary_sum_y, 0.000000, arm1angle,
                       arm2angle, arm3angle, arm4angle, 0.260000, 0.260000,
                       0.260000, 0.260000, &Lib__arm_ik_out_st);
      theta1 = Lib__arm_ik_out_st.theta1;
      theta2 = Lib__arm_ik_out_st.theta2;
      theta3 = Lib__arm_ik_out_st.theta3;
      theta4 = Lib__arm_ik_out_st.theta4;
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, 40.000000, 100.000000,
                               -100.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_Standard = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_Standard = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_Standard = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_Standard = Robot__setMotorLegs_out_st.kneeRspeed;
      if (action_a) {
        v_51 = true;
        v_50 = Robot__St_Jump;
      } else {
        v_51 = false;
        v_50 = Robot__St_Standard;
      };
      v_48 = (gyroscope<=260.000000);
      v_47 = (gyroscope>=90.000000);
      v_49 = (v_47&&v_48);
      if (v_49) {
        nr_St_Standard = true;
        ns_St_Standard = Robot__St_Upside_Down_Init;
      } else {
        nr_St_Standard = v_51;
        ns_St_Standard = v_50;
      };
      r_1 = r;
      if (r_1) {
        Robot__setMotorArm_reset(&self->setMotorArm);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              theta1, theta2, theta3, theta4,
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
  self->v_42 = secondary_sum_y;
  self->v_41 = false;
  self->v_35 = secondary_sum_x;
  self->v_34 = false;
  self->v_30 = gyroscope;
  self->v = false;;
}

