/* --- Generated the 7/1/2025 at 13:1 --- */
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

void Robot__robot_reset(Robot__robot_mem* self) {
  self->pnr = false;
  self->ck = Robot__St_On_the_ground;
}

void Robot__robot_step(float dt, float hipLangle, float hipRangle,
                       float kneeLangle, float kneeRangle, float gyroscope,
                       float calfLdistance, float calfRdistance,
                       Robot__robot_out* _out, Robot__robot_mem* self) {
  
  int nr_St_In_the_sky;
  Robot__st ns_St_In_the_sky;
  float kneeRspeed_St_In_the_sky;
  float kneeLspeed_St_In_the_sky;
  float hipRspeed_St_In_the_sky;
  float hipLspeed_St_In_the_sky;
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
  Robot__st ns;
  int r;
  int nr;
  r = self->pnr;
  switch (self->ck) {
    case Robot__St_On_the_ground:
      kneeRspeed_St_On_the_ground = (kneeRangle-30.000000);
      kneeLspeed_St_On_the_ground = (kneeLangle-30.000000);
      hipRspeed_St_On_the_ground = (hipRangle-30.000000);
      hipLspeed_St_On_the_ground = (hipLangle-30.000000);
      if (false) {
        nr_St_On_the_ground = true;
      } else {
        nr_St_On_the_ground = false;
      };
      if (false) {
        ns_St_On_the_ground = Robot__St_In_the_sky;
      } else {
        ns_St_On_the_ground = Robot__St_On_the_ground;
      };
      _out->hipLspeed = hipLspeed_St_On_the_ground;
      _out->hipRspeed = hipRspeed_St_On_the_ground;
      _out->kneeLspeed = kneeLspeed_St_On_the_ground;
      _out->kneeRspeed = kneeRspeed_St_On_the_ground;
      ns = ns_St_On_the_ground;
      nr = nr_St_On_the_ground;
      break;
    case Robot__St_Jump:
      kneeRspeed_St_Jump = 1.100000;
      kneeLspeed_St_Jump = 1.100000;
      hipRspeed_St_Jump = 1.100000;
      hipLspeed_St_Jump = 1.100000;
      if (false) {
        nr_St_Jump = true;
      } else {
        nr_St_Jump = false;
      };
      if (false) {
        ns_St_Jump = Robot__St_Jump;
      } else {
        ns_St_Jump = Robot__St_Jump;
      };
      _out->hipLspeed = hipLspeed_St_Jump;
      _out->hipRspeed = hipRspeed_St_Jump;
      _out->kneeLspeed = kneeLspeed_St_Jump;
      _out->kneeRspeed = kneeRspeed_St_Jump;
      ns = ns_St_Jump;
      nr = nr_St_Jump;
      break;
    case Robot__St_In_the_sky:
      kneeRspeed_St_In_the_sky = 1.100000;
      kneeLspeed_St_In_the_sky = 1.100000;
      hipRspeed_St_In_the_sky = 1.100000;
      hipLspeed_St_In_the_sky = 1.100000;
      if (false) {
        nr_St_In_the_sky = true;
      } else {
        nr_St_In_the_sky = false;
      };
      if (false) {
        ns_St_In_the_sky = Robot__St_On_the_ground;
      } else {
        ns_St_In_the_sky = Robot__St_In_the_sky;
      };
      _out->hipLspeed = hipLspeed_St_In_the_sky;
      _out->hipRspeed = hipRspeed_St_In_the_sky;
      _out->kneeLspeed = kneeLspeed_St_In_the_sky;
      _out->kneeRspeed = kneeRspeed_St_In_the_sky;
      ns = ns_St_In_the_sky;
      nr = nr_St_In_the_sky;
      break;
    default:
      break;
  };
  self->pnr = nr;
  self->ck = ns;;
}

