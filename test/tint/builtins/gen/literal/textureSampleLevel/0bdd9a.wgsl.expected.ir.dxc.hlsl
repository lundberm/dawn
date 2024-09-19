struct VertexOutput {
  float4 pos;
  float4 prevent_dce;
};

struct vertex_main_outputs {
  nointerpolation float4 VertexOutput_prevent_dce : TEXCOORD0;
  float4 VertexOutput_pos : SV_Position;
};


RWByteAddressBuffer prevent_dce : register(u0);
TextureCubeArray<float4> arg_0 : register(t0, space1);
SamplerState arg_1 : register(s1, space1);
float4 textureSampleLevel_0bdd9a() {
  TextureCubeArray<float4> v = arg_0;
  SamplerState v_1 = arg_1;
  float4 v_2 = float4((1.0f).xxx, float(int(1)));
  float4 res = v.SampleLevel(v_1, v_2, float(1.0f));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(textureSampleLevel_0bdd9a()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(textureSampleLevel_0bdd9a()));
}

VertexOutput vertex_main_inner() {
  VertexOutput tint_symbol = (VertexOutput)0;
  tint_symbol.pos = (0.0f).xxxx;
  tint_symbol.prevent_dce = textureSampleLevel_0bdd9a();
  VertexOutput v_3 = tint_symbol;
  return v_3;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_4 = vertex_main_inner();
  VertexOutput v_5 = v_4;
  VertexOutput v_6 = v_4;
  vertex_main_outputs v_7 = {v_6.prevent_dce, v_5.pos};
  return v_7;
}

