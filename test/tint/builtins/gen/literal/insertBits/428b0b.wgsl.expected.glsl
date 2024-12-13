//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  ivec3 inner;
} v;
ivec3 insertBits_428b0b() {
  ivec3 res = ivec3(3);
  return res;
}
void main() {
  v.inner = insertBits_428b0b();
}
//
// compute_main
//
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  ivec3 inner;
} v;
ivec3 insertBits_428b0b() {
  ivec3 res = ivec3(3);
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = insertBits_428b0b();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
  ivec3 prevent_dce;
};

layout(location = 0) flat out ivec3 tint_interstage_location0;
ivec3 insertBits_428b0b() {
  ivec3 res = ivec3(3);
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v = VertexOutput(vec4(0.0f), ivec3(0));
  v.pos = vec4(0.0f);
  v.prevent_dce = insertBits_428b0b();
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
