//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  uvec3 inner;
} v;
uvec3 firstLeadingBit_3fd7d0() {
  uvec3 arg_0 = uvec3(1u);
  uvec3 v_1 = arg_0;
  uvec3 res = mix((mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u))) | (mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u))) | (mix(uvec3(4u), uvec3(0u), equal((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u))) | (mix(uvec3(2u), uvec3(0u), equal(((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) & uvec3(12u)), uvec3(0u))) | mix(uvec3(1u), uvec3(0u), equal((((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) >> mix(uvec3(2u), uvec3(0u), equal(((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) & uvec3(12u)), uvec3(0u)))) & uvec3(2u)), uvec3(0u))))))), uvec3(4294967295u), equal(((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) >> mix(uvec3(2u), uvec3(0u), equal(((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) & uvec3(12u)), uvec3(0u)))), uvec3(0u)));
  return res;
}
void main() {
  v.inner = firstLeadingBit_3fd7d0();
}
//
// compute_main
//
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  uvec3 inner;
} v;
uvec3 firstLeadingBit_3fd7d0() {
  uvec3 arg_0 = uvec3(1u);
  uvec3 v_1 = arg_0;
  uvec3 res = mix((mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u))) | (mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u))) | (mix(uvec3(4u), uvec3(0u), equal((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u))) | (mix(uvec3(2u), uvec3(0u), equal(((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) & uvec3(12u)), uvec3(0u))) | mix(uvec3(1u), uvec3(0u), equal((((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) >> mix(uvec3(2u), uvec3(0u), equal(((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) & uvec3(12u)), uvec3(0u)))) & uvec3(2u)), uvec3(0u))))))), uvec3(4294967295u), equal(((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) >> mix(uvec3(2u), uvec3(0u), equal(((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v_1 >> mix(uvec3(16u), uvec3(0u), equal((v_1 & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) & uvec3(12u)), uvec3(0u)))), uvec3(0u)));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = firstLeadingBit_3fd7d0();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
  uvec3 prevent_dce;
};

layout(location = 0) flat out uvec3 tint_interstage_location0;
uvec3 firstLeadingBit_3fd7d0() {
  uvec3 arg_0 = uvec3(1u);
  uvec3 v = arg_0;
  uvec3 res = mix((mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u))) | (mix(uvec3(8u), uvec3(0u), equal(((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u))) | (mix(uvec3(4u), uvec3(0u), equal((((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u))) | (mix(uvec3(2u), uvec3(0u), equal(((((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) & uvec3(12u)), uvec3(0u))) | mix(uvec3(1u), uvec3(0u), equal((((((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) >> mix(uvec3(2u), uvec3(0u), equal(((((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) & uvec3(12u)), uvec3(0u)))) & uvec3(2u)), uvec3(0u))))))), uvec3(4294967295u), equal(((((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) >> mix(uvec3(2u), uvec3(0u), equal(((((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) >> mix(uvec3(4u), uvec3(0u), equal((((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) >> mix(uvec3(8u), uvec3(0u), equal(((v >> mix(uvec3(16u), uvec3(0u), equal((v & uvec3(4294901760u)), uvec3(0u)))) & uvec3(65280u)), uvec3(0u)))) & uvec3(240u)), uvec3(0u)))) & uvec3(12u)), uvec3(0u)))), uvec3(0u)));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v_1 = VertexOutput(vec4(0.0f), uvec3(0u));
  v_1.pos = vec4(0.0f);
  v_1.prevent_dce = firstLeadingBit_3fd7d0();
  return v_1;
}
void main() {
  VertexOutput v_2 = vertex_main_inner();
  gl_Position = v_2.pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  tint_interstage_location0 = v_2.prevent_dce;
  gl_PointSize = 1.0f;
}
