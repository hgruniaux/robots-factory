/* --- Generated the 14/1/2025 at 13:7 --- */
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
  Robot__setMotorArm_reset(&self->setMotorArm_6);
  Robot__setMotorArm_reset(&self->setMotorArm_5);
  Robot__setMotorArm_reset(&self->setMotorArm_4);
  Robot__setMotorArm_reset(&self->setMotorArm_3);
  Robot__setMotorArm_reset(&self->setMotorArm_2);
  Robot__setMotorArm_reset(&self->setMotorArm_1);
  Robot__setMotorArm_reset(&self->setMotorArm);
  self->v_76 = false;
  self->v_75 = Robot__St_Normal;
  self->v_73 = false;
  self->v_72 = Robot__St_1_Center;
  self->pnr_2 = false;
  self->ck = Robot__St_2_Falling;
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
  
  int nr_1_St_1_Center;
  Robot__st_1 ns_1_St_1_Center;
  float kneeRspeed_St_2_Standard_St_1_Center;
  float kneeLspeed_St_2_Standard_St_1_Center;
  float hipRspeed_St_2_Standard_St_1_Center;
  float hipLspeed_St_2_Standard_St_1_Center;
  Robot__st_1 ck_1;
  int v_74;
  int r_5;
  float theta1;
  float theta2;
  float theta3;
  float theta4;
  int r_6;
  int nr_St_Move;
  Robot__st ns_St_Move;
  float arm4speed_St_2_Standard_St_Move;
  float arm3speed_St_2_Standard_St_Move;
  float arm2speed_St_2_Standard_St_Move;
  float arm1speed_St_2_Standard_St_Move;
  int nr_St_Normal;
  Robot__st ns_St_Normal;
  float arm4speed_St_2_Standard_St_Normal;
  float arm3speed_St_2_Standard_St_Normal;
  float arm2speed_St_2_Standard_St_Normal;
  float arm1speed_St_2_Standard_St_Normal;
  Robot__st ck_2;
  int v_71;
  Robot__st_2 v_70;
  int v_69;
  int v_68;
  int v_67;
  Robot__st_1 ns_1;
  int r_3;
  int nr_1;
  int pnr_1;
  Robot__st ns;
  int r_2;
  int nr;
  int pnr;
  float v_83;
  float v_82;
  float v_81;
  float v_80;
  int v_79;
  int v_78;
  int v_77;
  int r_7;
  float v_97;
  int v_95;
  Robot__st_2 v_94;
  int v_93;
  int v_92;
  int v_91;
  int v_90;
  int v_89;
  int v_88;
  int v_87;
  int v_86;
  int v_85;
  int v_84;
  int r_8;
  float time_1;
  float v_111;
  int v_109;
  Robot__st_2 v_108;
  int v_107;
  int v_106;
  int v_105;
  int v_104;
  int v_103;
  int v_102;
  int v_101;
  int v_100;
  int v_99;
  int v_98;
  int r_9;
  float time;
  int v_128;
  int v_127;
  int v_126;
  Robot__st_2 v_125;
  int v_124;
  Robot__st_2 v_123;
  int v_122;
  int v_121;
  int v_120;
  int v_119;
  int v_118;
  int v_117;
  int v_116;
  int v_115;
  int v_114;
  int v_113;
  int v_112;
  int r_10;
  int isStatic;
  int v_130;
  int v_129;
  int r_11;
  int nr_2_St_2_Standard;
  Robot__st_2 ns_2_St_2_Standard;
  float arm4speed_St_2_Standard;
  float arm3speed_St_2_Standard;
  float arm2speed_St_2_Standard;
  float arm1speed_St_2_Standard;
  float kneeRspeed_St_2_Standard;
  float kneeLspeed_St_2_Standard;
  float hipRspeed_St_2_Standard;
  float hipLspeed_St_2_Standard;
  int nr_2_St_2_Jump;
  Robot__st_2 ns_2_St_2_Jump;
  float arm4speed_St_2_Jump;
  float arm3speed_St_2_Jump;
  float arm2speed_St_2_Jump;
  float arm1speed_St_2_Jump;
  float kneeRspeed_St_2_Jump;
  float kneeLspeed_St_2_Jump;
  float hipRspeed_St_2_Jump;
  float hipLspeed_St_2_Jump;
  int nr_2_St_2_Upside_Down_Right;
  Robot__st_2 ns_2_St_2_Upside_Down_Right;
  float arm4speed_St_2_Upside_Down_Right;
  float arm3speed_St_2_Upside_Down_Right;
  float arm2speed_St_2_Upside_Down_Right;
  float arm1speed_St_2_Upside_Down_Right;
  float kneeRspeed_St_2_Upside_Down_Right;
  float kneeLspeed_St_2_Upside_Down_Right;
  float hipRspeed_St_2_Upside_Down_Right;
  float hipLspeed_St_2_Upside_Down_Right;
  int nr_2_St_2_Upside_Down_Left;
  Robot__st_2 ns_2_St_2_Upside_Down_Left;
  float arm4speed_St_2_Upside_Down_Left;
  float arm3speed_St_2_Upside_Down_Left;
  float arm2speed_St_2_Upside_Down_Left;
  float arm1speed_St_2_Upside_Down_Left;
  float kneeRspeed_St_2_Upside_Down_Left;
  float kneeLspeed_St_2_Upside_Down_Left;
  float hipRspeed_St_2_Upside_Down_Left;
  float hipLspeed_St_2_Upside_Down_Left;
  int nr_2_St_2_Upside_Down_Init;
  Robot__st_2 ns_2_St_2_Upside_Down_Init;
  float arm4speed_St_2_Upside_Down_Init;
  float arm3speed_St_2_Upside_Down_Init;
  float arm2speed_St_2_Upside_Down_Init;
  float arm1speed_St_2_Upside_Down_Init;
  float kneeRspeed_St_2_Upside_Down_Init;
  float kneeLspeed_St_2_Upside_Down_Init;
  float hipRspeed_St_2_Upside_Down_Init;
  float hipLspeed_St_2_Upside_Down_Init;
  int nr_2_St_2_Falling;
  Robot__st_2 ns_2_St_2_Falling;
  float arm4speed_St_2_Falling;
  float arm3speed_St_2_Falling;
  float arm2speed_St_2_Falling;
  float arm1speed_St_2_Falling;
  float kneeRspeed_St_2_Falling;
  float kneeLspeed_St_2_Falling;
  float hipRspeed_St_2_Falling;
  float hipLspeed_St_2_Falling;
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
  Robot__st_2 ns_2;
  int r_4;
  int nr_2;
  float dgyroscope;
  float body_target_y;
  float alti;
  r_4 = self->pnr_2;
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
    case Robot__St_2_Falling:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, 40.000000, 100.000000,
                               -100.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_2_Falling = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_2_Falling = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_2_Falling = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_2_Falling = Robot__setMotorLegs_out_st.kneeRspeed;
      v_129 = (footLcontact||footRcontact);
      v_130 = (v_129||bodyContact);
      if (v_130) {
        nr_2_St_2_Falling = true;
        ns_2_St_2_Falling = Robot__St_2_Standard;
      } else {
        nr_2_St_2_Falling = false;
        ns_2_St_2_Falling = Robot__St_2_Falling;
      };
      r_11 = r_4;
      if (r_11) {
        Robot__setMotorArm_reset(&self->setMotorArm_6);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -40.000000, 30.000000, 80.000000, 60.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_6);
      arm1speed_St_2_Falling = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_2_Falling = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_2_Falling = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_2_Falling = Robot__setMotorArm_out_st.arm4speed;
      _out->hipLspeed = hipLspeed_St_2_Falling;
      _out->hipRspeed = hipRspeed_St_2_Falling;
      _out->kneeLspeed = kneeLspeed_St_2_Falling;
      _out->kneeRspeed = kneeRspeed_St_2_Falling;
      _out->arm1speed = arm1speed_St_2_Falling;
      _out->arm2speed = arm2speed_St_2_Falling;
      _out->arm3speed = arm3speed_St_2_Falling;
      _out->arm4speed = arm4speed_St_2_Falling;
      ns_2 = ns_2_St_2_Falling;
      nr_2 = nr_2_St_2_Falling;
      break;
    case Robot__St_2_Upside_Down_Init:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, 40.000000, 100.000000,
                               -100.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_2_Upside_Down_Init = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_2_Upside_Down_Init = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_2_Upside_Down_Init = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_2_Upside_Down_Init = Robot__setMotorLegs_out_st.kneeRspeed;
      v_121 = (gyroscope>40.000000);
      v_119 = (gyroscope<180.000000);
      v_117 = (gyroscope<320.000000);
      v_115 = (gyroscope>=180.000000);
      v_113 = (gyroscope>=320.000000);
      v_112 = (gyroscope<=40.000000);
      v_114 = (v_112&&v_113);
      r_10 = r_4;
      if (r_10) {
        Robot__setMotorArm_reset(&self->setMotorArm_5);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -0.000000, 0.000000, 0.000000, 0.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_5);
      arm1speed_St_2_Upside_Down_Init = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_2_Upside_Down_Init = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_2_Upside_Down_Init = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_2_Upside_Down_Init = Robot__setMotorArm_out_st.arm4speed;
      _out->hipLspeed = hipLspeed_St_2_Upside_Down_Init;
      _out->hipRspeed = hipRspeed_St_2_Upside_Down_Init;
      _out->kneeLspeed = kneeLspeed_St_2_Upside_Down_Init;
      _out->kneeRspeed = kneeRspeed_St_2_Upside_Down_Init;
      Robot__doNotMove_step(1.000000, _out->hipLspeed, _out->hipRspeed,
                            _out->kneeLspeed, _out->kneeRspeed,
                            &Robot__doNotMove_out_st);
      v_127 = Robot__doNotMove_out_st.ok;
      _out->arm1speed = arm1speed_St_2_Upside_Down_Init;
      _out->arm2speed = arm2speed_St_2_Upside_Down_Init;
      _out->arm3speed = arm3speed_St_2_Upside_Down_Init;
      _out->arm4speed = arm4speed_St_2_Upside_Down_Init;
      Robot__doNotMove_step(1.000000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_128 = Robot__doNotMove_out_st.ok;
      isStatic = (v_127&&v_128);
      v_120 = (isStatic&&v_119);
      v_122 = (v_120&&v_121);
      if (v_122) {
        v_124 = true;
        v_123 = Robot__St_2_Upside_Down_Right;
      } else {
        v_124 = false;
        v_123 = Robot__St_2_Upside_Down_Init;
      };
      v_116 = (isStatic&&v_115);
      v_118 = (v_116&&v_117);
      if (v_118) {
        v_126 = true;
      } else {
        v_126 = v_124;
      };
      if (v_114) {
        nr_2_St_2_Upside_Down_Init = true;
      } else {
        nr_2_St_2_Upside_Down_Init = v_126;
      };
      if (v_118) {
        v_125 = Robot__St_2_Upside_Down_Left;
      } else {
        v_125 = v_123;
      };
      if (v_114) {
        ns_2_St_2_Upside_Down_Init = Robot__St_2_Standard;
      } else {
        ns_2_St_2_Upside_Down_Init = v_125;
      };
      ns_2 = ns_2_St_2_Upside_Down_Init;
      nr_2 = nr_2_St_2_Upside_Down_Init;
      break;
    case Robot__St_2_Upside_Down_Left:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               60.000000, 40.000000, 40.000000, -100.000000,
                               &Robot__setMotorLegs_out_st);
      hipLspeed_St_2_Upside_Down_Left = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_2_Upside_Down_Left = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_2_Upside_Down_Left = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_2_Upside_Down_Left = Robot__setMotorLegs_out_st.kneeRspeed;
      v_111 = (self->v_110+dt);
      if (r_4) {
        time = 0.000000;
      } else {
        time = v_111;
      };
      v_106 = (time>=5.000000);
      v_104 = (dgyroscope<=1.000000);
      v_102 = (arm1angle<=-20.000000);
      v_99 = (gyroscope>=320.000000);
      v_98 = (gyroscope<=40.000000);
      v_100 = (v_98||v_99);
      r_9 = r_4;
      if (r_9) {
        Robot__setMotorArm_reset(&self->setMotorArm_4);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -65.000000, -0.000000, -10.000000, -30.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_4);
      arm1speed_St_2_Upside_Down_Left = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_2_Upside_Down_Left = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_2_Upside_Down_Left = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_2_Upside_Down_Left = Robot__setMotorArm_out_st.arm4speed;
      _out->hipLspeed = hipLspeed_St_2_Upside_Down_Left;
      _out->hipRspeed = hipRspeed_St_2_Upside_Down_Left;
      _out->kneeLspeed = kneeLspeed_St_2_Upside_Down_Left;
      _out->kneeRspeed = kneeRspeed_St_2_Upside_Down_Left;
      _out->arm1speed = arm1speed_St_2_Upside_Down_Left;
      _out->arm2speed = arm2speed_St_2_Upside_Down_Left;
      _out->arm3speed = arm3speed_St_2_Upside_Down_Left;
      _out->arm4speed = arm4speed_St_2_Upside_Down_Left;
      Robot__doNotMove_step(0.100000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_101 = Robot__doNotMove_out_st.ok;
      v_103 = (v_101&&v_102);
      v_105 = (v_103&&v_104);
      v_107 = (v_105&&v_106);
      if (v_107) {
        v_109 = true;
      } else {
        v_109 = false;
      };
      if (v_100) {
        nr_2_St_2_Upside_Down_Left = true;
      } else {
        nr_2_St_2_Upside_Down_Left = v_109;
      };
      if (v_107) {
        v_108 = Robot__St_2_Upside_Down_Right;
      } else {
        v_108 = Robot__St_2_Upside_Down_Left;
      };
      if (v_100) {
        ns_2_St_2_Upside_Down_Left = Robot__St_2_Standard;
      } else {
        ns_2_St_2_Upside_Down_Left = v_108;
      };
      ns_2 = ns_2_St_2_Upside_Down_Left;
      nr_2 = nr_2_St_2_Upside_Down_Left;
      self->v_110 = time;
      break;
    case Robot__St_2_Upside_Down_Right:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, -60.000000, 100.000000,
                               -40.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_2_Upside_Down_Right = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_2_Upside_Down_Right = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_2_Upside_Down_Right = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_2_Upside_Down_Right = Robot__setMotorLegs_out_st.kneeRspeed;
      v_97 = (self->v_96+dt);
      if (r_4) {
        time_1 = 0.000000;
      } else {
        time_1 = v_97;
      };
      v_92 = (time_1>=5.000000);
      v_90 = (dgyroscope<=1.000000);
      v_88 = (arm1angle>=20.000000);
      v_85 = (gyroscope>=320.000000);
      v_84 = (gyroscope<=40.000000);
      v_86 = (v_84||v_85);
      r_8 = r_4;
      if (r_8) {
        Robot__setMotorArm_reset(&self->setMotorArm_3);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              65.000000, 0.000000, 10.000000, 30.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_3);
      arm1speed_St_2_Upside_Down_Right = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_2_Upside_Down_Right = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_2_Upside_Down_Right = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_2_Upside_Down_Right = Robot__setMotorArm_out_st.arm4speed;
      _out->hipLspeed = hipLspeed_St_2_Upside_Down_Right;
      _out->hipRspeed = hipRspeed_St_2_Upside_Down_Right;
      _out->kneeLspeed = kneeLspeed_St_2_Upside_Down_Right;
      _out->kneeRspeed = kneeRspeed_St_2_Upside_Down_Right;
      _out->arm1speed = arm1speed_St_2_Upside_Down_Right;
      _out->arm2speed = arm2speed_St_2_Upside_Down_Right;
      _out->arm3speed = arm3speed_St_2_Upside_Down_Right;
      _out->arm4speed = arm4speed_St_2_Upside_Down_Right;
      Robot__doNotMove_step(0.100000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_87 = Robot__doNotMove_out_st.ok;
      v_89 = (v_87&&v_88);
      v_91 = (v_89&&v_90);
      v_93 = (v_91&&v_92);
      if (v_93) {
        v_95 = true;
      } else {
        v_95 = false;
      };
      if (v_86) {
        nr_2_St_2_Upside_Down_Right = true;
      } else {
        nr_2_St_2_Upside_Down_Right = v_95;
      };
      if (v_93) {
        v_94 = Robot__St_2_Upside_Down_Left;
      } else {
        v_94 = Robot__St_2_Upside_Down_Right;
      };
      if (v_86) {
        ns_2_St_2_Upside_Down_Right = Robot__St_2_Standard;
      } else {
        ns_2_St_2_Upside_Down_Right = v_94;
      };
      ns_2 = ns_2_St_2_Upside_Down_Right;
      nr_2 = nr_2_St_2_Upside_Down_Right;
      self->v_96 = time_1;
      break;
    case Robot__St_2_Jump:
      v_82 = (kneeRangle+hipRangle);
      v_83 = (v_82+gyroscope);
      Robot__setMotorFast_step(v_83, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeRspeed_St_2_Jump = Robot__setMotorFast_out_st.power;
      v_80 = (kneeLangle+hipLangle);
      v_81 = (v_80+gyroscope);
      Robot__setMotorFast_step(v_81, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeLspeed_St_2_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipRangle, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipRspeed_St_2_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipLangle, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipLspeed_St_2_Jump = Robot__setMotorFast_out_st.power;
      v_78 = !(footRcontact);
      v_77 = !(footLcontact);
      v_79 = (v_77&&v_78);
      if (v_79) {
        nr_2_St_2_Jump = true;
        ns_2_St_2_Jump = Robot__St_2_Falling;
      } else {
        nr_2_St_2_Jump = false;
        ns_2_St_2_Jump = Robot__St_2_Jump;
      };
      r_7 = r_4;
      if (r_7) {
        Robot__setMotorArm_reset(&self->setMotorArm_2);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              -10.000000, 10.000000, -10.000000, 10.000000,
                              &Robot__setMotorArm_out_st,
                              &self->setMotorArm_2);
      arm1speed_St_2_Jump = Robot__setMotorArm_out_st.arm1speed;
      arm2speed_St_2_Jump = Robot__setMotorArm_out_st.arm2speed;
      arm3speed_St_2_Jump = Robot__setMotorArm_out_st.arm3speed;
      arm4speed_St_2_Jump = Robot__setMotorArm_out_st.arm4speed;
      _out->hipLspeed = hipLspeed_St_2_Jump;
      _out->hipRspeed = hipRspeed_St_2_Jump;
      _out->kneeLspeed = kneeLspeed_St_2_Jump;
      _out->kneeRspeed = kneeRspeed_St_2_Jump;
      _out->arm1speed = arm1speed_St_2_Jump;
      _out->arm2speed = arm2speed_St_2_Jump;
      _out->arm3speed = arm3speed_St_2_Jump;
      _out->arm4speed = arm4speed_St_2_Jump;
      ns_2 = ns_2_St_2_Jump;
      nr_2 = nr_2_St_2_Jump;
      break;
    case Robot__St_2_Standard:
      if (r_4) {
        pnr = false;
      } else {
        pnr = self->v_76;
      };
      r_2 = pnr;
      if (r_4) {
        ck_2 = Robot__St_Normal;
        pnr_1 = false;
      } else {
        ck_2 = self->v_75;
        pnr_1 = self->v_73;
      };
      r_3 = pnr_1;
      if (r_4) {
        ck_1 = Robot__St_1_Center;
      } else {
        ck_1 = self->v_72;
      };
      if (action_a) {
        v_71 = true;
        v_70 = Robot__St_2_Jump;
      } else {
        v_71 = false;
        v_70 = Robot__St_2_Standard;
      };
      v_68 = (gyroscope<=260.000000);
      v_67 = (gyroscope>=90.000000);
      v_69 = (v_67&&v_68);
      if (v_69) {
        nr_2_St_2_Standard = true;
        ns_2_St_2_Standard = Robot__St_2_Upside_Down_Init;
      } else {
        nr_2_St_2_Standard = v_71;
        ns_2_St_2_Standard = v_70;
      };
      switch (ck_2) {
        case Robot__St_Normal:
          if (action_b) {
            nr_St_Normal = true;
            ns_St_Normal = Robot__St_Move;
          } else {
            nr_St_Normal = false;
            ns_St_Normal = Robot__St_Normal;
          };
          r_6 = (r_4||r_2);
          if (r_6) {
            Robot__setMotorArm_reset(&self->setMotorArm_1);
          };
          Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                                  -40.000000, 30.000000, 80.000000,
                                  60.000000, &Robot__setMotorArm_out_st,
                                  &self->setMotorArm_1);
          arm1speed_St_2_Standard_St_Normal = Robot__setMotorArm_out_st.arm1speed;
          arm2speed_St_2_Standard_St_Normal = Robot__setMotorArm_out_st.arm2speed;
          arm3speed_St_2_Standard_St_Normal = Robot__setMotorArm_out_st.arm3speed;
          arm4speed_St_2_Standard_St_Normal = Robot__setMotorArm_out_st.arm4speed;
          arm1speed_St_2_Standard = arm1speed_St_2_Standard_St_Normal;
          arm2speed_St_2_Standard = arm2speed_St_2_Standard_St_Normal;
          arm3speed_St_2_Standard = arm3speed_St_2_Standard_St_Normal;
          arm4speed_St_2_Standard = arm4speed_St_2_Standard_St_Normal;
          ns = ns_St_Normal;
          nr = nr_St_Normal;
          break;
        case Robot__St_Move:
          Lib__arm_ik_step(_out->arm_target_x, _out->arm_target_y, 0.000000,
                           arm1angle, arm2angle, arm3angle, arm4angle,
                           0.260000, 0.260000, 0.260000, 0.260000,
                           &Lib__arm_ik_out_st);
          theta1 = Lib__arm_ik_out_st.theta1;
          theta2 = Lib__arm_ik_out_st.theta2;
          theta3 = Lib__arm_ik_out_st.theta3;
          theta4 = Lib__arm_ik_out_st.theta4;
          v_74 = !(action_b);
          if (v_74) {
            nr_St_Move = true;
            ns_St_Move = Robot__St_Move;
          } else {
            nr_St_Move = false;
            ns_St_Move = Robot__St_Move;
          };
          r_5 = (r_4||r_2);
          if (r_5) {
            Robot__setMotorArm_reset(&self->setMotorArm);
          };
          Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                                  theta1, theta2, theta3, theta4,
                                  &Robot__setMotorArm_out_st,
                                  &self->setMotorArm);
          arm1speed_St_2_Standard_St_Move = Robot__setMotorArm_out_st.arm1speed;
          arm2speed_St_2_Standard_St_Move = Robot__setMotorArm_out_st.arm2speed;
          arm3speed_St_2_Standard_St_Move = Robot__setMotorArm_out_st.arm3speed;
          arm4speed_St_2_Standard_St_Move = Robot__setMotorArm_out_st.arm4speed;
          arm1speed_St_2_Standard = arm1speed_St_2_Standard_St_Move;
          arm2speed_St_2_Standard = arm2speed_St_2_Standard_St_Move;
          arm3speed_St_2_Standard = arm3speed_St_2_Standard_St_Move;
          arm4speed_St_2_Standard = arm4speed_St_2_Standard_St_Move;
          ns = ns_St_Move;
          nr = nr_St_Move;
          break;
        default:
          break;
      };
      switch (ck_1) {
        case Robot__St_1_Center:
          Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle,
                                   kneeRangle, -40.000000, 40.000000,
                                   100.000000, -100.000000,
                                   &Robot__setMotorLegs_out_st);
          hipLspeed_St_2_Standard_St_1_Center = Robot__setMotorLegs_out_st.hipLspeed;
          hipRspeed_St_2_Standard_St_1_Center = Robot__setMotorLegs_out_st.hipRspeed;
          kneeLspeed_St_2_Standard_St_1_Center = Robot__setMotorLegs_out_st.kneeLspeed;
          kneeRspeed_St_2_Standard_St_1_Center = Robot__setMotorLegs_out_st.kneeRspeed;
          nr_1_St_1_Center = false;
          ns_1_St_1_Center = Robot__St_1_Center;
          hipLspeed_St_2_Standard = hipLspeed_St_2_Standard_St_1_Center;
          hipRspeed_St_2_Standard = hipRspeed_St_2_Standard_St_1_Center;
          kneeLspeed_St_2_Standard = kneeLspeed_St_2_Standard_St_1_Center;
          kneeRspeed_St_2_Standard = kneeRspeed_St_2_Standard_St_1_Center;
          ns_1 = ns_1_St_1_Center;
          nr_1 = nr_1_St_1_Center;
          break;
        default:
          break;
      };
      _out->hipLspeed = hipLspeed_St_2_Standard;
      _out->hipRspeed = hipRspeed_St_2_Standard;
      _out->kneeLspeed = kneeLspeed_St_2_Standard;
      _out->kneeRspeed = kneeRspeed_St_2_Standard;
      _out->arm1speed = arm1speed_St_2_Standard;
      _out->arm2speed = arm2speed_St_2_Standard;
      _out->arm3speed = arm3speed_St_2_Standard;
      _out->arm4speed = arm4speed_St_2_Standard;
      ns_2 = ns_2_St_2_Standard;
      nr_2 = nr_2_St_2_Standard;
      self->v_76 = nr;
      self->v_75 = ns;
      self->v_73 = nr_1;
      self->v_72 = ns_1;
      break;
    default:
      break;
  };
  self->pnr_2 = nr_2;
  self->ck = ns_2;
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

