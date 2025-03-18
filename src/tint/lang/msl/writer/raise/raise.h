// Copyright 2023 The Dawn & Tint Authors
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

#ifndef SRC_TINT_LANG_MSL_WRITER_RAISE_RAISE_H_
#define SRC_TINT_LANG_MSL_WRITER_RAISE_RAISE_H_

#include "src/tint/lang/msl/writer/common/options.h"
#include "src/tint/utils/result.h"

// Forward declarations
namespace tint::core::ir {
class Module;
}  // namespace tint::core::ir

namespace tint::msl::writer {

/// The result of running Raise().
struct RaiseResult {
    /// `true` if the transformed module needs the storage buffer sizes UBO.
    bool needs_storage_buffer_sizes = false;
};

/// Raise a core IR module to the MSL dialect of the IR.
/// @param module the core IR module to raise to MSL dialect
/// @param options the printer options
/// @returns success or failure
Result<RaiseResult> Raise(core::ir::Module& module, const Options& options);

}  // namespace tint::msl::writer

#endif  // SRC_TINT_LANG_MSL_WRITER_RAISE_RAISE_H_
