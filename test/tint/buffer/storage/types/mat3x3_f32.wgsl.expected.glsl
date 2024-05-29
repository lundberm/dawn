#version 310 es

layout(binding = 0, std430) buffer tint_symbol_block_ssbo {
  mat3 inner;
} tint_symbol;

layout(binding = 1, std430) buffer tint_symbol_block_ssbo_1 {
  mat3 inner;
} tint_symbol_1;

void assign_and_preserve_padding_tint_symbol_1_inner(mat3 value) {
  tint_symbol_1.inner[0] = value[0u];
  tint_symbol_1.inner[1] = value[1u];
  tint_symbol_1.inner[2] = value[2u];
}

void tint_symbol_2() {
  assign_and_preserve_padding_tint_symbol_1_inner(tint_symbol.inner);
}

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  tint_symbol_2();
  return;
}
