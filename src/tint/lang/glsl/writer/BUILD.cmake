# Copyright 2023 The Dawn & Tint Authors
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

################################################################################
# File generated by 'tools/src/cmd/gen' using the template:
#   tools/src/cmd/gen/build/BUILD.cmake.tmpl
#
# To regenerate run: './tools/run gen'
#
#                       Do not modify this file directly
################################################################################

include(lang/glsl/writer/common/BUILD.cmake)
include(lang/glsl/writer/helpers/BUILD.cmake)
include(lang/glsl/writer/printer/BUILD.cmake)
include(lang/glsl/writer/raise/BUILD.cmake)

if(TINT_BUILD_GLSL_WRITER)
################################################################################
# Target:    tint_lang_glsl_writer
# Kind:      lib
# Condition: TINT_BUILD_GLSL_WRITER
################################################################################
tint_add_target(tint_lang_glsl_writer lib
  lang/glsl/writer/writer.cc
  lang/glsl/writer/writer.h
)

tint_target_add_dependencies(tint_lang_glsl_writer lib
  tint_api_common
  tint_lang_core
  tint_lang_core_constant
  tint_lang_core_intrinsic
  tint_lang_core_ir
  tint_lang_core_type
  tint_lang_wgsl
  tint_lang_wgsl_ast
  tint_lang_wgsl_ast_transform
  tint_lang_wgsl_program
  tint_lang_wgsl_sem
  tint_utils
  tint_utils_containers
  tint_utils_diagnostic
  tint_utils_ice
  tint_utils_macros
  tint_utils_math
  tint_utils_memory
  tint_utils_result
  tint_utils_rtti
  tint_utils_symbol
  tint_utils_text
)

tint_target_add_external_dependencies(tint_lang_glsl_writer lib
  "src_utils"
)

if(TINT_BUILD_GLSL_WRITER)
  tint_target_add_dependencies(tint_lang_glsl_writer lib
    tint_lang_glsl_writer_common
    tint_lang_glsl_writer_printer
    tint_lang_glsl_writer_raise
  )
endif(TINT_BUILD_GLSL_WRITER)

endif(TINT_BUILD_GLSL_WRITER)
if(TINT_BUILD_GLSL_WRITER AND TINT_BUILD_GLSL_VALIDATOR)
################################################################################
# Target:    tint_lang_glsl_writer_test
# Kind:      test
# Condition: TINT_BUILD_GLSL_WRITER AND TINT_BUILD_GLSL_VALIDATOR
################################################################################
tint_add_target(tint_lang_glsl_writer_test test
  lang/glsl/writer/access_test.cc
  lang/glsl/writer/binary_test.cc
  lang/glsl/writer/builtin_test.cc
  lang/glsl/writer/call_test.cc
  lang/glsl/writer/constant_test.cc
  lang/glsl/writer/constructor_test.cc
  lang/glsl/writer/convert_test.cc
  lang/glsl/writer/function_test.cc
  lang/glsl/writer/if_test.cc
  lang/glsl/writer/loop_test.cc
  lang/glsl/writer/switch_test.cc
  lang/glsl/writer/type_test.cc
  lang/glsl/writer/unary_test.cc
  lang/glsl/writer/var_and_let_test.cc
  lang/glsl/writer/writer_test.cc
)

tint_target_add_dependencies(tint_lang_glsl_writer_test test
  tint_api_common
  tint_lang_core
  tint_lang_core_constant
  tint_lang_core_intrinsic
  tint_lang_core_ir
  tint_lang_core_type
  tint_lang_wgsl
  tint_lang_wgsl_ast
  tint_lang_wgsl_ast_transform
  tint_lang_wgsl_program
  tint_lang_wgsl_sem
  tint_utils
  tint_utils_containers
  tint_utils_diagnostic
  tint_utils_ice
  tint_utils_macros
  tint_utils_math
  tint_utils_memory
  tint_utils_result
  tint_utils_rtti
  tint_utils_symbol
  tint_utils_text
)

tint_target_add_external_dependencies(tint_lang_glsl_writer_test test
  "gtest"
  "src_utils"
)

if(TINT_BUILD_GLSL_VALIDATOR)
  tint_target_add_sources(tint_lang_glsl_writer_test test
    "lang/glsl/writer/helper_test.h"
  )
  tint_target_add_dependencies(tint_lang_glsl_writer_test test
    tint_lang_glsl_validate
  )
endif(TINT_BUILD_GLSL_VALIDATOR)

if(TINT_BUILD_GLSL_WRITER)
  tint_target_add_dependencies(tint_lang_glsl_writer_test test
    tint_lang_glsl_writer
    tint_lang_glsl_writer_common
  )
endif(TINT_BUILD_GLSL_WRITER)

endif(TINT_BUILD_GLSL_WRITER AND TINT_BUILD_GLSL_VALIDATOR)
if(TINT_BUILD_GLSL_WRITER AND TINT_BUILD_WGSL_READER)
################################################################################
# Target:    tint_lang_glsl_writer_bench
# Kind:      bench
# Condition: TINT_BUILD_GLSL_WRITER AND TINT_BUILD_WGSL_READER
################################################################################
tint_add_target(tint_lang_glsl_writer_bench bench
  lang/glsl/writer/writer_bench.cc
)

tint_target_add_dependencies(tint_lang_glsl_writer_bench bench
  tint_api_common
  tint_lang_core
  tint_lang_core_constant
  tint_lang_core_ir
  tint_lang_core_ir_transform
  tint_lang_core_type
  tint_lang_wgsl
  tint_lang_wgsl_ast
  tint_lang_wgsl_ast_transform
  tint_lang_wgsl_common
  tint_lang_wgsl_features
  tint_lang_wgsl_inspector
  tint_lang_wgsl_program
  tint_lang_wgsl_sem
  tint_utils
  tint_utils_containers
  tint_utils_diagnostic
  tint_utils_ice
  tint_utils_macros
  tint_utils_math
  tint_utils_memory
  tint_utils_result
  tint_utils_rtti
  tint_utils_symbol
  tint_utils_text
)

tint_target_add_external_dependencies(tint_lang_glsl_writer_bench bench
  "google-benchmark"
  "src_utils"
)

if(TINT_BUILD_GLSL_WRITER)
  tint_target_add_dependencies(tint_lang_glsl_writer_bench bench
    tint_lang_glsl_writer
    tint_lang_glsl_writer_common
    tint_lang_glsl_writer_helpers
  )
endif(TINT_BUILD_GLSL_WRITER)

if(TINT_BUILD_WGSL_READER)
  tint_target_add_dependencies(tint_lang_glsl_writer_bench bench
    tint_cmd_bench_bench
    tint_lang_wgsl_reader
  )
endif(TINT_BUILD_WGSL_READER)

endif(TINT_BUILD_GLSL_WRITER AND TINT_BUILD_WGSL_READER)
if(TINT_BUILD_GLSL_WRITER)
################################################################################
# Target:    tint_lang_glsl_writer_fuzz
# Kind:      fuzz
# Condition: TINT_BUILD_GLSL_WRITER
################################################################################
tint_add_target(tint_lang_glsl_writer_fuzz fuzz
  lang/glsl/writer/writer_fuzz.cc
)

tint_target_add_dependencies(tint_lang_glsl_writer_fuzz fuzz
  tint_api_common
  tint_cmd_fuzz_ir_fuzz
  tint_lang_core
  tint_lang_core_constant
  tint_lang_core_intrinsic
  tint_lang_core_ir
  tint_lang_core_ir_transform
  tint_lang_core_type
  tint_lang_wgsl
  tint_lang_wgsl_ast
  tint_lang_wgsl_ast_transform
  tint_lang_wgsl_program
  tint_lang_wgsl_sem
  tint_utils
  tint_utils_bytes
  tint_utils_containers
  tint_utils_diagnostic
  tint_utils_ice
  tint_utils_macros
  tint_utils_math
  tint_utils_memory
  tint_utils_result
  tint_utils_rtti
  tint_utils_symbol
  tint_utils_text
)

tint_target_add_external_dependencies(tint_lang_glsl_writer_fuzz fuzz
  "src_utils"
)

if(TINT_BUILD_GLSL_WRITER)
  tint_target_add_dependencies(tint_lang_glsl_writer_fuzz fuzz
    tint_lang_glsl_writer
    tint_lang_glsl_writer_common
    tint_lang_glsl_writer_helpers
  )
endif(TINT_BUILD_GLSL_WRITER)

endif(TINT_BUILD_GLSL_WRITER)