//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

void cos_af7447() {
  vec2 res = vec2(1.0f);
}
void main() {
  cos_af7447();
}
//
// compute_main
//
#version 310 es

void cos_af7447() {
  vec2 res = vec2(1.0f);
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  cos_af7447();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
};

void cos_af7447() {
  vec2 res = vec2(1.0f);
}
VertexOutput vertex_main_inner() {
  VertexOutput v = VertexOutput(vec4(0.0f));
  v.pos = vec4(0.0f);
  cos_af7447();
  return v;
}
void main() {
  gl_Position = vertex_main_inner().pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  gl_PointSize = 1.0f;
}
