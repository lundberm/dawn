//
// fragment_main
//
#version 460
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  vec4 inner;
} v;
uniform highp samplerCubeArrayShadow arg_0_arg_1;
vec4 textureGather_7dd226() {
  vec4 res = textureGather(arg_0_arg_1, vec4(vec3(1.0f), float(1u)), 0.0f);
  return res;
}
void main() {
  v.inner = textureGather_7dd226();
}
//
// compute_main
//
#version 460

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  vec4 inner;
} v;
uniform highp samplerCubeArrayShadow arg_0_arg_1;
vec4 textureGather_7dd226() {
  vec4 res = textureGather(arg_0_arg_1, vec4(vec3(1.0f), float(1u)), 0.0f);
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureGather_7dd226();
}
//
// vertex_main
//
#version 460


struct VertexOutput {
  vec4 pos;
  vec4 prevent_dce;
};

uniform highp samplerCubeArrayShadow arg_0_arg_1;
layout(location = 0) flat out vec4 tint_interstage_location0;
vec4 textureGather_7dd226() {
  vec4 res = textureGather(arg_0_arg_1, vec4(vec3(1.0f), float(1u)), 0.0f);
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v = VertexOutput(vec4(0.0f), vec4(0.0f));
  v.pos = vec4(0.0f);
  v.prevent_dce = textureGather_7dd226();
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
