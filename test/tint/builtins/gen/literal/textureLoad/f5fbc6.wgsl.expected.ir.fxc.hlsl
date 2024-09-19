
RWByteAddressBuffer prevent_dce : register(u0);
RWTexture2DArray<float4> arg_0 : register(u0, space1);
float4 textureLoad_f5fbc6() {
  RWTexture2DArray<float4> v = arg_0;
  int2 v_1 = int2((int(1)).xx);
  float4 res = float4(v.Load(int4(v_1, int(int(1)), int(0))));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_f5fbc6()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_f5fbc6()));
}

