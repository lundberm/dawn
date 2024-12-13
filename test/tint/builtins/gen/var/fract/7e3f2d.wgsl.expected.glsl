//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

void fract_7e3f2d() {
  vec4 res = vec4(0.25f);
}
void main() {
  fract_7e3f2d();
}
//
// compute_main
//
#version 310 es

void fract_7e3f2d() {
  vec4 res = vec4(0.25f);
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  fract_7e3f2d();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
};

void fract_7e3f2d() {
  vec4 res = vec4(0.25f);
}
VertexOutput vertex_main_inner() {
  VertexOutput v = VertexOutput(vec4(0.0f));
  v.pos = vec4(0.0f);
  fract_7e3f2d();
  return v;
}
void main() {
  gl_Position = vertex_main_inner().pos;
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  gl_PointSize = 1.0f;
}
