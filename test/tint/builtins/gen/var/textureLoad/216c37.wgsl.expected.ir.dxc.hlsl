//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture1D<uint4> arg_0 : register(t0, space1);
uint4 textureLoad_216c37() {
  uint arg_1 = 1u;
  int arg_2 = int(1);
  uint v = arg_1;
  uint2 v_1 = (0u).xx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y);
  uint v_2 = min(uint(arg_2), (v_1.y - 1u));
  uint2 v_3 = (0u).xx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y);
  int v_4 = int(min(v, (v_3.x - 1u)));
  uint4 res = uint4(arg_0.Load(int2(v_4, int(v_2))));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, textureLoad_216c37());
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture1D<uint4> arg_0 : register(t0, space1);
uint4 textureLoad_216c37() {
  uint arg_1 = 1u;
  int arg_2 = int(1);
  uint v = arg_1;
  uint2 v_1 = (0u).xx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y);
  uint v_2 = min(uint(arg_2), (v_1.y - 1u));
  uint2 v_3 = (0u).xx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y);
  int v_4 = int(min(v, (v_3.x - 1u)));
  uint4 res = uint4(arg_0.Load(int2(v_4, int(v_2))));
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, textureLoad_216c37());
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
uint4 textureLoad_216c37() {
  uint arg_1 = 1u;
  int arg_2 = int(1);
  uint v = arg_1;
  uint2 v_1 = (0u).xx;
  arg_0.GetDimensions(0u, v_1.x, v_1.y);
  uint v_2 = min(uint(arg_2), (v_1.y - 1u));
  uint2 v_3 = (0u).xx;
  arg_0.GetDimensions(uint(v_2), v_3.x, v_3.y);
  int v_4 = int(min(v, (v_3.x - 1u)));
  uint4 res = uint4(arg_0.Load(int2(v_4, int(v_2))));
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_5 = (VertexOutput)0;
  v_5.pos = (0.0f).xxxx;
  v_5.prevent_dce = textureLoad_216c37();
  VertexOutput v_6 = v_5;
  return v_6;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_7 = vertex_main_inner();
  vertex_main_outputs v_8 = {v_7.prevent_dce, v_7.pos};
  return v_8;
}

