//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  uvec4 inner;
} v;
layout(binding = 0, rgba16ui) uniform highp readonly uimage2DArray arg_0;
uvec4 textureLoad_23ff89() {
  uvec2 arg_1 = uvec2(1u);
  uint arg_2 = 1u;
  uvec2 v_1 = arg_1;
  uint v_2 = arg_2;
  uint v_3 = min(v_2, (uint(imageSize(arg_0).z) - 1u));
  ivec2 v_4 = ivec2(min(v_1, (uvec2(imageSize(arg_0).xy) - uvec2(1u))));
  uvec4 res = imageLoad(arg_0, ivec3(v_4, int(v_3)));
  return res;
}
void main() {
  v.inner = textureLoad_23ff89();
}
//
// compute_main
//
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  uvec4 inner;
} v;
layout(binding = 0, rgba16ui) uniform highp readonly uimage2DArray arg_0;
uvec4 textureLoad_23ff89() {
  uvec2 arg_1 = uvec2(1u);
  uint arg_2 = 1u;
  uvec2 v_1 = arg_1;
  uint v_2 = arg_2;
  uint v_3 = min(v_2, (uint(imageSize(arg_0).z) - 1u));
  ivec2 v_4 = ivec2(min(v_1, (uvec2(imageSize(arg_0).xy) - uvec2(1u))));
  uvec4 res = imageLoad(arg_0, ivec3(v_4, int(v_3)));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureLoad_23ff89();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
  uvec4 prevent_dce;
};

layout(binding = 0, rgba16ui) uniform highp readonly uimage2DArray arg_0;
layout(location = 0) flat out uvec4 tint_interstage_location0;
uvec4 textureLoad_23ff89() {
  uvec2 arg_1 = uvec2(1u);
  uint arg_2 = 1u;
  uvec2 v = arg_1;
  uint v_1 = arg_2;
  uint v_2 = min(v_1, (uint(imageSize(arg_0).z) - 1u));
  ivec2 v_3 = ivec2(min(v, (uvec2(imageSize(arg_0).xy) - uvec2(1u))));
  uvec4 res = imageLoad(arg_0, ivec3(v_3, int(v_2)));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v_4 = VertexOutput(vec4(0.0f), uvec4(0u));
  v_4.pos = vec4(0.0f);
  v_4.prevent_dce = textureLoad_23ff89();
  return v_4;
}
void main() {
  VertexOutput v_5 = vertex_main_inner();
  gl_Position = v_5.pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  tint_interstage_location0 = v_5.prevent_dce;
  gl_PointSize = 1.0f;
}
