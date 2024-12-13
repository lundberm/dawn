//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  vec3 inner;
} v;
vec3 sinh_c9a5eb() {
  vec3 res = vec3(1.17520117759704589844f);
  return res;
}
void main() {
  v.inner = sinh_c9a5eb();
}
//
// compute_main
//
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  vec3 inner;
} v;
vec3 sinh_c9a5eb() {
  vec3 res = vec3(1.17520117759704589844f);
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = sinh_c9a5eb();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
  vec3 prevent_dce;
};

layout(location = 0) flat out vec3 tint_interstage_location0;
vec3 sinh_c9a5eb() {
  vec3 res = vec3(1.17520117759704589844f);
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v = VertexOutput(vec4(0.0f), vec3(0.0f));
  v.pos = vec4(0.0f);
  v.prevent_dce = sinh_c9a5eb();
  return v;
}
void main() {
  VertexOutput v_1 = vertex_main_inner();
  gl_Position = v_1.pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  tint_interstage_location0 = v_1.prevent_dce;
  gl_PointSize = 1.0f;
}
