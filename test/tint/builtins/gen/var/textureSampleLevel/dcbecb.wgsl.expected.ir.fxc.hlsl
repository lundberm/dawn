//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture3D<float4> arg_0 : register(t0, space1);
SamplerState arg_1 : register(s1, space1);
float4 textureSampleLevel_dcbecb() {
  float3 arg_2 = (1.0f).xxx;
  float arg_3 = 1.0f;
  float3 v = arg_2;
  float4 res = arg_0.SampleLevel(arg_1, v, float(arg_3), (int(1)).xxx);
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(textureSampleLevel_dcbecb()));
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture3D<float4> arg_0 : register(t0, space1);
SamplerState arg_1 : register(s1, space1);
float4 textureSampleLevel_dcbecb() {
  float3 arg_2 = (1.0f).xxx;
  float arg_3 = 1.0f;
  float3 v = arg_2;
  float4 res = arg_0.SampleLevel(arg_1, v, float(arg_3), (int(1)).xxx);
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(textureSampleLevel_dcbecb()));
}

//
// vertex_main
//
struct VertexOutput {
  float4 pos;
  float4 prevent_dce;
};

struct vertex_main_outputs {
  nointerpolation float4 VertexOutput_prevent_dce : TEXCOORD0;
  float4 VertexOutput_pos : SV_Position;
};


Texture3D<float4> arg_0 : register(t0, space1);
SamplerState arg_1 : register(s1, space1);
float4 textureSampleLevel_dcbecb() {
  float3 arg_2 = (1.0f).xxx;
  float arg_3 = 1.0f;
  float3 v = arg_2;
  float4 res = arg_0.SampleLevel(arg_1, v, float(arg_3), (int(1)).xxx);
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_1 = (VertexOutput)0;
  v_1.pos = (0.0f).xxxx;
  v_1.prevent_dce = textureSampleLevel_dcbecb();
  VertexOutput v_2 = v_1;
  return v_2;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_3 = vertex_main_inner();
  vertex_main_outputs v_4 = {v_3.prevent_dce, v_3.pos};
  return v_4;
}

