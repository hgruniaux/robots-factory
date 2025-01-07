/* --- Generated the 7/1/2025 at 11:59 --- */
/* --- heptagon compiler, version 1.05.00 (compiled mon. sep. 23 14:27:43 CET 2024) --- */
/* --- Command line: /home/vincent/.opam/heptagon/bin/heptc -target c robot.ept --- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "robot.h"

void Robot__robot_reset(Robot__robot_mem* self) {
  self->v = true;
}

void Robot__robot_step(float dt, float hipLangle, float hipRangle,
                       float kneeLangle, float kneeRangle, float gyroscope,
                       float calfLdistance, float calfRdistance,
                       Robot__robot_out* _out, Robot__robot_mem* self) {
  Lib__foo_out Lib__foo_out_st;
  
  float x;
  float y;
  Lib__foo_step(1.000000, false, &Lib__foo_out_st);
  x = Lib__foo_out_st.x;
  y = Lib__foo_out_st.y;
  _out->kneeRspeed = x;
  _out->kneeLspeed = 1.100000;
  if (self->v) {
    _out->hipRspeed = 1.100000;
  } else {
    _out->hipRspeed = x;
  };
  _out->hipLspeed = 1.100000;
  self->v = false;;
}

