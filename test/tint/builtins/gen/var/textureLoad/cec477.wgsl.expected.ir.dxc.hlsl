
RWByteAddressBuffer prevent_dce : register(u0);
RWTexture3D<int4> arg_0 : register(u0, space1);
int4 textureLoad_cec477() {
  uint3 arg_1 = (1u).xxx;
  RWTexture3D<int4> v = arg_0;
  int4 res = int4(v.Load(int4(int3(arg_1), int(0))));
  return res;
}

void fragment_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_cec477()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store4(0u, asuint(textureLoad_cec477()));
}

