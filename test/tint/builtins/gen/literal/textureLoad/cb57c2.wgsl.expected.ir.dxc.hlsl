//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2DArray arg_0 : register(t0, space1);
float textureLoad_cb57c2() {
  uint3 v = (0u).xxx;
  arg_0.GetDimensions(v.x, v.y, v.z);
  uint4 v_1 = (0u).xxxx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y, v_1.z, v_1.w);
  uint v_2 = min(uint(int(1)), (v_1.w - 1u));
  uint4 v_3 = (0u).xxxx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y, v_3.z, v_3.w);
  int2 v_4 = int2(min((1u).xx, (v_3.xy - (1u).xx)));
  int v_5 = int(min(1u, (v.z - 1u)));
  float res = arg_0.Load(int4(v_4, v_5, int(v_2))).x;
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(textureLoad_cb57c2()));
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2DArray arg_0 : register(t0, space1);
float textureLoad_cb57c2() {
  uint3 v = (0u).xxx;
  arg_0.GetDimensions(v.x, v.y, v.z);
  uint4 v_1 = (0u).xxxx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y, v_1.z, v_1.w);
  uint v_2 = min(uint(int(1)), (v_1.w - 1u));
  uint4 v_3 = (0u).xxxx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y, v_3.z, v_3.w);
  int2 v_4 = int2(min((1u).xx, (v_3.xy - (1u).xx)));
  int v_5 = int(min(1u, (v.z - 1u)));
  float res = arg_0.Load(int4(v_4, v_5, int(v_2))).x;
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(textureLoad_cb57c2()));
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
float textureLoad_cb57c2() {
  uint3 v = (0u).xxx;
  arg_0.GetDimensions(v.x, v.y, v.z);
  uint4 v_1 = (0u).xxxx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y, v_1.z, v_1.w);
  uint v_2 = min(uint(int(1)), (v_1.w - 1u));
  uint4 v_3 = (0u).xxxx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y, v_3.z, v_3.w);
  int2 v_4 = int2(min((1u).xx, (v_3.xy - (1u).xx)));
  int v_5 = int(min(1u, (v.z - 1u)));
  float res = arg_0.Load(int4(v_4, v_5, int(v_2))).x;
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_6 = (VertexOutput)0;
  v_6.pos = (0.0f).xxxx;
  v_6.prevent_dce = textureLoad_cb57c2();
  VertexOutput v_7 = v_6;
  return v_7;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_8 = vertex_main_inner();
  vertex_main_outputs v_9 = {v_8.prevent_dce, v_8.pos};
  return v_9;
}

