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
  vec4 inner;
} v;
layout(binding = 0, std140)
uniform f_tint_symbol_ubo {
  TintTextureUniformData inner;
} v_1;
uniform highp sampler2D arg_0;
vec4 textureLoad_3da3ed() {
  int arg_1 = 1;
  uint arg_2 = 1u;
  int v_2 = arg_1;
  uint v_3 = min(arg_2, (v_1.inner.tint_builtin_value_0 - 1u));
  uint v_4 = (uvec2(textureSize(arg_0, int(v_3))).x - 1u);
  ivec2 v_5 = ivec2(uvec2(min(uint(v_2), v_4), 0u));
  vec4 res = texelFetch(arg_0, v_5, int(v_3));
  return res;
}
void main() {
  v.inner = textureLoad_3da3ed();
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
  vec4 inner;
} v;
layout(binding = 0, std140)
uniform tint_symbol_1_ubo {
  TintTextureUniformData inner;
} v_1;
uniform highp sampler2D arg_0;
vec4 textureLoad_3da3ed() {
  int arg_1 = 1;
  uint arg_2 = 1u;
  int v_2 = arg_1;
  uint v_3 = min(arg_2, (v_1.inner.tint_builtin_value_0 - 1u));
  uint v_4 = (uvec2(textureSize(arg_0, int(v_3))).x - 1u);
  ivec2 v_5 = ivec2(uvec2(min(uint(v_2), v_4), 0u));
  vec4 res = texelFetch(arg_0, v_5, int(v_3));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureLoad_3da3ed();
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
  vec4 prevent_dce;
};

layout(binding = 0, std140)
uniform v_tint_symbol_ubo {
  TintTextureUniformData inner;
} v;
uniform highp sampler2D arg_0;
layout(location = 0) flat out vec4 tint_interstage_location0;
vec4 textureLoad_3da3ed() {
  int arg_1 = 1;
  uint arg_2 = 1u;
  int v_1 = arg_1;
  uint v_2 = min(arg_2, (v.inner.tint_builtin_value_0 - 1u));
  uint v_3 = (uvec2(textureSize(arg_0, int(v_2))).x - 1u);
  ivec2 v_4 = ivec2(uvec2(min(uint(v_1), v_3), 0u));
  vec4 res = texelFetch(arg_0, v_4, int(v_2));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v_5 = VertexOutput(vec4(0.0f), vec4(0.0f));
  v_5.pos = vec4(0.0f);
  v_5.prevent_dce = textureLoad_3da3ed();
  return v_5;
}
void main() {
  VertexOutput v_6 = vertex_main_inner();
  gl_Position = v_6.pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  tint_interstage_location0 = v_6.prevent_dce;
  gl_PointSize = 1.0f;
}
