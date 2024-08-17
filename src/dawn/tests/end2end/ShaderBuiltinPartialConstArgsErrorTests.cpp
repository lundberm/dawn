// Copyright 2024 The Dawn & Tint Authors
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

#include <string>
#include <vector>

#include "dawn/tests/DawnTest.h"
#include "dawn/utils/ComboRenderPipelineDescriptor.h"
#include "dawn/utils/WGPUHelpers.h"

namespace dawn {
namespace {

enum class Phase {
    kConst,
    kOverride,
    kRuntime,
};

std::ostream& operator<<(std::ostream& o, Phase p) {
    switch (p) {
        case Phase::kConst:
            o << "const";
            break;
        case Phase::kOverride:
            o << "override";
            break;
        case Phase::kRuntime:
            o << "runtime";
            break;
        default:
            DAWN_UNREACHABLE();
            break;
    }
    return o;
}

enum class Compare : int {
    kLess = -1,
    kEqual = 0,
    kMore = 1,
};

std::ostream& operator<<(std::ostream& o, Compare c) {
    switch (c) {
        case Compare::kLess:
            o << "less";
            break;
        case Compare::kEqual:
            o << "equal";
            break;
        case Compare::kMore:
            o << "more";
            break;
        default:
            DAWN_UNREACHABLE();
            break;
    }
    return o;
}

///////
// clamp, smoothstep
///////

template <class Params>
struct BuiltinPartialConstLowHighBase : public DawnTestWithParams<Params> {
    using DawnTestWithParams<Params>::GetParam;
    Phase mLowPhase = Phase::kConst;
    Phase mHighPhase = Phase::kConst;
    Compare mCompare = Compare::kLess;
};

using Builtin = std::string;
using LowPhase = Phase;
using HighPhase = Phase;
using Scalar = bool;
DAWN_TEST_PARAM_STRUCT(BuiltinPartialConstLowHighParams,
                       Builtin,
                       LowPhase,
                       HighPhase,
                       Compare,
                       Scalar);

class ShaderBuiltinPartialConstLowHighTest
    : public BuiltinPartialConstLowHighBase<BuiltinPartialConstLowHighParams> {
  protected:
    std::string Shader() {
        const auto builtin = GetParam().mBuiltin;
        const float high_val = 10;  // stay away from divide by zero for smoothstep
        const float low_val = high_val + static_cast<float>(GetParam().mCompare);

        std::stringstream code;
        auto module_var = [&](std::string ident, Phase p, float value) {
            if (p != Phase::kRuntime) {
                code << p << " " << ident << ": f32 = " << value << ";\n";
            }
        };
        auto function_var = [&](std::string ident, Phase p, int value) {
            if (p == Phase::kRuntime) {
                code << "  var " << ident << ": f32 = " << value << ";\n";
            }
        };
        module_var("low", GetParam().mLowPhase, low_val);
        module_var("high", GetParam().mHighPhase, high_val);
        code << "@compute @workgroup_size(1) fn main() {\n";
        function_var("low", GetParam().mLowPhase, low_val);
        function_var("high", GetParam().mHighPhase, high_val);
        code << "  var s: f32 = 0;\n";
        if (GetParam().mScalar) {
            if (builtin == "clamp") {
                code << " _ = clamp(s,low,high);\n";
            }
            if (builtin == "smoothstep") {
                code << " _ =  smoothstep(low,high,s);\n";
            }
        } else {
            if (builtin == "clamp") {
                code << " _ = clamp(vec3(s),vec3(0,low,0),vec3(1,high,1));\n";
            }
            if (builtin == "smoothstep") {
                code << " _ =  smoothstep(vec3(0,low,0),vec3(1,high,1),vec3(s));\n";
            }
        }
        code << "}";
        return code.str();
    }

    bool BadCaseForBuiltin() {
        if (GetParam().mBuiltin == "smoothstep") {
            // The more case is bad because low can't be more than high.
            // The equal case generates a divide by zero.
            return GetParam().mCompare != Compare::kLess;
        }
        if (GetParam().mBuiltin == "clamp") {
            return GetParam().mCompare == Compare::kMore;
        }
        DAWN_UNREACHABLE();
    }
};

TEST_P(ShaderBuiltinPartialConstLowHighTest, All) {
    const auto builtin = GetParam().mBuiltin;
    const auto lowPhase = GetParam().mLowPhase;
    const auto highPhase = GetParam().mHighPhase;
    const auto wgsl = Shader();

    const bool expect_create_shader_error = BadCaseForBuiltin()           //
                                            && lowPhase == Phase::kConst  //
                                            && highPhase == Phase::kConst;

    if (expect_create_shader_error) {
        ASSERT_DEVICE_ERROR(utils::CreateShaderModule(device, wgsl));
    } else {
        const bool expect_pipeline_error = BadCaseForBuiltin()             //
                                           && lowPhase != Phase::kRuntime  //
                                           && highPhase != Phase::kRuntime;
        auto shader = utils::CreateShaderModule(device, wgsl);
        wgpu::ComputePipelineDescriptor desc;
        desc.compute.module = shader;
        if (expect_pipeline_error) {
            ASSERT_DEVICE_ERROR(device.CreateComputePipeline(&desc));
        } else {
            device.CreateComputePipeline(&desc);
        }
    }
}

DAWN_INSTANTIATE_TEST_P(ShaderBuiltinPartialConstLowHighTest,
                        {D3D11Backend(), D3D12Backend(), MetalBackend(), NullBackend(),
                         OpenGLBackend(), OpenGLESBackend(), VulkanBackend()},
                        {"clamp", "smoothstep"},                             // mBuiltin
                        {Phase::kConst, Phase::kOverride, Phase::kRuntime},  // mLowPhase
                        {Phase::kConst, Phase::kOverride, Phase::kRuntime},  // mHighPhase
                        {Compare::kLess, Compare::kEqual, Compare::kMore},   // mCompare
                        {true, false});                                      // Scalar (else Vector)

///////
// insertBits
///////

template <class Params>
struct BuiltinPartialConstOffsetCountBase : public DawnTestWithParams<Params> {
    using DawnTestWithParams<Params>::GetParam;
    Phase mOffsetPhase = Phase::kConst;
    Phase mCountPhase = Phase::kConst;
};

using Builtin = std::string;
using OffsetPhase = Phase;
using CountPhase = Phase;
using OffsetTooBig = bool;
using CountTooBig = bool;
DAWN_TEST_PARAM_STRUCT(BuiltinPartialConstOffsetCountParams,
                       Builtin,
                       OffsetPhase,
                       CountPhase,
                       OffsetTooBig,
                       CountTooBig);

class ShaderBuiltinPartialConstOffsetCountTest
    : public BuiltinPartialConstOffsetCountBase<BuiltinPartialConstOffsetCountParams> {
  protected:
    std::string Shader() {
        // Assume 32-bit integers.
        const auto builtin = GetParam().mBuiltin;
        const int offset_val = 16 + (GetParam().mOffsetTooBig ? 1 : 0);
        const int count_val = 16 + (GetParam().mCountTooBig ? 1 : 0);

        std::stringstream code;
        auto module_var = [&](std::string ident, Phase p, float value) {
            if (p != Phase::kRuntime) {
                code << p << " " << ident << ": u32 = " << value << ";\n";
            }
        };
        auto function_var = [&](std::string ident, Phase p, int value) {
            if (p == Phase::kRuntime) {
                code << "  var " << ident << ": u32 = " << value << ";\n";
            }
        };
        module_var("offset", GetParam().mOffsetPhase, offset_val);
        module_var("count", GetParam().mCountPhase, count_val);
        code << "@compute @workgroup_size(1) fn main() {\n";
        function_var("offset", GetParam().mOffsetPhase, offset_val);
        function_var("count", GetParam().mCountPhase, count_val);
        code << "  var s: u32 = 0;\n";
        code << " _ = insertBits(s,s,offset,count);\n";
        code << "}";
        return code.str();
    }
};

TEST_P(ShaderBuiltinPartialConstOffsetCountTest, All) {
    const auto builtin = GetParam().mBuiltin;
    const auto offsetPhase = GetParam().mOffsetPhase;
    const auto countPhase = GetParam().mCountPhase;
    const auto offsetTooBig = GetParam().mOffsetTooBig;
    const auto countTooBig = GetParam().mCountTooBig;
    const auto wgsl = Shader();

    const bool expect_create_shader_error = (offsetTooBig || countTooBig)    //
                                            && offsetPhase == Phase::kConst  //
                                            && countPhase == Phase::kConst;

    if (expect_create_shader_error) {
        ASSERT_DEVICE_ERROR(utils::CreateShaderModule(device, wgsl));
    } else {
        const bool expect_pipeline_error = (offsetTooBig || countTooBig)      //
                                           && offsetPhase != Phase::kRuntime  //
                                           && countPhase != Phase::kRuntime;
        auto shader = utils::CreateShaderModule(device, wgsl);
        wgpu::ComputePipelineDescriptor desc;
        desc.compute.module = shader;
        if (expect_pipeline_error) {
            ASSERT_DEVICE_ERROR(device.CreateComputePipeline(&desc));
        } else {
            device.CreateComputePipeline(&desc);
        }
    }
}

DAWN_INSTANTIATE_TEST_P(ShaderBuiltinPartialConstOffsetCountTest,
                        {D3D11Backend(), D3D12Backend(), MetalBackend(), NullBackend(),
                         OpenGLBackend(), OpenGLESBackend(), VulkanBackend()},
                        {"insertBits"},                                      // mBuiltin
                        {Phase::kConst, Phase::kOverride, Phase::kRuntime},  // mOffsetPhase
                        {Phase::kConst, Phase::kOverride, Phase::kRuntime},  // mCountPhase
                        {true, false},                                       // mOffsetTooBig
                        {true, false});                                      // mCountTooBig

}  // anonymous namespace
}  // namespace dawn
