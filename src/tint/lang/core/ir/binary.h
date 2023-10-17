// Copyright 2022 The Tint Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SRC_TINT_LANG_CORE_IR_BINARY_H_
#define SRC_TINT_LANG_CORE_IR_BINARY_H_

#include <string>

#include "src/tint/lang/core/ir/operand_instruction.h"
#include "src/tint/utils/rtti/castable.h"

namespace tint::core::ir {

/// A binary operator.
enum class BinaryOp {
    kAdd,
    kSubtract,
    kMultiply,
    kDivide,
    kModulo,

    kAnd,
    kOr,
    kXor,

    kEqual,
    kNotEqual,
    kLessThan,
    kGreaterThan,
    kLessThanEqual,
    kGreaterThanEqual,

    kShiftLeft,
    kShiftRight
};

/// A binary instruction in the IR.
class Binary final : public Castable<Binary, OperandInstruction<2, 1>> {
  public:
    /// The offset in Operands() for the LHS
    static constexpr size_t kLhsOperandOffset = 0;

    /// The offset in Operands() for the RHS
    static constexpr size_t kRhsOperandOffset = 1;

    /// Constructor
    /// @param result the result value
    /// @param op the binary operator
    /// @param lhs the lhs of the instruction
    /// @param rhs the rhs of the instruction
    Binary(InstructionResult* result, BinaryOp op, Value* lhs, Value* rhs);
    ~Binary() override;

    /// @copydoc Instruction::Clone()
    Binary* Clone(CloneContext& ctx) override;

    /// @returns the binary operator
    BinaryOp Op() { return op_; }

    /// @returns the left-hand-side value for the instruction
    Value* LHS() { return operands_[kLhsOperandOffset]; }

    /// @returns the right-hand-side value for the instruction
    Value* RHS() { return operands_[kRhsOperandOffset]; }

    /// @returns the friendly name for the instruction
    std::string FriendlyName() override { return "binary"; }

  private:
    BinaryOp op_;
};

/// @param kind the enum value
/// @returns the string for the given enum value
std::string_view ToString(BinaryOp kind);

/// Emits the name of the intrinsic type.
template <typename STREAM, typename = traits::EnableIfIsOStream<STREAM>>
auto& operator<<(STREAM& out, BinaryOp kind) {
    return out << ToString(kind);
}

}  // namespace tint::core::ir

#endif  // SRC_TINT_LANG_CORE_IR_BINARY_H_
