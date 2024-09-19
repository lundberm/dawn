struct VertexOutput {
  float4 pos;
  int4 prevent_dce;
};

struct vertex_main_outputs {
  nointerpolation int4 VertexOutput_prevent_dce : TEXCOORD0;
  float4 VertexOutput_pos : SV_Position;
};


RWByteAddressBuffer prevent_dce : register(u0);
TextureCubeArray<int4> arg_1 : register(t1, space1);
SamplerState arg_2 : register(s2, space1);
int4 textureGather_c0640c() {
  TextureCubeArray<int4> v = arg_1;
  SamplerState v_1 = arg_2;
  int4 res = v.GatherGreen(v_1, float4((1.0f).xxx, float(int(1))));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(textureGather_c0640c()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(textureGather_c0640c()));
}

VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = (VertexOutput)0;
  tint_symbol.pos = (0.0f).xxxx;
  tint_symbol.prevent_dce = textureGather_c0640c();
  VertexOutput v_2 = tint_symbol;
  return v_2;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_3 = vertex_main_inner();
  VertexOutput v_4 = v_3;
  VertexOutput v_5 = v_3;
  vertex_main_outputs v_6 = {v_5.prevent_dce, v_4.pos};
  return v_6;
}

