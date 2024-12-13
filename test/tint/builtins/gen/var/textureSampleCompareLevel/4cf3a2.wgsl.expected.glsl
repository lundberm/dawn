//
// fragment_main
//
#version 460
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  float inner;
} v;
uniform highp samplerCubeArrayShadow arg_0_arg_1;
float textureSampleCompareLevel_4cf3a2() {
  vec3 arg_2 = vec3(1.0f);
  int arg_3 = 1;
  float arg_4 = 1.0f;
  vec3 v_1 = arg_2;
  float v_2 = arg_4;
  float res = texture(arg_0_arg_1, vec4(v_1, float(arg_3)), v_2);
  return res;
}
void main() {
  v.inner = textureSampleCompareLevel_4cf3a2();
}
//
// compute_main
//
#version 460

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  float inner;
} v;
uniform highp samplerCubeArrayShadow arg_0_arg_1;
float textureSampleCompareLevel_4cf3a2() {
  vec3 arg_2 = vec3(1.0f);
  int arg_3 = 1;
  float arg_4 = 1.0f;
  vec3 v_1 = arg_2;
  float v_2 = arg_4;
  float res = texture(arg_0_arg_1, vec4(v_1, float(arg_3)), v_2);
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureSampleCompareLevel_4cf3a2();
}
//
// vertex_main
//
#version 460


struct VertexOutput {
  vec4 pos;
  float prevent_dce;
};

uniform highp samplerCubeArrayShadow arg_0_arg_1;
layout(location = 0) flat out float tint_interstage_location0;
float textureSampleCompareLevel_4cf3a2() {
  vec3 arg_2 = vec3(1.0f);
  int arg_3 = 1;
  float arg_4 = 1.0f;
  vec3 v = arg_2;
  float v_1 = arg_4;
  float res = texture(arg_0_arg_1, vec4(v, float(arg_3)), v_1);
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v_2 = VertexOutput(vec4(0.0f), 0.0f);
  v_2.pos = vec4(0.0f);
  v_2.prevent_dce = textureSampleCompareLevel_4cf3a2();
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
