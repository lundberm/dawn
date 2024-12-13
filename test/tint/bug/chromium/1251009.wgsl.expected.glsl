#version 310 es


struct tint_push_constant_struct {
  uint tint_first_instance;
};

struct VertexInputs0 {
  uint vertex_index;
  int loc0;
};

struct VertexInputs1 {
  uint loc1;
  vec4 loc3;
};

layout(location = 0) uniform tint_push_constant_struct tint_push_constants;
layout(location = 0) in int main_loc0_Input;
layout(location = 1) in uint main_loc1_Input;
layout(location = 2) in uint main_loc2_Input;
layout(location = 3) in vec4 main_loc3_Input;
vec4 main_inner(VertexInputs0 inputs0, uint loc1, uint instance_index, VertexInputs1 inputs1) {
  uint foo = (inputs0.vertex_index + instance_index);
  return vec4(0.0f);
}
void main() {
  uint v = uint(gl_VertexID);
  VertexInputs0 v_1 = VertexInputs0(v, main_loc0_Input);
  uint v_2 = main_loc1_Input;
  uint v_3 = uint(gl_InstanceID);
  uint v_4 = (v_3 + tint_push_constants.tint_first_instance);
  gl_Position = main_inner(v_1, v_2, v_4, VertexInputs1(main_loc2_Input, main_loc3_Input));
  gl_Position.y = -(gl_Position.y);
  gl_Position.z = ((2.0f * gl_Position.z) - gl_Position.w);
  gl_PointSize = 1.0f;
}
