
RWTexture2DArray<float4> arg_0 : register(u0, space1);
void textureStore_9d9cd5() {
  RWTexture2DArray<float4> v = arg_0;
  v[int3((int(1)).xx, int(int(1)))] = (1.0f).xxxx;
}

void fragment_main() {
  textureStore_9d9cd5();
}

[numthreads(1, 1, 1)]
void compute_main() {
  textureStore_9d9cd5();
}

