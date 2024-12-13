//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2DMS<float4> arg_0 : register(t0, space1);
float4 textureLoad_b75d4a() {
  uint3 v = (0u).xxx;
  arg_0.GetDimensions(v.x, v.y, v.z);
  uint2 v_1 = (v.xy - (1u).xx);
  int2 v_2 = int2(min(uint2((int(1)).xx), v_1));
  float4 res = float4(arg_0.Load(v_2, int(1u)));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_b75d4a()));
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2DMS<float4> arg_0 : register(t0, space1);
float4 textureLoad_b75d4a() {
  uint3 v = (0u).xxx;
  arg_0.GetDimensions(v.x, v.y, v.z);
  uint2 v_1 = (v.xy - (1u).xx);
  int2 v_2 = int2(min(uint2((int(1)).xx), v_1));
  float4 res = float4(arg_0.Load(v_2, int(1u)));
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_b75d4a()));
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


Texture2DMS<float4> arg_0 : register(t0, space1);
float4 textureLoad_b75d4a() {
  uint3 v = (0u).xxx;
  arg_0.GetDimensions(v.x, v.y, v.z);
  uint2 v_1 = (v.xy - (1u).xx);
  int2 v_2 = int2(min(uint2((int(1)).xx), v_1));
  float4 res = float4(arg_0.Load(v_2, int(1u)));
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_3 = (VertexOutput)0;
  v_3.pos = (0.0f).xxxx;
  v_3.prevent_dce = textureLoad_b75d4a();
  VertexOutput v_4 = v_3;
  return v_4;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_5 = vertex_main_inner();
  vertex_main_outputs v_6 = {v_5.prevent_dce, v_5.pos};
  return v_6;
}

