//
// fragment_main
//
#version 310 es
#extension GL_AMD_gpu_shader_half_float: require
precision highp float;
precision highp int;


struct modf_result_f16 {
  float16_t member_0;
  float16_t whole;
};

void modf_8dbbbf() {
  modf_result_f16 res = modf_result_f16(-0.5hf, -1.0hf);
}
void main() {
  modf_8dbbbf();
}
//
// compute_main
//
#version 310 es
#extension GL_AMD_gpu_shader_half_float: require


struct modf_result_f16 {
  float16_t member_0;
  float16_t whole;
};

void modf_8dbbbf() {
  modf_result_f16 res = modf_result_f16(-0.5hf, -1.0hf);
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  modf_8dbbbf();
}
//
// vertex_main
//
#version 310 es
#extension GL_AMD_gpu_shader_half_float: require


struct modf_result_f16 {
  float16_t member_0;
  float16_t whole;
};

struct VertexOutput {
  vec4 pos;
};

void modf_8dbbbf() {
  modf_result_f16 res = modf_result_f16(-0.5hf, -1.0hf);
}
VertexOutput vertex_main_inner() {
  VertexOutput v = VertexOutput(vec4(0.0f));
  v.pos = vec4(0.0f);
  modf_8dbbbf();
  return v;
}
void main() {
  gl_Position = vertex_main_inner().pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  gl_PointSize = 1.0f;
}
