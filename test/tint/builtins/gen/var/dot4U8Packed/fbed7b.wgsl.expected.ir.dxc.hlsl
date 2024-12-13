//
// fragment_main
//

RWByteAddressBuffer prevent_dce : register(u0);
uint dot4U8Packed_fbed7b() {
  uint arg_0 = 1u;
  uint arg_1 = 1u;
  uint accumulator = 0u;
  uint res = dot4add_u8packed(arg_0, arg_1, accumulator);
  return res;
}

void fragment_main() {
  prevent_dce.Store(0u, dot4U8Packed_fbed7b());
}

//
// compute_main
//

RWByteAddressBuffer prevent_dce : register(u0);
uint dot4U8Packed_fbed7b() {
  uint arg_0 = 1u;
  uint arg_1 = 1u;
  uint accumulator = 0u;
  uint res = dot4add_u8packed(arg_0, arg_1, accumulator);
  return res;
}

[numthreads(1, 1, 1)]
void compute_main() {
  prevent_dce.Store(0u, dot4U8Packed_fbed7b());
}

//
// vertex_main
//
struct VertexOutput {
  float4 pos;
  uint prevent_dce;
};

struct vertex_main_outputs {
  nointerpolation uint VertexOutput_prevent_dce : TEXCOORD0;
  float4 VertexOutput_pos : SV_Position;
};


uint dot4U8Packed_fbed7b() {
  uint arg_0 = 1u;
  uint arg_1 = 1u;
  uint accumulator = 0u;
  uint res = dot4add_u8packed(arg_0, arg_1, accumulator);
  return res;
}

VertexOutput vertex_main_inner() {
  VertexOutput v = (VertexOutput)0;
  v.pos = (0.0f).xxxx;
  v.prevent_dce = dot4U8Packed_fbed7b();
  VertexOutput v_1 = v;
  return v_1;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_2 = vertex_main_inner();
  vertex_main_outputs v_3 = {v_2.prevent_dce, v_2.pos};
  return v_3;
}

