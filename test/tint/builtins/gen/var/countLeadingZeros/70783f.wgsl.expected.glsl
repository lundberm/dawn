//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  uvec2 inner;
} v;
uvec2 countLeadingZeros_70783f() {
  uvec2 arg_0 = uvec2(1u);
  uvec2 v_1 = arg_0;
  uvec2 res = ((mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u))) | (mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u))) | (mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u))) | (mix(uvec2(0u), uvec2(2u), lessThanEqual((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))), uvec2(1073741823u))) | (mix(uvec2(0u), uvec2(1u), lessThanEqual(((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))) << mix(uvec2(0u), uvec2(2u), lessThanEqual((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))), uvec2(1073741823u)))), uvec2(2147483647u))) | mix(uvec2(0u), uvec2(1u), equal(((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))) << mix(uvec2(0u), uvec2(2u), lessThanEqual((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))), uvec2(1073741823u)))), uvec2(0u)))))))) + mix(uvec2(0u), uvec2(1u), equal(((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))) << mix(uvec2(0u), uvec2(2u), lessThanEqual((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))), uvec2(1073741823u)))), uvec2(0u))));
  return res;
}
void main() {
  v.inner = countLeadingZeros_70783f();
}
//
// compute_main
//
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  uvec2 inner;
} v;
uvec2 countLeadingZeros_70783f() {
  uvec2 arg_0 = uvec2(1u);
  uvec2 v_1 = arg_0;
  uvec2 res = ((mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u))) | (mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u))) | (mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u))) | (mix(uvec2(0u), uvec2(2u), lessThanEqual((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))), uvec2(1073741823u))) | (mix(uvec2(0u), uvec2(1u), lessThanEqual(((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))) << mix(uvec2(0u), uvec2(2u), lessThanEqual((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))), uvec2(1073741823u)))), uvec2(2147483647u))) | mix(uvec2(0u), uvec2(1u), equal(((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))) << mix(uvec2(0u), uvec2(2u), lessThanEqual((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))), uvec2(1073741823u)))), uvec2(0u)))))))) + mix(uvec2(0u), uvec2(1u), equal(((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))) << mix(uvec2(0u), uvec2(2u), lessThanEqual((((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v_1 << mix(uvec2(0u), uvec2(16u), lessThanEqual(v_1, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))), uvec2(1073741823u)))), uvec2(0u))));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = countLeadingZeros_70783f();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
  uvec2 prevent_dce;
};

layout(location = 0) flat out uvec2 tint_interstage_location0;
uvec2 countLeadingZeros_70783f() {
  uvec2 arg_0 = uvec2(1u);
  uvec2 v = arg_0;
  uvec2 res = ((mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u))) | (mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u))) | (mix(uvec2(0u), uvec2(4u), lessThanEqual(((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u))) | (mix(uvec2(0u), uvec2(2u), lessThanEqual((((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))), uvec2(1073741823u))) | (mix(uvec2(0u), uvec2(1u), lessThanEqual(((((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))) << mix(uvec2(0u), uvec2(2u), lessThanEqual((((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))), uvec2(1073741823u)))), uvec2(2147483647u))) | mix(uvec2(0u), uvec2(1u), equal(((((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))) << mix(uvec2(0u), uvec2(2u), lessThanEqual((((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))), uvec2(1073741823u)))), uvec2(0u)))))))) + mix(uvec2(0u), uvec2(1u), equal(((((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))) << mix(uvec2(0u), uvec2(2u), lessThanEqual((((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))) << mix(uvec2(0u), uvec2(4u), lessThanEqual(((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))) << mix(uvec2(0u), uvec2(8u), lessThanEqual((v << mix(uvec2(0u), uvec2(16u), lessThanEqual(v, uvec2(65535u)))), uvec2(16777215u)))), uvec2(268435455u)))), uvec2(1073741823u)))), uvec2(0u))));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v_1 = VertexOutput(vec4(0.0f), uvec2(0u));
  v_1.pos = vec4(0.0f);
  v_1.prevent_dce = countLeadingZeros_70783f();
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
