//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture1D<uint4> arg_0 : register(t0, space1);
uint4 textureLoad_6b77d4() {
  int arg_1 = int(1);
  uint arg_2 = 1u;
  int v = arg_1;
  uint2 v_1 = (0u).xx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y);
  uint v_2 = min(arg_2, (v_1.y - 1u));
  uint2 v_3 = (0u).xx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y);
  uint v_4 = (v_3.x - 1u);
  int v_5 = int(min(uint(v), v_4));
  uint4 res = uint4(arg_0.Load(int2(v_5, int(v_2))));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, textureLoad_6b77d4());
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture1D<uint4> arg_0 : register(t0, space1);
uint4 textureLoad_6b77d4() {
  int arg_1 = int(1);
  uint arg_2 = 1u;
  int v = arg_1;
  uint2 v_1 = (0u).xx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y);
  uint v_2 = min(arg_2, (v_1.y - 1u));
  uint2 v_3 = (0u).xx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y);
  uint v_4 = (v_3.x - 1u);
  int v_5 = int(min(uint(v), v_4));
  uint4 res = uint4(arg_0.Load(int2(v_5, int(v_2))));
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, textureLoad_6b77d4());
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


Texture1D<uint4> arg_0 : register(t0, space1);
uint4 textureLoad_6b77d4() {
  int arg_1 = int(1);
  uint arg_2 = 1u;
  int v = arg_1;
  uint2 v_1 = (0u).xx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y);
  uint v_2 = min(arg_2, (v_1.y - 1u));
  uint2 v_3 = (0u).xx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y);
  uint v_4 = (v_3.x - 1u);
  int v_5 = int(min(uint(v), v_4));
  uint4 res = uint4(arg_0.Load(int2(v_5, int(v_2))));
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_6 = (VertexOutput)0;
  v_6.pos = (0.0f).xxxx;
  v_6.prevent_dce = textureLoad_6b77d4();
  VertexOutput v_7 = v_6;
  return v_7;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_8 = vertex_main_inner();
  vertex_main_outputs v_9 = {v_8.prevent_dce, v_8.pos};
  return v_9;
}

