/* --- Generated the 14/1/2025 at 16:45 --- */
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

void Robot__mod2_step(float x, float y, Robot__mod2_out* _out) {
  Lib__fmod_out Lib__fmod_out_st;
  
  float v_4;
  int v_3;
  float v;
  float x2;
  Lib__fmod_step(x, y, &Lib__fmod_out_st);
  x2 = Lib__fmod_out_st.x;
  v_4 = (x2-y);
  v = (2.000000*x2);
  v_3 = (v<=y);
  if (v_3) {
    _out->r = x2;
  } else {
    _out->r = v_4;
  };;
}

void Robot__setMotor_step(float current2, float goal2, float maxi,
                          Robot__setMotor_out* _out) {
  Robot__mod2_out Robot__mod2_out_st;
  Lib__min_out Lib__min_out_st;
  Lib__abs_out Lib__abs_out_st;
  
  float v_11;
  float v_10;
  float v_9;
  float v_8;
  float v_7;
  int v_6;
  int v_5;
  float v;
  float delta;
  float delta2;
  float current;
  float goal;
  Robot__mod2_step(goal2, 360.000000, &Robot__mod2_out_st);
  goal = Robot__mod2_out_st.r;
  Robot__mod2_step(current2, 360.000000, &Robot__mod2_out_st);
  current = Robot__mod2_out_st.r;
  delta = (goal-current);
  v_6 = (delta>=0.000000);
  Lib__abs_step(delta, &Lib__abs_out_st);
  v = Lib__abs_out_st.x;
  v_5 = (v<=1.000000);
  if (v_5) {
    delta2 = 0.000000;
  } else {
    delta2 = delta;
  };
  Lib__abs_step(delta2, &Lib__abs_out_st);
  v_9 = Lib__abs_out_st.x;
  Lib__min_step(v_9, maxi, &Lib__min_out_st);
  v_10 = Lib__min_out_st.x;
  v_11 = -(v_10);
  Lib__abs_step(delta2, &Lib__abs_out_st);
  v_7 = Lib__abs_out_st.x;
  Lib__min_step(v_7, maxi, &Lib__min_out_st);
  v_8 = Lib__min_out_st.x;
  if (v_6) {
    _out->power = v_8;
  } else {
    _out->power = v_11;
  };;
}

void Robot__setMotorFast_step(float current, float goal, float maxi,
                              Robot__setMotorFast_out* _out) {
  Lib__fmod_out Lib__fmod_out_st;
  
  float v_13;
  int v_12;
  float v;
  float delta;
  v_13 = -(maxi);
  v = (goal-current);
  Lib__fmod_step(v, 360.000000, &Lib__fmod_out_st);
  delta = Lib__fmod_out_st.x;
  v_12 = (delta<180.000000);
  if (v_12) {
    _out->power = maxi;
  } else {
    _out->power = v_13;
  };;
}

void Robot__setMotorPID_reset(Robot__setMotorPID_mem* self) {
  self->v_17 = true;
  self->v_14 = true;
}

void Robot__setMotorPID_step(float currentAngle, float goalAngle,
                             float maxSpeed, float kp, float ki, float kd,
                             Robot__setMotorPID_out* _out,
                             Robot__setMotorPID_mem* self) {
  Robot__mod2_out Robot__mod2_out_st;
  Lib__abs_out Lib__abs_out_st;
  
  float v_31;
  float v_30;
  int v_29;
  float v_28;
  float v_27;
  int v_26;
  float v_25;
  int v_24;
  float v_23;
  float v_22;
  float v_21;
  float v_20;
  float v_19;
  float v_16;
  float v;
  float error;
  float integral;
  float derivative;
  float lastError;
  float rawSpeed;
  v_27 = -(maxSpeed);
  v_25 = -(maxSpeed);
  v = (goalAngle-currentAngle);
  Robot__mod2_step(v, 360.000000, &Robot__mod2_out_st);
  error = Robot__mod2_out_st.r;
  lastError = error;
  v_20 = (kp*error);
  v_19 = (error-self->v_18);
  if (self->v_17) {
    derivative = 0.000000;
  } else {
    derivative = v_19;
  };
  v_23 = (kd*derivative);
  v_16 = (self->v_15+error);
  if (self->v_14) {
    integral = 0.000000;
  } else {
    integral = v_16;
  };
  v_21 = (ki*integral);
  v_22 = (v_20+v_21);
  rawSpeed = (v_22+v_23);
  Lib__abs_step(rawSpeed, &Lib__abs_out_st);
  v_28 = Lib__abs_out_st.x;
  v_29 = (v_28<1.000000);
  if (v_29) {
    v_30 = 0.000000;
  } else {
    v_30 = rawSpeed;
  };
  v_26 = (rawSpeed<v_25);
  if (v_26) {
    v_31 = v_27;
  } else {
    v_31 = v_30;
  };
  v_24 = (rawSpeed>maxSpeed);
  if (v_24) {
    _out->motorSpeed = maxSpeed;
  } else {
    _out->motorSpeed = v_31;
  };
  self->v_18 = lastError;
  self->v_17 = false;
  self->v_15 = integral;
  self->v_14 = false;;
}

void Robot__doNotMove_step(float limit, float speedA, float speedB,
                           float speedC, float speedD,
                           Robot__doNotMove_out* _out) {
  
  int v_36;
  int v_35;
  int v_34;
  int v_33;
  int v_32;
  int v;
  v_36 = (speedD<=limit);
  v_34 = (speedC<=limit);
  v_32 = (speedB<=limit);
  v = (speedA<=limit);
  v_33 = (v&&v_32);
  v_35 = (v_33&&v_34);
  _out->ok = (v_35&&v_36);;
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
  Lib__fmod_out Lib__fmod_out_st;
  Robot__setMotorPID_out Robot__setMotorPID_out_st;
  
  float v_39;
  float v_38;
  float v_37;
  float v;
  Lib__fmod_step(arm4goal, 360.000000, &Lib__fmod_out_st);
  v_39 = Lib__fmod_out_st.x;
  Robot__setMotorPID_step(arm4angle, v_39, 20.000000, 1.000000, 0.000000,
                          0.500000, &Robot__setMotorPID_out_st,
                          &self->setMotorPID_3);
  _out->arm4speed = Robot__setMotorPID_out_st.motorSpeed;
  Lib__fmod_step(arm3goal, 360.000000, &Lib__fmod_out_st);
  v_38 = Lib__fmod_out_st.x;
  Robot__setMotorPID_step(arm3angle, v_38, 20.000000, 0.900000, 0.000000,
                          0.400000, &Robot__setMotorPID_out_st,
                          &self->setMotorPID_2);
  _out->arm3speed = Robot__setMotorPID_out_st.motorSpeed;
  Lib__fmod_step(arm2goal, 360.000000, &Lib__fmod_out_st);
  v_37 = Lib__fmod_out_st.x;
  Robot__setMotorPID_step(arm2angle, v_37, 20.000000, 0.700000, 0.000000,
                          0.300000, &Robot__setMotorPID_out_st,
                          &self->setMotorPID_1);
  _out->arm2speed = Robot__setMotorPID_out_st.motorSpeed;
  Lib__fmod_step(arm1goal, 360.000000, &Lib__fmod_out_st);
  v = Lib__fmod_out_st.x;
  Robot__setMotorPID_step(arm1angle, v, 20.000000, 0.500000, 0.000000,
                          0.200000, &Robot__setMotorPID_out_st,
                          &self->setMotorPID);
  _out->arm1speed = Robot__setMotorPID_out_st.motorSpeed;;
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

void Robot__funHip_step(float x, Robot__funHip_out* _out) {
  Lib__clamp_out Lib__clamp_out_st;
  
  float v_43;
  float v_42;
  float v_41;
  float v_40;
  int v;
  float x2;
  Lib__clamp_step(x, 0.000000, 2.000000, &Lib__clamp_out_st);
  x2 = Lib__clamp_out_st.x;
  v_42 = (160.000000*x2);
  v_43 = (v_42-240.000000);
  v_40 = -(x2);
  v_41 = (v_40*80.000000);
  v = (x2<=1.000000);
  if (v) {
    _out->y = v_41;
  } else {
    _out->y = v_43;
  };;
}

void Robot__funKnee_step(float x, Robot__funKnee_out* _out) {
  Lib__clamp_out Lib__clamp_out_st;
  
  float v_46;
  float v_45;
  float v_44;
  int v;
  float x2;
  Lib__clamp_step(x, 0.000000, 2.000000, &Lib__clamp_out_st);
  x2 = Lib__clamp_out_st.x;
  v_45 = (-150.000000*x2);
  v_46 = (v_45+310.000000);
  v_44 = (160.000000*x2);
  v = (x2<=1.000000);
  if (v) {
    _out->y = v_44;
  } else {
    _out->y = v_46;
  };;
}

void Robot__legSpeedsWithoutContact_step(float hipAngle, float kneeAngle,
                                         float hipGoal, float kneeGoal,
                                         int footContact, float calfDistance,
                                         Robot__legSpeedsWithoutContact_out* _out) {
  Robot__setMotor_out Robot__setMotor_out_st;
  Lib__abs_out Lib__abs_out_st;
  
  int v_50;
  int v_49;
  float v_48;
  int v_47;
  int v;
  float kneeSpeed2;
  v = (calfDistance<=0.200000);
  v_47 = (footContact||v);
  Robot__setMotor_step(kneeAngle, kneeGoal, 60.000000,
                       &Robot__setMotor_out_st);
  kneeSpeed2 = Robot__setMotor_out_st.power;
  Robot__setMotor_step(hipAngle, hipGoal, 60.000000, &Robot__setMotor_out_st);
  _out->hipSpeed = Robot__setMotor_out_st.power;
  Lib__abs_step(_out->hipSpeed, &Lib__abs_out_st);
  v_48 = Lib__abs_out_st.x;
  v_49 = (v_48>=1.000000);
  v_50 = (v_47&&v_49);
  if (v_50) {
    _out->kneeSpeed = 0.000000;
  } else {
    _out->kneeSpeed = kneeSpeed2;
  };;
}

void Robot__legSpeedsWithContact_step(float hipAngle, float kneeAngle,
                                      float hipGoal, float kneeGoal,
                                      int footContact, float calfDistance,
                                      Robot__legSpeedsWithContact_out* _out) {
  Robot__setMotor_out Robot__setMotor_out_st;
  Lib__abs_out Lib__abs_out_st;
  
  int v_53;
  int v_52;
  float v_51;
  int v;
  float hipSpeed2;
  v = !(footContact);
  Robot__setMotor_step(kneeAngle, kneeGoal, 60.000000,
                       &Robot__setMotor_out_st);
  _out->kneeSpeed = Robot__setMotor_out_st.power;
  Lib__abs_step(_out->kneeSpeed, &Lib__abs_out_st);
  v_51 = Lib__abs_out_st.x;
  v_52 = (v_51>=1.000000);
  v_53 = (v&&v_52);
  Robot__setMotor_step(hipAngle, hipGoal, 60.000000, &Robot__setMotor_out_st);
  hipSpeed2 = Robot__setMotor_out_st.power;
  if (v_53) {
    _out->hipSpeed = 0.000000;
  } else {
    _out->hipSpeed = hipSpeed2;
  };;
}

void Robot__robot_reset(Robot__robot_mem* self) {
  Robot__setMotorArm_reset(&self->setMotorArm_6);
  Robot__setMotorArm_reset(&self->setMotorArm_5);
  Robot__setMotorArm_reset(&self->setMotorArm_4);
  Robot__setMotorArm_reset(&self->setMotorArm_3);
  Robot__setMotorArm_reset(&self->setMotorArm_2);
  Robot__setMotorArm_reset(&self->setMotorArm_1);
  Robot__setMotorArm_reset(&self->setMotorArm);
  self->pnr_2 = false;
  self->pnr_3 = false;
  self->ck = Robot__St_3_Hand_Open;
  self->v_77 = true;
  self->v_71 = true;
  self->v_65 = true;
  self->v_59 = true;
  self->v = true;
  self->v_140 = false;
  self->v_139 = Robot__St_Normal;
  self->v_137 = false;
  self->v_136 = Robot__St_1_Center;
  self->ck_1 = Robot__St_2_Falling;
}

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
                       Robot__robot_out* _out, Robot__robot_mem* self) {
  Robot__funKnee_out Robot__funKnee_out_st;
  Lib__arm_ik_out Lib__arm_ik_out_st;
  Robot__setMotorLegs_out Robot__setMotorLegs_out_st;
  Robot__setMotorFast_out Robot__setMotorFast_out_st;
  Robot__funHip_out Robot__funHip_out_st;
  Robot__legSpeedsWithoutContact_out Robot__legSpeedsWithoutContact_out_st;
  Robot__legSpeedsWithContact_out Robot__legSpeedsWithContact_out_st;
  Robot__setMotor_out Robot__setMotor_out_st;
  Lib__abs_out Lib__abs_out_st;
  Robot__mux_out Robot__mux_out_st;
  Lib__fmod_out Lib__fmod_out_st;
  Lib__clamp_out Lib__clamp_out_st;
  Robot__doNotMove_out Robot__doNotMove_out_st;
  Robot__setMotorArm_out Robot__setMotorArm_out_st;
  
  int v_89;
  int v_88;
  int v_87;
  int v_93;
  int v_92;
  int v_91;
  int nr_3_St_3_Hand_Close;
  Robot__st_3 ns_3_St_3_Hand_Close;
  float fingerR2speed_St_3_Hand_Close;
  float fingerR1speed_St_3_Hand_Close;
  float fingerL2speed_St_3_Hand_Close;
  float fingerL1speed_St_3_Hand_Close;
  int nr_3_St_3_Hand_Open;
  Robot__st_3 ns_3_St_3_Hand_Open;
  float fingerR2speed_St_3_Hand_Open;
  float fingerR1speed_St_3_Hand_Open;
  float fingerL2speed_St_3_Hand_Open;
  float fingerL1speed_St_3_Hand_Open;
  float v_109;
  float v_108;
  float v_107;
  float v_106;
  float v_105;
  float v_104;
  int v_103;
  float factorL_1;
  float factorR_1;
  float kneeLspeed2;
  float hipRspeed2;
  float v_120;
  float v_119;
  float v_118;
  float v_117;
  float v_116;
  float v_115;
  int v_114;
  int v_113;
  int v_112;
  int v_111;
  int v_110;
  float factorL;
  float factorR;
  int v_128;
  int v_127;
  int v_126;
  int v_125;
  int v_124;
  int v_123;
  int v_122;
  int v_121;
  float v_135;
  float v_134;
  float v_133;
  float v_132;
  float v_131;
  float v_130;
  int v_129;
  float factor;
  int nr_1_St_1_Go_Off_Center_Up;
  Robot__st_1 ns_1_St_1_Go_Off_Center_Up;
  float kneeRspeed_St_2_Standard_St_1_Go_Off_Center_Up;
  float kneeLspeed_St_2_Standard_St_1_Go_Off_Center_Up;
  float hipRspeed_St_2_Standard_St_1_Go_Off_Center_Up;
  float hipLspeed_St_2_Standard_St_1_Go_Off_Center_Up;
  int nr_1_St_1_Go_Off_Center_Down;
  Robot__st_1 ns_1_St_1_Go_Off_Center_Down;
  float kneeRspeed_St_2_Standard_St_1_Go_Off_Center_Down;
  float kneeLspeed_St_2_Standard_St_1_Go_Off_Center_Down;
  float hipRspeed_St_2_Standard_St_1_Go_Off_Center_Down;
  float hipLspeed_St_2_Standard_St_1_Go_Off_Center_Down;
  int nr_1_St_1_Go_Down;
  Robot__st_1 ns_1_St_1_Go_Down;
  float kneeRspeed_St_2_Standard_St_1_Go_Down;
  float kneeLspeed_St_2_Standard_St_1_Go_Down;
  float hipRspeed_St_2_Standard_St_1_Go_Down;
  float hipLspeed_St_2_Standard_St_1_Go_Down;
  int nr_1_St_1_Center;
  Robot__st_1 ns_1_St_1_Center;
  float kneeRspeed_St_2_Standard_St_1_Center;
  float kneeLspeed_St_2_Standard_St_1_Center;
  float hipRspeed_St_2_Standard_St_1_Center;
  float hipLspeed_St_2_Standard_St_1_Center;
  Robot__st_1 ck_2;
  int v_138;
  int r_7;
  float theta1;
  float theta2;
  float theta3;
  float theta4;
  int r_8;
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
  Robot__st ck_3;
  int v_102;
  Robot__st_2 v_101;
  int v_100;
  int v_99;
  int v_98;
  int v_97;
  int v_96;
  int v_95;
  int v_94;
  Robot__st_1 ns_1;
  int r_4;
  int nr_1;
  int pnr_1;
  Robot__st ns;
  int r_3;
  int nr;
  int pnr;
  float v_147;
  float v_146;
  float v_145;
  float v_144;
  int v_143;
  int v_142;
  int v_141;
  int r_9;
  float v_161;
  int v_159;
  Robot__st_2 v_158;
  int v_157;
  int v_156;
  int v_155;
  int v_154;
  int v_153;
  int v_152;
  int v_151;
  int v_150;
  int v_149;
  int v_148;
  int r_10;
  float time_1;
  float v_175;
  int v_173;
  Robot__st_2 v_172;
  int v_171;
  int v_170;
  int v_169;
  int v_168;
  int v_167;
  int v_166;
  int v_165;
  int v_164;
  int v_163;
  int v_162;
  int r_11;
  float time;
  int v_192;
  int v_191;
  int v_190;
  Robot__st_2 v_189;
  int v_188;
  Robot__st_2 v_187;
  int v_186;
  int v_185;
  int v_184;
  int v_183;
  int v_182;
  int v_181;
  int v_180;
  int v_179;
  int v_178;
  int v_177;
  int v_176;
  int r_12;
  int isStatic;
  int v_194;
  int v_193;
  int r_13;
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
  float v_85;
  float v_84;
  float v_83;
  float v_82;
  float v_81;
  float v_80;
  float v_79;
  float v_76;
  float v_75;
  float v_74;
  float v_73;
  float v_70;
  float v_69;
  float v_68;
  float v_67;
  float v_64;
  float v_63;
  float v_62;
  float v_61;
  float v_58;
  float v_57;
  float v_56;
  float v_55;
  Robot__st_3 ns_3;
  int r_6;
  int nr_3;
  Robot__st_2 ns_2;
  int r_5;
  int nr_2;
  float dgyroscope;
  float body_target_y;
  float alti;
  r_5 = self->pnr_2;
  r_6 = self->pnr_3;
  Robot__mux_step(action_rb, -1.000000, 0.000000, &Robot__mux_out_st);
  v_80 = Robot__mux_out_st.r;
  Robot__mux_step(action_lb, 1.000000, 0.000000, &Robot__mux_out_st);
  v_79 = Robot__mux_out_st.r;
  v_81 = (v_79+v_80);
  v_82 = (v_81*dt);
  v_83 = (v_82*10.000000);
  v_84 = (self->v_78+v_83);
  Lib__fmod_step(v_84, 360.000000, &Lib__fmod_out_st);
  v_85 = Lib__fmod_out_st.x;
  if (self->v_77) {
    _out->arm_target_angle = 0.000000;
  } else {
    _out->arm_target_angle = v_85;
  };
  v_73 = (main_y*dt);
  v_74 = (v_73*0.500000);
  v_75 = (self->v_72+v_74);
  if (self->v_71) {
    v_76 = 0.200000;
  } else {
    v_76 = v_75;
  };
  Lib__clamp_step(v_76, 0.000000, 2.000000, &Lib__clamp_out_st);
  body_target_y = Lib__clamp_out_st.x;
  v_67 = (secondary_y*dt);
  v_68 = (v_67*0.500000);
  v_69 = (self->v_66+v_68);
  if (self->v_65) {
    v_70 = 0.200000;
  } else {
    v_70 = v_69;
  };
  Lib__clamp_step(v_70, 0.250000, 1.000000, &Lib__clamp_out_st);
  _out->arm_target_y = Lib__clamp_out_st.x;
  v_61 = (secondary_x*dt);
  v_62 = (v_61*0.500000);
  v_63 = (self->v_60+v_62);
  if (self->v_59) {
    v_64 = 0.000000;
  } else {
    v_64 = v_63;
  };
  Lib__clamp_step(v_64, -1.000000, 1.000000, &Lib__clamp_out_st);
  _out->arm_target_x = Lib__clamp_out_st.x;
  alti = bodyDistance;
  v_55 = (gyroscope-self->v_54);
  Lib__fmod_step(v_55, 360.000000, &Lib__fmod_out_st);
  v_56 = Lib__fmod_out_st.x;
  Lib__abs_step(v_56, &Lib__abs_out_st);
  v_57 = Lib__abs_out_st.x;
  v_58 = (v_57/dt);
  if (self->v) {
    dgyroscope = 100.000000;
  } else {
    dgyroscope = v_58;
  };
  switch (self->ck_1) {
    case Robot__St_2_Falling:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, 40.000000, 100.000000,
                               -100.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_2_Falling = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_2_Falling = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_2_Falling = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_2_Falling = Robot__setMotorLegs_out_st.kneeRspeed;
      v_193 = (footLcontact||footRcontact);
      v_194 = (v_193||bodyContact);
      if (v_194) {
        nr_2_St_2_Falling = true;
        ns_2_St_2_Falling = Robot__St_2_Standard;
      } else {
        nr_2_St_2_Falling = false;
        ns_2_St_2_Falling = Robot__St_2_Falling;
      };
      r_13 = r_5;
      if (r_13) {
        Robot__setMotorArm_reset(&self->setMotorArm_6);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              320.000000, 30.000000, 80.000000, 60.000000,
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
      v_185 = (gyroscope>40.000000);
      v_183 = (gyroscope<180.000000);
      v_181 = (gyroscope<320.000000);
      v_179 = (gyroscope>=180.000000);
      v_177 = (gyroscope>=320.000000);
      v_176 = (gyroscope<=40.000000);
      v_178 = (v_176||v_177);
      r_12 = r_5;
      if (r_12) {
        Robot__setMotorArm_reset(&self->setMotorArm_5);
      };
      Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                              0.000000, 0.000000, 0.000000, 0.000000,
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
      v_191 = Robot__doNotMove_out_st.ok;
      _out->arm1speed = arm1speed_St_2_Upside_Down_Init;
      _out->arm2speed = arm2speed_St_2_Upside_Down_Init;
      _out->arm3speed = arm3speed_St_2_Upside_Down_Init;
      _out->arm4speed = arm4speed_St_2_Upside_Down_Init;
      Robot__doNotMove_step(1.000000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_192 = Robot__doNotMove_out_st.ok;
      isStatic = (v_191&&v_192);
      v_184 = (isStatic&&v_183);
      v_186 = (v_184&&v_185);
      if (v_186) {
        v_188 = true;
        v_187 = Robot__St_2_Upside_Down_Right;
      } else {
        v_188 = false;
        v_187 = Robot__St_2_Upside_Down_Init;
      };
      v_180 = (isStatic&&v_179);
      v_182 = (v_180&&v_181);
      if (v_182) {
        v_190 = true;
      } else {
        v_190 = v_188;
      };
      if (v_178) {
        nr_2_St_2_Upside_Down_Init = true;
      } else {
        nr_2_St_2_Upside_Down_Init = v_190;
      };
      if (v_182) {
        v_189 = Robot__St_2_Upside_Down_Left;
      } else {
        v_189 = v_187;
      };
      if (v_178) {
        ns_2_St_2_Upside_Down_Init = Robot__St_2_Standard;
      } else {
        ns_2_St_2_Upside_Down_Init = v_189;
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
      v_175 = (self->v_174+dt);
      if (r_5) {
        time = 0.000000;
      } else {
        time = v_175;
      };
      v_170 = (time>=5.000000);
      v_168 = (dgyroscope<=1.000000);
      v_166 = (arm1angle<=-20.000000);
      v_163 = (gyroscope>=320.000000);
      v_162 = (gyroscope<=40.000000);
      v_164 = (v_162||v_163);
      r_11 = r_5;
      if (r_11) {
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
      v_165 = Robot__doNotMove_out_st.ok;
      v_167 = (v_165&&v_166);
      v_169 = (v_167&&v_168);
      v_171 = (v_169&&v_170);
      if (v_171) {
        v_173 = true;
      } else {
        v_173 = false;
      };
      if (v_164) {
        nr_2_St_2_Upside_Down_Left = true;
      } else {
        nr_2_St_2_Upside_Down_Left = v_173;
      };
      if (v_171) {
        v_172 = Robot__St_2_Upside_Down_Right;
      } else {
        v_172 = Robot__St_2_Upside_Down_Left;
      };
      if (v_164) {
        ns_2_St_2_Upside_Down_Left = Robot__St_2_Standard;
      } else {
        ns_2_St_2_Upside_Down_Left = v_172;
      };
      ns_2 = ns_2_St_2_Upside_Down_Left;
      nr_2 = nr_2_St_2_Upside_Down_Left;
      break;
    case Robot__St_2_Upside_Down_Right:
      Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle, kneeRangle,
                               -40.000000, -60.000000, 100.000000,
                               -40.000000, &Robot__setMotorLegs_out_st);
      hipLspeed_St_2_Upside_Down_Right = Robot__setMotorLegs_out_st.hipLspeed;
      hipRspeed_St_2_Upside_Down_Right = Robot__setMotorLegs_out_st.hipRspeed;
      kneeLspeed_St_2_Upside_Down_Right = Robot__setMotorLegs_out_st.kneeLspeed;
      kneeRspeed_St_2_Upside_Down_Right = Robot__setMotorLegs_out_st.kneeRspeed;
      v_161 = (self->v_160+dt);
      if (r_5) {
        time_1 = 0.000000;
      } else {
        time_1 = v_161;
      };
      v_156 = (time_1>=5.000000);
      v_154 = (dgyroscope<=1.000000);
      v_152 = (arm1angle>=20.000000);
      v_149 = (gyroscope>=320.000000);
      v_148 = (gyroscope<=40.000000);
      v_150 = (v_148||v_149);
      r_10 = r_5;
      if (r_10) {
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
      v_151 = Robot__doNotMove_out_st.ok;
      v_153 = (v_151&&v_152);
      v_155 = (v_153&&v_154);
      v_157 = (v_155&&v_156);
      if (v_157) {
        v_159 = true;
      } else {
        v_159 = false;
      };
      if (v_150) {
        nr_2_St_2_Upside_Down_Right = true;
      } else {
        nr_2_St_2_Upside_Down_Right = v_159;
      };
      if (v_157) {
        v_158 = Robot__St_2_Upside_Down_Left;
      } else {
        v_158 = Robot__St_2_Upside_Down_Right;
      };
      if (v_150) {
        ns_2_St_2_Upside_Down_Right = Robot__St_2_Standard;
      } else {
        ns_2_St_2_Upside_Down_Right = v_158;
      };
      ns_2 = ns_2_St_2_Upside_Down_Right;
      nr_2 = nr_2_St_2_Upside_Down_Right;
      break;
    case Robot__St_2_Jump:
      v_146 = (kneeRangle+hipRangle);
      v_147 = (v_146+gyroscope);
      Robot__setMotorFast_step(v_147, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeRspeed_St_2_Jump = Robot__setMotorFast_out_st.power;
      v_144 = (kneeLangle+hipLangle);
      v_145 = (v_144+gyroscope);
      Robot__setMotorFast_step(v_145, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeLspeed_St_2_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipRangle, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipRspeed_St_2_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipLangle, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipLspeed_St_2_Jump = Robot__setMotorFast_out_st.power;
      v_142 = !(footRcontact);
      v_141 = !(footLcontact);
      v_143 = (v_141&&v_142);
      if (v_143) {
        nr_2_St_2_Jump = true;
        ns_2_St_2_Jump = Robot__St_2_Falling;
      } else {
        nr_2_St_2_Jump = false;
        ns_2_St_2_Jump = Robot__St_2_Jump;
      };
      r_9 = r_5;
      if (r_9) {
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
      if (r_5) {
        pnr = false;
      } else {
        pnr = self->v_140;
      };
      r_3 = pnr;
      if (r_5) {
        ck_3 = Robot__St_Normal;
        pnr_1 = false;
      } else {
        ck_3 = self->v_139;
        pnr_1 = self->v_137;
      };
      r_4 = pnr_1;
      if (r_5) {
        ck_2 = Robot__St_1_Center;
      } else {
        ck_2 = self->v_136;
      };
      v_99 = (hipRangle<=180.000000);
      v_97 = (hipLangle>=180.000000);
      v_98 = (action_a&&v_97);
      v_100 = (v_98&&v_99);
      if (v_100) {
        v_102 = true;
        v_101 = Robot__St_2_Jump;
      } else {
        v_102 = false;
        v_101 = Robot__St_2_Standard;
      };
      v_95 = (gyroscope<=260.000000);
      v_94 = (gyroscope>=90.000000);
      v_96 = (v_94&&v_95);
      if (v_96) {
        nr_2_St_2_Standard = true;
        ns_2_St_2_Standard = Robot__St_2_Upside_Down_Init;
      } else {
        nr_2_St_2_Standard = v_102;
        ns_2_St_2_Standard = v_101;
      };
      switch (ck_3) {
        case Robot__St_Normal:
          if (action_b) {
            nr_St_Normal = true;
            ns_St_Normal = Robot__St_Move;
          } else {
            nr_St_Normal = false;
            ns_St_Normal = Robot__St_Normal;
          };
          r_8 = (r_5||r_3);
          if (r_8) {
            Robot__setMotorArm_reset(&self->setMotorArm_1);
          };
          Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                                  320.000000, 30.000000, 80.000000,
                                  80.000000, &Robot__setMotorArm_out_st,
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
                           0.260000, 0.260000, 0.260000, 0.300000,
                           &Lib__arm_ik_out_st);
          theta1 = Lib__arm_ik_out_st.theta1;
          theta2 = Lib__arm_ik_out_st.theta2;
          theta3 = Lib__arm_ik_out_st.theta3;
          theta4 = Lib__arm_ik_out_st.theta4;
          v_138 = !(action_b);
          if (v_138) {
            nr_St_Move = true;
            ns_St_Move = Robot__St_Normal;
          } else {
            nr_St_Move = false;
            ns_St_Move = Robot__St_Move;
          };
          r_7 = (r_5||r_3);
          if (r_7) {
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
      switch (ck_2) {
        case Robot__St_1_Center:
          factor = body_target_y;
          Robot__funKnee_step(factor, &Robot__funKnee_out_st);
          v_134 = Robot__funKnee_out_st.y;
          v_135 = -(v_134);
          Robot__funKnee_step(factor, &Robot__funKnee_out_st);
          v_133 = Robot__funKnee_out_st.y;
          Robot__funHip_step(factor, &Robot__funHip_out_st);
          v_131 = Robot__funHip_out_st.y;
          v_132 = -(v_131);
          Robot__funHip_step(factor, &Robot__funHip_out_st);
          v_130 = Robot__funHip_out_st.y;
          Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle,
                                   kneeRangle, v_130, v_132, v_133, v_135,
                                   &Robot__setMotorLegs_out_st);
          hipLspeed_St_2_Standard_St_1_Center = Robot__setMotorLegs_out_st.hipLspeed;
          hipRspeed_St_2_Standard_St_1_Center = Robot__setMotorLegs_out_st.hipRspeed;
          kneeLspeed_St_2_Standard_St_1_Center = Robot__setMotorLegs_out_st.kneeLspeed;
          kneeRspeed_St_2_Standard_St_1_Center = Robot__setMotorLegs_out_st.kneeRspeed;
          v_129 = (main_x>=0.100000);
          if (v_129) {
            nr_1_St_1_Center = true;
            ns_1_St_1_Center = Robot__St_1_Go_Down;
          } else {
            nr_1_St_1_Center = false;
            ns_1_St_1_Center = Robot__St_1_Center;
          };
          hipLspeed_St_2_Standard = hipLspeed_St_2_Standard_St_1_Center;
          hipRspeed_St_2_Standard = hipRspeed_St_2_Standard_St_1_Center;
          kneeLspeed_St_2_Standard = kneeLspeed_St_2_Standard_St_1_Center;
          kneeRspeed_St_2_Standard = kneeRspeed_St_2_Standard_St_1_Center;
          break;
        case Robot__St_1_Go_Down:
          Robot__setMotorLegs_step(hipLangle, hipRangle, kneeLangle,
                                   kneeRangle, 310.000000, -310.000000,
                                   340.000000, 20.000000,
                                   &Robot__setMotorLegs_out_st);
          hipLspeed_St_2_Standard_St_1_Go_Down = Robot__setMotorLegs_out_st.hipLspeed;
          hipRspeed_St_2_Standard_St_1_Go_Down = Robot__setMotorLegs_out_st.hipRspeed;
          kneeLspeed_St_2_Standard_St_1_Go_Down = Robot__setMotorLegs_out_st.kneeLspeed;
          kneeRspeed_St_2_Standard_St_1_Go_Down = Robot__setMotorLegs_out_st.kneeRspeed;
          v_127 = (kneeRangle<=180.000000);
          v_125 = (kneeLangle>=180.000000);
          v_123 = (hipRangle<=180.000000);
          v_121 = (hipLangle>=180.000000);
          v_122 = (bodyContact&&v_121);
          v_124 = (v_122&&v_123);
          v_126 = (v_124&&v_125);
          v_128 = (v_126&&v_127);
          if (v_128) {
            nr_1_St_1_Go_Down = true;
            ns_1_St_1_Go_Down = Robot__St_1_Go_Off_Center_Down;
          } else {
            nr_1_St_1_Go_Down = false;
            ns_1_St_1_Go_Down = Robot__St_1_Go_Down;
          };
          hipLspeed_St_2_Standard = hipLspeed_St_2_Standard_St_1_Go_Down;
          hipRspeed_St_2_Standard = hipRspeed_St_2_Standard_St_1_Go_Down;
          kneeLspeed_St_2_Standard = kneeLspeed_St_2_Standard_St_1_Go_Down;
          kneeRspeed_St_2_Standard = kneeRspeed_St_2_Standard_St_1_Go_Down;
          break;
        case Robot__St_1_Go_Off_Center_Down:
          factorR = 0.100000;
          Robot__funKnee_step(factorR, &Robot__funKnee_out_st);
          v_119 = Robot__funKnee_out_st.y;
          v_120 = -(v_119);
          Robot__funHip_step(factorR, &Robot__funHip_out_st);
          v_117 = Robot__funHip_out_st.y;
          v_118 = -(v_117);
          Robot__legSpeedsWithoutContact_step(hipRangle, kneeRangle, v_118,
                                              v_120, footRcontact,
                                              calfRdistance,
                                              &Robot__legSpeedsWithoutContact_out_st);
          hipRspeed_St_2_Standard_St_1_Go_Off_Center_Down = Robot__legSpeedsWithoutContact_out_st.hipSpeed;
          kneeRspeed_St_2_Standard_St_1_Go_Off_Center_Down = Robot__legSpeedsWithoutContact_out_st.kneeSpeed;
          factorL = 1.000000;
          Robot__funKnee_step(factorL, &Robot__funKnee_out_st);
          v_116 = Robot__funKnee_out_st.y;
          Robot__funHip_step(factorL, &Robot__funHip_out_st);
          v_115 = Robot__funHip_out_st.y;
          Robot__legSpeedsWithoutContact_step(hipLangle, kneeLangle, v_115,
                                              v_116, footLcontact,
                                              calfLdistance,
                                              &Robot__legSpeedsWithoutContact_out_st);
          hipLspeed_St_2_Standard_St_1_Go_Off_Center_Down = Robot__legSpeedsWithoutContact_out_st.hipSpeed;
          kneeLspeed_St_2_Standard_St_1_Go_Off_Center_Down = Robot__legSpeedsWithoutContact_out_st.kneeSpeed;
          v_113 = (hipLangle<=330.000000);
          v_111 = (hipLangle>=180.000000);
          v_110 = (footLcontact&&footRcontact);
          v_112 = (v_110&&v_111);
          v_114 = (v_112&&v_113);
          if (v_114) {
            nr_1_St_1_Go_Off_Center_Down = true;
            ns_1_St_1_Go_Off_Center_Down = Robot__St_1_Go_Off_Center_Up;
          } else {
            nr_1_St_1_Go_Off_Center_Down = false;
            ns_1_St_1_Go_Off_Center_Down = Robot__St_1_Go_Off_Center_Down;
          };
          hipLspeed_St_2_Standard = hipLspeed_St_2_Standard_St_1_Go_Off_Center_Down;
          hipRspeed_St_2_Standard = hipRspeed_St_2_Standard_St_1_Go_Off_Center_Down;
          kneeLspeed_St_2_Standard = kneeLspeed_St_2_Standard_St_1_Go_Off_Center_Down;
          kneeRspeed_St_2_Standard = kneeRspeed_St_2_Standard_St_1_Go_Off_Center_Down;
          break;
        case Robot__St_1_Go_Off_Center_Up:
          factorR_1 = 1.000000;
          Robot__funKnee_step(factorR_1, &Robot__funKnee_out_st);
          v_108 = Robot__funKnee_out_st.y;
          v_109 = -(v_108);
          Robot__funHip_step(factorR_1, &Robot__funHip_out_st);
          v_106 = Robot__funHip_out_st.y;
          v_107 = -(v_106);
          Robot__legSpeedsWithContact_step(hipRangle, kneeRangle, v_107,
                                           v_109, footRcontact,
                                           calfRdistance,
                                           &Robot__legSpeedsWithContact_out_st);
          hipRspeed2 = Robot__legSpeedsWithContact_out_st.hipSpeed;
          kneeRspeed_St_2_Standard_St_1_Go_Off_Center_Up = Robot__legSpeedsWithContact_out_st.kneeSpeed;
          hipRspeed_St_2_Standard_St_1_Go_Off_Center_Up = (0.500000*hipRspeed2);
          factorL_1 = 0.200000;
          Robot__funKnee_step(factorL_1, &Robot__funKnee_out_st);
          v_105 = Robot__funKnee_out_st.y;
          Robot__funHip_step(factorL_1, &Robot__funHip_out_st);
          v_104 = Robot__funHip_out_st.y;
          Robot__legSpeedsWithContact_step(hipLangle, kneeLangle, v_104,
                                           v_105, footLcontact,
                                           calfLdistance,
                                           &Robot__legSpeedsWithContact_out_st);
          hipLspeed_St_2_Standard_St_1_Go_Off_Center_Up = Robot__legSpeedsWithContact_out_st.hipSpeed;
          kneeLspeed2 = Robot__legSpeedsWithContact_out_st.kneeSpeed;
          kneeLspeed_St_2_Standard_St_1_Go_Off_Center_Up = (0.500000*kneeLspeed2);
          hipLspeed_St_2_Standard = hipLspeed_St_2_Standard_St_1_Go_Off_Center_Up;
          hipRspeed_St_2_Standard = hipRspeed_St_2_Standard_St_1_Go_Off_Center_Up;
          kneeLspeed_St_2_Standard = kneeLspeed_St_2_Standard_St_1_Go_Off_Center_Up;
          kneeRspeed_St_2_Standard = kneeRspeed_St_2_Standard_St_1_Go_Off_Center_Up;
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
      switch (ck_2) {
        case Robot__St_1_Go_Off_Center_Up:
          Robot__doNotMove_step(1.000000, _out->hipLspeed, _out->hipRspeed,
                                _out->kneeLspeed, _out->kneeRspeed,
                                &Robot__doNotMove_out_st);
          v_103 = Robot__doNotMove_out_st.ok;
          if (v_103) {
            nr_1_St_1_Go_Off_Center_Up = true;
            ns_1_St_1_Go_Off_Center_Up = Robot__St_1_Go_Down;
          } else {
            nr_1_St_1_Go_Off_Center_Up = false;
            ns_1_St_1_Go_Off_Center_Up = Robot__St_1_Go_Off_Center_Up;
          };
          ns_1 = ns_1_St_1_Go_Off_Center_Up;
          nr_1 = nr_1_St_1_Go_Off_Center_Up;
          break;
        case Robot__St_1_Go_Off_Center_Down:
          ns_1 = ns_1_St_1_Go_Off_Center_Down;
          nr_1 = nr_1_St_1_Go_Off_Center_Down;
          break;
        case Robot__St_1_Go_Down:
          ns_1 = ns_1_St_1_Go_Down;
          nr_1 = nr_1_St_1_Go_Down;
          break;
        case Robot__St_1_Center:
          ns_1 = ns_1_St_1_Center;
          nr_1 = nr_1_St_1_Center;
          break;
        default:
          break;
      };
      break;
    default:
      break;
  };
  switch (self->ck) {
    case Robot__St_3_Hand_Open:
      Robot__setMotor_step(fingerR2angle, -300.000000, 1000.000000,
                           &Robot__setMotor_out_st);
      fingerR2speed_St_3_Hand_Open = Robot__setMotor_out_st.power;
      Robot__setMotor_step(fingerR1angle, -80.000000, 1000.000000,
                           &Robot__setMotor_out_st);
      fingerR1speed_St_3_Hand_Open = Robot__setMotor_out_st.power;
      Robot__setMotor_step(fingerL2angle, 300.000000, 1000.000000,
                           &Robot__setMotor_out_st);
      fingerL2speed_St_3_Hand_Open = Robot__setMotor_out_st.power;
      Robot__setMotor_step(fingerL1angle, 80.000000, 1000.000000,
                           &Robot__setMotor_out_st);
      fingerL1speed_St_3_Hand_Open = Robot__setMotor_out_st.power;
      v_91 = !(self->v_90);
      v_92 = (v_91&&action_x);
      if (r_6) {
        v_93 = false;
      } else {
        v_93 = v_92;
      };
      if (v_93) {
        nr_3_St_3_Hand_Open = true;
        ns_3_St_3_Hand_Open = Robot__St_3_Hand_Close;
      } else {
        nr_3_St_3_Hand_Open = false;
        ns_3_St_3_Hand_Open = Robot__St_3_Hand_Open;
      };
      _out->fingerL1speed = fingerL1speed_St_3_Hand_Open;
      _out->fingerL2speed = fingerL2speed_St_3_Hand_Open;
      _out->fingerR1speed = fingerR1speed_St_3_Hand_Open;
      _out->fingerR2speed = fingerR2speed_St_3_Hand_Open;
      ns_3 = ns_3_St_3_Hand_Open;
      nr_3 = nr_3_St_3_Hand_Open;
      self->v_90 = action_x;
      break;
    case Robot__St_3_Hand_Close:
      Robot__setMotor_step(fingerR2angle, -300.000000, 1000.000000,
                           &Robot__setMotor_out_st);
      fingerR2speed_St_3_Hand_Close = Robot__setMotor_out_st.power;
      Robot__setMotor_step(fingerR1angle, -20.000000, 1000.000000,
                           &Robot__setMotor_out_st);
      fingerR1speed_St_3_Hand_Close = Robot__setMotor_out_st.power;
      Robot__setMotor_step(fingerL2angle, 300.000000, 1000.000000,
                           &Robot__setMotor_out_st);
      fingerL2speed_St_3_Hand_Close = Robot__setMotor_out_st.power;
      Robot__setMotor_step(fingerL1angle, 20.000000, 1000.000000,
                           &Robot__setMotor_out_st);
      fingerL1speed_St_3_Hand_Close = Robot__setMotor_out_st.power;
      v_87 = !(self->v_86);
      v_88 = (v_87&&action_x);
      if (r_6) {
        v_89 = false;
      } else {
        v_89 = v_88;
      };
      if (v_89) {
        nr_3_St_3_Hand_Close = true;
        ns_3_St_3_Hand_Close = Robot__St_3_Hand_Open;
      } else {
        nr_3_St_3_Hand_Close = false;
        ns_3_St_3_Hand_Close = Robot__St_3_Hand_Close;
      };
      _out->fingerL1speed = fingerL1speed_St_3_Hand_Close;
      _out->fingerL2speed = fingerL2speed_St_3_Hand_Close;
      _out->fingerR1speed = fingerR1speed_St_3_Hand_Close;
      _out->fingerR2speed = fingerR2speed_St_3_Hand_Close;
      ns_3 = ns_3_St_3_Hand_Close;
      nr_3 = nr_3_St_3_Hand_Close;
      self->v_86 = action_x;
      break;
    default:
      break;
  };
  self->pnr_2 = nr_2;
  self->pnr_3 = nr_3;
  self->ck = ns_3;
  self->v_78 = _out->arm_target_angle;
  self->v_77 = false;
  self->v_72 = body_target_y;
  self->v_71 = false;
  self->v_66 = _out->arm_target_y;
  self->v_65 = false;
  self->v_60 = _out->arm_target_x;
  self->v_59 = false;
  self->v_54 = gyroscope;
  self->v = false;
  switch (self->ck_1) {
    case Robot__St_2_Upside_Down_Left:
      self->v_174 = time;
      break;
    case Robot__St_2_Upside_Down_Right:
      self->v_160 = time_1;
      break;
    case Robot__St_2_Standard:
      self->v_140 = nr;
      self->v_139 = ns;
      self->v_137 = nr_1;
      self->v_136 = ns_1;
      break;
    default:
      break;
  };
  self->ck_1 = ns_2;;
}

