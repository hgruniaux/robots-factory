/* --- Generated the 13/1/2025 at 17:49 --- */
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
  v_4 = (v_3<=1.000000);
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
  v_27 = (v_26<1.000000);
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
  self->v_73 = false;
  self->v_72 = Robot__St_Center;
  self->pnr_1 = false;
  self->ck = Robot__St_1_Falling;
  self->v_58 = true;
  self->v_52 = true;
  self->v_46 = true;
  self->v_40 = true;
  self->v = true;
}

void Robot__robot_step(float dt, float hipLangle, float hipRangle,
                       float kneeLangle, float kneeRangle, float arm1angle,
                       float arm2angle, float arm3angle, float arm4angle,
                       float gyroscope, float calfLdistance,
                       float calfRdistance, float bodyDistance,
                       int footLcontact, int footRcontact, int bodyContact,
                       float main_x, float main_y, float secondary_x,
                       float secondary_y, int action_a, int action_x,
                       int action_b, int action_lb, int action_rb,
                       Robot__robot_out* _out, Robot__robot_mem* self) {
  Lib__arm_ik_out Lib__arm_ik_out_st;
  Lib__abs_out Lib__abs_out_st;
  Robot__mux_out Robot__mux_out_st;
  Robot__setMotorLegs_out Robot__setMotorLegs_out_st;
  Lib__fmod_out Lib__fmod_out_st;
  Lib__clamp_out Lib__clamp_out_st;
  Robot__setMotorFast_out Robot__setMotorFast_out_st;
  Robot__doNotMove_out Robot__doNotMove_out_st;
  Robot__setMotorArm_out Robot__setMotorArm_out_st;
  
  int nr_St_Center;
  Robot__st ns_St_Center;
  float kneeRspeed_St_1_Standard_St_Center;
  float kneeLspeed_St_1_Standard_St_Center;
  float hipRspeed_St_1_Standard_St_Center;
  float hipLspeed_St_1_Standard_St_Center;
  Robot__st ck_1;
  int v_71;
  Robot__st_1 v_70;
  int v_69;
  int v_68;
  int v_67;
  int r_4;
  Robot__st ns;
  int r_2;
  int nr;
  int pnr;
  float theta1;
  float theta2;
  float theta3;
  float theta4;
  float v_80;
  float v_79;
  float v_78;
  float v_77;
  int v_76;
  int v_75;
  int v_74;
  int r_5;
  float v_94;
  int v_92;
  Robot__st_1 v_91;
  int v_90;
  int v_89;
  int v_88;
  int v_87;
  int v_86;
  int v_85;
  int v_84;
  int v_83;
  int v_82;
  int v_81;
  int r_6;
  float time_1;
  float v_108;
  int v_106;
  Robot__st_1 v_105;
  int v_104;
  int v_103;
  int v_102;
  int v_101;
  int v_100;
  int v_99;
  int v_98;
  int v_97;
  int v_96;
  int v_95;
  int r_7;
  float time;
  int v_125;
  int v_124;
  int v_123;
  Robot__st_1 v_122;
  int v_121;
  Robot__st_1 v_120;
  int v_119;
  int v_118;
  int v_117;
  int v_116;
  int v_115;
  int v_114;
  int v_113;
  int v_112;
  int v_111;
  int v_110;
  int v_109;
  int r_8;
  int isStatic;
  int v_127;
  int v_126;
  int r_9;
  int nr_1_St_1_Standard;
  Robot__st_1 ns_1_St_1_Standard;
  float arm4speed_St_1_Standard;
  float arm3speed_St_1_Standard;
  float arm2speed_St_1_Standard;
  float arm1speed_St_1_Standard;
  float kneeRspeed_St_1_Standard;
  float kneeLspeed_St_1_Standard;
  float hipRspeed_St_1_Standard;
  float hipLspeed_St_1_Standard;
  int nr_1_St_1_Jump;
  Robot__st_1 ns_1_St_1_Jump;
  float arm4speed_St_1_Jump;
  float arm3speed_St_1_Jump;
  float arm2speed_St_1_Jump;
  float arm1speed_St_1_Jump;
  float kneeRspeed_St_1_Jump;
  float kneeLspeed_St_1_Jump;
  float hipRspeed_St_1_Jump;
  float hipLspeed_St_1_Jump;
  int nr_1_St_1_Upside_Down_Right;
  Robot__st_1 ns_1_St_1_Upside_Down_Right;
  float arm4speed_St_1_Upside_Down_Right;
  float arm3speed_St_1_Upside_Down_Right;
  float arm2speed_St_1_Upside_Down_Right;
  float arm1speed_St_1_Upside_Down_Right;
  float kneeRspeed_St_1_Upside_Down_Right;
  float kneeLspeed_St_1_Upside_Down_Right;
  float hipRspeed_St_1_Upside_Down_Right;
  float hipLspeed_St_1_Upside_Down_Right;
  int nr_1_St_1_Upside_Down_Left;
  Robot__st_1 ns_1_St_1_Upside_Down_Left;
  float arm4speed_St_1_Upside_Down_Left;
  float arm3speed_St_1_Upside_Down_Left;
  float arm2speed_St_1_Upside_Down_Left;
  float arm1speed_St_1_Upside_Down_Left;
  float kneeRspeed_St_1_Upside_Down_Left;
  float kneeLspeed_St_1_Upside_Down_Left;
  float hipRspeed_St_1_Upside_Down_Left;
  float hipLspeed_St_1_Upside_Down_Left;
  int nr_1_St_1_Upside_Down_Init;
  Robot__st_1 ns_1_St_1_Upside_Down_Init;
  float arm4speed_St_1_Upside_Down_Init;
  float arm3speed_St_1_Upside_Down_Init;
  float arm2speed_St_1_Upside_Down_Init;
  float arm1speed_St_1_Upside_Down_Init;
  float kneeRspeed_St_1_Upside_Down_Init;
  float kneeLspeed_St_1_Upside_Down_Init;
  float hipRspeed_St_1_Upside_Down_Init;
  float hipLspeed_St_1_Upside_Down_Init;
  int nr_1_St_1_Falling;
  Robot__st_1 ns_1_St_1_Falling;
  float arm4speed_St_1_Falling;
  float arm3speed_St_1_Falling;
  float arm2speed_St_1_Falling;
  float arm1speed_St_1_Falling;
  float kneeRspeed_St_1_Falling;
  float kneeLspeed_St_1_Falling;
  float hipRspeed_St_1_Falling;
  float hipLspeed_St_1_Falling;
  float v_66;
  float v_65;
  float v_64;
  float v_63;
  float v_62;
  float v_61;
  float v_60;
  float v_57;
  float v_56;
  float v_55;
  float v_54;
  float v_51;
  float v_50;
  float v_49;
  float v_48;
  float v_45;
  float v_44;
  float v_43;
  float v_42;
  float v_39;
  float v_38;
  float v_37;
  float v_36;
  Robot__st_1 ns_1;
  int r_3;
  int nr_1;
  float dgyroscope;
  float body_target_y;
  float alti;
  r_3 = self->pnr_1;
  Robot__mux_step(action_rb, -1.000000, 0.000000, &Robot__mux_out_st);
  v_61 = Robot__mux_out_st.r;
  Robot__mux_step(action_lb, 1.000000, 0.000000, &Robot__mux_out_st);
  v_60 = Robot__mux_out_st.r;
  v_62 = (v_60+v_61);
  v_63 = (v_62*dt);
  v_64 = (v_63*10.000000);
  v_65 = (self->v_59+v_64);
  Lib__fmod_step(v_65, 360.000000, &Lib__fmod_out_st);
  v_66 = Lib__fmod_out_st.x;
  if (self->v_58) {
    _out->arm_target_angle = 0.000000;
  } else {
    _out->arm_target_angle = v_66;
  };
  v_54 = (main_y*dt);
  v_55 = (v_54*0.500000);
  v_56 = (self->v_53+v_55);
  if (self->v_52) {
    v_57 = 0.200000;
  } else {
    v_57 = v_56;
  };
  Lib__clamp_step(v_57, 0.000000, 0.400000, &Lib__clamp_out_st);
  body_target_y = Lib__clamp_out_st.x;
  v_48 = (secondary_y*dt);
  v_49 = (v_48*0.500000);
  v_50 = (self->v_47+v_49);
  if (self->v_46) {
    v_51 = 0.200000;
  } else {
    v_51 = v_50;
  };
  Lib__clamp_step(v_51, 0.250000, 1.000000, &Lib__clamp_out_st);
  _out->arm_target_y = Lib__clamp_out_st.x;
  v_42 = (secondary_x*dt);
  v_43 = (v_42*0.500000);
  v_44 = (self->v_41+v_43);
  if (self->v_40) {
    v_45 = 0.000000;
  } else {
    v_45 = v_44;
  };
  Lib__clamp_step(v_45, -1.000000, 1.000000, &Lib__clamp_out_st);
  _out->arm_target_x = Lib__clamp_out_st.x;
  alti = bodyDistance;
  v_36 = (gyroscope-self->v_35);
  Lib__fmod_step(v_36, 360.000000, &Lib__fmod_out_st);
  v_37 = Lib__fmod_out_st.x;
  Lib__abs_step(v_37, &Lib__abs_out_st);
  v_38 = Lib__abs_out_st.x;
  v_39 = (v_38/dt);
  if (self->v) {
    dgyroscope = 100.000000;
  } else {
    dgyroscope = v_39;
  };
  switch (self->ck) {
    case Robot__St_1_Falling:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, 40.000000, 100.000000,
                               -100.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_1_Falling = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_1_Falling = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_1_Falling = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_1_Falling = Robot__setMotorLegs_out_st.kneeRspeed;
      v_126 = (footLcontact||footRcontact);
      v_127 = (v_126||bodyContact);
      if (v_127) {
        nr_1_St_1_Falling = true;
        ns_1_St_1_Falling = Robot__St_1_Standard;
      } else {
        nr_1_St_1_Falling = false;
        ns_1_St_1_Falling = Robot__St_1_Falling;
      };
      r_9 = r_3;
      if (r_9) {
        Robot__setMotorArm_reset(&self->setMotorArm_5);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -40.000000, 30.000000, 80.000000, 60.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_5);
      arm1speed_St_1_Falling = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_1_Falling = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_1_Falling = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_1_Falling = Robot__setMotorArm_out_st.arm4speed;
      _out->arm1speed = arm1speed_St_1_Falling;
      _out->arm2speed = arm2speed_St_1_Falling;
      _out->arm3speed = arm3speed_St_1_Falling;
      _out->arm4speed = arm4speed_St_1_Falling;
      _out->hipLspeed = hipLspeed_St_1_Falling;
      _out->hipRspeed = hipRspeed_St_1_Falling;
      _out->kneeLspeed = kneeLspeed_St_1_Falling;
      _out->kneeRspeed = kneeRspeed_St_1_Falling;
      ns_1 = ns_1_St_1_Falling;
      nr_1 = nr_1_St_1_Falling;
      break;
    case Robot__St_1_Upside_Down_Init:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, 40.000000, 100.000000,
                               -100.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_1_Upside_Down_Init = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_1_Upside_Down_Init = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_1_Upside_Down_Init = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_1_Upside_Down_Init = Robot__setMotorLegs_out_st.kneeRspeed;
      v_118 = (gyroscope>40.000000);
      v_116 = (gyroscope<180.000000);
      v_114 = (gyroscope<320.000000);
      v_112 = (gyroscope>=180.000000);
      v_110 = (gyroscope>=320.000000);
      v_109 = (gyroscope<=40.000000);
      v_111 = (v_109&&v_110);
      r_8 = r_3;
      if (r_8) {
        Robot__setMotorArm_reset(&self->setMotorArm_4);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -0.000000, 0.000000, 0.000000, 0.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_4);
      arm1speed_St_1_Upside_Down_Init = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_1_Upside_Down_Init = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_1_Upside_Down_Init = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_1_Upside_Down_Init = Robot__setMotorArm_out_st.arm4speed;
      _out->arm1speed = arm1speed_St_1_Upside_Down_Init;
      _out->arm2speed = arm2speed_St_1_Upside_Down_Init;
      _out->arm3speed = arm3speed_St_1_Upside_Down_Init;
      _out->arm4speed = arm4speed_St_1_Upside_Down_Init;
      Robot__doNotMove_step(1.000000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_125 = Robot__doNotMove_out_st.ok;
      _out->hipLspeed = hipLspeed_St_1_Upside_Down_Init;
      _out->hipRspeed = hipRspeed_St_1_Upside_Down_Init;
      _out->kneeLspeed = kneeLspeed_St_1_Upside_Down_Init;
      _out->kneeRspeed = kneeRspeed_St_1_Upside_Down_Init;
      Robot__doNotMove_step(1.000000, _out->hipLspeed, _out->hipRspeed,
                            _out->kneeLspeed, _out->kneeRspeed,
                            &Robot__doNotMove_out_st);
      v_124 = Robot__doNotMove_out_st.ok;
      isStatic = (v_124&&v_125);
      v_117 = (isStatic&&v_116);
      v_119 = (v_117&&v_118);
      if (v_119) {
        v_121 = true;
        v_120 = Robot__St_1_Upside_Down_Right;
      } else {
        v_121 = false;
        v_120 = Robot__St_1_Upside_Down_Init;
      };
      v_113 = (isStatic&&v_112);
      v_115 = (v_113&&v_114);
      if (v_115) {
        v_123 = true;
      } else {
        v_123 = v_121;
      };
      if (v_111) {
        nr_1_St_1_Upside_Down_Init = true;
      } else {
        nr_1_St_1_Upside_Down_Init = v_123;
      };
      if (v_115) {
        v_122 = Robot__St_1_Upside_Down_Left;
      } else {
        v_122 = v_120;
      };
      if (v_111) {
        ns_1_St_1_Upside_Down_Init = Robot__St_1_Standard;
      } else {
        ns_1_St_1_Upside_Down_Init = v_122;
      };
      ns_1 = ns_1_St_1_Upside_Down_Init;
      nr_1 = nr_1_St_1_Upside_Down_Init;
      break;
    case Robot__St_1_Upside_Down_Left:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               60.000000, 40.000000, 40.000000, -100.000000,
                               &Robot__setMotorLegs_out_st);
      hipLspeed_St_1_Upside_Down_Left = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_1_Upside_Down_Left = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_1_Upside_Down_Left = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_1_Upside_Down_Left = Robot__setMotorLegs_out_st.kneeRspeed;
      v_108 = (self->v_107+dt);
      if (r_3) {
        time = 0.000000;
      } else {
        time = v_108;
      };
      v_103 = (time>=5.000000);
      v_101 = (dgyroscope<=1.000000);
      v_99 = (arm1angle<=-20.000000);
      v_96 = (gyroscope>=320.000000);
      v_95 = (gyroscope<=40.000000);
      v_97 = (v_95||v_96);
      r_7 = r_3;
      if (r_7) {
        Robot__setMotorArm_reset(&self->setMotorArm_3);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -65.000000, -0.000000, -10.000000, -30.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_3);
      arm1speed_St_1_Upside_Down_Left = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_1_Upside_Down_Left = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_1_Upside_Down_Left = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_1_Upside_Down_Left = Robot__setMotorArm_out_st.arm4speed;
      _out->arm1speed = arm1speed_St_1_Upside_Down_Left;
      _out->arm2speed = arm2speed_St_1_Upside_Down_Left;
      _out->arm3speed = arm3speed_St_1_Upside_Down_Left;
      _out->arm4speed = arm4speed_St_1_Upside_Down_Left;
      Robot__doNotMove_step(0.100000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_98 = Robot__doNotMove_out_st.ok;
      v_100 = (v_98&&v_99);
      v_102 = (v_100&&v_101);
      v_104 = (v_102&&v_103);
      if (v_104) {
        v_106 = true;
      } else {
        v_106 = false;
      };
      if (v_97) {
        nr_1_St_1_Upside_Down_Left = true;
      } else {
        nr_1_St_1_Upside_Down_Left = v_106;
      };
      if (v_104) {
        v_105 = Robot__St_1_Upside_Down_Right;
      } else {
        v_105 = Robot__St_1_Upside_Down_Left;
      };
      if (v_97) {
        ns_1_St_1_Upside_Down_Left = Robot__St_1_Standard;
      } else {
        ns_1_St_1_Upside_Down_Left = v_105;
      };
      _out->hipLspeed = hipLspeed_St_1_Upside_Down_Left;
      _out->hipRspeed = hipRspeed_St_1_Upside_Down_Left;
      _out->kneeLspeed = kneeLspeed_St_1_Upside_Down_Left;
      _out->kneeRspeed = kneeRspeed_St_1_Upside_Down_Left;
      ns_1 = ns_1_St_1_Upside_Down_Left;
      nr_1 = nr_1_St_1_Upside_Down_Left;
      self->v_107 = time;
      break;
    case Robot__St_1_Upside_Down_Right:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, -60.000000, 100.000000,
                               -40.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_1_Upside_Down_Right = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_1_Upside_Down_Right = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_1_Upside_Down_Right = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_1_Upside_Down_Right = Robot__setMotorLegs_out_st.kneeRspeed;
      v_94 = (self->v_93+dt);
      if (r_3) {
        time_1 = 0.000000;
      } else {
        time_1 = v_94;
      };
      v_89 = (time_1>=5.000000);
      v_87 = (dgyroscope<=1.000000);
      v_85 = (arm1angle>=20.000000);
      v_82 = (gyroscope>=320.000000);
      v_81 = (gyroscope<=40.000000);
      v_83 = (v_81||v_82);
      r_6 = r_3;
      if (r_6) {
        Robot__setMotorArm_reset(&self->setMotorArm_2);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              65.000000, 0.000000, 10.000000, 30.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_2);
      arm1speed_St_1_Upside_Down_Right = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_1_Upside_Down_Right = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_1_Upside_Down_Right = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_1_Upside_Down_Right = Robot__setMotorArm_out_st.arm4speed;
      _out->arm1speed = arm1speed_St_1_Upside_Down_Right;
      _out->arm2speed = arm2speed_St_1_Upside_Down_Right;
      _out->arm3speed = arm3speed_St_1_Upside_Down_Right;
      _out->arm4speed = arm4speed_St_1_Upside_Down_Right;
      Robot__doNotMove_step(0.100000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_84 = Robot__doNotMove_out_st.ok;
      v_86 = (v_84&&v_85);
      v_88 = (v_86&&v_87);
      v_90 = (v_88&&v_89);
      if (v_90) {
        v_92 = true;
      } else {
        v_92 = false;
      };
      if (v_83) {
        nr_1_St_1_Upside_Down_Right = true;
      } else {
        nr_1_St_1_Upside_Down_Right = v_92;
      };
      if (v_90) {
        v_91 = Robot__St_1_Upside_Down_Left;
      } else {
        v_91 = Robot__St_1_Upside_Down_Right;
      };
      if (v_83) {
        ns_1_St_1_Upside_Down_Right = Robot__St_1_Standard;
      } else {
        ns_1_St_1_Upside_Down_Right = v_91;
      };
      _out->hipLspeed = hipLspeed_St_1_Upside_Down_Right;
      _out->hipRspeed = hipRspeed_St_1_Upside_Down_Right;
      _out->kneeLspeed = kneeLspeed_St_1_Upside_Down_Right;
      _out->kneeRspeed = kneeRspeed_St_1_Upside_Down_Right;
      ns_1 = ns_1_St_1_Upside_Down_Right;
      nr_1 = nr_1_St_1_Upside_Down_Right;
      self->v_93 = time_1;
      break;
    case Robot__St_1_Jump:
      v_79 = (kneeRangle+hipRangle);
      v_80 = (v_79+gyroscope);
      Robot__setMotorFast_step(v_80, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeRspeed_St_1_Jump = Robot__setMotorFast_out_st.power;
      v_77 = (kneeLangle+hipLangle);
      v_78 = (v_77+gyroscope);
      Robot__setMotorFast_step(v_78, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeLspeed_St_1_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipRangle, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipRspeed_St_1_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipLangle, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipLspeed_St_1_Jump = Robot__setMotorFast_out_st.power;
      v_75 = !(footRcontact);
      v_74 = !(footLcontact);
      v_76 = (v_74&&v_75);
      if (v_76) {
        nr_1_St_1_Jump = true;
        ns_1_St_1_Jump = Robot__St_1_Falling;
      } else {
        nr_1_St_1_Jump = false;
        ns_1_St_1_Jump = Robot__St_1_Jump;
      };
      r_5 = r_3;
      if (r_5) {
        Robot__setMotorArm_reset(&self->setMotorArm_1);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -10.000000, 10.000000, -10.000000, 10.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_1);
      arm1speed_St_1_Jump = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_1_Jump = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_1_Jump = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_1_Jump = Robot__setMotorArm_out_st.arm4speed;
      _out->arm1speed = arm1speed_St_1_Jump;
      _out->arm2speed = arm2speed_St_1_Jump;
      _out->arm3speed = arm3speed_St_1_Jump;
      _out->arm4speed = arm4speed_St_1_Jump;
      _out->hipLspeed = hipLspeed_St_1_Jump;
      _out->hipRspeed = hipRspeed_St_1_Jump;
      _out->kneeLspeed = kneeLspeed_St_1_Jump;
      _out->kneeRspeed = kneeRspeed_St_1_Jump;
      ns_1 = ns_1_St_1_Jump;
      nr_1 = nr_1_St_1_Jump;
      break;
    case Robot__St_1_Standard:
      Lib__arm_ik_step(_out->arm_target_x, _out->arm_target_y, 0.000000,
                       arm1angle, arm2angle, arm3angle, arm4angle, 0.260000,
                       0.260000, 0.260000, 0.260000, &Lib__arm_ik_out_st);
      theta1 = Lib__arm_ik_out_st.theta1;
      theta2 = Lib__arm_ik_out_st.theta2;
      theta3 = Lib__arm_ik_out_st.theta3;
      theta4 = Lib__arm_ik_out_st.theta4;
      if (r_3) {
        pnr = false;
      } else {
        pnr = self->v_73;
      };
      r_2 = pnr;
      if (r_3) {
        ck_1 = Robot__St_Center;
      } else {
        ck_1 = self->v_72;
      };
      if (action_a) {
        v_71 = true;
        v_70 = Robot__St_1_Jump;
      } else {
        v_71 = false;
        v_70 = Robot__St_1_Standard;
      };
      v_68 = (gyroscope<=260.000000);
      v_67 = (gyroscope>=90.000000);
      v_69 = (v_67&&v_68);
      if (v_69) {
        nr_1_St_1_Standard = true;
        ns_1_St_1_Standard = Robot__St_1_Upside_Down_Init;
      } else {
        nr_1_St_1_Standard = v_71;
        ns_1_St_1_Standard = v_70;
      };
      r_4 = r_3;
      if (r_4) {
        Robot__setMotorArm_reset(&self->setMotorArm);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              theta1, theta2, theta3, theta4,
                              &Robot__setMotorArm_out_st, &self->setMotorArm);
      arm1speed_St_1_Standard = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_1_Standard = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_1_Standard = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_1_Standard = Robot__setMotorArm_out_st.arm4speed;
      _out->arm1speed = arm1speed_St_1_Standard;
      _out->arm2speed = arm2speed_St_1_Standard;
      _out->arm3speed = arm3speed_St_1_Standard;
      _out->arm4speed = arm4speed_St_1_Standard;
      switch (ck_1) {
        case Robot__St_Center:
          Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle,
                                   kneeRangle, -40.000000, 40.000000,
                                   100.000000, -100.000000,
                                   &Robot__setMotorLegs_out_st);
          hipLspeed_St_1_Standard_St_Center = Robot__setMotorLegs_out_st.hipLspeed;
          hipRspeed_St_1_Standard_St_Center = Robot__setMotorLegs_out_st.hipRspeed;
          kneeLspeed_St_1_Standard_St_Center = Robot__setMotorLegs_out_st.kneeLspeed;
          kneeRspeed_St_1_Standard_St_Center = Robot__setMotorLegs_out_st.kneeRspeed;
          nr_St_Center = false;
          ns_St_Center = Robot__St_Center;
          hipLspeed_St_1_Standard = hipLspeed_St_1_Standard_St_Center;
          hipRspeed_St_1_Standard = hipRspeed_St_1_Standard_St_Center;
          kneeLspeed_St_1_Standard = kneeLspeed_St_1_Standard_St_Center;
          kneeRspeed_St_1_Standard = kneeRspeed_St_1_Standard_St_Center;
          ns = ns_St_Center;
          nr = nr_St_Center;
          break;
        default:
          break;
      };
      _out->hipLspeed = hipLspeed_St_1_Standard;
      _out->hipRspeed = hipRspeed_St_1_Standard;
      _out->kneeLspeed = kneeLspeed_St_1_Standard;
      _out->kneeRspeed = kneeRspeed_St_1_Standard;
      ns_1 = ns_1_St_1_Standard;
      nr_1 = nr_1_St_1_Standard;
      self->v_73 = nr;
      self->v_72 = ns;
      break;
    default:
      break;
  };
  self->pnr_1 = nr_1;
  self->ck = ns_1;
  self->v_59 = _out->arm_target_angle;
  self->v_58 = false;
  self->v_53 = body_target_y;
  self->v_52 = false;
  self->v_47 = _out->arm_target_y;
  self->v_46 = false;
  self->v_41 = _out->arm_target_x;
  self->v_40 = false;
  self->v_35 = gyroscope;
  self->v = false;;
}

