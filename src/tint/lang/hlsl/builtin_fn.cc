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

////////////////////////////////////////////////////////////////////////////////
// File generated by 'tools/src/cmd/gen' using the template:
//   src/tint/lang/hlsl/builtin_fn.cc.tmpl
//
// To regenerate run: './tools/run gen'
//
//                       Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#include "src/tint/lang/hlsl/builtin_fn.h"

namespace tint::hlsl {

const char* str(BuiltinFn i) {
    switch (i) {
        case BuiltinFn::kNone:
            return "<none>";
        case BuiltinFn::kAsint:
            return "asint";
        case BuiltinFn::kAsuint:
            return "asuint";
        case BuiltinFn::kAsfloat:
            return "asfloat";
        case BuiltinFn::kDot4AddI8Packed:
            return "dot4add_i8packed";
        case BuiltinFn::kDot4AddU8Packed:
            return "dot4add_u8packed";
        case BuiltinFn::kF32Tof16:
            return "f32tof16";
        case BuiltinFn::kF16Tof32:
            return "f16tof32";
        case BuiltinFn::kInterlockedCompareExchange:
            return "InterlockedCompareExchange";
        case BuiltinFn::kInterlockedExchange:
            return "InterlockedExchange";
        case BuiltinFn::kInterlockedAdd:
            return "InterlockedAdd";
        case BuiltinFn::kInterlockedMax:
            return "InterlockedMax";
        case BuiltinFn::kInterlockedMin:
            return "InterlockedMin";
        case BuiltinFn::kInterlockedAnd:
            return "InterlockedAnd";
        case BuiltinFn::kInterlockedOr:
            return "InterlockedOr";
        case BuiltinFn::kInterlockedXor:
            return "InterlockedXor";
        case BuiltinFn::kMul:
            return "mul";
        case BuiltinFn::kSign:
            return "sign";
        case BuiltinFn::kTextureStore:
            return "textureStore";
        case BuiltinFn::kUnpackS8S32:
            return "unpack_s8s32";
        case BuiltinFn::kUnpackU8U32:
            return "unpack_u8u32";
        case BuiltinFn::kLoad:
            return "Load";
        case BuiltinFn::kLoad2:
            return "Load2";
        case BuiltinFn::kLoad3:
            return "Load3";
        case BuiltinFn::kLoad4:
            return "Load4";
        case BuiltinFn::kLoadF16:
            return "LoadF16";
        case BuiltinFn::kLoad2F16:
            return "Load2F16";
        case BuiltinFn::kLoad3F16:
            return "Load3F16";
        case BuiltinFn::kLoad4F16:
            return "Load4F16";
        case BuiltinFn::kStore:
            return "Store";
        case BuiltinFn::kStore2:
            return "Store2";
        case BuiltinFn::kStore3:
            return "Store3";
        case BuiltinFn::kStore4:
            return "Store4";
        case BuiltinFn::kStoreF16:
            return "StoreF16";
        case BuiltinFn::kStore2F16:
            return "Store2F16";
        case BuiltinFn::kStore3F16:
            return "Store3F16";
        case BuiltinFn::kStore4F16:
            return "Store4F16";
        case BuiltinFn::kGetDimensions:
            return "GetDimensions";
    }
    return "<unknown>";
}

}  // namespace tint::hlsl
