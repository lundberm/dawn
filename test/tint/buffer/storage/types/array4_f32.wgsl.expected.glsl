#version 310 es

layout(binding = 0, std430)
buffer in_block_1_ssbo {
  float inner[4];
} v;
layout(binding = 1, std430)
buffer out_block_1_ssbo {
  float inner[4];
} v_1;
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v_1.inner = v.inner;
}
