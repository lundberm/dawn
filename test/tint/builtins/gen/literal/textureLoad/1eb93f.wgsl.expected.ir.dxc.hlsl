//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2D<float4> arg_0 : register(t0, space1);
float4 textureLoad_1eb93f() {
  uint2 v = (0u).xx;
  arg_0.GetDimensions(v.x, v.y);
  float4 res = float4(arg_0.Load(int3(int2(min((1u).xx, (v - (1u).xx))), int(0))));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_1eb93f()));
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
Texture2D<float4> arg_0 : register(t0, space1);
float4 textureLoad_1eb93f() {
  uint2 v = (0u).xx;
  arg_0.GetDimensions(v.x, v.y);
  float4 res = float4(arg_0.Load(int3(int2(min((1u).xx, (v - (1u).xx))), int(0))));
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_1eb93f()));
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


Texture2D<float4> arg_0 : register(t0, space1);
float4 textureLoad_1eb93f() {
  uint2 v = (0u).xx;
  arg_0.GetDimensions(v.x, v.y);
  float4 res = float4(arg_0.Load(int3(int2(min((1u).xx, (v - (1u).xx))), int(0))));
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v_1 = (VertexOutput)0;
  v_1.pos = (0.0f).xxxx;
  v_1.prevent_dce = textureLoad_1eb93f();
  VertexOutput v_2 = v_1;
  return v_2;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_3 = vertex_main_inner();
  vertex_main_outputs v_4 = {v_3.prevent_dce, v_3.pos};
  return v_4;
}

