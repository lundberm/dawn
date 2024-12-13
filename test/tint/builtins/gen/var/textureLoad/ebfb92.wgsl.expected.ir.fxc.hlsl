//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2D<uint4> arg_0 : register(t0, space1);
uint4 textureLoad_ebfb92() {
  int2 arg_1 = (int(1)).xx;
  uint arg_2 = 1u;
  int2 v = arg_1;
  uint3 v_1 = (0u).xxx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y, v_1.z);
  uint v_2 = min(arg_2, (v_1.z - 1u));
  uint3 v_3 = (0u).xxx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y, v_3.z);
  uint2 v_4 = (v_3.xy - (1u).xx);
  int2 v_5 = int2(min(uint2(v), v_4));
  uint4 res = uint4(arg_0.Load(int3(v_5, int(v_2))));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, textureLoad_ebfb92());
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2D<uint4> arg_0 : register(t0, space1);
uint4 textureLoad_ebfb92() {
  int2 arg_1 = (int(1)).xx;
  uint arg_2 = 1u;
  int2 v = arg_1;
  uint3 v_1 = (0u).xxx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y, v_1.z);
  uint v_2 = min(arg_2, (v_1.z - 1u));
  uint3 v_3 = (0u).xxx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y, v_3.z);
  uint2 v_4 = (v_3.xy - (1u).xx);
  int2 v_5 = int2(min(uint2(v), v_4));
  uint4 res = uint4(arg_0.Load(int3(v_5, int(v_2))));
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, textureLoad_ebfb92());
}

//
// vertex_main
//
struct VertexOutput {
  float4 pos;
  uint4 prevent_dce;
};

struct vertex_main_outputs {
  nointerpolation uint4 VertexOutput_prevent_dce : TEXCOORD0;
  float4 VertexOutput_pos : SV_Position;
};


Texture2D<uint4> arg_0 : register(t0, space1);
uint4 textureLoad_ebfb92() {
  int2 arg_1 = (int(1)).xx;
  uint arg_2 = 1u;
  int2 v = arg_1;
  uint3 v_1 = (0u).xxx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y, v_1.z);
  uint v_2 = min(arg_2, (v_1.z - 1u));
  uint3 v_3 = (0u).xxx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y, v_3.z);
  uint2 v_4 = (v_3.xy - (1u).xx);
  int2 v_5 = int2(min(uint2(v), v_4));
  uint4 res = uint4(arg_0.Load(int3(v_5, int(v_2))));
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_6 = (VertexOutput)0;
  v_6.pos = (0.0f).xxxx;
  v_6.prevent_dce = textureLoad_ebfb92();
  VertexOutput v_7 = v_6;
  return v_7;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_8 = vertex_main_inner();
  vertex_main_outputs v_9 = {v_8.prevent_dce, v_8.pos};
  return v_9;
}

