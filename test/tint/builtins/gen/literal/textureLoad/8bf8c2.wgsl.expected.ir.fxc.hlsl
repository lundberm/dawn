
RWByteAddressBuffer prevent_dce : register(u0);
RWTexture1D<float4> arg_0 : register(u0, space1);
float4 textureLoad_8bf8c2() {
  RWTexture1D<float4> v = arg_0;
  float4 res = float4(v.Load(int2(int(1u), int(0))));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_8bf8c2()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_8bf8c2()));
}

