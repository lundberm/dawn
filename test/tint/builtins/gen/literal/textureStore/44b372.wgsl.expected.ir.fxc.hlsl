
RWTexture2D<int4> arg_0 : register(u0, space1);
void textureStore_44b372() {
  arg_0[(int(1)).xx] = (int(1)).xxxx;
}

void fragment_main() {
  textureStore_44b372();
}

[numthreads(1, 1, 1)]
void compute_main() {
  textureStore_44b372();
}

