//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2DArray arg_0 : register(t0, space1);
float textureLoad_66be47() {
  int2 arg_1 = (int(1)).xx;
  uint arg_2 = 1u;
  uint arg_3 = 1u;
  int2 v = arg_1;
  uint3 v_1 = (0u).xxx;
  arg_0.GetDimensions(v_1.x, v_1.y, v_1.z);
  uint v_2 = min(arg_2, (v_1.z - 1u));
  uint4 v_3 = (0u).xxxx;
  arg_0.GetDimensions(0u, v_3.x, v_3.y, v_3.z, v_3.w);
  uint v_4 = min(arg_3, (v_3.w - 1u));
  uint4 v_5 = (0u).xxxx;
  arg_0.GetDimensions(uint(v_4), v_5.x, v_5.y, v_5.z, v_5.w);
  uint2 v_6 = (v_5.xy - (1u).xx);
  int2 v_7 = int2(min(uint2(v), v_6));
  int v_8 = int(v_2);
  float res = arg_0.Load(int4(v_7, v_8, int(v_4))).x;
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(textureLoad_66be47()));
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2DArray arg_0 : register(t0, space1);
float textureLoad_66be47() {
  int2 arg_1 = (int(1)).xx;
  uint arg_2 = 1u;
  uint arg_3 = 1u;
  int2 v = arg_1;
  uint3 v_1 = (0u).xxx;
  arg_0.GetDimensions(v_1.x, v_1.y, v_1.z);
  uint v_2 = min(arg_2, (v_1.z - 1u));
  uint4 v_3 = (0u).xxxx;
  arg_0.GetDimensions(0u, v_3.x, v_3.y, v_3.z, v_3.w);
  uint v_4 = min(arg_3, (v_3.w - 1u));
  uint4 v_5 = (0u).xxxx;
  arg_0.GetDimensions(uint(v_4), v_5.x, v_5.y, v_5.z, v_5.w);
  uint2 v_6 = (v_5.xy - (1u).xx);
  int2 v_7 = int2(min(uint2(v), v_6));
  int v_8 = int(v_2);
  float res = arg_0.Load(int4(v_7, v_8, int(v_4))).x;
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(textureLoad_66be47()));
}

//
// vertex_main
//
struct VertexOutput {
  float4 pos;
  float prevent_dce;
};

struct vertex_main_outputs {
  nointerpolation float VertexOutput_prevent_dce : TEXCOORD0;
  float4 VertexOutput_pos : SV_Position;
};


Texture2DArray arg_0 : register(t0, space1);
float textureLoad_66be47() {
  int2 arg_1 = (int(1)).xx;
  uint arg_2 = 1u;
  uint arg_3 = 1u;
  int2 v = arg_1;
  uint3 v_1 = (0u).xxx;
  arg_0.GetDimensions(v_1.x, v_1.y, v_1.z);
  uint v_2 = min(arg_2, (v_1.z - 1u));
  uint4 v_3 = (0u).xxxx;
  arg_0.GetDimensions(0u, v_3.x, v_3.y, v_3.z, v_3.w);
  uint v_4 = min(arg_3, (v_3.w - 1u));
  uint4 v_5 = (0u).xxxx;
  arg_0.GetDimensions(uint(v_4), v_5.x, v_5.y, v_5.z, v_5.w);
  uint2 v_6 = (v_5.xy - (1u).xx);
  int2 v_7 = int2(min(uint2(v), v_6));
  int v_8 = int(v_2);
  float res = arg_0.Load(int4(v_7, v_8, int(v_4))).x;
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_9 = (VertexOutput)0;
  v_9.pos = (0.0f).xxxx;
  v_9.prevent_dce = textureLoad_66be47();
  VertexOutput v_10 = v_9;
  return v_10;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_11 = vertex_main_inner();
  vertex_main_outputs v_12 = {v_11.prevent_dce, v_11.pos};
  return v_12;
}

