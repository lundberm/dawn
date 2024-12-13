//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture1D<int4> arg_0 : register(t0, space1);
int4 textureLoad_f06b69() {
  int arg_1 = int(1);
  uint v = 0u;
  arg_0.GetDimensions(v);
  uint v_1 = (v - 1u);
  int4 res = int4(arg_0.Load(int2(int(min(uint(arg_1), v_1)), int(0))));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_f06b69()));
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture1D<int4> arg_0 : register(t0, space1);
int4 textureLoad_f06b69() {
  int arg_1 = int(1);
  uint v = 0u;
  arg_0.GetDimensions(v);
  uint v_1 = (v - 1u);
  int4 res = int4(arg_0.Load(int2(int(min(uint(arg_1), v_1)), int(0))));
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_f06b69()));
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


Texture1D<int4> arg_0 : register(t0, space1);
int4 textureLoad_f06b69() {
  int arg_1 = int(1);
  uint v = 0u;
  arg_0.GetDimensions(v);
  uint v_1 = (v - 1u);
  int4 res = int4(arg_0.Load(int2(int(min(uint(arg_1), v_1)), int(0))));
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_2 = (VertexOutput)0;
  v_2.pos = (0.0f).xxxx;
  v_2.prevent_dce = textureLoad_f06b69();
  VertexOutput v_3 = v_2;
  return v_3;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_4 = vertex_main_inner();
  vertex_main_outputs v_5 = {v_4.prevent_dce, v_4.pos};
  return v_5;
}

