//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  ivec4 inner;
} v;
uniform highp isampler2DArray arg_1_arg_2;
ivec4 textureGather_bd33b6() {
  vec2 arg_3 = vec2(1.0f);
  uint arg_4 = 1u;
  vec2 v_1 = arg_3;
  vec3 v_2 = vec3(v_1, float(arg_4));
  ivec4 res = textureGatherOffset(arg_1_arg_2, v_2, ivec2(1), int(1));
  return res;
}
void main() {
  v.inner = textureGather_bd33b6();
}
//
// compute_main
//
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  ivec4 inner;
} v;
uniform highp isampler2DArray arg_1_arg_2;
ivec4 textureGather_bd33b6() {
  vec2 arg_3 = vec2(1.0f);
  uint arg_4 = 1u;
  vec2 v_1 = arg_3;
  vec3 v_2 = vec3(v_1, float(arg_4));
  ivec4 res = textureGatherOffset(arg_1_arg_2, v_2, ivec2(1), int(1));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureGather_bd33b6();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
  ivec4 prevent_dce;
};

uniform highp isampler2DArray arg_1_arg_2;
layout(location = 0) flat out ivec4 tint_interstage_location0;
ivec4 textureGather_bd33b6() {
  vec2 arg_3 = vec2(1.0f);
  uint arg_4 = 1u;
  vec2 v = arg_3;
  vec3 v_1 = vec3(v, float(arg_4));
  ivec4 res = textureGatherOffset(arg_1_arg_2, v_1, ivec2(1), int(1));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v_2 = VertexOutput(vec4(0.0f), ivec4(0));
  v_2.pos = vec4(0.0f);
  v_2.prevent_dce = textureGather_bd33b6();
  return v_2;
}
void main() {
  VertexOutput v_3 = vertex_main_inner();
  gl_Position = v_3.pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  tint_interstage_location0 = v_3.prevent_dce;
  gl_PointSize = 1.0f;
}
