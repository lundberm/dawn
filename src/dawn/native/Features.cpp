// Copyright 2019 The Dawn & Tint Authors
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

#include "dawn/native/Features.h"

#include <array>
#include <utility>

#include "dawn/common/Assert.h"
#include "dawn/common/BitSetIterator.h"
#include "dawn/common/ityp_array.h"

namespace dawn::native {
namespace {

struct ManualFeatureInfo {
    const char* description;
    const char* url;
    FeatureInfo::FeatureState featureState;
};

struct FeatureEnumAndInfo {
    Feature feature;
    ManualFeatureInfo info;
};

static constexpr FeatureEnumAndInfo kFeatureInfo[] = {
    {Feature::TextureCompressionBC,
     {"Support Block Compressed (BC) texture formats",
      "https://gpuweb.github.io/gpuweb/#texture-compression-bc",
      FeatureInfo::FeatureState::Stable}},
    {Feature::TextureCompressionETC2,
     {"Support Ericsson Texture Compressed (ETC2/EAC) texture formats",
      "https://gpuweb.github.io/gpuweb/#texture-compression-etc2",
      FeatureInfo::FeatureState::Stable}},
    {Feature::TextureCompressionASTC,
     {"Support Adaptable Scalable Texture Compressed (ASTC) "
      "texture formats",
      "https://gpuweb.github.io/gpuweb/#texture-compression-astc",
      FeatureInfo::FeatureState::Stable}},
    {Feature::TimestampQuery,
     {"Support Timestamp Query", "https://gpuweb.github.io/gpuweb/#timestamp-query",
      FeatureInfo::FeatureState::Stable}},
    {Feature::ChromiumExperimentalTimestampQueryInsidePasses,
     {"Support experimental Timestamp Query inside render/compute pass",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "timestamp_query_inside_passes.md",
      FeatureInfo::FeatureState::Experimental}},
    {Feature::DepthClipControl,
     {"Disable depth clipping of primitives to the clip volume",
      "https://gpuweb.github.io/gpuweb/#depth-clip-control", FeatureInfo::FeatureState::Stable}},
    {Feature::Depth32FloatStencil8,
     {"Support depth32float-stencil8 texture format",
      "https://gpuweb.github.io/gpuweb/#depth32float-stencil8", FeatureInfo::FeatureState::Stable}},
    {Feature::IndirectFirstInstance,
     {"Support non-zero first instance values on indirect draw calls",
      "https://gpuweb.github.io/gpuweb/#indirect-first-instance",
      FeatureInfo::FeatureState::Stable}},
    {Feature::ShaderF16,
     {"Supports the \"enable f16;\" directive in WGSL",
      "https://gpuweb.github.io/gpuweb/#shader-f16", FeatureInfo::FeatureState::Stable}},
    {Feature::RG11B10UfloatRenderable,
     {"Allows the RENDER_ATTACHMENT usage on textures with format \"rg11b10ufloat\", and also "
      "allows textures of that format to be multisampled.",
      "https://gpuweb.github.io/gpuweb/#rg11b10ufloat-renderable",
      FeatureInfo::FeatureState::Stable}},
    {Feature::BGRA8UnormStorage,
     {"Allows the STORAGE usage on textures with format \"bgra8unorm\".",
      "https://gpuweb.github.io/gpuweb/#bgra8unorm-storage", FeatureInfo::FeatureState::Stable}},
    {Feature::Float32Filterable,
     {"Allows textures with formats \"r32float\" \"rg32float\" and \"rgba32float\" to be filtered.",
      "https://gpuweb.github.io/gpuweb/#float32-filterable", FeatureInfo::FeatureState::Stable}},
    {Feature::ChromiumExperimentalSubgroups,
     {"Experimental, allows using subgroup and supports the \"enable "
      "chromium_experimental_subgroups\" directive in WGSL. Only used to investigate the semantic "
      "of subgroups and should not be relied upon.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/shader_features.md",
      FeatureInfo::FeatureState::Experimental}},
    {Feature::ChromiumExperimentalSubgroupUniformControlFlow,
     {"Experimental, supports VK_KHR_shader_subgroup_uniform_control_flow on Vulkan devices. Only "
      "used to investigate the semantic of subgroups and should not be relied upon.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/shader_features.md",
      FeatureInfo::FeatureState::Experimental}},
    {Feature::DawnInternalUsages,
     {"Add internal usages to resources to affect how the texture is allocated, but not "
      "frontend validation. Other internal commands may access this usage.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "dawn_internal_usages.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::DawnMultiPlanarFormats,
     {"Import and use multi-planar texture formats with per plane views",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "multi_planar_formats.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::MultiPlanarFormatExtendedUsages,
     {"Enable creating multi-planar formatted textures directly without importing. Also allows "
      "including CopyDst as texture's usage and per plane copies between a texture and a buffer.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "multi_planar_formats.md",
      FeatureInfo::FeatureState::Experimental}},
    {Feature::MultiPlanarFormatP010,
     {"Import and use the P010 multi-planar texture format with per plane views",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "multi_planar_formats.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::MultiPlanarFormatNv12a,
     {"Import and use the NV12A multi-planar texture format with per plane views",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "multi_planar_formats.md",
      FeatureInfo::FeatureState::Experimental}},
    {Feature::MultiPlanarRenderTargets,
     {"Import and use multi-planar texture formats as render attachments",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "multi_planar_formats.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::DawnNative,
     {"WebGPU is running on top of dawn_native, granting some additional capabilities.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "dawn_native.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::ImplicitDeviceSynchronization,
     {"Public API methods (except encoding) will have implicit device synchronization. So they "
      "will be safe to be used on multiple threads.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "implicit_device_synchronization.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SurfaceCapabilities,
     {"Support querying Surface's capabilities such as supporte usage flags. This feature also "
      "enables swap chain to be created with usage other than RenderAttachment.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "surface_capabilities.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::TransientAttachments,
     {"Support transient attachments that allow render pass operations to stay in tile memory, "
      "avoiding VRAM traffic and potentially avoiding VRAM allocation for the textures.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "transient_attachments.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::MSAARenderToSingleSampled,
     {"Support multisampled rendering on single-sampled attachments efficiently.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "msaa_render_to_single_sampled.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::DualSourceBlending,
     {"Support dual source blending. Enables Src1, OneMinusSrc1, Src1Alpha, and OneMinusSrc1Alpha "
      "blend factors along with @blend_src WGSL output attribute.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "dual_source_blending.md",
      FeatureInfo::FeatureState::Experimental}},
    {Feature::D3D11MultithreadProtected,
     {"Enable ID3D11Multithread protection for interop with external users of the D3D11 device.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "d3d11_multithread_protected.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::ANGLETextureSharing,
     {"Enable ANGLE texture sharing to allow the OpenGL ES backend to share textures by external "
      "OpenGL texture ID.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "angle_texture_sharing.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::PixelLocalStorageCoherent,
     {"Supports passing information between invocation in a render pass that cover the same pixel."
      "This helps more efficiently implement algorithms that would otherwise require ping-ponging"
      "between render targets. The coherent version of this extension means that no barrier calls"
      "are needed to prevent data races between fragment shaders on the same pixel.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "pixel_local_storage.md",
      FeatureInfo::FeatureState::Experimental}},
    {Feature::PixelLocalStorageNonCoherent,
     {"Supports passing information between invocation in a render pass that cover the same pixel."
      "This helps more efficiently implement algorithms that would otherwise require ping-ponging"
      "between render targets. The non-coherent version of this extension means that barrier calls"
      "are needed to prevent data races between fragment shaders on the same pixels (note that "
      "overlapping fragments from the same draw cannot be made data race free).",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "pixel_local_storage.md",
      FeatureInfo::FeatureState::Experimental}},
    {Feature::Norm16TextureFormats,
     {"Supports R/RG/RGBA16 norm texture formats",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "norm16_texture_formats.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedTextureMemoryVkDedicatedAllocation,
     {"Support specifying whether a Vulkan allocation for shared texture memory is a dedicated "
      "memory allocation.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "shared_texture_memory.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedTextureMemoryAHardwareBuffer,
     {"Support importing AHardwareBuffer as shared texture memory.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "shared_texture_memory.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedTextureMemoryDmaBuf,
     {"Support importing DmaBuf as shared texture memory.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "shared_texture_memory.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedTextureMemoryOpaqueFD,
     {"Support importing OpaqueFD as shared texture memory.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "shared_texture_memory.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedTextureMemoryZirconHandle,
     {"Support importing ZirconHandle as shared texture memory.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "shared_texture_memory.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedTextureMemoryDXGISharedHandle,
     {"Support importing DXGISharedHandle as shared texture memory.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "shared_texture_memory.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedTextureMemoryD3D11Texture2D,
     {"Support importing D3D11Texture2D as shared texture memory.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "shared_texture_memory.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedTextureMemoryIOSurface,
     {"Support importing IOSurface as shared texture memory.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "shared_texture_memory.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedTextureMemoryEGLImage,
     {"Support importing EGLImage as shared texture memory.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "shared_texture_memory.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedFenceVkSemaphoreOpaqueFD,
     {"Support for importing and exporting VkSemaphoreOpaqueFD used for GPU synchronization.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/shared_fence.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedFenceVkSemaphoreSyncFD,
     {"Support for importing and exporting VkSemaphoreSyncFD used for GPU synchronization.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/shared_fence.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedFenceVkSemaphoreZirconHandle,
     {"Support for importing and exporting VkSemaphoreZirconHandle used for GPU synchronization.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/shared_fence.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedFenceDXGISharedHandle,
     {"Support for importing and exporting DXGISharedHandle used for GPU synchronization.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/shared_fence.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedFenceMTLSharedEvent,
     {"Support for importing and exporting MTLSharedEvent used for GPU synchronization.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/shared_fence.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::HostMappedPointer,
     {"Support creation of buffers from host-mapped pointers.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "host_mapped_pointer.md",
      FeatureInfo::FeatureState::Experimental}},
    {Feature::FramebufferFetch,
     {"Support loading the current framebuffer value in fragment shaders.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "framebuffer_fetch.md",
      FeatureInfo::FeatureState::Experimental}},
    {Feature::BufferMapExtendedUsages,
     {"Support creating all kinds of buffers with MapRead and/or MapWrite usage.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "buffer_map_extended_usages.md",
      FeatureInfo::FeatureState::Experimental}},
    {Feature::AdapterPropertiesMemoryHeaps,
     {"Support querying memory heap info from the adapter.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "adapter_properties.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::AdapterPropertiesD3D,
     {"Support querying D3D info from the adapter.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/"
      "adapter_properties.md",
      FeatureInfo::FeatureState::Stable}},
    {Feature::SharedBufferMemoryD3D12Resource,
     {"Support importing ID3D12Resource as shared buffer memory.",
      "https://dawn.googlesource.com/dawn/+/refs/heads/main/docs/dawn/features/shared_buffer.md",
      FeatureInfo::FeatureState::Experimental}},
};

}  // anonymous namespace

void FeaturesSet::EnableFeature(Feature feature) {
    DAWN_ASSERT(feature != Feature::InvalidEnum);
    featuresBitSet.set(feature);
}

void FeaturesSet::EnableFeature(wgpu::FeatureName feature) {
    EnableFeature(FromAPI(feature));
}

bool FeaturesSet::IsEnabled(Feature feature) const {
    DAWN_ASSERT(feature != Feature::InvalidEnum);
    return featuresBitSet[feature];
}

bool FeaturesSet::IsEnabled(wgpu::FeatureName feature) const {
    Feature f = FromAPI(feature);
    return f != Feature::InvalidEnum && IsEnabled(f);
}

size_t FeaturesSet::EnumerateFeatures(wgpu::FeatureName* features) const {
    for (Feature f : IterateBitSet(featuresBitSet)) {
        wgpu::FeatureName feature = ToAPI(f);
        if (features != nullptr) {
            *features = feature;
            features += 1;
        }
    }
    return featuresBitSet.count();
}

std::vector<const char*> FeaturesSet::GetEnabledFeatureNames() const {
    std::vector<const char*> enabledFeatureNames(featuresBitSet.count());

    uint32_t index = 0;
    for (Feature feature : IterateBitSet(featuresBitSet)) {
        DAWN_ASSERT(feature != Feature::InvalidEnum);
        enabledFeatureNames[index] = kFeatureNameAndInfoList[feature].name;
        ++index;
    }
    return enabledFeatureNames;
}

}  // namespace dawn::native

#include "dawn/native/Features_autogen.inl"
