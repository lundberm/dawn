//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;


struct frexp_result_vec2_f32 {
  vec2 member_0;
  ivec2 member_1;
};

void frexp_6fb3ad() {
  frexp_result_vec2_f32 res = frexp_result_vec2_f32(vec2(0.5f), ivec2(1));
}
void main() {
  frexp_6fb3ad();
}
//
// compute_main
//
#version 310 es


struct frexp_result_vec2_f32 {
  vec2 member_0;
  ivec2 member_1;
};

void frexp_6fb3ad() {
  frexp_result_vec2_f32 res = frexp_result_vec2_f32(vec2(0.5f), ivec2(1));
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  frexp_6fb3ad();
}
//
// vertex_main
//
#version 310 es


struct frexp_result_vec2_f32 {
  vec2 member_0;
  ivec2 member_1;
};

struct VertexOutput {
  vec4 pos;
};

void frexp_6fb3ad() {
  frexp_result_vec2_f32 res = frexp_result_vec2_f32(vec2(0.5f), ivec2(1));
}
VertexOutput vertex_main_inner() {
  VertexOutput v = VertexOutput(vec4(0.0f));
  v.pos = vec4(0.0f);
  frexp_6fb3ad();
  return v;
}
void main() {
  gl_Position = vertex_main_inner().pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  gl_PointSize = 1.0f;
}
