
RWByteAddressBuffer prevent_dce : register(u0);
int subgroupMin_a96a2e() {
  int res = WaveActiveMin(int(1));
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, asuint(subgroupMin_a96a2e()));
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, asuint(subgroupMin_a96a2e()));
}

