/* --- Generated the 14/1/2025 at 14:12 --- */
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

void Robot__setMotor_step(float current, float goal, float maxi,
                          Robot__setMotor_out* _out) {
  Lib__fmod_out Lib__fmod_out_st;
  Lib__min_out Lib__min_out_st;
  Lib__abs_out Lib__abs_out_st;
  
  float v_12;
  float v_11;
  float v_10;
  float v_9;
  float v_8;
  int v_7;
  int v_6;
  float v_5;
  float v;
  float delta;
  float delta2;
  v = (goal-current);
  Lib__fmod_step(v, 360.000000, &Lib__fmod_out_st);
  delta = Lib__fmod_out_st.x;
  Lib__abs_step(delta, &Lib__abs_out_st);
  v_5 = Lib__abs_out_st.x;
  v_6 = (v_5<=1.000000);
  if (v_6) {
    delta2 = 0.000000;
  } else {
    delta2 = delta;
  };
  Lib__abs_step(delta2, &Lib__abs_out_st);
  v_10 = Lib__abs_out_st.x;
  Lib__min_step(v_10, maxi, &Lib__min_out_st);
  v_11 = Lib__min_out_st.x;
  v_12 = -(v_11);
  Lib__abs_step(delta2, &Lib__abs_out_st);
  v_8 = Lib__abs_out_st.x;
  Lib__min_step(v_8, maxi, &Lib__min_out_st);
  v_9 = Lib__min_out_st.x;
  v_7 = (delta2<180.000000);
  if (v_7) {
    _out->power = v_9;
  } else {
    _out->power = v_12;
  };;
}

void Robot__setMotorFast_step(float current, float goal, float maxi,
                              Robot__setMotorFast_out* _out) {
  Lib__fmod_out Lib__fmod_out_st;
  
  float v_14;
  int v_13;
  float v;
  float delta;
  v_14 = -(maxi);
  v = (goal-current);
  Lib__fmod_step(v, 360.000000, &Lib__fmod_out_st);
  delta = Lib__fmod_out_st.x;
  v_13 = (delta<180.000000);
  if (v_13) {
    _out->power = maxi;
  } else {
    _out->power = v_14;
  };;
}

void Robot__setMotorPID_reset(Robot__setMotorPID_mem* self) {
  self->v_18 = true;
  self->v_15 = true;
}

void Robot__setMotorPID_step(float currentAngle, float goalAngle,
                             float maxSpeed, float kp, float ki, float kd,
                             Robot__setMotorPID_out* _out,
                             Robot__setMotorPID_mem* self) {
  Robot__mod2_out Robot__mod2_out_st;
  Lib__abs_out Lib__abs_out_st;
  
  float v_32;
  float v_31;
  int v_30;
  float v_29;
  float v_28;
  int v_27;
  float v_26;
  int v_25;
  float v_24;
  float v_23;
  float v_22;
  float v_21;
  float v_20;
  float v_17;
  float v;
  float error;
  float integral;
  float derivative;
  float lastError;
  float rawSpeed;
  v_28 = -(maxSpeed);
  v_26 = -(maxSpeed);
  v = (goalAngle-currentAngle);
  Robot__mod2_step(v, 360.000000, &Robot__mod2_out_st);
  error = Robot__mod2_out_st.r;
  lastError = error;
  v_21 = (kp*error);
  v_20 = (error-self->v_19);
  if (self->v_18) {
    derivative = 0.000000;
  } else {
    derivative = v_20;
  };
  v_24 = (kd*derivative);
  v_17 = (self->v_16+error);
  if (self->v_15) {
    integral = 0.000000;
  } else {
    integral = v_17;
  };
  v_22 = (ki*integral);
  v_23 = (v_21+v_22);
  rawSpeed = (v_23+v_24);
  Lib__abs_step(rawSpeed, &Lib__abs_out_st);
  v_29 = Lib__abs_out_st.x;
  v_30 = (v_29<1.000000);
  if (v_30) {
    v_31 = 0.000000;
  } else {
    v_31 = rawSpeed;
  };
  v_27 = (rawSpeed<v_26);
  if (v_27) {
    v_32 = v_28;
  } else {
    v_32 = v_31;
  };
  v_25 = (rawSpeed>maxSpeed);
  if (v_25) {
    _out->motorSpeed = maxSpeed;
  } else {
    _out->motorSpeed = v_32;
  };
  self->v_19 = lastError;
  self->v_18 = false;
  self->v_16 = integral;
  self->v_15 = false;;
}

void Robot__doNotMove_step(float limit, float speedA, float speedB,
                           float speedC, float speedD,
                           Robot__doNotMove_out* _out) {
  
  int v_37;
  int v_36;
  int v_35;
  int v_34;
  int v_33;
  int v;
  v_37 = (speedD<=limit);
  v_35 = (speedC<=limit);
  v_33 = (speedB<=limit);
  v = (speedA<=limit);
  v_34 = (v&&v_33);
  v_36 = (v_34&&v_35);
  _out->ok = (v_36&&v_37);;
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
  
  float v_40;
  float v_39;
  float v_38;
  float v;
  Lib__fmod_step(arm4goal, 360.000000, &Lib__fmod_out_st);
  v_40 = Lib__fmod_out_st.x;
  Robot__setMotorPID_step(arm4angle, v_40, 20.000000, 1.000000, 0.000000,
                          0.500000, &Robot__setMotorPID_out_st,
                          &self->setMotorPID_3);
  _out->arm4speed = Robot__setMotorPID_out_st.motorSpeed;
  Lib__fmod_step(arm3goal, 360.000000, &Lib__fmod_out_st);
  v_39 = Lib__fmod_out_st.x;
  Robot__setMotorPID_step(arm3angle, v_39, 20.000000, 0.900000, 0.000000,
                          0.400000, &Robot__setMotorPID_out_st,
                          &self->setMotorPID_2);
  _out->arm3speed = Robot__setMotorPID_out_st.motorSpeed;
  Lib__fmod_step(arm2goal, 360.000000, &Lib__fmod_out_st);
  v_38 = Lib__fmod_out_st.x;
  Robot__setMotorPID_step(arm2angle, v_38, 20.000000, 0.700000, 0.000000,
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
  self->v_64 = true;
  self->v_58 = true;
  self->v_52 = true;
  self->v_46 = true;
  self->v = true;
  self->v_94 = false;
  self->v_93 = Robot__St_Normal;
  self->v_91 = false;
  self->v_90 = Robot__St_1_Center;
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
  Lib__arm_ik_out Lib__arm_ik_out_st;
  Robot__setMotorLegs_out Robot__setMotorLegs_out_st;
  Robot__setMotorFast_out Robot__setMotorFast_out_st;
  Robot__setMotor_out Robot__setMotor_out_st;
  Lib__abs_out Lib__abs_out_st;
  Robot__mux_out Robot__mux_out_st;
  Lib__fmod_out Lib__fmod_out_st;
  Lib__clamp_out Lib__clamp_out_st;
  Robot__doNotMove_out Robot__doNotMove_out_st;
  Robot__setMotorArm_out Robot__setMotorArm_out_st;
  
  int v_76;
  int v_75;
  int v_74;
  int v_80;
  int v_79;
  int v_78;
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
  int nr_1_St_1_Center;
  Robot__st_1 ns_1_St_1_Center;
  float kneeRspeed_St_2_Standard_St_1_Center;
  float kneeLspeed_St_2_Standard_St_1_Center;
  float hipRspeed_St_2_Standard_St_1_Center;
  float hipLspeed_St_2_Standard_St_1_Center;
  Robot__st_1 ck_2;
  int v_92;
  int r_6;
  float theta1;
  float theta2;
  float theta3;
  float theta4;
  int r_7;
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
  int v_89;
  Robot__st_2 v_88;
  int v_87;
  int v_86;
  int v_85;
  int v_84;
  int v_83;
  int v_82;
  int v_81;
  Robot__st_1 ns_1;
  int r_3;
  int nr_1;
  int pnr_1;
  Robot__st ns;
  int r_2;
  int nr;
  int pnr;
  float v_101;
  float v_100;
  float v_99;
  float v_98;
  int v_97;
  int v_96;
  int v_95;
  int r_8;
  float v_115;
  int v_113;
  Robot__st_2 v_112;
  int v_111;
  int v_110;
  int v_109;
  int v_108;
  int v_107;
  int v_106;
  int v_105;
  int v_104;
  int v_103;
  int v_102;
  int r_9;
  float time_1;
  float v_129;
  int v_127;
  Robot__st_2 v_126;
  int v_125;
  int v_124;
  int v_123;
  int v_122;
  int v_121;
  int v_120;
  int v_119;
  int v_118;
  int v_117;
  int v_116;
  int r_10;
  float time;
  int v_146;
  int v_145;
  int v_144;
  Robot__st_2 v_143;
  int v_142;
  Robot__st_2 v_141;
  int v_140;
  int v_139;
  int v_138;
  int v_137;
  int v_136;
  int v_135;
  int v_134;
  int v_133;
  int v_132;
  int v_131;
  int v_130;
  int r_11;
  int isStatic;
  int v_148;
  int v_147;
  int r_12;
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
  float v_72;
  float v_71;
  float v_70;
  float v_69;
  float v_68;
  float v_67;
  float v_66;
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
  Robot__st_3 ns_3;
  int r_5;
  int nr_3;
  Robot__st_2 ns_2;
  int r_4;
  int nr_2;
  float dgyroscope;
  float body_target_y;
  float alti;
  r_4 = self->pnr_2;
  r_5 = self->pnr_3;
  Robot__mux_step(action_rb, -1.000000, 0.000000, &Robot__mux_out_st);
  v_67 = Robot__mux_out_st.r;
  Robot__mux_step(action_lb, 1.000000, 0.000000, &Robot__mux_out_st);
  v_66 = Robot__mux_out_st.r;
  v_68 = (v_66+v_67);
  v_69 = (v_68*dt);
  v_70 = (v_69*10.000000);
  v_71 = (self->v_65+v_70);
  Lib__fmod_step(v_71, 360.000000, &Lib__fmod_out_st);
  v_72 = Lib__fmod_out_st.x;
  if (self->v_64) {
    _out->arm_target_angle = 0.000000;
  } else {
    _out->arm_target_angle = v_72;
  };
  v_60 = (main_y*dt);
  v_61 = (v_60*0.500000);
  v_62 = (self->v_59+v_61);
  if (self->v_58) {
    v_63 = 0.200000;
  } else {
    v_63 = v_62;
  };
  Lib__clamp_step(v_63, 0.000000, 0.400000, &Lib__clamp_out_st);
  body_target_y = Lib__clamp_out_st.x;
  v_54 = (secondary_y*dt);
  v_55 = (v_54*0.500000);
  v_56 = (self->v_53+v_55);
  if (self->v_52) {
    v_57 = 0.200000;
  } else {
    v_57 = v_56;
  };
  Lib__clamp_step(v_57, 0.250000, 1.000000, &Lib__clamp_out_st);
  _out->arm_target_y = Lib__clamp_out_st.x;
  v_48 = (secondary_x*dt);
  v_49 = (v_48*0.500000);
  v_50 = (self->v_47+v_49);
  if (self->v_46) {
    v_51 = 0.000000;
  } else {
    v_51 = v_50;
  };
  Lib__clamp_step(v_51, -1.000000, 1.000000, &Lib__clamp_out_st);
  _out->arm_target_x = Lib__clamp_out_st.x;
  alti = bodyDistance;
  v_42 = (gyroscope-self->v_41);
  Lib__fmod_step(v_42, 360.000000, &Lib__fmod_out_st);
  v_43 = Lib__fmod_out_st.x;
  Lib__abs_step(v_43, &Lib__abs_out_st);
  v_44 = Lib__abs_out_st.x;
  v_45 = (v_44/dt);
  if (self->v) {
    dgyroscope = 100.000000;
  } else {
    dgyroscope = v_45;
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
      v_147 = (footLcontact||footRcontact);
      v_148 = (v_147||bodyContact);
      if (v_148) {
        nr_2_St_2_Falling = true;
        ns_2_St_2_Falling = Robot__St_2_Standard;
      } else {
        nr_2_St_2_Falling = false;
        ns_2_St_2_Falling = Robot__St_2_Falling;
      };
      r_12 = r_4;
      if (r_12) {
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
      v_139 = (gyroscope>40.000000);
      v_137 = (gyroscope<180.000000);
      v_135 = (gyroscope<320.000000);
      v_133 = (gyroscope>=180.000000);
      v_131 = (gyroscope>=320.000000);
      v_130 = (gyroscope<=40.000000);
      v_132 = (v_130||v_131);
      r_11 = r_4;
      if (r_11) {
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
      v_145 = Robot__doNotMove_out_st.ok;
      _out->arm1speed = arm1speed_St_2_Upside_Down_Init;
      _out->arm2speed = arm2speed_St_2_Upside_Down_Init;
      _out->arm3speed = arm3speed_St_2_Upside_Down_Init;
      _out->arm4speed = arm4speed_St_2_Upside_Down_Init;
      Robot__doNotMove_step(1.000000, _out->arm1speed, _out->arm2speed,
                            _out->arm3speed, _out->arm4speed,
                            &Robot__doNotMove_out_st);
      v_146 = Robot__doNotMove_out_st.ok;
      isStatic = (v_145&&v_146);
      v_138 = (isStatic&&v_137);
      v_140 = (v_138&&v_139);
      if (v_140) {
        v_142 = true;
        v_141 = Robot__St_2_Upside_Down_Right;
      } else {
        v_142 = false;
        v_141 = Robot__St_2_Upside_Down_Init;
      };
      v_134 = (isStatic&&v_133);
      v_136 = (v_134&&v_135);
      if (v_136) {
        v_144 = true;
      } else {
        v_144 = v_142;
      };
      if (v_132) {
        nr_2_St_2_Upside_Down_Init = true;
      } else {
        nr_2_St_2_Upside_Down_Init = v_144;
      };
      if (v_136) {
        v_143 = Robot__St_2_Upside_Down_Left;
      } else {
        v_143 = v_141;
      };
      if (v_132) {
        ns_2_St_2_Upside_Down_Init = Robot__St_2_Standard;
      } else {
        ns_2_St_2_Upside_Down_Init = v_143;
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
      v_129 = (self->v_128+dt);
      if (r_4) {
        time = 0.000000;
      } else {
        time = v_129;
      };
      v_124 = (time>=5.000000);
      v_122 = (dgyroscope<=1.000000);
      v_120 = (arm1angle<=-20.000000);
      v_117 = (gyroscope>=320.000000);
      v_116 = (gyroscope<=40.000000);
      v_118 = (v_116||v_117);
      r_10 = r_4;
      if (r_10) {
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
      v_119 = Robot__doNotMove_out_st.ok;
      v_121 = (v_119&&v_120);
      v_123 = (v_121&&v_122);
      v_125 = (v_123&&v_124);
      if (v_125) {
        v_127 = true;
      } else {
        v_127 = false;
      };
      if (v_118) {
        nr_2_St_2_Upside_Down_Left = true;
      } else {
        nr_2_St_2_Upside_Down_Left = v_127;
      };
      if (v_125) {
        v_126 = Robot__St_2_Upside_Down_Right;
      } else {
        v_126 = Robot__St_2_Upside_Down_Left;
      };
      if (v_118) {
        ns_2_St_2_Upside_Down_Left = Robot__St_2_Standard;
      } else {
        ns_2_St_2_Upside_Down_Left = v_126;
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
      v_115 = (self->v_114+dt);
      if (r_4) {
        time_1 = 0.000000;
      } else {
        time_1 = v_115;
      };
      v_110 = (time_1>=5.000000);
      v_108 = (dgyroscope<=1.000000);
      v_106 = (arm1angle>=20.000000);
      v_103 = (gyroscope>=320.000000);
      v_102 = (gyroscope<=40.000000);
      v_104 = (v_102||v_103);
      r_9 = r_4;
      if (r_9) {
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
      v_105 = Robot__doNotMove_out_st.ok;
      v_107 = (v_105&&v_106);
      v_109 = (v_107&&v_108);
      v_111 = (v_109&&v_110);
      if (v_111) {
        v_113 = true;
      } else {
        v_113 = false;
      };
      if (v_104) {
        nr_2_St_2_Upside_Down_Right = true;
      } else {
        nr_2_St_2_Upside_Down_Right = v_113;
      };
      if (v_111) {
        v_112 = Robot__St_2_Upside_Down_Left;
      } else {
        v_112 = Robot__St_2_Upside_Down_Right;
      };
      if (v_104) {
        ns_2_St_2_Upside_Down_Right = Robot__St_2_Standard;
      } else {
        ns_2_St_2_Upside_Down_Right = v_112;
      };
      ns_2 = ns_2_St_2_Upside_Down_Right;
      nr_2 = nr_2_St_2_Upside_Down_Right;
      break;
    case Robot__St_2_Jump:
      v_100 = (kneeRangle+hipRangle);
      v_101 = (v_100+gyroscope);
      Robot__setMotorFast_step(v_101, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeRspeed_St_2_Jump = Robot__setMotorFast_out_st.power;
      v_98 = (kneeLangle+hipLangle);
      v_99 = (v_98+gyroscope);
      Robot__setMotorFast_step(v_99, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      kneeLspeed_St_2_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipRangle, -90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipRspeed_St_2_Jump = Robot__setMotorFast_out_st.power;
      Robot__setMotorFast_step(hipLangle, 90.000000, 300.000000,
                               &Robot__setMotorFast_out_st);
      hipLspeed_St_2_Jump = Robot__setMotorFast_out_st.power;
      v_96 = !(footRcontact);
      v_95 = !(footLcontact);
      v_97 = (v_95&&v_96);
      if (v_97) {
        nr_2_St_2_Jump = true;
        ns_2_St_2_Jump = Robot__St_2_Falling;
      } else {
        nr_2_St_2_Jump = false;
        ns_2_St_2_Jump = Robot__St_2_Jump;
      };
      r_8 = r_4;
      if (r_8) {
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
        pnr = self->v_94;
      };
      r_2 = pnr;
      if (r_4) {
        ck_3 = Robot__St_Normal;
        pnr_1 = false;
      } else {
        ck_3 = self->v_93;
        pnr_1 = self->v_91;
      };
      r_3 = pnr_1;
      if (r_4) {
        ck_2 = Robot__St_1_Center;
      } else {
        ck_2 = self->v_90;
      };
      v_86 = (hipRangle<=180.000000);
      v_84 = (hipLangle>=180.000000);
      v_85 = (action_a&&v_84);
      v_87 = (v_85&&v_86);
      if (v_87) {
        v_89 = true;
        v_88 = Robot__St_2_Jump;
      } else {
        v_89 = false;
        v_88 = Robot__St_2_Standard;
      };
      v_82 = (gyroscope<=260.000000);
      v_81 = (gyroscope>=90.000000);
      v_83 = (v_81&&v_82);
      if (v_83) {
        nr_2_St_2_Standard = true;
        ns_2_St_2_Standard = Robot__St_2_Upside_Down_Init;
      } else {
        nr_2_St_2_Standard = v_89;
        ns_2_St_2_Standard = v_88;
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
          r_7 = (r_4||r_2);
          if (r_7) {
            Robot__setMotorArm_reset(&self->setMotorArm_1);
          };
          Robot__setMotorArm_step(arm1angle, arm2angle, arm3angle, arm4angle,
                                  320.000000, 30.000000, 80.000000,
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
                           0.260000, 0.260000, 0.260000, 0.300000,
                           &Lib__arm_ik_out_st);
          theta1 = Lib__arm_ik_out_st.theta1;
          theta2 = Lib__arm_ik_out_st.theta2;
          theta3 = Lib__arm_ik_out_st.theta3;
          theta4 = Lib__arm_ik_out_st.theta4;
          v_92 = !(action_b);
          if (v_92) {
            nr_St_Move = true;
            ns_St_Move = Robot__St_Normal;
          } else {
            nr_St_Move = false;
            ns_St_Move = Robot__St_Move;
          };
          r_6 = (r_4||r_2);
          if (r_6) {
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
      v_78 = !(self->v_77);
      v_79 = (v_78&&action_x);
      if (r_5) {
        v_80 = false;
      } else {
        v_80 = v_79;
      };
      if (v_80) {
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
      self->v_77 = action_x;
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
      v_74 = !(self->v_73);
      v_75 = (v_74&&action_x);
      if (r_5) {
        v_76 = false;
      } else {
        v_76 = v_75;
      };
      if (v_76) {
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
      self->v_73 = action_x;
      break;
    default:
      break;
  };
  self->pnr_2 = nr_2;
  self->pnr_3 = nr_3;
  self->ck = ns_3;
  self->v_65 = _out->arm_target_angle;
  self->v_64 = false;
  self->v_59 = body_target_y;
  self->v_58 = false;
  self->v_53 = _out->arm_target_y;
  self->v_52 = false;
  self->v_47 = _out->arm_target_x;
  self->v_46 = false;
  self->v_41 = gyroscope;
  self->v = false;
  switch (self->ck_1) {
    case Robot__St_2_Upside_Down_Left:
      self->v_128 = time;
      break;
    case Robot__St_2_Upside_Down_Right:
      self->v_114 = time_1;
      break;
    case Robot__St_2_Standard:
      self->v_94 = nr;
      self->v_93 = ns;
      self->v_91 = nr_1;
      self->v_90 = ns_1;
      break;
    default:
      break;
  };
  self->ck_1 = ns_2;;
}

