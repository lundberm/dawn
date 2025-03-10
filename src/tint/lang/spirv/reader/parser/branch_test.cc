// Copyright 2025 The Dawn & Tint Authors
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "src/tint/lang/spirv/reader/parser/helper_test.h"

namespace tint::spirv::reader {
namespace {

TEST_F(SpirvParserTest, Branch_Forward) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
 %main_start = OpLabel
          %1 = OpCopyObject %i32 %one
               OpBranch %bb_2
       %bb_2 = OpLabel
          %2 = OpCopyObject %i32 %two
               OpBranch %bb_3
       %bb_3 = OpLabel
          %3 = OpCopyObject %i32 %three
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:i32 = let 1i
    %3:i32 = let 2i
    %4:i32 = let 3i
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, BranchConditional) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
 %10 = OpLabel
          %1 = OpCopyObject %i32 %one
               OpSelectionMerge %bb_merge None
               OpBranchConditional %true %bb_true %bb_false
    %bb_true = OpLabel
          %2 = OpCopyObject %i32 %two
               OpReturn
   %bb_false = OpLabel
          %3 = OpCopyObject %i32 %three
               OpBranch %bb_merge
   %bb_merge = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:i32 = let 1i
    if true [t: $B2, f: $B3] {  # if_1
      $B2: {  # true
        %3:i32 = let 2i
        ret
      }
      $B3: {  # false
        %4:i32 = let 3i
        exit_if  # if_1
      }
    }
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, DISABLED_BranchConditional_Empty) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpBranchConditional %cond %99 %99
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, BranchConditional_TrueToMerge) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
 %main_start = OpLabel
          %1 = OpCopyObject %i32 %one
               OpSelectionMerge %bb_merge None
               OpBranchConditional %true %bb_merge %bb_false
   %bb_false = OpLabel
          %3 = OpCopyObject %i32 %three
               OpBranch %bb_merge
   %bb_merge = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:i32 = let 1i
    if true [t: $B2, f: $B3] {  # if_1
      $B2: {  # true
        exit_if  # if_1
      }
      $B3: {  # false
        %3:i32 = let 3i
        exit_if  # if_1
      }
    }
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, BranchConditional_FalseToMerge) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
 %main_start = OpLabel
          %1 = OpCopyObject %i32 %one
               OpSelectionMerge %bb_merge None
               OpBranchConditional %true %bb_true %bb_merge
    %bb_true = OpLabel
          %2 = OpCopyObject %i32 %two
               OpReturn
   %bb_merge = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:i32 = let 1i
    if true [t: $B2, f: $B3] {  # if_1
      $B2: {  # true
        %3:i32 = let 2i
        ret
      }
      $B3: {  # false
        exit_if  # if_1
      }
    }
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, BranchConditional_TrueMatchesFalse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
 %main_start = OpLabel
          %1 = OpCopyObject %i32 %one
               OpBranchConditional %true %bb_true %bb_true
    %bb_true = OpLabel
          %2 = OpCopyObject %i32 %two
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:i32 = let 1i
    %3:bool = or true, true
    if %3 [t: $B2, f: $B3] {  # if_1
      $B2: {  # true
        %4:i32 = let 2i
        ret
      }
      $B3: {  # false
        unreachable
      }
    }
    unreachable
  }
}
)");
}

TEST_F(SpirvParserTest, BranchConditional_Nested_FalseExit) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
      %false = OpConstantFalse %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
 %main_start = OpLabel
          %1 = OpCopyObject %i32 %one
               OpSelectionMerge %bb_merge None
               OpBranchConditional %true %bb_true %bb_merge
    %bb_true = OpLabel
               OpBranchConditional %false %99 %bb_merge
         %99 = OpLabel
          %2 = OpCopyObject %i32 %two
               OpBranch %bb_merge
   %bb_merge = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:i32 = let 1i
    if true [t: $B2, f: $B3] {  # if_1
      $B2: {  # true
        if false [t: $B4, f: $B5] {  # if_2
          $B4: {  # true
            %3:i32 = let 2i
            exit_if  # if_2
          }
          $B5: {  # false
            exit_if  # if_2
          }
        }
        exit_if  # if_1
      }
      $B3: {  # false
        exit_if  # if_1
      }
    }
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, BranchConditional_Nested_TrueExit) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
      %false = OpConstantFalse %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
 %main_start = OpLabel
          %1 = OpCopyObject %i32 %one
               OpSelectionMerge %bb_merge None
               OpBranchConditional %true %bb_true %bb_merge
    %bb_true = OpLabel
               OpBranchConditional %false %bb_merge %99
         %99 = OpLabel
          %2 = OpCopyObject %i32 %two
               OpBranch %bb_merge
   %bb_merge = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:i32 = let 1i
    if true [t: $B2, f: $B3] {  # if_1
      $B2: {  # true
        if false [t: $B4, f: $B5] {  # if_2
          $B4: {  # true
            exit_if  # if_2
          }
          $B5: {  # false
            %3:i32 = let 2i
            exit_if  # if_2
          }
        }
        exit_if  # if_1
      }
      $B3: {  # false
        exit_if  # if_1
      }
    }
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, DISABLED_BranchConditional_TrueBranchesToFalse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %20 = OpLabel
               OpSelectionMerge %99 None
               OpBranchConditional %cond %20 %30
         %30 = OpLabel
               OpReturn
         %20 = OpLabel
               OpBranch %30 ; backtrack, but does dominate %30
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, BranchConditional_Hoisting) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
 %main_start = OpLabel
          %1 = OpCopyObject %i32 %one
               OpSelectionMerge %bb_merge None
               OpBranchConditional %true %bb_true %bb_false
    %bb_true = OpLabel
          %2 = OpCopyObject %i32 %two
               OpBranch %bb_merge
   %bb_false = OpLabel
               OpReturn
   %bb_merge = OpLabel
          %3 = OpIAdd %i32 %2 %2
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:i32 = let 1i
    %3:i32 = if true [t: $B2, f: $B3] {  # if_1
      $B2: {  # true
        %4:i32 = let 2i
        exit_if %4  # if_1
      }
      $B3: {  # false
        ret
      }
    }
    %5:i32 = spirv.add<i32> %3, %3
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, BranchConditional_HoistingIntoNested) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %two = OpConstant %i32 2
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
        %502 = OpLabel
               OpSelectionMerge %50 None
               OpBranchConditional %true %20 %30
         %20 = OpLabel
        %200 = OpCopyObject %i32 %two
               OpBranch %50
         %30 = OpLabel
               OpReturn
         %50 = OpLabel
               OpSelectionMerge %60 None
               OpBranchConditional %true %70 %80
         %70 = OpLabel
        %201 = OpIAdd %i32 %200 %200
               OpBranch %60
         %80 = OpLabel
               OpReturn
         %60 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:i32 = if true [t: $B2, f: $B3] {  # if_1
      $B2: {  # true
        %3:i32 = let 2i
        exit_if %3  # if_1
      }
      $B3: {  # false
        ret
      }
    }
    if true [t: $B4, f: $B5] {  # if_2
      $B4: {  # true
        %4:i32 = spirv.add<i32> %2, %2
        exit_if  # if_2
      }
      $B5: {  # false
        ret
      }
    }
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, BranchConditional_HoistingIntoParentNested) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %two = OpConstant %i32 2
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
          %1 = OpLabel
               OpSelectionMerge %90 None
               OpBranchConditional %true %10 %15
         %15 = OpLabel
               OpReturn
         %10 = OpLabel
               OpSelectionMerge %50 None
               OpBranchConditional %true %20 %30
         %20 = OpLabel
        %200 = OpCopyObject %i32 %two
               OpBranch %50
         %30 = OpLabel
               OpReturn
         %50 = OpLabel
               OpSelectionMerge %60 None
               OpBranchConditional %true %70 %80
         %70 = OpLabel
        %201 = OpIAdd %i32 %200 %200
               OpBranch %60
         %80 = OpLabel
               OpReturn
         %60 = OpLabel
               OpBranch %90
         %90 = OpLabel
        %202 = OpIAdd %i32 %200 %200
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:i32 = if true [t: $B2, f: $B3] {  # if_1
      $B2: {  # true
        %3:i32 = if true [t: $B4, f: $B5] {  # if_2
          $B4: {  # true
            %4:i32 = let 2i
            exit_if %4  # if_2
          }
          $B5: {  # false
            ret
          }
        }
        if true [t: $B6, f: $B7] {  # if_3
          $B6: {  # true
            %5:i32 = spirv.add<i32> %3, %3
            exit_if  # if_3
          }
          $B7: {  # false
            ret
          }
        }
        exit_if %3  # if_1
      }
      $B3: {  # false
        ret
      }
    }
    %6:i32 = spirv.add<i32> %2, %2
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, BranchConditional_DuplicateTrue_UsedValue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
 %main_start = OpLabel
          %1 = OpCopyObject %i32 %one
               OpBranchConditional %true %bb_true %bb_true
    %bb_true = OpLabel
          %2 = OpCopyObject %i32 %two
         %22 = OpCopyObject %i32 %2
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:i32 = let 1i
    %3:bool = or true, true
    if %3 [t: $B2, f: $B3] {  # if_1
      $B2: {  # true
        %4:i32 = let 2i
        %5:i32 = let %4
        ret
      }
      $B3: {  # false
        unreachable
      }
    }
    unreachable
  }
}
)");
}

TEST_F(SpirvParserTest, BranchConditional_DuplicateTrue_Premerge) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %12 = OpLabel
               OpSelectionMerge %13 None
               OpBranchConditional %true %14 %15
         %14 = OpLabel
               OpBranch %16
         %15 = OpLabel
               OpBranch %16
         %16 = OpLabel
          %1 = OpCopyObject %i32 %one
          %2 = OpCopyObject %i32 %1
               OpBranch %13
         %13 = OpLabel
          %3 = OpCopyObject %i32 %1
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    if true [t: $B2, f: $B3] {  # if_1
      $B2: {  # true
        exit_if  # if_1
      }
      $B3: {  # false
        exit_if  # if_1
      }
    }
    %2:i32 = if true [t: $B4, f: $B5] {  # if_2
      $B4: {  # true
        %3:i32 = let 1i
        %4:i32 = let %3
        exit_if %3  # if_2
      }
      $B5: {  # false
        unreachable
      }
    }
    %5:i32 = let %2
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, Switch) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %two %98 20 %20
         %20 = OpLabel
         %21 = OpIAdd %i32 %one %two
               OpBranch %99
         %98 = OpLabel
         %22 = OpIAdd %i32 %two %two
               OpBranch %99
         %99 = OpLabel
         %23 = OpIAdd %i32 %three %two
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    switch 2i [c: (default, $B2), c: (20i, $B3)] {  # switch_1
      $B2: {  # case
        %2:i32 = spirv.add<i32> 2i, 2i
        exit_switch  # switch_1
      }
      $B3: {  # case
        %3:i32 = spirv.add<i32> 1i, 2i
        exit_switch  # switch_1
      }
    }
    %4:i32 = spirv.add<i32> 3i, 2i
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, Switch_DefaultIsMerge) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %two = OpConstant %i32 2
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %two %99 20 %20
         %20 = OpLabel
         %21 = OpIAdd %i32 %two %two
               OpBranch %99
         %99 = OpLabel
         %98 = OpIAdd %i32 %two %two
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    switch 2i [c: (default, $B2), c: (20i, $B3)] {  # switch_1
      $B2: {  # case
        exit_switch  # switch_1
      }
      $B3: {  # case
        %2:i32 = spirv.add<i32> 2i, 2i
        exit_switch  # switch_1
      }
    }
    %3:i32 = spirv.add<i32> 2i, 2i
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, Switch_DefaultIsCase) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %two = OpConstant %i32 2
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %two %20 20 %20
         %20 = OpLabel
         %21 = OpIAdd %i32 %two %two
               OpBranch %99
         %99 = OpLabel
         %98 = OpIAdd %i32 %two %two
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    switch 2i [c: (20i default, $B2)] {  # switch_1
      $B2: {  # case
        %2:i32 = spirv.add<i32> 2i, 2i
        exit_switch  # switch_1
      }
    }
    %3:i32 = spirv.add<i32> 2i, 2i
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, Switch_MuliselectorCase) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %two = OpConstant %i32 2
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %two %50 20 %20 30 %20
         %20 = OpLabel
         %21 = OpIAdd %i32 %two %two
               OpBranch %99
         %50 = OpLabel
               OpReturn
         %99 = OpLabel
         %98 = OpIAdd %i32 %two %two
               OpReturn
               OpFunctionEnd
)",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    switch 2i [c: (default, $B2), c: (20i 30i, $B3)] {  # switch_1
      $B2: {  # case
        ret
      }
      $B3: {  # case
        %2:i32 = spirv.add<i32> 2i, 2i
        exit_switch  # switch_1
      }
    }
    %3:i32 = spirv.add<i32> 2i, 2i
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, Switch_OnlyDefault) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %two %20
         %20 = OpLabel
         %51 = OpIAdd %i32 %one %two
               OpBranch %99
         %99 = OpLabel
         %52 = OpIAdd %i32 %two %two
               OpReturn
               OpFunctionEnd
  )",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    switch 2i [c: (default, $B2)] {  # switch_1
      $B2: {  # case
        %2:i32 = spirv.add<i32> 1i, 2i
        exit_switch  # switch_1
      }
    }
    %3:i32 = spirv.add<i32> 2i, 2i
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, Switch_IfBreak) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %u32 = OpTypeInt 32 0
       %bool = OpTypeBool
        %one = OpConstant %u32 1
        %two = OpConstant %u32 2
      %three = OpConstant %u32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %two %50 20 %20 50 %50
         %99 = OpLabel
               OpReturn
         %20 = OpLabel
               OpSelectionMerge %49 None
               OpBranchConditional %true %99 %40 ; break-if
         %40 = OpLabel
               OpBranch %49
         %49 = OpLabel
        %101 = OpIAdd %u32 %two %three
               OpBranch %99
         %50 = OpLabel
               OpSelectionMerge %79 None
               OpBranchConditional %true %60 %99 ; break-unless
         %60 = OpLabel
               OpBranch %79
         %79 = OpLabel ; dominated by 60, so must follow 60
        %100 = OpIAdd %u32 %one %two
               OpBranch %99
               OpFunctionEnd
  )",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    switch 2u [c: (50u default, $B2), c: (20u, $B3)] {  # switch_1
      $B2: {  # case
        if true [t: $B4, f: $B5] {  # if_1
          $B4: {  # true
            exit_if  # if_1
          }
          $B5: {  # false
            exit_switch  # switch_1
          }
        }
        %2:u32 = spirv.add<u32> 1u, 2u
        exit_switch  # switch_1
      }
      $B3: {  # case
        if true [t: $B6, f: $B7] {  # if_2
          $B6: {  # true
            exit_switch  # switch_1
          }
          $B7: {  # false
            exit_if  # if_2
          }
        }
        %3:u32 = spirv.add<u32> 2u, 3u
        exit_switch  # switch_1
      }
    }
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, Switch_Nest_If_In_Case) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %u32 = OpTypeInt 32 0
       %bool = OpTypeBool
        %one = OpConstant %u32 1
        %two = OpConstant %u32 2
      %three = OpConstant %u32 3
       %true = OpConstantTrue %bool
      %false = OpConstantFalse %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %two %50 20 %20 50 %50
         %99 = OpLabel
               OpReturn
         %20 = OpLabel
               OpSelectionMerge %49 None
               OpBranchConditional %true %30 %40
         %49 = OpLabel
               OpBranch %99
         %30 = OpLabel
               OpBranch %49
         %40 = OpLabel
               OpBranch %49
         %50 = OpLabel
               OpSelectionMerge %79 None
               OpBranchConditional %false %60 %70
         %79 = OpLabel
               OpBranch %99
         %60 = OpLabel
               OpBranch %79
         %70 = OpLabel
               OpBranch %79
               OpFunctionEnd
  )",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    switch 2u [c: (50u default, $B2), c: (20u, $B3)] {  # switch_1
      $B2: {  # case
        if false [t: $B4, f: $B5] {  # if_1
          $B4: {  # true
            exit_if  # if_1
          }
          $B5: {  # false
            exit_if  # if_1
          }
        }
        exit_switch  # switch_1
      }
      $B3: {  # case
        if true [t: $B6, f: $B7] {  # if_2
          $B6: {  # true
            exit_if  # if_2
          }
          $B7: {  # false
            exit_if  # if_2
          }
        }
        exit_switch  # switch_1
      }
    }
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, Switch_HoistFromDefault) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %u32 = OpTypeInt 32 0
       %bool = OpTypeBool
        %one = OpConstant %u32 1
        %two = OpConstant %u32 2
      %three = OpConstant %u32 3
       %true = OpConstantTrue %bool
      %false = OpConstantFalse %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %two %20
         %20 = OpLabel
        %100 = OpCopyObject %u32 %one
               OpBranch %99
         %99 = OpLabel
        %102 = OpIAdd %u32 %100 %100
               OpReturn
               OpFunctionEnd
  )",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:u32 = switch 2u [c: (default, $B2)] {  # switch_1
      $B2: {  # case
        %3:u32 = let 1u
        exit_switch %3  # switch_1
      }
    }
    %4:u32 = spirv.add<u32> %2, %2
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, Switch_HoistFromCase) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %u32 = OpTypeInt 32 0
       %bool = OpTypeBool
        %one = OpConstant %u32 1
        %two = OpConstant %u32 2
      %three = OpConstant %u32 3
       %true = OpConstantTrue %bool
      %false = OpConstantFalse %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %two %50 20 %20
         %20 = OpLabel
        %100 = OpCopyObject %u32 %one
               OpBranch %99
         %50 = OpLabel
               OpReturn
         %99 = OpLabel
        %102 = OpIAdd %u32 %100 %100
               OpReturn
               OpFunctionEnd
  )",
              R"(
%main = @compute @workgroup_size(1u, 1u, 1u) func():void {
  $B1: {
    %2:u32 = switch 2u [c: (default, $B2), c: (20u, $B3)] {  # switch_1
      $B2: {  # case
        ret
      }
      $B3: {  # case
        %3:u32 = let 1u
        exit_switch %3  # switch_1
      }
    }
    %4:u32 = spirv.add<u32> %2, %2
    ret
  }
}
)");
}

TEST_F(SpirvParserTest, DISABLED_Switch_Fallthrough) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %two %80 20 %20 30 %30
         %20 = OpLabel
         %50 = OpIAdd %i32 %one %two
               OpBranch %30 ; fall through
         %30 = OpLabel
         %51 = OpIAdd %i32 %two %two
               OpBranch %99
         %80 = OpLabel
         %52 = OpIAdd %i32 %three %two
               OpBranch %99
         %99 = OpLabel
         %53 = OpIAdd %i32 %one %three
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Switch_IfBreakInCase) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %selector %50 20 %20 50 %50
         %99 = OpLabel
               OpReturn
         %20 = OpLabel
               OpSelectionMerge %49 None
               OpBranchConditional %cond %99 %40 ; break-if
         %40 = OpLabel
               OpBranch %49
         %49 = OpLabel
               OpBranch %99
         %50 = OpLabel
               OpSelectionMerge %79 None
               OpBranchConditional %cond %60 %99 ; break-unless
         %60 = OpLabel
               OpBranch %79
         %79 = OpLabel ; dominated by 60, so must follow 60
               OpBranch %99
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Switch_CaseCanBeMerge) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %selector %99 20 %99
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %20 None
               OpBranchConditional %true %20 %99
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Infinite_Branch) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %20 None
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Infinite_BranchConditional) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %20 None
               OpBranchConditional %cond %20 %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_BranchConditional) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %40 None
               OpBranchConditional %true %30 %99
         %30 = OpLabel
               OpBranch %40
         %40 = OpLabel
               OpBranch %20 ; back edge
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Branch) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %40 None
               OpBranch %30
         %30 = OpLabel
               OpBranchConditional %cond %40 %99
         %40 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_ContinueIsHeader) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %20 None
               OpBranch %40
         %40 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_BranchMergeOrContinue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %20 None
               OpBranchConditional %cond %99 %20
         %99 = OpLabel
               OpStore %var %999
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_HeaderHasBreakIf) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99 ; like While
         %30 = OpLabel ; trivial body
               OpBranch %50
         %50 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_HeaderHasBreakUnless) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %99 %30 ; has break-unless
         %30 = OpLabel ; trivial body
               OpBranch %50
         %50 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_BodyHasBreak) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpBranch %99 ; break
         %50 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_BodyHasBreakIf) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpBranchConditional %cond2 %99 %40 ; break-if
         %40 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_BodyHasBreakUnless) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpBranchConditional %cond2 %40 %99 ; break-unless
         %40 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_BodyIf) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpSelectionMerge %49 None
               OpBranchConditional %cond2 %40 %45 ; nested if
         %40 = OpLabel
               OpBranch %49
         %45 = OpLabel
               OpBranch %49
         %49 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_BodyIfBreak) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpSelectionMerge %49 None
               OpBranchConditional %cond2 %40 %49 ; nested if
         %40 = OpLabel
               OpBranch %99   ; break from nested if
         %49 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_BodyHasContinueIf) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpBranchConditional %cond2 %50 %40 ; continue-if
         %40 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_BodyHasContinueUnless) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpBranchConditional %cond2 %40 %50 ; continue-unless
         %40 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Body_If_Continue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpSelectionMerge %49 None
               OpBranchConditional %cond2 %40 %49 ; nested if
         %40 = OpLabel
               OpBranch %50   ; continue from nested if
         %49 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Body_Switch) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpSelectionMerge %49 None
               OpSwitch %selector %49 40 %40 45 %45 ; fully nested switch
         %40 = OpLabel
               OpBranch %49
         %45 = OpLabel
               OpBranch %49
         %49 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Body_Switch_CaseContinues) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpSelectionMerge %49 None
               OpSwitch %selector %49 40 %40 45 %45
         %40 = OpLabel
               OpBranch %50   ; continue bypasses switch merge
         %45 = OpLabel
               OpBranch %49
         %49 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Continue_Sequence) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranch %60
         %60 = OpLabel
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Continue_ContainsIf) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpSelectionMerge %89 None
               OpBranchConditional %cond2 %60 %70
         %89 = OpLabel
               OpBranch %20 ; backedge
         %60 = OpLabel
               OpBranch %89
         %70 = OpLabel
               OpBranch %89
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Continue_HasBreakIf) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranchConditional %cond2 %99 %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Continue_HasBreakUnless) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranchConditional %cond2 %20 %99
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Loop) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpLoopMerge %49 %40 None
               OpBranchConditional %cond2 %35 %49
         %35 = OpLabel
               OpBranch %37
         %37 = OpLabel
               OpBranch %40
         %40 = OpLabel ; inner loop's continue
               OpBranch %30 ; backedge
         %49 = OpLabel ; inner loop's merge
               OpBranch %50
         %50 = OpLabel ; outer loop's continue
               OpBranch %20 ; outer loop's backege
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Loop_InnerBreak) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpLoopMerge %49 %40 None
               OpBranchConditional %cond2 %35 %49
         %35 = OpLabel
               OpBranchConditional %cond3 %49 %37 ; break to inner merge
         %37 = OpLabel
               OpBranch %40
         %40 = OpLabel ; inner loop's continue
               OpBranch %30 ; backedge
         %49 = OpLabel ; inner loop's merge
               OpBranch %50
         %50 = OpLabel ; outer loop's continue
               OpBranch %20 ; outer loop's backege
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Loop_InnerContinue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpLoopMerge %49 %40 None
               OpBranchConditional %cond2 %35 %49
         %35 = OpLabel
               OpBranchConditional %cond3 %37 %49 ; continue to inner continue target
         %37 = OpLabel
               OpBranch %40
         %40 = OpLabel ; inner loop's continue
               OpBranch %30 ; backedge
         %49 = OpLabel ; inner loop's merge
               OpBranch %50
         %50 = OpLabel ; outer loop's continue
               OpBranch %20 ; outer loop's backege
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_Loop_InnerContinueBreaks) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpLoopMerge %49 %40 None
               OpBranchConditional %cond2 %35 %49
         %35 = OpLabel
               OpBranch %37
         %37 = OpLabel
               OpBranch %40
         %40 = OpLabel ; inner loop's continue
               OpBranchConditional %cond3 %30 %49 ; backedge and inner break
         %49 = OpLabel ; inner loop's merge
               OpBranch %50
         %50 = OpLabel ; outer loop's continue
               OpBranch %20 ; outer loop's backege
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_MergeBlockIsLoop) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %50 None
               OpBranchConditional %cond %20 %50
         %20 = OpLabel
               OpBranch %50
         ; %50 is the merge block for the selection starting at 10,
         ; and its own continue target.
         %50 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %50 %99
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_MergeIsAlsoMultiBlockLoopHeader) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %50 None
               OpBranchConditional %cond %20 %50
         %20 = OpLabel
               OpBranch %50
         ; %50 is the merge block for the selection starting at 10,
         ; and a loop block header but not its own continue target.
         %50 = OpLabel
               OpLoopMerge %99 %60 None
               OpBranchConditional %cond %60 %99
         %60 = OpLabel
               OpBranch %50
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

// Exercises the hard case where we a single OpBranchConditional has both
// IfBreak and Forward edges, within the true-branch clause.
TEST_F(SpirvParserTest, DISABLED_IfBreak_FromThen_ForwardWithinThen) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpSelectionMerge %99 None
               OpBranchConditional %cond %20 %50
         %20 = OpLabel
               OpStore %var %uint_2
               OpBranchConditional %cond2 %99 %30 ; kIfBreak with kForward
         %30 = OpLabel ; still in then clause
               OpStore %var %uint_3
               OpBranch %99
         %50 = OpLabel ; else clause
               OpStore %var %uint_4
               OpBranch %99
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    // var guard10 = true;
    // if (false) {
    //   var_1 = 2u;
    //   if (true) {
    //     guard10 = false;
    //   }
    //   if (guard10) {
    //     var_1 = 3u;
    //     guard10 = false;
    //   }
    // } else {
    //   if (guard10) {
    //     var_1 = 4u;
    //     guard10 = false;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

// Exercises the hard case where we a single OpBranchConditional has both
// IfBreak and Forward edges, within the false-branch clause.
TEST_F(SpirvParserTest, DISABLED_IfBreak_FromElse_ForwardWithinElse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpSelectionMerge %99 None
               OpBranchConditional %cond %20 %50
         %20 = OpLabel
               OpStore %var %uint_2
               OpBranch %99
         %50 = OpLabel ; else clause
               OpStore %var %uint_3
               OpBranchConditional %cond2 %99 %80 ; kIfBreak with kForward
         %80 = OpLabel ; still in then clause
               OpStore %var %uint_4
               OpBranch %99
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

// This is a combination of the previous two, but also adding a premerge.
// We have IfBreak and Forward edges from the same OpBranchConditional, and
// this occurs in the true-branch clause, the false-branch clause, and within
// the premerge clause.  Flow guards have to be sprinkled in lots of places.
TEST_F(SpirvParserTest, DISABLED_IfBreak_FromThenAndElseWithForward_Premerge) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpSelectionMerge %99 None
               OpBranchConditional %cond %20 %50
         %20 = OpLabel ; then
               OpStore %var %uint_2
               OpBranchConditional %cond2 %21 %99 ; kForward and kIfBreak
         %21 = OpLabel ; still in then clause
               OpStore %var %uint_3
               OpBranch %80 ; to premerge
         %50 = OpLabel ; else clause
               OpStore %var %uint_4
               OpBranchConditional %cond2 %99 %51 ; kIfBreak with kForward
         %51 = OpLabel ; still in else clause
               OpStore %var %uint_5
               OpBranch %80 ; to premerge
         %80 = OpLabel ; premerge
               OpStore %var %uint_6
               OpBranchConditional %cond3 %81 %99
         %81 = OpLabel ; premerge
               OpStore %var %uint_7
               OpBranch %99
         %99 = OpLabel
               OpStore %var %uint_8
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");
}

TEST_F(SpirvParserTest, DISABLED_Loop_SingleBlock_BothBackedge) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %20 None
               OpBranchConditional %cond %20 %20
         %99 = OpLabel
               OpStore %var %999
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //    auto* expect = R"(var_1 = 0u;
    // loop {
    //  var_1 = 1u;
    // }
    // var_1 = 999u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Loop_SingleBlock_UnconditionalBackege) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %20 None
               OpBranch %20
         %99 = OpLabel
               OpStore %var %999
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    // }
    // var_1 = 999u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Loop_Unconditional_Body_SingleBlockContinue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %50 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               OpBranch %50
         %50 = OpLabel
               OpStore %var %uint_3
               OpBranch %20
         %99 = OpLabel
               OpStore %var %999
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   var_1 = 2u;
    //
    //   continuing {
    //     var_1 = 3u;
    //   }
    // }
    // var_1 = 999u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Loop_Unconditional_Body_MultiBlockContinue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %50 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               OpBranch %50
         %50 = OpLabel
               OpStore %var %uint_3
               OpBranch %60
         %60 = OpLabel
               OpStore %var %uint_4
               OpBranch %20
         %99 = OpLabel
               OpStore %var %999
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   var_1 = 2u;
    //
    //   continuing {
    //     var_1 = 3u;
    //     var_1 = 4u;
    //   }
    // }
    // var_1 = 999u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Loop_Unconditional_Body_ContinueNestIf) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %50 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               OpBranch %50
         %50 = OpLabel ; continue target; also if-header
               OpStore %var %uint_3
               OpSelectionMerge %80 None
               OpBranchConditional %cond2 %60 %80
         %60 = OpLabel
               OpStore %var %uint_4
               OpBranch %80
         %80 = OpLabel
               OpStore %var %uint_5
               OpBranch %20
         %99 = OpLabel
               OpStore %var %999
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   var_1 = 2u;
    //
    //   continuing {
    //     var_1 = 3u;
    //     if (true) {
    //       var_1 = 4u;
    //     }
    //     var_1 = 5u;
    //   }
    // }
    // var_1 = 999u;
    // return;
}

// Test case where both branches exit. e.g both go to merge.
TEST_F(SpirvParserTest, DISABLED__Loop_Never) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranchConditional %cond %99 %99
         %80 = OpLabel ; continue target
               OpStore %var %uint_2
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_3
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   var_1 = 1u;
    //   break;
    //
    //   continuing {
    //     var_1 = 2u;
    //   }
    // }
    // var_1 = 3u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Loop_TrueToBody_FalseBreaks) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpStore %var %uint_2
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_3
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_4
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   var_1 = 1u;
    //   if (false) {
    //   } else {
    //     break;
    //   }
    //   var_1 = 2u;
    //
    //   continuing {
    //     var_1 = 3u;
    //   }
    // }
    // var_1 = 4u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Loop_FalseToBody_TrueBreaks) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpStore %var %uint_2
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_3
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_4
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   var_1 = 1u;
    //   if (false) {
    //   } else {
    //     break;
    //   }
    //   var_1 = 2u;
    //
    //   continuing {
    //     var_1 = 3u;
    //   }
    // }
    // var_1 = 4u;
    // return;
}

// By construction, it has to come from nested code.
TEST_F(SpirvParserTest, DISABLED__Loop_NestedIfContinue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpSelectionMerge %50 None
               OpBranchConditional %cond %40 %50
         %40 = OpLabel
               OpStore %var %uint_1
               OpBranch %80 ; continue edge
         %50 = OpLabel ; inner selection merge
               OpStore %var %uint_2
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_3
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   if (false) {
    //     var_1 = 1u;
    //     continue;
    //   }
    //   var_1 = 2u;
    //
    //   continuing {
    //     var_1 = 3u;
    //   }
    // }
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Loop_BodyAlwaysBreaks) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_1
               OpBranch %99 ; break is here
         %80 = OpLabel
               OpStore %var %uint_2
               OpBranch %20 ; backedge
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   var_1 = 1u;
    //   break;
    //
    //   continuing {
    //     var_1 = 2u;
    //   }
    // }
    // return;
}

// The else-branch has a continue but it's skipped because it's from a
// block that immediately precedes the continue construct.
TEST_F(SpirvParserTest, DISABLED__Loop_BodyConditionallyBreaks_FromTrue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_1
               OpBranchConditional %cond %99 %80
         %80 = OpLabel
               OpStore %var %uint_2
               OpBranch %20 ; backedge
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   var_1 = 1u;
    //   if (false) {
    //     break;
    //   }
    //
    //   continuing {
    //     var_1 = 2u;
    //   }
    // }
    // return;
}

// The else-branch has a continue but it's skipped because it's from a
// block that immediately precedes the continue construct.
TEST_F(SpirvParserTest, DISABLED__Loop_BodyConditionallyBreaks_FromFalse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_1
               OpBranchConditional %cond %80 %99
         %80 = OpLabel
               OpStore %var %uint_2
               OpBranch %20 ; backedge
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   var_1 = 1u;
    //   if (false) {
    //   } else {
    //     break;
    //   }
    //
    //   continuing {
    //     var_1 = 2u;
    //   }
    // }
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Loop_BodyConditionallyBreaks_FromTrue_Early) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_1
               OpBranchConditional %cond %99 %70
         %70 = OpLabel
               OpStore %var %uint_3
               OpBranch %80
         %80 = OpLabel
               OpStore %var %uint_2
               OpBranch %20 ; backedge
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   var_1 = 1u;
    //   if (false) {
    //     break;
    //   }
    //   var_1 = 3u;
    //
    //   continuing {
    //     var_1 = 2u;
    //   }
    // }
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Loop_BodyConditionallyBreaks_FromFalse_Early) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_1
               OpBranchConditional %cond %70 %99
         %70 = OpLabel
               OpStore %var %uint_3
               OpBranch %80
         %80 = OpLabel
               OpStore %var %uint_2
               OpBranch %20 ; backedge
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   var_1 = 1u;
    //   if (false) {
    //   } else {
    //     break;
    //   }
    //   var_1 = 3u;
    //
    //   continuing {
    //     var_1 = 2u;
    //   }
    // }
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Switch_Case_SintValue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpSelectionMerge %99 None
               ; SPIR-V assembler doesn't support negative literals in switch
               OpSwitch %signed_selector %99 20 %20 2000000000 %30 !4000000000 %40
         %20 = OpLabel
               OpStore %var %uint_20
               OpBranch %99
         %30 = OpLabel
               OpStore %var %uint_30
               OpBranch %99
         %40 = OpLabel
               OpStore %var %uint_40
               OpBranch %99
         %99 = OpLabel
               OpStore %var %uint_7
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 1u;
    // switch(42i) {
    //   case -294967296i: {
    //     var_1 = 40u;
    //   }
    //   case 2000000000i: {
    //     var_1 = 30u;
    //   }
    //   case 20i: {
    //     var_1 = 20u;
    //   }
    //   default: {
    //   }
    // }
    // var_1 = 7u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Switch_Case_UintValue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpSelectionMerge %99 None
               OpSwitch %selector %99 20 %20 2000000000 %30 50 %40
         %20 = OpLabel
               OpStore %var %uint_20
               OpBranch %99
         %30 = OpLabel
               OpStore %var %uint_30
               OpBranch %99
         %40 = OpLabel
               OpStore %var %uint_40
               OpBranch %99
         %99 = OpLabel
               OpStore %var %uint_7
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 1u;
    // switch(42u) {
    //   case 50u: {
    //     var_1 = 40u;
    //   }
    //   case 2000000000u: {
    //     var_1 = 30u;
    //   }
    //   case 20u: {
    //     var_1 = 20u;
    //   }
    //   default: {
    //   }
    // }
    // var_1 = 7u;
    // return;
}

// When the break is not last in its case, we must emit a 'break'
TEST_F(SpirvParserTest, DISABLED__Branch_SwitchBreak_NotLastInCase) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpSelectionMerge %99 None
               OpSwitch %selector %99 20 %20
         %20 = OpLabel
               OpStore %var %uint_20
               OpSelectionMerge %50 None
               OpBranchConditional %cond %40 %50
         %40 = OpLabel
               OpStore %var %uint_40
               OpBranch %99 ; branch to merge. Not last in case
         %50 = OpLabel ; inner merge
               OpStore %var %uint_50
               OpBranch %99
         %99 = OpLabel
               OpStore %var %uint_7
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 1u;
    // switch(42u) {
    //   case 20u: {
    //     var_1 = 20u;
    //     if (false) {
    //       var_1 = 40u;
    //       break;
    //     }
    //     var_1 = 50u;
    //   }
    //   default: {
    //   }
    // }
    // var_1 = 7u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Branch_LoopBreak_MultiBlockLoop_FromBody) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_1
               OpBranch %99 ; break is here
         %80 = OpLabel
               OpStore %var %uint_2
               OpBranch %20 ; backedge
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   var_1 = 1u;
    //   break;
    //
    //   continuing {
    //     var_1 = 2u;
    //   }
    // }
    // return;
}

TEST_F(SpirvParserTest,
       DISABLED_Branch_LoopBreak_MultiBlockLoop_FromContinueConstructEnd_Conditional_BreakIf) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %80 None
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_1
               OpBranchConditional %cond %99 %20  ; exit, and backedge
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //
    //   continuing {
    //     var_1 = 1u;
    //     break if false;
    //   }
    // }
    // return;
}

TEST_F(SpirvParserTest,
       DISABLED_Branch_LoopBreak_MultiBlockLoop_FromContinueConstructEnd_Conditional) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %80 None
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_1
               OpBranchConditional %cond %20 %99  ; backedge, and exit
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //
    //   continuing {
    //     var_1 = 1u;
    //     break if !(false);
    //   }
    // }
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Branch_LoopBreak_FromContinueConstructTail) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %50 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpBranch %50
         %50 = OpLabel
               OpBranch %60
         %60 = OpLabel
               OpBranchConditional %cond %20 %99
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   if (false) {
    //   } else {
    //     break;
    //   }
    //
    //   continuing {
    //     break if !(false);
    //   }
    // }
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Branch_LoopContinue_LastInLoopConstruct) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_1
               OpBranch %80 ; continue edge from last block before continue target
         %80 = OpLabel ; continue target
               OpStore %var %uint_2
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   var_1 = 1u;
    //
    //   continuing {
    //     var_1 = 2u;
    //   }
    // }
    // return;
}

// By construction, it has to come from nested code.
TEST_F(SpirvParserTest, DISABLED__Branch_LoopContinue_BeforeLast) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpBranch %20
         %20 = OpLabel
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpSelectionMerge %50 None
               OpBranchConditional %cond %40 %50
         %40 = OpLabel
               OpStore %var %uint_1
               OpBranch %80 ; continue edge
         %50 = OpLabel ; inner selection merge
               OpStore %var %uint_2
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_3
               OpBranch %20
         %99 = OpLabel
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   if (false) {
    //     var_1 = 1u;
    //     continue;
    //   }
    //   var_1 = 2u;
    //
    //   continuing {
    //     var_1 = 3u;
    //   }
    // }
    // return;
}

TEST_F(SpirvParserTest, DISABLED__Branch_LoopContinue_FromSwitch) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_2
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_3
               OpSelectionMerge %79 None
               OpSwitch %selector %79 40 %40
         %40 = OpLabel
               OpStore %var %uint_4
               OpBranch %80 ; continue edge
         %79 = OpLabel ; switch merge
               OpStore %var %uint_5
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_6
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_7
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 1u;
    // loop {
    //   var_1 = 2u;
    //   var_1 = 3u;
    //   switch(42u) {
    //     case 40u: {
    //       var_1 = 4u;
    //       continue;
    //     }
    //     default: {
    //     }
    //   }
    //   var_1 = 5u;
    //
    //   continuing {
    //     var_1 = 6u;
    //   }
    // }
    // var_1 = 7u;
    // return;
}

// When unconditional, the if-break must be last in the then clause.
TEST_F(SpirvParserTest, DISABLED__Branch_IfBreak_FromThen) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpBranchConditional %cond %30 %99
         %30 = OpLabel
               OpStore %var %uint_1
               OpBranch %99
         %99 = OpLabel
               OpStore %var %uint_2
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //    auto* expect = R"(if (false) {
    //  var_1 = 1u;
    //}
    // var_1 = 2u;
    // return;
}

// When unconditional, the if-break must be last in the else clause.
TEST_F(SpirvParserTest, DISABLED__Branch_IfBreak_FromElse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpBranchConditional %cond %99 %30
         %30 = OpLabel
               OpStore %var %uint_1
               OpBranch %99
         %99 = OpLabel
               OpStore %var %uint_2
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(if (false) {
    // } else {
    //   var_1 = 1u;
    // }
    // var_1 = 2u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_Back_SingleBlock_LoopBreak_OnTrue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %20 None
               OpBranchConditional %cond %99 %20
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   if (false) {
    //     break;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_Back_SingleBlock_LoopBreak_OnFalse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %20 None
               OpBranchConditional %cond %20 %99
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //    auto* expect = R"(var_1 = 0u;
    // loop {
    //  var_1 = 1u;
    //  if (false) {
    //  } else {
    //    break;
    //  }
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_Back_MultiBlock_LoopBreak_OnTrue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %80
         %80 = OpLabel
               OpBranchConditional %cond %99 %20
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //
    //   continuing {
    //     break if false;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_Back_MultiBlock_LoopBreak_OnFalse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %80
         %80 = OpLabel
               OpBranchConditional %cond %20 %99
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //
    //   continuing {
    //     break if !(false);
    //   }
    // }
    // var_1 = 5u;
    // return;
}

// When the break is not last in its case, we must emit a 'break'
TEST_F(SpirvParserTest, DISABLED__BranchConditional_SwitchBreak_SwitchBreak_NotLastInCase) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpSelectionMerge %99 None
               OpSwitch %selector %99 20 %20
         %20 = OpLabel
               OpStore %var %uint_20
               OpSelectionMerge %50 None
               OpBranchConditional %cond %40 %50
         %40 = OpLabel
               OpStore %var %uint_40
               OpBranchConditional %cond2 %99 %99 ; branch to merge. Not last in case
         %50 = OpLabel ; inner merge
               OpStore %var %uint_50
               OpBranch %99
         %99 = OpLabel
               OpStore %var %uint_7
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 1u;
    // switch(42u) {
    //   case 20u: {
    //     var_1 = 20u;
    //     if (false) {
    //       var_1 = 40u;
    //       break;
    //     }
    //     var_1 = 50u;
    //   }
    //   default: {
    //   }
    // }
    // var_1 = 7u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_SwitchBreak_Continue_OnTrue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_2
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_3
               OpSelectionMerge %79 None
               OpSwitch %selector %79 40 %40
         %40 = OpLabel
               OpStore %var %uint_40
               OpBranchConditional %cond %80 %79 ; break; continue on true
         %79 = OpLabel
               OpStore %var %uint_6
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_7
               OpBranch %20
         %99 = OpLabel ; loop merge
               OpStore %var %uint_8
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //    auto* expect = R"(var_1 = 1u;
    // loop {
    //  var_1 = 2u;
    //  var_1 = 3u;
    //  switch(42u) {
    //    case 40u: {
    //      var_1 = 40u;
    //      if (false) {
    //        continue;
    //      }
    //    }
    //    default: {
    //    }
    //  }
    //  var_1 = 6u;
    //
    //  continuing {
    //    var_1 = 7u;
    //  }
    // }
    // var_1 = 8u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_SwitchBreak_Continue_OnFalse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_2
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_3
               OpSelectionMerge %79 None
               OpSwitch %selector %79 40 %40
         %40 = OpLabel
               OpStore %var %uint_40
               OpBranchConditional %cond %79 %80 ; break; continue on false
         %79 = OpLabel
               OpStore %var %uint_6
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_7
               OpBranch %20
         %99 = OpLabel ; loop merge
               OpStore %var %uint_8
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 1u;
    // loop {
    //   var_1 = 2u;
    //   var_1 = 3u;
    //   switch(42u) {
    //     case 40u: {
    //       var_1 = 40u;
    //       if (false) {
    //       } else {
    //         continue;
    //       }
    //     }
    //     default: {
    //     }
    //   }
    //   var_1 = 6u;
    //
    //   continuing {
    //     var_1 = 7u;
    //   }
    // }
    // var_1 = 8u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_SwitchBreak_Forward_OnTrue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpSelectionMerge %99 None
               OpSwitch %selector %99 20 %20
         %20 = OpLabel
               OpStore %var %uint_20
               OpBranchConditional %cond %30 %99 ; break; forward on true
         %30 = OpLabel
               OpStore %var %uint_30
               OpBranch %99
         %99 = OpLabel ; switch merge
               OpStore %var %uint_8
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 1u;
    // switch(42u) {
    //   case 20u: {
    //     var_1 = 20u;
    //     if (false) {
    //     } else {
    //       break;
    //     }
    //     var_1 = 30u;
    //   }
    //   default: {
    //   }
    // }
    // var_1 = 8u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_SwitchBreak_Forward_OnFalse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpSelectionMerge %99 None
               OpSwitch %selector %99 20 %20
         %20 = OpLabel
               OpStore %var %uint_20
               OpBranchConditional %cond %99 %30 ; break; forward on false
         %30 = OpLabel
               OpStore %var %uint_30
               OpBranch %99
         %99 = OpLabel ; switch merge
               OpStore %var %uint_8
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 1u;
    // switch(42u) {
    //   case 20u: {
    //     var_1 = 20u;
    //     if (false) {
    //       break;
    //     }
    //     var_1 = 30u;
    //   }
    //   default: {
    //   }
    // }
    // var_1 = 8u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_LoopBreak_SingleBlock_LoopBreak) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranchConditional %cond %99 %99
         %80 = OpLabel ; continue target
               OpStore %var %uint_4
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   break;
    //
    //   continuing {
    //     var_1 = 4u;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_LoopBreak_MultiBlock_LoopBreak) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               OpBranchConditional %cond %99 %99
         %80 = OpLabel ; continue target
               OpStore %var %uint_4
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   var_1 = 2u;
    //   break;
    //
    //   continuing {
    //     var_1 = 4u;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_LoopBreak_Continue_OnTrue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %25
         ; Need this extra selection to make another block between
         ; %30 and the continue target, so we actually induce a Continue
         ; statement to exist.
         %25 = OpLabel
               OpSelectionMerge %40 None
               OpBranchConditional %cond2 %30 %40
         %30 = OpLabel
               OpStore %var %uint_2
               ; break; continue on true
               OpBranchConditional %cond %80 %99
         %40 = OpLabel
               OpStore %var %uint_3
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_4
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   if (true) {
    //     var_1 = 2u;
    //     if (false) {
    //       continue;
    //     } else {
    //       break;
    //     }
    //   }
    //   var_1 = 3u;
    //
    //   continuing {
    //     var_1 = 4u;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_LoopBreak_Continue_OnFalse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %25
         ; Need this extra selection to make another block between
         ; %30 and the continue target, so we actually induce a Continue
         ; statement to exist.
         %25 = OpLabel
               OpSelectionMerge %40 None
               OpBranchConditional %cond2 %30 %40
         %30 = OpLabel
               OpStore %var %uint_2
               ; break; continue on false
               OpBranchConditional %cond %99 %80
         %40 = OpLabel
               OpStore %var %uint_3
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_4
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   if (true) {
    //     var_1 = 2u;
    //     if (false) {
    //       break;
    //     } else {
    //       continue;
    //     }
    //   }
    //   var_1 = 3u;
    //
    //   continuing {
    //     var_1 = 4u;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_LoopBreak_Forward_OnTrue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               ; break; forward on true
               OpBranchConditional %cond %40 %99
         %40 = OpLabel
               OpStore %var %uint_3
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_4
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   var_1 = 2u;
    //   if (false) {
    //   } else {
    //     break;
    //   }
    //   var_1 = 3u;
    //
    //   continuing {
    //     var_1 = 4u;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_LoopBreak_Forward_OnFalse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               ; break; forward on false
               OpBranchConditional %cond %99 %40
         %40 = OpLabel
               OpStore %var %uint_3
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_4
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   var_1 = 2u;
    //   if (false) {
    //     break;
    //   }
    //   var_1 = 3u;
    //
    //   continuing {
    //     var_1 = 4u;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_Continue_Continue_FromHeader) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranchConditional %cond %80 %80 ; to continue
         %80 = OpLabel ; continue target
               OpStore %var %uint_4
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //
    //   continuing {
    //     var_1 = 4u;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_Continue_Continue_AfterHeader_Unconditional) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               OpBranchConditional %cond %80 %80 ; to continue
         %80 = OpLabel ; continue target
               OpStore %var %uint_4
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   var_1 = 2u;
    //
    //   continuing {
    //     var_1 = 4u;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

// Create an intervening block so we actually require a "continue" statement
TEST_F(SpirvParserTest, DISABLED__BranchConditional_Continue_Continue_AfterHeader_Conditional) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               OpSelectionMerge %50 None
               OpBranchConditional %cond2 %40 %50
         %40 = OpLabel
               OpStore %var %uint_3
               OpBranchConditional %cond3 %80 %80 ; to continue
         %50 = OpLabel ; merge for selection
               OpStore %var %uint_4
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_5
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_6
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   var_1 = 2u;
    //   if (true) {
    //     var_1 = 3u;
    //     continue;
    //   }
    //   var_1 = 4u;
    //
    //   continuing {
    //     var_1 = 5u;
    //   }
    // }
    // var_1 = 6u;
    // return;
}

// Like the previous tests, but with an empty continuing clause.
TEST_F(SpirvParserTest,
       DISABLED_BranchConditional_Continue_Continue_AfterHeader_Conditional_EmptyContinuing) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               OpSelectionMerge %50 None
               OpBranchConditional %cond2 %40 %50
         %40 = OpLabel
               OpStore %var %uint_3
               OpBranchConditional %cond3 %80 %80 ; to continue
         %50 = OpLabel ; merge for selection
               OpStore %var %uint_4
               OpBranch %80
         %80 = OpLabel ; continue target
               ; no statements here.
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_6
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   var_1 = 2u;
    //   if (true) {
    //     var_1 = 3u;
    //     continue;
    //   }
    //   var_1 = 4u;
    // }
    // var_1 = 6u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_LoopContinue_FromSwitch) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_2
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_3
               OpSelectionMerge %79 None
               OpSwitch %selector %79 40 %40
         %40 = OpLabel
               OpStore %var %uint_4
               OpBranchConditional %cond2 %80 %80; dup continue edge
         %79 = OpLabel ; switch merge
               OpStore %var %uint_5
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_6
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_7
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 1u;
    // loop {
    //   var_1 = 2u;
    //   var_1 = 3u;
    //   switch(42u) {
    //     case 40u: {
    //       var_1 = 4u;
    //       continue;
    //     }
    //     default: {
    //     }
    //   }
    //   var_1 = 5u;
    //
    //   continuing {
    //     var_1 = 6u;
    //   }
    // }
    // var_1 = 7u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_Continue_IfBreak_OnTrue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               OpSelectionMerge %50 None
               OpBranchConditional %cond2 %40 %50
         %40 = OpLabel
               OpStore %var %uint_3
               ; true to if's merge;  false to continue
               OpBranchConditional %cond3 %50 %80
         %50 = OpLabel ; merge for selection
               OpStore %var %uint_4
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_5
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_6
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //    auto* expect = R"(var_1 = 0u;
    // loop {
    //  var_1 = 1u;
    //  var_1 = 2u;
    //  if (true) {
    //    var_1 = 3u;
    //    if (false) {
    //    } else {
    //      continue;
    //    }
    //  }
    //  var_1 = 4u;
    //
    //  continuing {
    //    var_1 = 5u;
    //  }
    // }
    // var_1 = 6u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_Continue_IfBreak_OnFalse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               OpSelectionMerge %50 None
               OpBranchConditional %cond2 %40 %50
         %40 = OpLabel
               OpStore %var %uint_3
               ; false to if's merge;  true to continue
               OpBranchConditional %cond3 %80 %50
         %50 = OpLabel ; merge for selection
               OpStore %var %uint_4
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_5
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_6
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //    auto* expect = R"(var_1 = 0u;
    // loop {
    //  var_1 = 1u;
    //  var_1 = 2u;
    //  if (true) {
    //    var_1 = 3u;
    //    if (false) {
    //      continue;
    //    }
    //  }
    //  var_1 = 4u;
    //
    //  continuing {
    //    var_1 = 5u;
    //  }
    // }
    // var_1 = 6u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_Continue_Forward_OnTrue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               ; continue; forward on true
               OpBranchConditional %cond %40 %80
         %40 = OpLabel
               OpStore %var %uint_3
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_4
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   var_1 = 2u;
    //   if (false) {
    //   } else {
    //     continue;
    //   }
    //   var_1 = 3u;
    //
    //   continuing {
    //     var_1 = 4u;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_Continue_Forward_OnFalse) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_1
               OpLoopMerge %99 %80 None
               OpBranch %30
         %30 = OpLabel
               OpStore %var %uint_2
               ; continue; forward on true
               OpBranchConditional %cond %80 %40
         %40 = OpLabel
               OpStore %var %uint_3
               OpBranch %80
         %80 = OpLabel ; continue target
               OpStore %var %uint_4
               OpBranch %20
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // loop {
    //   var_1 = 1u;
    //   var_1 = 2u;
    //   if (false) {
    //     continue;
    //   }
    //   var_1 = 3u;
    //
    //   continuing {
    //     var_1 = 4u;
    //   }
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_IfBreak_IfBreak_Same) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_0
               OpSelectionMerge %99 None
               OpBranchConditional %cond %99 %99
         %20 = OpLabel ; dead
               OpStore %var %uint_1
               OpBranch %99
         %99 = OpLabel
               OpStore %var %uint_5
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 0u;
    // if (false) {
    // }
    // var_1 = 5u;
    // return;
}

TEST_F(SpirvParserTest, DISABLED__BranchConditional_Forward_Forward_Same) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_1
               OpBranchConditional %cond %99 %99; forward
         %99 = OpLabel
               OpStore %var %uint_2
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 1u;
    // var_1 = 2u;
    // return;
}

// crbug.com/tint/243
TEST_F(SpirvParserTest, DISABLED__IfSelection_TrueBranch_LoopBreak) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
          %5 = OpLabel
               OpBranch %10
         %10 = OpLabel
               OpLoopMerge %99 %90 None
               OpBranch %20
         %20 = OpLabel
               OpSelectionMerge %40 None
               OpBranchConditional %cond %99 %30 ; true branch breaking is ok
         %30 = OpLabel
               OpBranch %40
         %40 = OpLabel ; selection merge
               OpBranch %90
         %90 = OpLabel ; continue target
               OpBranch %10 ; backedge
         %99 = OpLabel ; loop merge
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   if (false) {
    //     break;
    //   }
    // }
    // return;
}

// crbug.com/tint/243
TEST_F(SpirvParserTest, DISABLED__TrueBranch_LoopContinue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
          %5 = OpLabel
               OpBranch %10
         %10 = OpLabel
               OpLoopMerge %99 %90 None
               OpBranch %20
         %20 = OpLabel
               OpSelectionMerge %40 None
               OpBranchConditional %cond %90 %30 ; true branch continue is ok
         %30 = OpLabel
               OpBranch %40
         %40 = OpLabel ; selection merge
               OpBranch %90
         %90 = OpLabel ; continue target
               OpBranch %10 ; backedge
         %99 = OpLabel ; loop merge
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   if (false) {
    //     continue;
    //   }
    // }
    // return;
}

// crbug.com/tint/243
TEST_F(SpirvParserTest, DISABLED__TrueBranch_SwitchBreak) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %uint_20 %99 20 %20
         %20 = OpLabel
               OpSelectionMerge %40 None
               OpBranchConditional %cond %99 %30 ; true branch switch break is ok
         %30 = OpLabel
               OpBranch %40
         %40 = OpLabel ; if-selection merge
               OpBranch %99
         %99 = OpLabel ; switch merge
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(switch(20u) {
    //   case 20u: {
    //     if (false) {
    //       break;
    //     }
    //   }
    //   default: {
    //   }
    // }
    // return;
}

// crbug.com/tint/243
TEST_F(SpirvParserTest, DISABLED__FalseBranch_LoopBreak) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
          %5 = OpLabel
               OpBranch %10
         %10 = OpLabel
               OpLoopMerge %99 %90 None
               OpBranch %20
         %20 = OpLabel
               OpSelectionMerge %40 None
               OpBranchConditional %cond %30 %99 ; false branch breaking is ok
         %30 = OpLabel
               OpBranch %40
         %40 = OpLabel ; selection merge
               OpBranch %90
         %90 = OpLabel ; continue target
               OpBranch %10 ; backedge
         %99 = OpLabel ; loop merge
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   if (false) {
    //   } else {
    //     break;
    //   }
    // }
    // return;
}

// crbug.com/tint/243
TEST_F(SpirvParserTest, DISABLED__FalseBranch_LoopContinue) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
          %5 = OpLabel
               OpBranch %10
         %10 = OpLabel
               OpLoopMerge %99 %90 None
               OpBranch %20
         %20 = OpLabel
               OpSelectionMerge %40 None
               OpBranchConditional %cond %30 %90 ; false branch continue is ok
         %30 = OpLabel
               OpBranch %40
         %40 = OpLabel ; selection merge
               OpBranch %90
         %90 = OpLabel ; continue target
               OpBranch %10 ; backedge
         %99 = OpLabel ; loop merge
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(loop {
    //   if (false) {
    //   } else {
    //     continue;
    //   }
    // }
    // return;
}

// crbug.com/tint/243
TEST_F(SpirvParserTest, DISABLED__FalseBranch_SwitchBreak) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpSelectionMerge %99 None
               OpSwitch %uint_20 %99 20 %20
         %20 = OpLabel
               OpSelectionMerge %40 None
               OpBranchConditional %cond %30 %99 ; false branch switch break is ok
         %30 = OpLabel
               OpBranch %40
         %40 = OpLabel ; if-selection merge
               OpBranch %99
         %99 = OpLabel ; switch merge
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(switch(20u) {
    //   case 20u: {
    //     if (false) {
    //     } else {
    //       break;
    //     }
    //   }
    //   default: {
    //   }
    // }
    // return;
}

// crbug.com/tint/524
TEST_F(SpirvParserTest, DISABLED__LoopInternallyDiverge_Simple) {
    EXPECT_IR(R"(
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %main "main"
               OpExecutionMode %main LocalSize 1 1 1
       %void = OpTypeVoid
        %i32 = OpTypeInt 32 1
       %bool = OpTypeBool
        %one = OpConstant %i32 1
        %two = OpConstant %i32 2
      %three = OpConstant %i32 3
       %true = OpConstantTrue %bool
    %ep_type = OpTypeFunction %void
       %main = OpFunction %void None %ep_type
         %10 = OpLabel
               OpStore %var %uint_10
               OpBranch %20
         %20 = OpLabel
               OpStore %var %uint_20
               OpLoopMerge %99 %90 None
               OpBranchConditional %cond %30 %40 ; divergence
           %30 = OpLabel
                 OpStore %var %uint_30
                 OpBranch %90
           %40 = OpLabel
                 OpStore %var %uint_40
                 OpBranch %90
         %90 = OpLabel ; continue target
               OpStore %var %uint_90
               OpBranch %20
         %99 = OpLabel ; loop merge
               OpStore %var %uint_99
               OpReturn
               OpFunctionEnd
)",
              R"(UNIMPLEMENTED)");

    //     auto* expect = R"(var_1 = 10u;
    // loop {
    //   var_1 = 20u;
    //   if (false) {
    //     var_1 = 30u;
    //     continue;
    //   } else {
    //     var_1 = 40u;
    //   }
    //
    //   continuing {
    //     var_1 = 90u;
    //   }
    // }
    // var_1 = 99u;
    // return;
}

}  // namespace
}  // namespace tint::spirv::reader
