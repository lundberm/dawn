//
// fragment_main
//

void dot_eb9fbf() {
  int res = int(4);
}

void fragment_main() {
  dot_eb9fbf();
}

//
// compute_main
//

void dot_eb9fbf() {
  int res = int(4);
}

[numthreads(1, 1, 1)]
void compute_main() {
  dot_eb9fbf();
}

//
// vertex_main
//
struct VertexOutput {
  float4 pos;
};

struct vertex_main_outputs {
  float4 VertexOutput_pos : SV_Position;
};


void dot_eb9fbf() {
  int res = int(4);
}

VertexOutput vertex_main_inner() {
  VertexOutput v = (VertexOutput)0;
  v.pos = (0.0f).xxxx;
  dot_eb9fbf();
  VertexOutput v_1 = v;
  return v_1;
}

vertex_main_outputs vertex_main() {
  VertexOutput v_2 = vertex_main_inner();
  vertex_main_outputs v_3 = {v_2.pos};
  return v_3;
}

