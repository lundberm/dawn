//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;


struct frexp_result_vec3_f32 {
  vec3 member_0;
  ivec3 member_1;
};

void frexp_bf45ae() {
  frexp_result_vec3_f32 res = frexp_result_vec3_f32(vec3(0.5f), ivec3(1));
}
void main() {
  frexp_bf45ae();
}
//
// compute_main
//
#version 310 es


struct frexp_result_vec3_f32 {
  vec3 member_0;
  ivec3 member_1;
};

void frexp_bf45ae() {
  frexp_result_vec3_f32 res = frexp_result_vec3_f32(vec3(0.5f), ivec3(1));
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  frexp_bf45ae();
}
//
// vertex_main
//
#version 310 es


struct frexp_result_vec3_f32 {
  vec3 member_0;
  ivec3 member_1;
};

struct VertexOutput {
  vec4 pos;
};

void frexp_bf45ae() {
  frexp_result_vec3_f32 res = frexp_result_vec3_f32(vec3(0.5f), ivec3(1));
}
VertexOutput vertex_main_inner() {
  VertexOutput v = VertexOutput(vec4(0.0f));
  v.pos = vec4(0.0f);
  frexp_bf45ae();
  return v;
}
void main() {
  gl_Position = vertex_main_inner().pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  gl_PointSize = 1.0f;
}
