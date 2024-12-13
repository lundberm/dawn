//
// fragment_main
//
struct frexp_result_vec3_f32 {
  float3 fract;
  int3 exp;
};


void frexp_979800() {
  float3 arg_0 = (1.0f).xxx;
  float3 v = arg_0;
  float3 v_1 = (0.0f).xxx;
  float3 v_2 = frexp(v, v_1);
  float3 v_3 = (float3(sign(v)) * v_2);
  frexp_result_vec3_f32 res = {v_3, int3(v_1)};
}

void fragment_main() {
  frexp_979800();
}

//
// compute_main
//
struct frexp_result_vec3_f32 {
  float3 fract;
  int3 exp;
};


void frexp_979800() {
  float3 arg_0 = (1.0f).xxx;
  float3 v = arg_0;
  float3 v_1 = (0.0f).xxx;
  float3 v_2 = frexp(v, v_1);
  float3 v_3 = (float3(sign(v)) * v_2);
  frexp_result_vec3_f32 res = {v_3, int3(v_1)};
}

[numthreads(1, 1, 1)]
void compute_main() {
  frexp_979800();
}

//
// vertex_main
//
struct frexp_result_vec3_f32 {
  float3 fract;
  int3 exp;
};

struct VertexOutput {
  float4 pos;
};

struct vertex_main_outputs {
  float4 VertexOutput_pos : SV_Position;
};


void frexp_979800() {
  float3 arg_0 = (1.0f).xxx;
  float3 v = arg_0;
  float3 v_1 = (0.0f).xxx;
  float3 v_2 = frexp(v, v_1);
  float3 v_3 = (float3(sign(v)) * v_2);
  frexp_result_vec3_f32 res = {v_3, int3(v_1)};
}

VertexOutput vertex_main_inner() {
  VertexOutput v_4 = (VertexOutput)0;
  v_4.pos = (0.0f).xxxx;
  frexp_979800();
  VertexOutput v_5 = v_4;
  return v_5;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_6 = vertex_main_inner();
  vertex_main_outputs v_7 = {v_6.pos};
  return v_7;
}

