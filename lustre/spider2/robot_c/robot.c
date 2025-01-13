/* --- Generated the 13/1/2025 at 12:15 --- */
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
  
  float v_10;
  float v_9;
  float v_8;
  float v_7;
  float v_6;
  int v_5;
  int v_4;
  float v_3;
  float v;
  float delta;
  float delta2;
  v = (goal-current);
  Lib__fmod_step(v, 360.000000, &Lib__fmod_out_st);
  delta = Lib__fmod_out_st.x;
  Lib__abs_step(delta, &Lib__abs_out_st);
  v_3 = Lib__abs_out_st.x;
  v_4 = (v_3<=0.100000);
  if (v_4) {
    delta2 = 0.000000;
  } else {
    delta2 = delta;
  };
  Lib__abs_step(delta2, &Lib__abs_out_st);
  v_8 = Lib__abs_out_st.x;
  Lib__min_step(v_8, maxi, &Lib__min_out_st);
  v_9 = Lib__min_out_st.x;
  v_10 = -(v_9);
  Lib__abs_step(delta2, &Lib__abs_out_st);
  v_6 = Lib__abs_out_st.x;
  Lib__min_step(v_6, maxi, &Lib__min_out_st);
  v_7 = Lib__min_out_st.x;
  v_5 = (delta2<180.000000);
  if (v_5) {
    _out->power = v_7;
  } else {
    _out->power = v_10;
  };;
}

void Robot__setMotorFast_step(float current, float goal, float maxi,
                              Robot__setMotorFast_out* _out) {
  Lib__fmod_out Lib__fmod_out_st;
  
  float v_12;
  int v_11;
  float v;
  float delta;
  v_12 = -(maxi);
  v = (goal-current);
  Lib__fmod_step(v, 360.000000, &Lib__fmod_out_st);
  delta = Lib__fmod_out_st.x;
  v_11 = (delta<180.000000);
  if (v_11) {
    _out->power = maxi;
  } else {
    _out->power = v_12;
  };;
}

void Robot__setMotorPID_reset(Robot__setMotorPID_mem* self) {
  self->v_15 = true;
  self->v = true;
}

void Robot__setMotorPID_step(float currentAngle, float goalAngle,
                             float maxSpeed, float kp, float ki, float kd,
                             Robot__setMotorPID_out* _out,
                             Robot__setMotorPID_mem* self) {
  Lib__abs_out Lib__abs_out_st;
  
  float v_29;
  float v_28;
  int v_27;
  float v_26;
  float v_25;
  int v_24;
  float v_23;
  int v_22;
  float v_21;
  float v_20;
  float v_19;
  float v_18;
  float v_17;
  float v_14;
  float error;
  float integral;
  float derivative;
  float lastError;
  float rawSpeed;
  v_25 = -(maxSpeed);
  v_23 = -(maxSpeed);
  error = (goalAngle-currentAngle);
  lastError = error;
  v_18 = (kp*error);
  v_17 = (error-self->v_16);
  if (self->v_15) {
    derivative = 0.000000;
  } else {
    derivative = v_17;
  };
  v_21 = (kd*derivative);
  v_14 = (self->v_13+error);
  if (self->v) {
    integral = 0.000000;
  } else {
    integral = v_14;
  };
  v_19 = (ki*integral);
  v_20 = (v_18+v_19);
  rawSpeed = (v_20+v_21);
  Lib__abs_step(rawSpeed, &Lib__abs_out_st);
  v_26 = Lib__abs_out_st.x;
  v_27 = (v_26<0.100000);
  if (v_27) {
    v_28 = 0.000000;
  } else {
    v_28 = rawSpeed;
  };
  v_24 = (rawSpeed<v_23);
  if (v_24) {
    v_29 = v_25;
  } else {
    v_29 = v_28;
  };
  v_22 = (rawSpeed>maxSpeed);
  if (v_22) {
    _out->motorSpeed = maxSpeed;
  } else {
    _out->motorSpeed = v_29;
  };
  self->v_16 = lastError;
  self->v_15 = false;
  self->v_13 = integral;
  self->v = false;;
}

void Robot__doNotMove_step(float limit, float speedA, float speedB,
                           float speedC, float speedD,
                           Robot__doNotMove_out* _out) {
  
  int v_34;
  int v_33;
  int v_32;
  int v_31;
  int v_30;
  int v;
  v_34 = (speedD<=limit);
  v_32 = (speedC<=limit);
  v_30 = (speedB<=limit);
  v = (speedA<=limit);
  v_31 = (v&&v_30);
  v_33 = (v_31&&v_32);
  _out->ok = (v_33&&v_34);;
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
  self->v_45 = true;
  self->v_39 = true;
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
  
  int v_55;
  Robot__st v_54;
  int v_53;
  int v_52;
  int v_51;
  int r_1;
  float theta1;
  float theta2;
  float theta3;
  float theta4;
  float v_62;
  float v_61;
  float v_60;
  float v_59;
  int v_58;
  int v_57;
  int v_56;
  int r_2;
  int v_72;
  Robot__st v_71;
  int v_70;
  int v_69;
  int v_68;
  int v_67;
  int v_66;
  int v_65;
  int v_64;
  int v_63;
  int r_3;
  int v_82;
  Robot__st v_81;
  int v_80;
  int v_79;
  int v_78;
  int v_77;
  int v_76;
  int v_75;
  int v_74;
  int v_73;
  int r_4;
  int v_90;
  int v_89;
  int v_88;
  Robot__st v_87;
  int v_86;
  int v_85;
  int v_84;
  int v_83;
  int r_5;
  int isStatic;
  int v_92;
  int v_91;
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
  float v_50;
  float v_49;
  float v_48;
  float v_47;
  float v_44;
  float v_43;
  float v_42;
  float v_41;
  float v_38;
  float v_37;
  float v_36;
  Robot__st ns;
  int r;
  int nr;
  float gyroscope;
  float dgyroscope;
  float secondary_sum_x;
  float secondary_sum_y;
  r = self->pnr;
  v_47 = (secondary_y*dt);
  v_48 = (v_47*0.500000);
  v_49 = (self->v_46+v_48);
  if (self->v_45) {
    v_50 = 0.200000;
  } else {
    v_50 = v_49;
  };
  Lib__clamp_step(v_50, 0.250000, 1.000000, &Lib__clamp_out_st);
  secondary_sum_y = Lib__clamp_out_st.x;
  v_41 = (secondary_x*dt);
  v_42 = (v_41*0.500000);
  v_43 = (self->v_40+v_42);
  if (self->v_39) {
    v_44 = 0.000000;
  } else {
    v_44 = v_43;
  };
  Lib__clamp_step(v_44, -1.000000, 1.000000, &Lib__clamp_out_st);
  secondary_sum_x = Lib__clamp_out_st.x;
  Lib__fmod_step(gyroscopeTot, 360.000000, &Lib__fmod_out_st);
  gyroscope = Lib__fmod_out_st.x;
  v_36 = (gyroscope-self->v_35);
  Lib__abs_step(v_36, &Lib__abs_out_st);
  v_37 = Lib__abs_out_st.x;
  v_38 = (v_37/dt);
  if (self->v) {
    dgyroscope = 100.000000;
  } else {
    dgyroscope = v_38;
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
      v_91 = (footLcontact||footRcontact);
      v_92 = (v_91||bodyContact);
      if (v_92) {
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
      v_85 = (gyroscope<180.000000);
      v_83 = (gyroscope>=180.000000);
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
      v_89 = Robot__doNotMove_out_st.ok;
      _out->arm1speed = arm1speed_St_Upside_Down_Init;
      _out->arm2speed = arm2speed_St_Upside_Down_Init;
      _out->arm3speed = arm3speed_St_Upside_Down_Init;
      _out->arm4speed = arm4speed_St_Upside_Down_Init;
      Robot__doNotMove_step(1.000000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_90 = Robot__doNotMove_out_st.ok;
      isStatic = (v_89&&v_90);
      v_86 = (isStatic&&v_85);
      if (v_86) {
        v_88 = true;
        v_87 = Robot__St_Upside_Down_Right;
      } else {
        v_88 = false;
        v_87 = Robot__St_Upside_Down_Init;
      };
      v_84 = (isStatic&&v_83);
      if (v_84) {
        nr_St_Upside_Down_Init = true;
        ns_St_Upside_Down_Init = Robot__St_Upside_Down_Left;
      } else {
        nr_St_Upside_Down_Init = v_88;
        ns_St_Upside_Down_Init = v_87;
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
      v_79 = (dgyroscope<=1.000000);
      v_77 = (arm1angle<=-20.000000);
      v_74 = (gyroscope>=320.000000);
      v_73 = (gyroscope<=40.000000);
      v_75 = (v_73||v_74);
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
      v_76 = Robot__doNotMove_out_st.ok;
      v_78 = (v_76&&v_77);
      v_80 = (v_78&&v_79);
      if (v_80) {
        v_82 = true;
      } else {
        v_82 = false;
      };
      if (v_75) {
        nr_St_Upside_Down_Left = true;
      } else {
        nr_St_Upside_Down_Left = v_82;
      };
      if (v_80) {
        v_81 = Robot__St_Upside_Down_Right;
      } else {
        v_81 = Robot__St_Upside_Down_Left;
      };
      if (v_75) {
        ns_St_Upside_Down_Left = Robot__St_Standard;
      } else {
        ns_St_Upside_Down_Left = v_81;
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
      v_69 = (dgyroscope<=1.000000);
      v_67 = (arm1angle>=20.000000);
      v_64 = (gyroscope>=320.000000);
      v_63 = (gyroscope<=40.000000);
      v_65 = (v_63||v_64);
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
      v_66 = Robot__doNotMove_out_st.ok;
      v_68 = (v_66&&v_67);
      v_70 = (v_68&&v_69);
      if (v_70) {
        v_72 = true;
      } else {
        v_72 = false;
      };
      if (v_65) {
        nr_St_Upside_Down_Right = true;
      } else {
        nr_St_Upside_Down_Right = v_72;
      };
      if (v_70) {
        v_71 = Robot__St_Upside_Down_Left;
      } else {
        v_71 = Robot__St_Upside_Down_Right;
      };
      if (v_65) {
        ns_St_Upside_Down_Right = Robot__St_Standard;
      } else {
        ns_St_Upside_Down_Right = v_71;
      };
      ns = ns_St_Upside_Down_Right;
      nr = nr_St_Upside_Down_Right;
      break;
    case Robot__St_Jump:
      v_61 = (kneeRangle+hipRangle);
      v_62 = (v_61+gyroscope);
      Robot__setMotorFast_step(v_62, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeRspeed_St_Jump = Robot__setMotorFast_out_st.power;
      v_59 = (kneeLangle+hipLangle);
      v_60 = (v_59+gyroscope);
      Robot__setMotorFast_step(v_60, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeLspeed_St_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipRangle, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipRspeed_St_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipLangle, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipLspeed_St_Jump = Robot__setMotorFast_out_st.power;
      v_57 = !(footRcontact);
      v_56 = !(footLcontact);
      v_58 = (v_56&&v_57);
      if (v_58) {
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
        v_55 = true;
        v_54 = Robot__St_Jump;
      } else {
        v_55 = false;
        v_54 = Robot__St_Standard;
      };
      v_52 = (gyroscope<=260.000000);
      v_51 = (gyroscope>=90.000000);
      v_53 = (v_51&&v_52);
      if (v_53) {
        nr_St_Standard = true;
        ns_St_Standard = Robot__St_Upside_Down_Init;
      } else {
        nr_St_Standard = v_55;
        ns_St_Standard = v_54;
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
  self->v_46 = secondary_sum_y;
  self->v_45 = false;
  self->v_40 = secondary_sum_x;
  self->v_39 = false;
  self->v_35 = gyroscope;
  self->v = false;;
}

