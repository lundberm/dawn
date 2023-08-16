# Copyright 2023 The Tint Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

################################################################################
# File generated by tools/src/cmd/gen
# using the template:
#   tools/src/cmd/gen/build/BUILD.cmake.tmpl
#
# Do not modify this file directly
################################################################################

if(TINT_BUILD_SPV_READER)
tint_add_target("lang/spirv/reader/ast_parser"
  lang/spirv/reader/ast_parser/ast_parser.cc
  lang/spirv/reader/ast_parser/ast_parser.h
  lang/spirv/reader/ast_parser/attributes.h
  lang/spirv/reader/ast_parser/construct.cc
  lang/spirv/reader/ast_parser/construct.h
  lang/spirv/reader/ast_parser/entry_point_info.cc
  lang/spirv/reader/ast_parser/entry_point_info.h
  lang/spirv/reader/ast_parser/enum_converter.cc
  lang/spirv/reader/ast_parser/enum_converter.h
  lang/spirv/reader/ast_parser/fail_stream.h
  lang/spirv/reader/ast_parser/function.cc
  lang/spirv/reader/ast_parser/function.h
  lang/spirv/reader/ast_parser/namer.cc
  lang/spirv/reader/ast_parser/namer.h
  lang/spirv/reader/ast_parser/parse.cc
  lang/spirv/reader/ast_parser/parse.h
  lang/spirv/reader/ast_parser/type.cc
  lang/spirv/reader/ast_parser/type.h
  lang/spirv/reader/ast_parser/usage.cc
  lang/spirv/reader/ast_parser/usage.h
)

tint_target_add_dependencies("lang/spirv/reader/ast_parser"
  "api/common"
  "lang/core"
  "lang/core/constant"
  "lang/core/type"
  "lang/spirv/reader/common"
  "lang/wgsl/ast"
  "lang/wgsl/ast/transform"
  "lang/wgsl/program"
  "lang/wgsl/resolver"
  "lang/wgsl/sem"
  "utils/containers"
  "utils/diagnostic"
  "utils/ice"
  "utils/id"
  "utils/macros"
  "utils/math"
  "utils/memory"
  "utils/reflection"
  "utils/result"
  "utils/rtti"
  "utils/symbol"
  "utils/text"
  "utils/traits"
)

if (TINT_BUILD_SPV_READER  OR  TINT_BUILD_SPV_WRITER)
  tint_target_add_external_dependencies("lang/spirv/reader/ast_parser"
    "spirv-headers"
    "spirv-opt-internal"
  )
endif(TINT_BUILD_SPV_READER  OR  TINT_BUILD_SPV_WRITER)

endif(TINT_BUILD_SPV_READER)
if(TINT_BUILD_SPV_READER)
tint_add_target("lang/spirv/reader/ast_parser:test"
  lang/spirv/reader/ast_parser/ast_parser_test.cc
  lang/spirv/reader/ast_parser/barrier_test.cc
  lang/spirv/reader/ast_parser/constant_test.cc
  lang/spirv/reader/ast_parser/convert_member_decoration_test.cc
  lang/spirv/reader/ast_parser/convert_type_test.cc
  lang/spirv/reader/ast_parser/enum_converter_test.cc
  lang/spirv/reader/ast_parser/fail_stream_test.cc
  lang/spirv/reader/ast_parser/function_arithmetic_test.cc
  lang/spirv/reader/ast_parser/function_bit_test.cc
  lang/spirv/reader/ast_parser/function_call_test.cc
  lang/spirv/reader/ast_parser/function_cfg_test.cc
  lang/spirv/reader/ast_parser/function_composite_test.cc
  lang/spirv/reader/ast_parser/function_conversion_test.cc
  lang/spirv/reader/ast_parser/function_decl_test.cc
  lang/spirv/reader/ast_parser/function_glsl_std_450_test.cc
  lang/spirv/reader/ast_parser/function_logical_test.cc
  lang/spirv/reader/ast_parser/function_memory_test.cc
  lang/spirv/reader/ast_parser/function_misc_test.cc
  lang/spirv/reader/ast_parser/function_var_test.cc
  lang/spirv/reader/ast_parser/get_decorations_test.cc
  lang/spirv/reader/ast_parser/handle_test.cc
  lang/spirv/reader/ast_parser/helper_test.cc
  lang/spirv/reader/ast_parser/helper_test.h
  lang/spirv/reader/ast_parser/import_test.cc
  lang/spirv/reader/ast_parser/module_function_decl_test.cc
  lang/spirv/reader/ast_parser/module_var_test.cc
  lang/spirv/reader/ast_parser/named_types_test.cc
  lang/spirv/reader/ast_parser/namer_test.cc
  lang/spirv/reader/ast_parser/parser_test.cc
  lang/spirv/reader/ast_parser/spirv_tools_helpers_test.cc
  lang/spirv/reader/ast_parser/spirv_tools_helpers_test.h
  lang/spirv/reader/ast_parser/type_test.cc
  lang/spirv/reader/ast_parser/usage_test.cc
  lang/spirv/reader/ast_parser/user_name_test.cc
)

tint_target_add_dependencies("lang/spirv/reader/ast_parser:test"
  "api/common"
  "lang/core"
  "lang/core/constant"
  "lang/core/type"
  "lang/spirv/reader/common"
  "lang/wgsl/ast"
  "lang/wgsl/program"
  "lang/wgsl/sem"
  "lang/wgsl/writer/ast_printer"
  "utils/containers"
  "utils/diagnostic"
  "utils/generator"
  "utils/ice"
  "utils/id"
  "utils/macros"
  "utils/math"
  "utils/memory"
  "utils/reflection"
  "utils/result"
  "utils/rtti"
  "utils/symbol"
  "utils/text"
  "utils/traits"
)

if (TINT_BUILD_SPV_READER)
  tint_target_add_dependencies("lang/spirv/reader/ast_parser:test"
    "lang/spirv/reader/ast_parser"
  )
endif(TINT_BUILD_SPV_READER)

if (TINT_BUILD_SPV_READER  OR  TINT_BUILD_SPV_WRITER)
  tint_target_add_external_dependencies("lang/spirv/reader/ast_parser:test"
    "spirv-headers"
    "spirv-opt-internal"
    "spirv-tools"
  )
endif(TINT_BUILD_SPV_READER  OR  TINT_BUILD_SPV_WRITER)

endif(TINT_BUILD_SPV_READER)