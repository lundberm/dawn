enable chromium_experimental_subgroup_matrix;
enable f16;

@group(0) @binding(0) var<storage, read_write> prevent_dce : array<f16, 1024>;

var<workgroup> arg_0 : array<f16, 64>;

fn subgroupMatrixLoad_6b52bc() -> subgroup_matrix_result<f16, 8, 8> {
  var arg_1 = 1u;
  var arg_2 = true;
  var arg_3 = 1u;
  var res : subgroup_matrix_result<f16, 8, 8> = subgroupMatrixLoad<subgroup_matrix_result<f16, 8, 8>>(&(arg_0), arg_1, arg_2, arg_3);
  return res;
}

@compute @workgroup_size(1)
fn compute_main() {
  subgroupMatrixStore(&(prevent_dce), 0, subgroupMatrixLoad_6b52bc(), false, 64);
}
