//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2DArray<int4> arg_0 : register(t0, space1);
int4 textureLoad_582015() {
  uint3 v = (0u).xxx;
  arg_0.GetDimensions(v.x, v.y, v.z);
  uint v_1 = min(uint(int(1)), (v.z - 1u));
  uint3 v_2 = (0u).xxx;
  arg_0.GetDimensions(v_2.x, v_2.y, v_2.z);
  uint2 v_3 = (v_2.xy - (1u).xx);
  int2 v_4 = int2(min(uint2((int(1)).xx), v_3));
  int4 res = int4(arg_0.Load(int4(v_4, int(v_1), int(0))));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_582015()));
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2DArray<int4> arg_0 : register(t0, space1);
int4 textureLoad_582015() {
  uint3 v = (0u).xxx;
  arg_0.GetDimensions(v.x, v.y, v.z);
  uint v_1 = min(uint(int(1)), (v.z - 1u));
  uint3 v_2 = (0u).xxx;
  arg_0.GetDimensions(v_2.x, v_2.y, v_2.z);
  uint2 v_3 = (v_2.xy - (1u).xx);
  int2 v_4 = int2(min(uint2((int(1)).xx), v_3));
  int4 res = int4(arg_0.Load(int4(v_4, int(v_1), int(0))));
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_582015()));
}

//
// vertex_main
//
struct VertexOutput {
  float4 pos;
  int4 prevent_dce;
};

struct vertex_main_outputs {
  nointerpolation int4 VertexOutput_prevent_dce : TEXCOORD0;
  float4 VertexOutput_pos : SV_Position;
};


Texture2DArray<int4> arg_0 : register(t0, space1);
int4 textureLoad_582015() {
  uint3 v = (0u).xxx;
  arg_0.GetDimensions(v.x, v.y, v.z);
  uint v_1 = min(uint(int(1)), (v.z - 1u));
  uint3 v_2 = (0u).xxx;
  arg_0.GetDimensions(v_2.x, v_2.y, v_2.z);
  uint2 v_3 = (v_2.xy - (1u).xx);
  int2 v_4 = int2(min(uint2((int(1)).xx), v_3));
  int4 res = int4(arg_0.Load(int4(v_4, int(v_1), int(0))));
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_5 = (VertexOutput)0;
  v_5.pos = (0.0f).xxxx;
  v_5.prevent_dce = textureLoad_582015();
  VertexOutput v_6 = v_5;
  return v_6;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_7 = vertex_main_inner();
  vertex_main_outputs v_8 = {v_7.prevent_dce, v_7.pos};
  return v_8;
}

