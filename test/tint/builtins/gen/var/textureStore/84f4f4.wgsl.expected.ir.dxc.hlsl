
RWTexture2DArray<int4> arg_0 : register(u0, space1);
void textureStore_84f4f4() {
  uint2 arg_1 = (1u).xx;
  int arg_2 = int(1);
  int4 arg_3 = (int(1)).xxxx;
  RWTexture2DArray<int4> v = arg_0;
  uint2 v_1 = arg_1;
  int4 v_2 = arg_3;
  v[uint3(v_1, uint(arg_2))] = v_2;
}

void fragment_main() {
  textureStore_84f4f4();
}

[numthreads(1, 1, 1)]
void compute_main() {
  textureStore_84f4f4();
}

