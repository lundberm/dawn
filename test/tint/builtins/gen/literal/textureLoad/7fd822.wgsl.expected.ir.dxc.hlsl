//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2D arg_0 : register(t0, space1);
float textureLoad_7fd822() {
  uint3 v = (0u).xxx;
  arg_0.GetDimensions(0u, v.x, v.y, v.z);
  uint v_1 = min(uint(int(1)), (v.z - 1u));
  uint3 v_2 = (0u).xxx;
  arg_0.GetDimensions(uint(v_1), v_2.x, v_2.y, v_2.z);
  int2 v_3 = int2(min((1u).xx, (v_2.xy - (1u).xx)));
  float res = arg_0.Load(int3(v_3, int(v_1))).x;
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
  uint3 v = (0u).xxx;
  arg_0.GetDimensions(0u, v.x, v.y, v.z);
  uint v_1 = min(uint(int(1)), (v.z - 1u));
  uint3 v_2 = (0u).xxx;
  arg_0.GetDimensions(uint(v_1), v_2.x, v_2.y, v_2.z);
  int2 v_3 = int2(min((1u).xx, (v_2.xy - (1u).xx)));
  float res = arg_0.Load(int3(v_3, int(v_1))).x;
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
  uint3 v = (0u).xxx;
  arg_0.GetDimensions(0u, v.x, v.y, v.z);
  uint v_1 = min(uint(int(1)), (v.z - 1u));
  uint3 v_2 = (0u).xxx;
  arg_0.GetDimensions(uint(v_1), v_2.x, v_2.y, v_2.z);
  int2 v_3 = int2(min((1u).xx, (v_2.xy - (1u).xx)));
  float res = arg_0.Load(int3(v_3, int(v_1))).x;
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_4 = (VertexOutput)0;
  v_4.pos = (0.0f).xxxx;
  v_4.prevent_dce = textureLoad_7fd822();
  VertexOutput v_5 = v_4;
  return v_5;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_6 = vertex_main_inner();
  vertex_main_outputs v_7 = {v_6.prevent_dce, v_6.pos};
  return v_7;
}

