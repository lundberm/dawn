fn frexp_bee870() {
  const arg_0 = 1.0;
  var res = frexp(arg_0);
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  frexp_bee870();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  frexp_bee870();
}

@compute @workgroup_size(1)
fn compute_main() {
  frexp_bee870();
}
