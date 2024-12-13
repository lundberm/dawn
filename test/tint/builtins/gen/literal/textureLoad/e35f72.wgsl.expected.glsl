//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;


struct TintTextureUniformData {
  uint tint_builtin_value_0;
};

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  ivec4 inner;
} v;
layout(binding = 0, std140)
uniform f_tint_symbol_ubo {
  TintTextureUniformData inner;
} v_1;
uniform highp isampler3D arg_0;
ivec4 textureLoad_e35f72() {
  uint v_2 = min(1u, (v_1.inner.tint_builtin_value_0 - 1u));
  uvec3 v_3 = (uvec3(textureSize(arg_0, int(v_2))) - uvec3(1u));
  ivec3 v_4 = ivec3(min(uvec3(ivec3(1)), v_3));
  ivec4 res = texelFetch(arg_0, v_4, int(v_2));
  return res;
}
void main() {
  v.inner = textureLoad_e35f72();
}
//
// compute_main
//
#version 310 es


struct TintTextureUniformData {
  uint tint_builtin_value_0;
};

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  ivec4 inner;
} v;
layout(binding = 0, std140)
uniform tint_symbol_1_ubo {
  TintTextureUniformData inner;
} v_1;
uniform highp isampler3D arg_0;
ivec4 textureLoad_e35f72() {
  uint v_2 = min(1u, (v_1.inner.tint_builtin_value_0 - 1u));
  uvec3 v_3 = (uvec3(textureSize(arg_0, int(v_2))) - uvec3(1u));
  ivec3 v_4 = ivec3(min(uvec3(ivec3(1)), v_3));
  ivec4 res = texelFetch(arg_0, v_4, int(v_2));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureLoad_e35f72();
}
//
// vertex_main
//
#version 310 es


struct TintTextureUniformData {
  uint tint_builtin_value_0;
};

struct VertexOutput {
  vec4 pos;
  ivec4 prevent_dce;
};

layout(binding = 0, std140)
uniform v_tint_symbol_ubo {
  TintTextureUniformData inner;
} v;
uniform highp isampler3D arg_0;
layout(location = 0) flat out ivec4 tint_interstage_location0;
ivec4 textureLoad_e35f72() {
  uint v_1 = min(1u, (v.inner.tint_builtin_value_0 - 1u));
  uvec3 v_2 = (uvec3(textureSize(arg_0, int(v_1))) - uvec3(1u));
  ivec3 v_3 = ivec3(min(uvec3(ivec3(1)), v_2));
  ivec4 res = texelFetch(arg_0, v_3, int(v_1));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v_4 = VertexOutput(vec4(0.0f), ivec4(0));
  v_4.pos = vec4(0.0f);
  v_4.prevent_dce = textureLoad_e35f72();
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
