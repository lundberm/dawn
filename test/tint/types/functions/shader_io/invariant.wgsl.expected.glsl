#version 310 es

vec4 main_inner() {
  return vec4(0.0f);
}
void main() {
  gl_Position = main_inner();
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  gl_PointSize = 1.0f;
}
