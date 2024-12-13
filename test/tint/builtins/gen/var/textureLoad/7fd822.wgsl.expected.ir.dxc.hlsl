//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2D arg_0 : register(t0, space1);
float textureLoad_7fd822() {
  uint2 arg_1 = (1u).xx;
  int arg_2 = int(1);
  uint2 v = arg_1;
  uint3 v_1 = (0u).xxx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y, v_1.z);
  uint v_2 = min(uint(arg_2), (v_1.z - 1u));
  uint3 v_3 = (0u).xxx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y, v_3.z);
  int2 v_4 = int2(min(v, (v_3.xy - (1u).xx)));
  float res = arg_0.Load(int3(v_4, int(v_2))).x;
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(textureLoad_7fd822()));
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2D arg_0 : register(t0, space1);
float textureLoad_7fd822() {
  uint2 arg_1 = (1u).xx;
  int arg_2 = int(1);
  uint2 v = arg_1;
  uint3 v_1 = (0u).xxx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y, v_1.z);
  uint v_2 = min(uint(arg_2), (v_1.z - 1u));
  uint3 v_3 = (0u).xxx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y, v_3.z);
  int2 v_4 = int2(min(v, (v_3.xy - (1u).xx)));
  float res = arg_0.Load(int3(v_4, int(v_2))).x;
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(textureLoad_7fd822()));
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


Texture2D arg_0 : register(t0, space1);
float textureLoad_7fd822() {
  uint2 arg_1 = (1u).xx;
  int arg_2 = int(1);
  uint2 v = arg_1;
  uint3 v_1 = (0u).xxx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y, v_1.z);
  uint v_2 = min(uint(arg_2), (v_1.z - 1u));
  uint3 v_3 = (0u).xxx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y, v_3.z);
  int2 v_4 = int2(min(v, (v_3.xy - (1u).xx)));
  float res = arg_0.Load(int3(v_4, int(v_2))).x;
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_5 = (VertexOutput)0;
  v_5.pos = (0.0f).xxxx;
  v_5.prevent_dce = textureLoad_7fd822();
  VertexOutput v_6 = v_5;
  return v_6;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_7 = vertex_main_inner();
  vertex_main_outputs v_8 = {v_7.prevent_dce, v_7.pos};
  return v_8;
}

