SKIP: FAILED


enable chromium_experimental_subgroup_matrix;
enable f16;

struct SB_RW {
  arg_0 : array<f16, 64>,
}

@group(0) @binding(0) var<storage, read_write> sb_rw : SB_RW;

fn subgroupMatrixStore_1466ba() {
  subgroupMatrixStore(&(sb_rw.arg_0), 1u, subgroup_matrix_result<f16, 8, 8>(), true, 1u);
}

@compute @workgroup_size(1)
fn compute_main() {
  subgroupMatrixStore_1466ba();
}

Failed to generate: <dawn>/test/tint/builtins/gen/literal/subgroupMatrixStore/1466ba.wgsl:51:3 error: no matching call to 'subgroupMatrixStore(array<f16, 64>, u32, subgroup_matrix_result<f16, 8, 8>, bool, u32)'

1 candidate function:
 • 'subgroupMatrixStore(ptr<workgroup' or 'storage, array<S, AC>, write' or 'read_write>  ✗ , u32  ✓ , subgroup_matrix<K, S, C, R>  ✓ , bool  ✓ , u32  ✓ )' where:
      ✓  'S' is 'f32', 'i32', 'u32' or 'f16'

  subgroupMatrixStore(&sb_rw.arg_0, 1u, subgroup_matrix_result<f16, 8, 8>(), true, 1u);
  ^^^^^^^^^^^^^^^^^^^


tint executable returned error: exit status 1
