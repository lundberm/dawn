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

#include "src/tint/lang/glsl/writer/raise/texture_polyfill.h"

#include <string>
#include <utility>
#include <vector>

#include "src/tint/lang/core/fluent_types.h"  // IWYU pragma: export
#include "src/tint/lang/core/ir/builder.h"
#include "src/tint/lang/core/ir/module.h"
#include "src/tint/lang/core/ir/validator.h"
#include "src/tint/lang/core/type/depth_multisampled_texture.h"
#include "src/tint/lang/core/type/depth_texture.h"
#include "src/tint/lang/core/type/multisampled_texture.h"
#include "src/tint/lang/core/type/sampled_texture.h"
#include "src/tint/lang/core/type/storage_texture.h"
#include "src/tint/lang/glsl/ir/builtin_call.h"
#include "src/tint/lang/glsl/ir/member_builtin_call.h"

namespace tint::glsl::writer::raise {
namespace {

using namespace tint::core::fluent_types;     // NOLINT
using namespace tint::core::number_suffixes;  // NOLINT

/// PIMPL state for the transform.
struct State {
    /// The IR module.
    core::ir::Module& ir;

    /// The configuration
    const TexturePolyfillConfig& cfg;

    /// The IR builder.
    core::ir::Builder b{ir};

    /// The type manager.
    core::type::Manager& ty{ir.Types()};

    // A map of the <texture,sampler> binding pair to the replacement var
    Hashmap<binding::CombinedTextureSamplerPair, core::ir::Var*, 2>
        texture_sampler_to_replacement_{};

    // The list of textures and samplers that were replaced. There may have been textures which
    // existed but were unused. We don't want to delete them, so we only delete replaced values.
    Hashset<core::ir::Var*, 4> replaced_textures_and_samplers_{};

    /// Process the module.
    void Process() {
        /// Converts all 1D texture types and accesses to 2D. This is required for GLSL ES, which
        /// does not support 1D textures. We do this for desktop GL as well for consistency. This
        /// could be relaxed in the future if desired.
        UpgradeTexture1DVars();
        UpgradeTexture1DParams();

        PopulateTextureInformation();

        Vector<core::ir::CoreBuiltinCall*, 4> call_worklist;
        for (auto* inst : ir.Instructions()) {
            if (auto* call = inst->As<core::ir::CoreBuiltinCall>()) {
                switch (call->Func()) {
                    case core::BuiltinFn::kTextureDimensions:
                    case core::BuiltinFn::kTextureGather:
                    case core::BuiltinFn::kTextureGatherCompare:
                    case core::BuiltinFn::kTextureLoad:
                    case core::BuiltinFn::kTextureNumLayers:
                    case core::BuiltinFn::kTextureSample:
                    case core::BuiltinFn::kTextureSampleBias:
                    case core::BuiltinFn::kTextureSampleCompare:
                    case core::BuiltinFn::kTextureSampleCompareLevel:
                    case core::BuiltinFn::kTextureSampleGrad:
                    case core::BuiltinFn::kTextureSampleLevel:
                    case core::BuiltinFn::kTextureStore:
                        call_worklist.Push(call);
                        break;
                    default:
                        break;
                }
                continue;
            }
        }

        // Replace the builtin calls that we found
        for (auto* call : call_worklist) {
            switch (call->Func()) {
                case core::BuiltinFn::kTextureDimensions:
                    TextureDimensions(call);
                    break;
                case core::BuiltinFn::kTextureGather:
                    TextureGather(call);
                    break;
                case core::BuiltinFn::kTextureGatherCompare:
                    TextureGatherCompare(call);
                    break;
                case core::BuiltinFn::kTextureLoad:
                    TextureLoad(call);
                    break;
                case core::BuiltinFn::kTextureNumLayers:
                    TextureNumLayers(call);
                    break;
                case core::BuiltinFn::kTextureSample:
                    TextureSample(call);
                    break;
                case core::BuiltinFn::kTextureStore:
                    TextureStore(call);
                    break;
                case core::BuiltinFn::kTextureSampleBias:
                case core::BuiltinFn::kTextureSampleCompare:
                case core::BuiltinFn::kTextureSampleCompareLevel:
                case core::BuiltinFn::kTextureSampleGrad:
                case core::BuiltinFn::kTextureSampleLevel:
                default:
                    TINT_UNREACHABLE() << "TODO(dsinclair): " << call->Func();
            }
        }

        // Remove all replaced textures and samplers as they have been replaced by new globals.
        for (auto* var : replaced_textures_and_samplers_.Vector()) {
            var->Result(0)->ForEachUseUnsorted([](core::ir::Usage use) {
                TINT_ASSERT(use.instruction->Is<core::ir::Load>());
                use.instruction->Destroy();
            });
            var->Destroy();
        }
    }

    core::ir::Var* GetReplacement(core::ir::Var* tex, core::ir::Var* sampler) {
        // Don't change storage textures
        if (tex->Result(0)->Type()->UnwrapPtr()->Is<core::type::StorageTexture>()) {
            return tex;
        }

        auto tex_bp = tex->BindingPoint();
        auto samp_bp = sampler ? sampler->BindingPoint() : cfg.placeholder_sampler_bind_point;
        TINT_ASSERT(tex_bp.has_value() && samp_bp.has_value());

        replaced_textures_and_samplers_.Add(tex);
        if (sampler) {
            replaced_textures_and_samplers_.Add(sampler);
        }

        binding::CombinedTextureSamplerPair key{tex_bp.value(), samp_bp.value()};
        auto var = texture_sampler_to_replacement_.Get(key);
        TINT_ASSERT(var);
        return *(var.value);
    }

    // Get the `var` for a texture/sampler value. This means the value must be the result of a load.
    core::ir::Var* VarForValue(core::ir::Value* val) {
        if (!val) {
            return nullptr;
        }

        auto* load = LoadForValue(val);
        TINT_ASSERT(load);
        auto* from = load->From()->As<core::ir::InstructionResult>();
        TINT_ASSERT(from);
        auto* var = from->Instruction()->As<core::ir::Var>();
        TINT_ASSERT(var);
        return var;
    }

    core::ir::Load* LoadForValue(core::ir::Value* val) {
        if (!val) {
            return nullptr;
        }

        auto* res = val->As<core::ir::InstructionResult>();
        TINT_ASSERT(res);
        auto* load = res->Instruction()->As<core::ir::Load>();
        TINT_ASSERT(load);
        return load;
    }

    struct SamplerTextureVars {
        core::ir::Var* texture;
        core::ir::Var* sampler;
    };
    SamplerTextureVars GetTextureSamplerFor(core::ir::CoreBuiltinCall* call) {
        auto args = call->Args();
        switch (call->Func()) {
            case core::BuiltinFn::kTextureDimensions:
            case core::BuiltinFn::kTextureLoad:
            case core::BuiltinFn::kTextureNumLayers:
            case core::BuiltinFn::kTextureStore:
                return {VarForValue(args[0]), nullptr};
            case core::BuiltinFn::kTextureGather: {
                if (args[0]->Type()->Is<core::type::Texture>()) {
                    return {VarForValue(args[0]), VarForValue(args[1])};
                }
                return {VarForValue(args[1]), VarForValue(args[2])};
            }
            case core::BuiltinFn::kTextureGatherCompare:
            case core::BuiltinFn::kTextureSample:
            case core::BuiltinFn::kTextureSampleBaseClampToEdge:
            case core::BuiltinFn::kTextureSampleBias:
            case core::BuiltinFn::kTextureSampleCompare:
            case core::BuiltinFn::kTextureSampleCompareLevel:
            case core::BuiltinFn::kTextureSampleGrad:
            case core::BuiltinFn::kTextureSampleLevel:
                return {VarForValue(args[0]), VarForValue(args[1])};
            default:
                TINT_UNREACHABLE() << "unhandled texture function: " << call->Func();
        }
    }

    core::ir::Var* MakeVar(binding::CombinedTextureSamplerPair& key,
                           core::ir::Var* tex,
                           core::ir::Var* sampler) {
        std::string name;
        auto it = (cfg.sampler_texture_to_name.find(key));
        if (it != cfg.sampler_texture_to_name.end()) {
            name = it->second;
        } else {
            name = ir.NameOf(tex).Name();
            if (name.empty()) {
                name = "t";
            }

            if (sampler) {
                auto sampler_name = ir.NameOf(sampler).Name();
                if (sampler_name.empty()) {
                    sampler_name = "s";
                }
                name += "_" + sampler_name;
            }
            if (name.empty()) {
                name = "v";
            }
        }

        core::ir::Var* var = nullptr;
        // We may already be inside an insert block, so make a new insert block instead of
        // appending directly to the root block.
        b.Append(ir.root_block,
                 [&] { var = b.Var(name, tex->Result(0)->Type()->As<core::type::Pointer>()); });
        return var;
    }

    // This function builds up replacement combined textures. It creates a global mapping, one for
    // all the texture,sampler pairs and one with individual textures. The individual textures will
    // attempt to populate with the first texture from a texture,sampler pair but if we didn't see
    // the texture in any pair we'll create it on the fly when getting the replacemnt later.
    void PopulateTextureInformation() {
        for (auto* inst : ir.Instructions()) {
            auto* call = inst->As<core::ir::CoreBuiltinCall>();
            if (!call || (!core::IsTexture(call->Func()) && !core::IsImageQuery(call->Func()))) {
                continue;
            }

            auto tex_sampler = GetTextureSamplerFor(call);
            auto* tex = tex_sampler.texture;
            auto* sampler = tex_sampler.sampler;

            // No change to storage textures.
            if (tex->Result(0)->Type()->UnwrapPtr()->Is<core::type::StorageTexture>()) {
                continue;
            }

            BindingPoint tex_bp = tex->BindingPoint().value();

            // No sampler, use the placeholder binding
            BindingPoint samp_bp =
                sampler ? sampler->BindingPoint().value() : cfg.placeholder_sampler_bind_point;

            binding::CombinedTextureSamplerPair key{tex_bp, samp_bp};
            texture_sampler_to_replacement_.GetOrAdd(key,
                                                     [&] { return MakeVar(key, tex, sampler); });
        }
    }

    std::optional<const core::type::Type*> UpgradeTexture1D(core::ir::Value* value) {
        bool is_1d = false;
        const core::type::Type* new_type = nullptr;
        tint::Switch(
            value->Type()->UnwrapPtr(),
            [&](const core::type::SampledTexture* s) {
                is_1d = s->Dim() == core::type::TextureDimension::k1d;
                new_type = ty.Get<core::type::SampledTexture>(core::type::TextureDimension::k2d,
                                                              s->Type());
            },
            [&](const core::type::StorageTexture* s) {
                is_1d = s->Dim() == core::type::TextureDimension::k1d;
                new_type = ty.Get<core::type::StorageTexture>(
                    core::type::TextureDimension::k2d, s->TexelFormat(), s->Access(), s->Type());
            });
        if (!is_1d) {
            return std::nullopt;
        }

        if (auto* ptr = value->Type()->As<core::type::Pointer>()) {
            new_type = ty.ptr(ptr->AddressSpace(), new_type, ptr->Access());
        }

        // For each 1d texture usage we have to make sure return values and arguments are modified
        // to fit the 2d texture.
        for (auto usage : value->UsagesUnsorted()) {
            if (auto* call = usage->instruction->As<core::ir::CoreBuiltinCall>()) {
                switch (call->Func()) {
                    case core::BuiltinFn::kTextureDimensions: {
                        // Upgrade result to a vec2 and swizzle out the `x` component.
                        auto* res = call->DetachResult();
                        call->SetResults(b.InstructionResult(ty.vec2<u32>()));

                        b.InsertAfter(call, [&] {
                            auto* s = b.Swizzle(res->Type(), call, Vector<uint32_t, 1>{0});
                            res->ReplaceAllUsesWith(s->Result(0));
                        });
                        break;
                    }
                    case core::BuiltinFn::kTextureLoad:
                    case core::BuiltinFn::kTextureStore: {
                        // Add a new coord item so it's a vec2.
                        auto arg = call->Args()[1];
                        b.InsertBefore(call, [&] {
                            call->SetArg(1,
                                         b.Construct(ty.vec2(arg->Type()), arg, b.Zero(arg->Type()))
                                             ->Result(0));
                        });
                        break;
                    }
                    case core::BuiltinFn::kTextureSample: {
                        // Add a new coord item so it's a vec2.
                        auto arg = call->Args()[2];
                        b.InsertBefore(call, [&] {
                            call->SetArg(2,
                                         b.Construct(ty.vec2(arg->Type()), arg, 0.5_f)->Result(0));
                        });
                        break;
                    }
                    default:
                        TINT_UNREACHABLE() << "unknown usage instruction for texture";
                }
            }
        }

        return {new_type};
    }

    void UpgradeTexture1DVars() {
        for (auto* inst : *ir.root_block) {
            auto* var = inst->As<core::ir::Var>();
            if (!var) {
                continue;
            }
            auto new_type = UpgradeTexture1D(var->Result(0));
            if (!new_type.has_value()) {
                continue;
            }
            var->Result(0)->SetType(new_type.value());

            // All of the usages of the textures should involve loading them as the `var`
            // declarations will be pointers and the function usages require non-pointer textures.
            for (auto usage : var->Result(0)->UsagesUnsorted()) {
                UpgradeLoadOf1DTexture(usage->instruction);
            }
        }
    }

    void UpgradeTexture1DParams() {
        for (auto func : ir.functions) {
            for (auto* param : func->Params()) {
                auto new_type = UpgradeTexture1D(param);
                if (!new_type.has_value()) {
                    continue;
                }
                param->SetType(new_type.value());
            }
        }
    }

    void UpgradeLoadOf1DTexture(core::ir::Instruction* inst) {
        auto* ld = inst->As<core::ir::Load>();
        TINT_ASSERT(ld);

        auto new_type = UpgradeTexture1D(ld->Result(0));
        if (!new_type.has_value()) {
            return;
        }
        ld->Result(0)->SetType(new_type.value());
    }

    // Must be called inside an insertion block
    core::ir::Value* GetNewTexture(core::ir::Value* tex, core::ir::Value* sampler = nullptr) {
        auto* t = VarForValue(tex);
        auto* s = VarForValue(sampler);

        auto* replacement = GetReplacement(t, s);
        TINT_ASSERT(replacement);

        // In the storage case, we'll return the original texture. Nothing else to do in that case.
        if (replacement == t) {
            return tex;
        }

        return b.Load(replacement)->Result(0);
    }

    // `textureDimensions` returns an unsigned scalar / vector in WGSL. `textureSize` and
    // `imageSize` return a signed scalar / vector in GLSL.  So, we  need to cast the result
    // to the needed WGSL type.
    void TextureDimensions(core::ir::BuiltinCall* call) {
        auto args = call->Args();

        b.InsertBefore(call, [&] {
            auto func = glsl::BuiltinFn::kTextureSize;

            Vector<core::ir::Value*, 2> new_args;
            auto* tex = GetNewTexture(args[0]);
            auto* tex_ty = tex->Type()->As<core::type::Texture>();

            new_args.Push(tex);

            if (tex_ty->Is<core::type::StorageTexture>()) {
                func = glsl::BuiltinFn::kImageSize;
            }

            if (!(tex_ty->Is<core::type::StorageTexture>() ||
                  tex_ty->Is<core::type::MultisampledTexture>() ||
                  tex_ty->Is<core::type::DepthMultisampledTexture>())) {
                // Add a LOD to any texture other then storage, and multi-sampled textures
                // which does not already have an LOD.
                if (args.Length() == 1) {
                    new_args.Push(b.Constant(0_i));
                } else {
                    // Make sure the LOD is a i32
                    new_args.Push(b.Bitcast(ty.i32(), args[1])->Result(0));
                }
            }

            auto ret_type = call->Result(0)->Type();

            // In GLSL the array dimensions return a 3rd parameter.
            if (tex_ty->Dim() == core::type::TextureDimension::k2dArray ||
                tex_ty->Dim() == core::type::TextureDimension::kCubeArray) {
                ret_type = ty.vec(ty.i32(), 3);
            } else {
                ret_type = ty.MatchWidth(ty.i32(), call->Result(0)->Type());
            }

            core::ir::Value* result =
                b.Call<glsl::ir::BuiltinCall>(ret_type, func, new_args)->Result(0);

            // `textureSize` on array samplers returns the array size in the final
            // component, WGSL requires a 2 component response, so drop the array size
            if (tex_ty->Dim() == core::type::TextureDimension::k2dArray ||
                tex_ty->Dim() == core::type::TextureDimension::kCubeArray) {
                ret_type = ty.MatchWidth(ty.i32(), call->Result(0)->Type());
                result = b.Swizzle(ret_type, result, {0, 1})->Result(0);
            }

            b.BitcastWithResult(call->DetachResult(), result)->Result(0);
        });
        call->Destroy();
    }

    // `textureNumLayers` returns an unsigned scalar in WGSL. `textureSize` and `imageSize`
    // return a signed scalar / vector in GLSL.
    //
    // For the `textureSize` and `imageSize` calls the valid WGSL values always produce a
    // `vec3` in GLSL so we extract the `z` component for the number of layers.
    void TextureNumLayers(core::ir::BuiltinCall* call) {
        b.InsertBefore(call, [&] {
            auto args = call->Args();
            auto* tex = GetNewTexture(args[0]);
            auto* tex_ty = tex->Type()->As<core::type::Texture>();

            auto func = glsl::BuiltinFn::kTextureSize;
            if (tex_ty->Is<core::type::StorageTexture>()) {
                func = glsl::BuiltinFn::kImageSize;
            }

            Vector<core::ir::Value*, 2> new_args;
            new_args.Push(tex);

            // Non-storage textures require a LOD
            if (!tex_ty->Is<core::type::StorageTexture>()) {
                new_args.Push(b.Constant(0_i));
            }

            auto* new_call = b.Call<glsl::ir::BuiltinCall>(ty.vec(ty.i32(), 3), func, new_args);

            auto* swizzle = b.Swizzle(ty.i32(), new_call, {2});
            b.BitcastWithResult(call->DetachResult(), swizzle->Result(0));
        });
        call->Destroy();
    }

    void TextureLoad(core::ir::CoreBuiltinCall* call) {
        b.InsertBefore(call, [&] {
            auto args = call->Args();
            auto* tex = GetNewTexture(args[0]);

            // No loading from a depth texture in GLSL, so we should never have gotten here.
            TINT_ASSERT(!tex->Type()->Is<core::type::DepthTexture>());

            auto* tex_type = tex->Type()->As<core::type::Texture>();

            glsl::BuiltinFn func = glsl::BuiltinFn::kNone;
            if (tex_type->Is<core::type::StorageTexture>()) {
                func = glsl::BuiltinFn::kImageLoad;
            } else {
                func = glsl::BuiltinFn::kTexelFetch;
            }

            bool is_ms = tex_type->Is<core::type::MultisampledTexture>();
            bool is_storage = tex_type->Is<core::type::StorageTexture>();

            Vector<core::ir::Value*, 3> call_args{tex};
            switch (tex_type->Dim()) {
                case core::type::TextureDimension::k2d: {
                    call_args.Push(b.Convert(ty.vec2<i32>(), args[1])->Result(0));
                    if (is_ms) {
                        call_args.Push(b.Convert(ty.i32(), args[2])->Result(0));
                    } else {
                        if (!is_storage) {
                            call_args.Push(b.Convert(ty.i32(), args[2])->Result(0));
                        }
                    }
                    break;
                }
                case core::type::TextureDimension::k2dArray: {
                    auto* coord = b.Convert(ty.vec2<i32>(), args[1]);
                    auto* ary_idx = b.Convert(ty.i32(), args[2]);
                    call_args.Push(b.Construct(ty.vec3<i32>(), coord, ary_idx)->Result(0));

                    if (!is_storage) {
                        call_args.Push(b.Convert(ty.i32(), args[3])->Result(0));
                    }
                    break;
                }
                case core::type::TextureDimension::k3d: {
                    call_args.Push(b.Convert(ty.vec3<i32>(), args[1])->Result(0));

                    if (!is_storage) {
                        call_args.Push(b.Convert(ty.i32(), args[2])->Result(0));
                    }
                    break;
                }
                default:
                    TINT_UNREACHABLE();
            }

            b.CallWithResult<glsl::ir::BuiltinCall>(call->DetachResult(), func,
                                                    std::move(call_args));
        });
        call->Destroy();
    }

    void TextureStore(core::ir::BuiltinCall* call) {
        b.InsertBefore(call, [&] {
            auto args = call->Args();
            auto* tex = GetNewTexture(args[0]);
            auto* tex_type = tex->Type()->As<core::type::StorageTexture>();
            TINT_ASSERT(tex_type);

            Vector<core::ir::Value*, 3> new_args;
            new_args.Push(tex);

            if (tex_type->Dim() == core::type::TextureDimension::k2dArray) {
                auto* coords = args[1];
                auto* array_idx = args[2];

                auto* coords_ty = coords->Type()->As<core::type::Vector>();
                TINT_ASSERT(coords_ty);

                auto* new_coords = b.Construct(ty.vec3(coords_ty->Type()), coords,
                                               b.Convert(coords_ty->Type(), array_idx));
                new_args.Push(new_coords->Result(0));

                new_args.Push(args[3]);
            } else {
                new_args.Push(args[1]);
                new_args.Push(args[2]);
            }

            b.CallWithResult<glsl::ir::BuiltinCall>(
                call->DetachResult(), glsl::BuiltinFn::kImageStore, std::move(new_args));
        });
        call->Destroy();
    }

    void TextureGather(core::ir::BuiltinCall* call) {
        auto args = call->Args();
        b.InsertBefore(call, [&] {
            core::ir::Value* coords = nullptr;
            Vector<core::ir::Value*, 4> params;

            uint32_t idx = 0;
            core::ir::Value* component = nullptr;
            if (!args[idx]->Type()->Is<core::type::Texture>()) {
                component = args[idx++];
                TINT_ASSERT(component);
            }

            uint32_t tex_arg = idx++;
            uint32_t sampler_arg = idx++;

            auto* tex = GetNewTexture(args[tex_arg], args[sampler_arg]);
            auto* tex_type = tex->Type()->As<core::type::Texture>();
            TINT_ASSERT(tex_type);

            bool is_depth = tex_type->Is<core::type::DepthTexture>();
            params.Push(tex);

            coords = args[idx++];

            switch (tex_type->Dim()) {
                case core::type::TextureDimension::k2d:
                    params.Push(coords);
                    break;
                case core::type::TextureDimension::k2dArray:
                    params.Push(b.Construct(ty.vec3<f32>(), coords, b.Convert<f32>(args[idx++]))
                                    ->Result(0));
                    break;
                case core::type::TextureDimension::kCube:
                    params.Push(coords);
                    break;
                case core::type::TextureDimension::kCubeArray:
                    params.Push(b.Construct(ty.vec4<f32>(), coords, b.Convert<f32>(args[idx++]))
                                    ->Result(0));
                    break;
                default:
                    TINT_UNREACHABLE();
            }
            // Depth gather requires a `refz` param in GLSL.
            if (is_depth) {
                params.Push(b.Constant(0.0_f));
            }

            auto fn = glsl::BuiltinFn::kTextureGather;
            if (idx < args.Length()) {
                fn = glsl::BuiltinFn::kTextureGatherOffset;
                params.Push(args[idx++]);
            }

            // Push the component onto the end of the list if needed.
            if (component != nullptr) {
                params.Push(b.Convert(ty.i32(), component)->Result(0));
            }

            b.CallWithResult<glsl::ir::BuiltinCall>(call->DetachResult(), fn, params);
        });
        call->Destroy();
    }

    void TextureGatherCompare(core::ir::BuiltinCall* call) {
        auto args = call->Args();
        b.InsertBefore(call, [&] {
            uint32_t idx = 0;
            uint32_t tex_arg = idx++;
            uint32_t sampler_arg = idx++;

            auto* tex = GetNewTexture(args[tex_arg], args[sampler_arg]);
            auto* tex_type = tex->Type()->As<core::type::Texture>();
            TINT_ASSERT(tex_type);

            Vector<core::ir::Value*, 4> params;
            params.Push(tex);

            auto* coords = args[idx++];

            switch (tex_type->Dim()) {
                case core::type::TextureDimension::k2d:
                    params.Push(coords);
                    break;
                case core::type::TextureDimension::k2dArray:
                    params.Push(b.Construct(ty.vec3<f32>(), coords, b.Convert<f32>(args[idx++]))
                                    ->Result(0));
                    break;
                case core::type::TextureDimension::kCube:
                    params.Push(coords);
                    break;
                case core::type::TextureDimension::kCubeArray:
                    params.Push(b.Construct(ty.vec4<f32>(), coords, b.Convert<f32>(args[idx++]))
                                    ->Result(0));
                    break;
                default:
                    TINT_UNREACHABLE();
            }

            params.Push(args[idx++]);

            auto fn = glsl::BuiltinFn::kTextureGather;
            if (idx < args.Length()) {
                fn = glsl::BuiltinFn::kTextureGatherOffset;
                params.Push(args[idx++]);
            }

            b.CallWithResult<glsl::ir::BuiltinCall>(call->DetachResult(), fn, params);
        });
        call->Destroy();
    }

    void TextureSample(core::ir::BuiltinCall* call) {
        auto args = call->Args();
        b.InsertBefore(call, [&] {
            Vector<core::ir::Value*, 4> params;

            uint32_t idx = 0;
            uint32_t tex_arg = idx++;
            uint32_t sampler_arg = idx++;

            auto* tex = GetNewTexture(args[tex_arg], args[sampler_arg]);
            auto* tex_type = tex->Type()->As<core::type::Texture>();
            TINT_ASSERT(tex_type);

            params.Push(tex);

            bool is_depth = tex_type->Is<core::type::DepthTexture>();
            bool is_array = false;

            auto depth_ref = 0_f;

            core::ir::Value* coords = args[idx++];
            switch (tex_type->Dim()) {
                case core::type::TextureDimension::k1d:
                case core::type::TextureDimension::k2d:
                    if (is_depth) {
                        coords = b.Construct(ty.vec3<f32>(), coords, depth_ref)->Result(0);
                    }
                    params.Push(coords);

                    break;
                case core::type::TextureDimension::k2dArray: {
                    is_array = true;

                    Vector<core::ir::Value*, 3> new_coords;
                    new_coords.Push(coords);
                    new_coords.Push(b.Convert<f32>(args[idx++])->Result(0));

                    uint32_t vec_width = 3;
                    if (is_depth) {
                        new_coords.Push(b.Value(depth_ref));
                        ++vec_width;
                    }
                    params.Push(b.Construct(ty.vec(ty.f32(), vec_width), new_coords)->Result(0));
                    break;
                }
                case core::type::TextureDimension::k3d:
                case core::type::TextureDimension::kCube:
                    if (is_depth) {
                        coords = b.Construct(ty.vec4<f32>(), coords, depth_ref)->Result(0);
                    }
                    params.Push(coords);
                    break;
                case core::type::TextureDimension::kCubeArray:
                    is_array = true;
                    params.Push(b.Construct(ty.vec4<f32>(), coords, b.Convert<f32>(args[idx++]))
                                    ->Result(0));

                    if (is_depth) {
                        params.Push(b.Value(depth_ref));
                    }
                    break;
                default:
                    TINT_UNREACHABLE();
            }

            auto fn = glsl::BuiltinFn::kTexture;
            if (idx < args.Length()) {
                if (is_depth && is_array) {
                    fn = glsl::BuiltinFn::kTextureGradOffset;

                    auto* dpdx = b.Call(coords->Type(), core::BuiltinFn::kDpdx, coords);
                    auto* dpdy = b.Call(coords->Type(), core::BuiltinFn::kDpdy, coords);

                    params.Push(dpdx->Result(0));
                    params.Push(dpdy->Result(0));
                } else {
                    fn = glsl::BuiltinFn::kTextureOffset;
                }

                params.Push(args[idx++]);
            }

            b.CallWithResult<glsl::ir::BuiltinCall>(call->DetachResult(), fn, params);
        });
        call->Destroy();
    }
};

}  // namespace

Result<SuccessType> TexturePolyfill(core::ir::Module& ir, const TexturePolyfillConfig& cfg) {
    auto result = ValidateAndDumpIfNeeded(ir, "glsl.TexturePolyfill transform");
    if (result != Success) {
        return result.Failure();
    }

    State{ir, cfg}.Process();

    return Success;
}

}  // namespace tint::glsl::writer::raise
