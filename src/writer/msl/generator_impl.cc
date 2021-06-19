// Copyright 2020 The Tint Authors.
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

#include "src/writer/msl/generator_impl.h"

#include <algorithm>
#include <iomanip>
#include <utility>
#include <vector>

#include "src/ast/alias.h"
#include "src/ast/bool_literal.h"
#include "src/ast/call_statement.h"
#include "src/ast/disable_validation_decoration.h"
#include "src/ast/fallthrough_statement.h"
#include "src/ast/float_literal.h"
#include "src/ast/module.h"
#include "src/ast/override_decoration.h"
#include "src/ast/sint_literal.h"
#include "src/ast/uint_literal.h"
#include "src/ast/variable_decl_statement.h"
#include "src/ast/void.h"
#include "src/sem/array.h"
#include "src/sem/bool_type.h"
#include "src/sem/call.h"
#include "src/sem/depth_texture_type.h"
#include "src/sem/f32_type.h"
#include "src/sem/function.h"
#include "src/sem/i32_type.h"
#include "src/sem/matrix_type.h"
#include "src/sem/member_accessor_expression.h"
#include "src/sem/multisampled_texture_type.h"
#include "src/sem/pointer_type.h"
#include "src/sem/reference_type.h"
#include "src/sem/sampled_texture_type.h"
#include "src/sem/storage_texture_type.h"
#include "src/sem/struct.h"
#include "src/sem/u32_type.h"
#include "src/sem/variable.h"
#include "src/sem/vector_type.h"
#include "src/sem/void_type.h"
#include "src/writer/float_to_string.h"

namespace tint {
namespace writer {
namespace msl {
namespace {

bool last_is_break_or_fallthrough(const ast::BlockStatement* stmts) {
  if (stmts->empty()) {
    return false;
  }

  return stmts->last()->Is<ast::BreakStatement>() ||
         stmts->last()->Is<ast::FallthroughStatement>();
}

}  // namespace

GeneratorImpl::GeneratorImpl(const Program* program)
    : TextGenerator(), program_(program) {}

GeneratorImpl::~GeneratorImpl() = default;

bool GeneratorImpl::Generate() {
  out_ << "#include <metal_stdlib>" << std::endl << std::endl;
  out_ << "using namespace metal;" << std::endl;

  for (auto* const type_decl : program_->AST().TypeDecls()) {
    if (!type_decl->Is<ast::Alias>()) {
      if (!EmitTypeDecl(TypeOf(type_decl))) {
        return false;
      }
    }
  }

  if (!program_->AST().TypeDecls().empty()) {
    out_ << std::endl;
  }

  for (auto* var : program_->AST().GlobalVariables()) {
    if (var->is_const()) {
      if (!EmitProgramConstVariable(var)) {
        return false;
      }
    } else {
      auto* sem = program_->Sem().Get(var);
      switch (sem->StorageClass()) {
        case ast::StorageClass::kPrivate:
        case ast::StorageClass::kWorkgroup:
          // These are pushed into the entry point by the sanitizer.
          TINT_ICE(diagnostics_)
              << "module-scope variables in the private/workgroup storage "
                 "class should have been handled by the MSL sanitizer";
          break;
        default:
          break;  // Handled by another code path
      }
    }
  }

  for (auto* func : program_->AST().Functions()) {
    if (!func->IsEntryPoint()) {
      if (!EmitFunction(func)) {
        return false;
      }
    } else {
      if (!EmitEntryPointFunction(func)) {
        return false;
      }
    }
  }

  return true;
}

bool GeneratorImpl::EmitTypeDecl(const sem::Type* ty) {
  make_indent();

  if (auto* str = ty->As<sem::Struct>()) {
    if (!EmitStructType(str)) {
      return false;
    }
  } else {
    diagnostics_.add_error("unknown alias type: " + ty->type_name());
    return false;
  }

  return true;
}

bool GeneratorImpl::EmitArrayAccessor(ast::ArrayAccessorExpression* expr) {
  bool paren_lhs =
      !expr->array()
           ->IsAnyOf<ast::ArrayAccessorExpression, ast::CallExpression,
                     ast::IdentifierExpression, ast::MemberAccessorExpression,
                     ast::TypeConstructorExpression>();
  if (paren_lhs) {
    out_ << "(";
  }
  if (!EmitExpression(expr->array())) {
    return false;
  }
  if (paren_lhs) {
    out_ << ")";
  }

  out_ << "[";

  if (!EmitExpression(expr->idx_expr())) {
    return false;
  }
  out_ << "]";

  return true;
}

bool GeneratorImpl::EmitBitcast(ast::BitcastExpression* expr) {
  out_ << "as_type<";
  if (!EmitType(TypeOf(expr)->UnwrapRef(), "")) {
    return false;
  }

  out_ << ">(";
  if (!EmitExpression(expr->expr())) {
    return false;
  }

  out_ << ")";
  return true;
}

bool GeneratorImpl::EmitAssign(ast::AssignmentStatement* stmt) {
  make_indent();

  if (!EmitExpression(stmt->lhs())) {
    return false;
  }

  out_ << " = ";

  if (!EmitExpression(stmt->rhs())) {
    return false;
  }

  out_ << ";" << std::endl;

  return true;
}

bool GeneratorImpl::EmitBinary(ast::BinaryExpression* expr) {
  out_ << "(";

  if (!EmitExpression(expr->lhs())) {
    return false;
  }
  out_ << " ";

  switch (expr->op()) {
    case ast::BinaryOp::kAnd:
      out_ << "&";
      break;
    case ast::BinaryOp::kOr:
      out_ << "|";
      break;
    case ast::BinaryOp::kXor:
      out_ << "^";
      break;
    case ast::BinaryOp::kLogicalAnd:
      out_ << "&&";
      break;
    case ast::BinaryOp::kLogicalOr:
      out_ << "||";
      break;
    case ast::BinaryOp::kEqual:
      out_ << "==";
      break;
    case ast::BinaryOp::kNotEqual:
      out_ << "!=";
      break;
    case ast::BinaryOp::kLessThan:
      out_ << "<";
      break;
    case ast::BinaryOp::kGreaterThan:
      out_ << ">";
      break;
    case ast::BinaryOp::kLessThanEqual:
      out_ << "<=";
      break;
    case ast::BinaryOp::kGreaterThanEqual:
      out_ << ">=";
      break;
    case ast::BinaryOp::kShiftLeft:
      out_ << "<<";
      break;
    case ast::BinaryOp::kShiftRight:
      // TODO(dsinclair): MSL is based on C++14, and >> in C++14 has
      // implementation-defined behaviour for negative LHS.  We may have to
      // generate extra code to implement WGSL-specified behaviour for negative
      // LHS.
      out_ << R"(>>)";
      break;

    case ast::BinaryOp::kAdd:
      out_ << "+";
      break;
    case ast::BinaryOp::kSubtract:
      out_ << "-";
      break;
    case ast::BinaryOp::kMultiply:
      out_ << "*";
      break;
    case ast::BinaryOp::kDivide:
      out_ << "/";
      break;
    case ast::BinaryOp::kModulo:
      out_ << "%";
      break;
    case ast::BinaryOp::kNone:
      diagnostics_.add_error("missing binary operation type");
      return false;
  }
  out_ << " ";

  if (!EmitExpression(expr->rhs())) {
    return false;
  }

  out_ << ")";
  return true;
}

bool GeneratorImpl::EmitBreak(ast::BreakStatement*) {
  make_indent();
  out_ << "break;" << std::endl;
  return true;
}

bool GeneratorImpl::EmitCall(ast::CallExpression* expr) {
  auto* ident = expr->func();
  auto* call = program_->Sem().Get(expr);
  if (auto* intrinsic = call->Target()->As<sem::Intrinsic>()) {
    return EmitIntrinsicCall(expr, intrinsic);
  }

  auto* func = program_->AST().Functions().Find(ident->symbol());
  if (func == nullptr) {
    diagnostics_.add_error("Unable to find function: " +
                           program_->Symbols().NameFor(ident->symbol()));
    return false;
  }

  out_ << program_->Symbols().NameFor(ident->symbol()) << "(";

  bool first = true;
  auto* func_sem = program_->Sem().Get(func);
  for (const auto& data : func_sem->ReferencedUniformVariables()) {
    auto* var = data.first;
    if (!first) {
      out_ << ", ";
    }
    first = false;
    out_ << program_->Symbols().NameFor(var->Declaration()->symbol());
  }

  for (const auto& data : func_sem->ReferencedStorageBufferVariables()) {
    auto* var = data.first;
    if (!first) {
      out_ << ", ";
    }
    first = false;
    out_ << program_->Symbols().NameFor(var->Declaration()->symbol());
  }

  const auto& params = expr->params();
  for (auto* param : params) {
    if (!first) {
      out_ << ", ";
    }
    first = false;

    if (!EmitExpression(param)) {
      return false;
    }
  }

  out_ << ")";

  return true;
}

bool GeneratorImpl::EmitIntrinsicCall(ast::CallExpression* expr,
                                      const sem::Intrinsic* intrinsic) {
  if (intrinsic->IsTexture()) {
    return EmitTextureCall(expr, intrinsic);
  }

  switch (intrinsic->Type()) {
    case sem::IntrinsicType::kPack2x16float:
    case sem::IntrinsicType::kUnpack2x16float: {
      make_indent();
      if (intrinsic->Type() == sem::IntrinsicType::kPack2x16float) {
        out_ << "as_type<uint>(half2(";
      } else {
        out_ << "float2(as_type<half2>(";
      }
      if (!EmitExpression(expr->params()[0])) {
        return false;
      }
      out_ << "))";
      return true;
    }
    // TODO(crbug.com/tint/661): Combine sequential barriers to a single
    // instruction.
    case sem::IntrinsicType::kStorageBarrier: {
      make_indent();
      out_ << "threadgroup_barrier(mem_flags::mem_device)";
      return true;
    }
    case sem::IntrinsicType::kWorkgroupBarrier: {
      make_indent();
      out_ << "threadgroup_barrier(mem_flags::mem_threadgroup)";
      return true;
    }
    case sem::IntrinsicType::kIgnore: {
      out_ << "(void) ";
      if (!EmitExpression(expr->params()[0])) {
        return false;
      }
      return true;
    }
    default:
      break;
  }

  auto name = generate_builtin_name(intrinsic);
  if (name.empty()) {
    return false;
  }

  make_indent();
  out_ << name << "(";

  bool first = true;
  const auto& params = expr->params();
  for (auto* param : params) {
    if (!first) {
      out_ << ", ";
    }
    first = false;

    if (!EmitExpression(param)) {
      return false;
    }
  }

  out_ << ")";
  return true;
}

bool GeneratorImpl::EmitTextureCall(ast::CallExpression* expr,
                                    const sem::Intrinsic* intrinsic) {
  using Usage = sem::ParameterUsage;

  auto parameters = intrinsic->Parameters();
  auto arguments = expr->params();

  // Returns the argument with the given usage
  auto arg = [&](Usage usage) {
    int idx = sem::IndexOf(parameters, usage);
    return (idx >= 0) ? arguments[idx] : nullptr;
  };

  auto* texture = arg(Usage::kTexture);
  if (!texture) {
    TINT_ICE(diagnostics_) << "missing texture arg";
    return false;
  }

  auto* texture_type = TypeOf(texture)->UnwrapRef()->As<sem::Texture>();

  // Helper to emit the texture expression, wrapped in parentheses if the
  // expression includes an operator with lower precedence than the member
  // accessor used for the function calls.
  auto texture_expr = [&]() {
    bool paren_lhs =
        !texture
             ->IsAnyOf<ast::ArrayAccessorExpression, ast::CallExpression,
                       ast::IdentifierExpression, ast::MemberAccessorExpression,
                       ast::TypeConstructorExpression>();
    if (paren_lhs) {
      out_ << "(";
    }
    if (!EmitExpression(texture)) {
      return false;
    }
    if (paren_lhs) {
      out_ << ")";
    }
    return true;
  };

  switch (intrinsic->Type()) {
    case sem::IntrinsicType::kTextureDimensions: {
      std::vector<const char*> dims;
      switch (texture_type->dim()) {
        case ast::TextureDimension::kNone:
          diagnostics_.add_error("texture dimension is kNone");
          return false;
        case ast::TextureDimension::k1d:
          dims = {"width"};
          break;
        case ast::TextureDimension::k2d:
        case ast::TextureDimension::k2dArray:
          dims = {"width", "height"};
          break;
        case ast::TextureDimension::k3d:
          dims = {"width", "height", "depth"};
          break;
        case ast::TextureDimension::kCube:
        case ast::TextureDimension::kCubeArray:
          // width == height == depth for cubes
          // See https://github.com/gpuweb/gpuweb/issues/1345
          dims = {"width", "height", "height"};
          break;
      }

      auto get_dim = [&](const char* name) {
        if (!texture_expr()) {
          return false;
        }
        out_ << ".get_" << name << "(";
        if (auto* level = arg(Usage::kLevel)) {
          if (!EmitExpression(level)) {
            return false;
          }
        }
        out_ << ")";
        return true;
      };

      if (dims.size() == 1) {
        out_ << "int(";
        get_dim(dims[0]);
        out_ << ")";
      } else {
        EmitType(TypeOf(expr)->UnwrapRef(), "");
        out_ << "(";
        for (size_t i = 0; i < dims.size(); i++) {
          if (i > 0) {
            out_ << ", ";
          }
          get_dim(dims[i]);
        }
        out_ << ")";
      }
      return true;
    }
    case sem::IntrinsicType::kTextureNumLayers: {
      out_ << "int(";
      if (!texture_expr()) {
        return false;
      }
      out_ << ".get_array_size())";
      return true;
    }
    case sem::IntrinsicType::kTextureNumLevels: {
      out_ << "int(";
      if (!texture_expr()) {
        return false;
      }
      out_ << ".get_num_mip_levels())";
      return true;
    }
    case sem::IntrinsicType::kTextureNumSamples: {
      out_ << "int(";
      if (!texture_expr()) {
        return false;
      }
      out_ << ".get_num_samples())";
      return true;
    }
    default:
      break;
  }

  if (!texture_expr()) {
    return false;
  }

  bool lod_param_is_named = true;

  switch (intrinsic->Type()) {
    case sem::IntrinsicType::kTextureSample:
    case sem::IntrinsicType::kTextureSampleBias:
    case sem::IntrinsicType::kTextureSampleLevel:
    case sem::IntrinsicType::kTextureSampleGrad:
      out_ << ".sample(";
      break;
    case sem::IntrinsicType::kTextureSampleCompare:
    case sem::IntrinsicType::kTextureSampleCompareLevel:
      out_ << ".sample_compare(";
      break;
    case sem::IntrinsicType::kTextureLoad:
      out_ << ".read(";
      lod_param_is_named = false;
      break;
    case sem::IntrinsicType::kTextureStore:
      out_ << ".write(";
      break;
    default:
      TINT_UNREACHABLE(diagnostics_)
          << "Unhandled texture intrinsic '" << intrinsic->str() << "'";
      return false;
  }

  bool first_arg = true;
  auto maybe_write_comma = [&] {
    if (!first_arg) {
      out_ << ", ";
    }
    first_arg = false;
  };

  for (auto usage :
       {Usage::kValue, Usage::kSampler, Usage::kCoords, Usage::kArrayIndex,
        Usage::kDepthRef, Usage::kSampleIndex}) {
    if (auto* e = arg(usage)) {
      auto* sem_e = program_->Sem().Get(e);

      maybe_write_comma();

      // Cast the coordinates to unsigned integers if necessary.
      bool casted = false;
      if (usage == Usage::kCoords &&
          sem_e->Type()->UnwrapRef()->is_integer_scalar_or_vector()) {
        casted = true;
        switch (texture_type->dim()) {
          case ast::TextureDimension::k1d:
            out_ << "uint(";
            break;
          case ast::TextureDimension::k2d:
          case ast::TextureDimension::k2dArray:
            out_ << "uint2(";
            break;
          case ast::TextureDimension::k3d:
            out_ << "uint3(";
            break;
          default:
            TINT_ICE(diagnostics_) << "unhandled texture dimensionality";
            break;
        }
      }

      if (!EmitExpression(e))
        return false;

      if (casted) {
        out_ << ")";
      }
    }
  }

  if (auto* bias = arg(Usage::kBias)) {
    maybe_write_comma();
    out_ << "bias(";
    if (!EmitExpression(bias)) {
      return false;
    }
    out_ << ")";
  }
  if (auto* level = arg(Usage::kLevel)) {
    maybe_write_comma();
    if (lod_param_is_named) {
      out_ << "level(";
    }
    if (!EmitExpression(level)) {
      return false;
    }
    if (lod_param_is_named) {
      out_ << ")";
    }
  }
  if (intrinsic->Type() == sem::IntrinsicType::kTextureSampleCompareLevel) {
    maybe_write_comma();
    out_ << "level(0)";
  }
  if (auto* ddx = arg(Usage::kDdx)) {
    auto dim = texture_type->dim();
    switch (dim) {
      case ast::TextureDimension::k2d:
      case ast::TextureDimension::k2dArray:
        maybe_write_comma();
        out_ << "gradient2d(";
        break;
      case ast::TextureDimension::k3d:
        maybe_write_comma();
        out_ << "gradient3d(";
        break;
      case ast::TextureDimension::kCube:
      case ast::TextureDimension::kCubeArray:
        maybe_write_comma();
        out_ << "gradientcube(";
        break;
      default: {
        std::stringstream err;
        err << "MSL does not support gradients for " << dim << " textures";
        diagnostics_.add_error(err.str());
        return false;
      }
    }
    if (!EmitExpression(ddx)) {
      return false;
    }
    out_ << ", ";
    if (!EmitExpression(arg(Usage::kDdy))) {
      return false;
    }
    out_ << ")";
  }

  if (auto* offset = arg(Usage::kOffset)) {
    maybe_write_comma();
    if (!EmitExpression(offset)) {
      return false;
    }
  }

  out_ << ")";

  return true;
}

std::string GeneratorImpl::generate_builtin_name(
    const sem::Intrinsic* intrinsic) {
  std::string out = "";
  switch (intrinsic->Type()) {
    case sem::IntrinsicType::kAcos:
    case sem::IntrinsicType::kAll:
    case sem::IntrinsicType::kAny:
    case sem::IntrinsicType::kAsin:
    case sem::IntrinsicType::kAtan:
    case sem::IntrinsicType::kAtan2:
    case sem::IntrinsicType::kCeil:
    case sem::IntrinsicType::kCos:
    case sem::IntrinsicType::kCosh:
    case sem::IntrinsicType::kCross:
    case sem::IntrinsicType::kDeterminant:
    case sem::IntrinsicType::kDistance:
    case sem::IntrinsicType::kDot:
    case sem::IntrinsicType::kExp:
    case sem::IntrinsicType::kExp2:
    case sem::IntrinsicType::kFloor:
    case sem::IntrinsicType::kFma:
    case sem::IntrinsicType::kFract:
    case sem::IntrinsicType::kLength:
    case sem::IntrinsicType::kLdexp:
    case sem::IntrinsicType::kLog:
    case sem::IntrinsicType::kLog2:
    case sem::IntrinsicType::kMix:
    case sem::IntrinsicType::kNormalize:
    case sem::IntrinsicType::kPow:
    case sem::IntrinsicType::kReflect:
    case sem::IntrinsicType::kSelect:
    case sem::IntrinsicType::kSin:
    case sem::IntrinsicType::kSinh:
    case sem::IntrinsicType::kSqrt:
    case sem::IntrinsicType::kStep:
    case sem::IntrinsicType::kTan:
    case sem::IntrinsicType::kTanh:
    case sem::IntrinsicType::kTranspose:
    case sem::IntrinsicType::kTrunc:
    case sem::IntrinsicType::kSign:
    case sem::IntrinsicType::kClamp:
      out += intrinsic->str();
      break;
    case sem::IntrinsicType::kAbs:
      if (intrinsic->ReturnType()->is_float_scalar_or_vector()) {
        out += "fabs";
      } else {
        out += "abs";
      }
      break;
    case sem::IntrinsicType::kCountOneBits:
      out += "popcount";
      break;
    case sem::IntrinsicType::kDpdx:
    case sem::IntrinsicType::kDpdxCoarse:
    case sem::IntrinsicType::kDpdxFine:
      out += "dfdx";
      break;
    case sem::IntrinsicType::kDpdy:
    case sem::IntrinsicType::kDpdyCoarse:
    case sem::IntrinsicType::kDpdyFine:
      out += "dfdy";
      break;
    case sem::IntrinsicType::kFwidth:
    case sem::IntrinsicType::kFwidthCoarse:
    case sem::IntrinsicType::kFwidthFine:
      out += "fwidth";
      break;
    case sem::IntrinsicType::kIsFinite:
      out += "isfinite";
      break;
    case sem::IntrinsicType::kIsInf:
      out += "isinf";
      break;
    case sem::IntrinsicType::kIsNan:
      out += "isnan";
      break;
    case sem::IntrinsicType::kIsNormal:
      out += "isnormal";
      break;
    case sem::IntrinsicType::kMax:
      if (intrinsic->ReturnType()->is_float_scalar_or_vector()) {
        out += "fmax";
      } else {
        out += "max";
      }
      break;
    case sem::IntrinsicType::kMin:
      if (intrinsic->ReturnType()->is_float_scalar_or_vector()) {
        out += "fmin";
      } else {
        out += "min";
      }
      break;
    case sem::IntrinsicType::kFaceForward:
      out += "faceforward";
      break;
    case sem::IntrinsicType::kPack4x8snorm:
      out += "pack_float_to_snorm4x8";
      break;
    case sem::IntrinsicType::kPack4x8unorm:
      out += "pack_float_to_unorm4x8";
      break;
    case sem::IntrinsicType::kPack2x16snorm:
      out += "pack_float_to_snorm2x16";
      break;
    case sem::IntrinsicType::kPack2x16unorm:
      out += "pack_float_to_unorm2x16";
      break;
    case sem::IntrinsicType::kReverseBits:
      out += "reverse_bits";
      break;
    case sem::IntrinsicType::kRound:
      out += "rint";
      break;
    case sem::IntrinsicType::kSmoothStep:
      out += "smoothstep";
      break;
    case sem::IntrinsicType::kInverseSqrt:
      out += "rsqrt";
      break;
    case sem::IntrinsicType::kUnpack4x8snorm:
      out += "unpack_snorm4x8_to_float";
      break;
    case sem::IntrinsicType::kUnpack4x8unorm:
      out += "unpack_unorm4x8_to_float";
      break;
    case sem::IntrinsicType::kUnpack2x16snorm:
      out += "unpack_snorm2x16_to_float";
      break;
    case sem::IntrinsicType::kUnpack2x16unorm:
      out += "unpack_unorm2x16_to_float";
      break;
    default:
      diagnostics_.add_error("Unknown import method: " +
                             std::string(intrinsic->str()));
      return "";
  }
  return out;
}

bool GeneratorImpl::EmitCase(ast::CaseStatement* stmt) {
  make_indent();

  if (stmt->IsDefault()) {
    out_ << "default:";
  } else {
    bool first = true;
    for (auto* selector : stmt->selectors()) {
      if (!first) {
        out_ << std::endl;
        make_indent();
      }
      first = false;

      out_ << "case ";
      if (!EmitLiteral(selector)) {
        return false;
      }
      out_ << ":";
    }
  }

  out_ << " {" << std::endl;

  increment_indent();

  for (auto* s : *stmt->body()) {
    if (!EmitStatement(s)) {
      return false;
    }
  }

  if (!last_is_break_or_fallthrough(stmt->body())) {
    make_indent();
    out_ << "break;" << std::endl;
  }

  decrement_indent();
  make_indent();
  out_ << "}" << std::endl;

  return true;
}

bool GeneratorImpl::EmitConstructor(ast::ConstructorExpression* expr) {
  if (auto* scalar = expr->As<ast::ScalarConstructorExpression>()) {
    return EmitScalarConstructor(scalar);
  }
  return EmitTypeConstructor(expr->As<ast::TypeConstructorExpression>());
}

bool GeneratorImpl::EmitContinue(ast::ContinueStatement*) {
  make_indent();
  out_ << "continue;" << std::endl;
  return true;
}

bool GeneratorImpl::EmitTypeConstructor(ast::TypeConstructorExpression* expr) {
  auto* type = TypeOf(expr)->UnwrapRef();

  if (type->IsAnyOf<sem::Array, sem::Struct>()) {
    out_ << "{";
  } else {
    if (!EmitType(type, "")) {
      return false;
    }
    out_ << "(";
  }

  int i = 0;
  for (auto* e : expr->values()) {
    if (i > 0) {
      out_ << ", ";
    }

    if (auto* struct_ty = type->As<sem::Struct>()) {
      // Emit field designators for structures to account for padding members.
      auto* member = struct_ty->Members()[i]->Declaration();
      auto name = program_->Symbols().NameFor(member->symbol());
      out_ << "." << name << "=";
    }

    if (!EmitExpression(e)) {
      return false;
    }

    i++;
  }

  if (type->IsAnyOf<sem::Array, sem::Struct>()) {
    out_ << "}";
  } else {
    out_ << ")";
  }
  return true;
}

bool GeneratorImpl::EmitZeroValue(const sem::Type* type) {
  if (type->Is<sem::Bool>()) {
    out_ << "false";
  } else if (type->Is<sem::F32>()) {
    out_ << "0.0f";
  } else if (type->Is<sem::I32>()) {
    out_ << "0";
  } else if (type->Is<sem::U32>()) {
    out_ << "0u";
  } else if (auto* vec = type->As<sem::Vector>()) {
    return EmitZeroValue(vec->type());
  } else if (auto* mat = type->As<sem::Matrix>()) {
    if (!EmitType(mat, "")) {
      return false;
    }
    out_ << "(";
    if (!EmitZeroValue(mat->type())) {
      return false;
    }
    out_ << ")";
  } else if (auto* arr = type->As<sem::Array>()) {
    out_ << "{";
    if (!EmitZeroValue(arr->ElemType())) {
      return false;
    }
    out_ << "}";
  } else if (type->As<sem::Struct>()) {
    out_ << "{}";
  } else {
    diagnostics_.add_error("Invalid type for zero emission: " +
                           type->type_name());
    return false;
  }
  return true;
}

bool GeneratorImpl::EmitScalarConstructor(
    ast::ScalarConstructorExpression* expr) {
  return EmitLiteral(expr->literal());
}

bool GeneratorImpl::EmitLiteral(ast::Literal* lit) {
  if (auto* l = lit->As<ast::BoolLiteral>()) {
    out_ << (l->IsTrue() ? "true" : "false");
  } else if (auto* fl = lit->As<ast::FloatLiteral>()) {
    out_ << FloatToString(fl->value()) << "f";
  } else if (auto* sl = lit->As<ast::SintLiteral>()) {
    out_ << sl->value();
  } else if (auto* ul = lit->As<ast::UintLiteral>()) {
    out_ << ul->value() << "u";
  } else {
    diagnostics_.add_error("unknown literal type");
    return false;
  }
  return true;
}

bool GeneratorImpl::EmitExpression(ast::Expression* expr) {
  if (auto* a = expr->As<ast::ArrayAccessorExpression>()) {
    return EmitArrayAccessor(a);
  }
  if (auto* b = expr->As<ast::BinaryExpression>()) {
    return EmitBinary(b);
  }
  if (auto* b = expr->As<ast::BitcastExpression>()) {
    return EmitBitcast(b);
  }
  if (auto* c = expr->As<ast::CallExpression>()) {
    return EmitCall(c);
  }
  if (auto* c = expr->As<ast::ConstructorExpression>()) {
    return EmitConstructor(c);
  }
  if (auto* i = expr->As<ast::IdentifierExpression>()) {
    return EmitIdentifier(i);
  }
  if (auto* m = expr->As<ast::MemberAccessorExpression>()) {
    return EmitMemberAccessor(m);
  }
  if (auto* u = expr->As<ast::UnaryOpExpression>()) {
    return EmitUnaryOp(u);
  }

  diagnostics_.add_error("unknown expression type: " + program_->str(expr));
  return false;
}

void GeneratorImpl::EmitStage(ast::PipelineStage stage) {
  switch (stage) {
    case ast::PipelineStage::kFragment:
      out_ << "fragment";
      break;
    case ast::PipelineStage::kVertex:
      out_ << "vertex";
      break;
    case ast::PipelineStage::kCompute:
      out_ << "kernel";
      break;
    case ast::PipelineStage::kNone:
      break;
  }
  return;
}

bool GeneratorImpl::EmitFunction(ast::Function* func) {
  make_indent();

  auto* func_sem = program_->Sem().Get(func);

  if (!EmitType(func_sem->ReturnType(), "")) {
    return false;
  }
  out_ << " " << program_->Symbols().NameFor(func->symbol()) << "(";

  bool first = true;
  for (const auto& data : func_sem->ReferencedUniformVariables()) {
    auto* var = data.first;
    if (!first) {
      out_ << ", ";
    }
    first = false;

    out_ << "constant ";
    // TODO(dsinclair): Can arrays be uniform? If so, fix this ...
    if (!EmitType(var->Type()->UnwrapRef(), "")) {
      return false;
    }
    out_ << "& " << program_->Symbols().NameFor(var->Declaration()->symbol());
  }

  for (const auto& data : func_sem->ReferencedStorageBufferVariables()) {
    auto* var = data.first;
    if (!first) {
      out_ << ", ";
    }
    first = false;

    if (var->Access() == ast::Access::kRead) {
      out_ << "const ";
    }

    out_ << "device ";
    if (!EmitType(var->Type()->UnwrapRef(), "")) {
      return false;
    }
    out_ << "& " << program_->Symbols().NameFor(var->Declaration()->symbol());
  }

  for (auto* v : func->params()) {
    if (!first) {
      out_ << ", ";
    }
    first = false;

    auto* type = program_->Sem().Get(v)->Type();

    std::string param_name =
        "const " + program_->Symbols().NameFor(v->symbol());
    if (!EmitType(type, param_name)) {
      return false;
    }
    // Parameter name is output as part of the type for arrays and pointers.
    if (!type->Is<sem::Array>() && !type->Is<sem::Pointer>()) {
      out_ << " " << program_->Symbols().NameFor(v->symbol());
    }
  }

  out_ << ") ";

  if (!EmitBlockAndNewline(func->body())) {
    return false;
  }

  out_ << std::endl;

  return true;
}

std::string GeneratorImpl::builtin_to_attribute(ast::Builtin builtin) const {
  switch (builtin) {
    case ast::Builtin::kPosition:
      return "position";
    case ast::Builtin::kVertexIndex:
      return "vertex_id";
    case ast::Builtin::kInstanceIndex:
      return "instance_id";
    case ast::Builtin::kFrontFacing:
      return "front_facing";
    case ast::Builtin::kFragDepth:
      return "depth(any)";
    case ast::Builtin::kLocalInvocationId:
      return "thread_position_in_threadgroup";
    case ast::Builtin::kLocalInvocationIndex:
      return "thread_index_in_threadgroup";
    case ast::Builtin::kGlobalInvocationId:
      return "thread_position_in_grid";
    case ast::Builtin::kWorkgroupId:
      return "threadgroup_position_in_grid";
    case ast::Builtin::kSampleIndex:
      return "sample_id";
    case ast::Builtin::kSampleMask:
      return "sample_mask";
    default:
      break;
  }
  return "";
}

bool GeneratorImpl::EmitEntryPointFunction(ast::Function* func) {
  auto* func_sem = program_->Sem().Get(func);

  make_indent();

  EmitStage(func->pipeline_stage());
  out_ << " " << func->return_type()->FriendlyName(program_->Symbols());
  out_ << " " << program_->Symbols().NameFor(func->symbol()) << "(";

  // Emit entry point parameters.
  bool first = true;
  for (auto* var : func->params()) {
    if (!first) {
      out_ << ", ";
    }
    first = false;

    auto* type = program_->Sem().Get(var)->Type()->UnwrapRef();

    if (!EmitType(type, "")) {
      return false;
    }

    out_ << " " << program_->Symbols().NameFor(var->symbol());

    if (type->Is<sem::Struct>()) {
      out_ << " [[stage_in]]";
    } else if (var->type()->is_handle()) {
      auto* binding =
          ast::GetDecoration<ast::BindingDecoration>(var->decorations());
      if (binding == nullptr) {
        TINT_ICE(diagnostics_)
            << "missing binding attribute for entry point parameter";
        return false;
      }
      if (var->type()->Is<ast::Sampler>()) {
        out_ << " [[sampler(" << binding->value() << ")]]";
      } else if (var->type()->Is<ast::Texture>()) {
        out_ << " [[texture(" << binding->value() << ")]]";
      } else {
        TINT_ICE(diagnostics_) << "invalid handle type entry point parameter";
        return false;
      }
    } else {
      auto& decos = var->decorations();
      bool builtin_found = false;
      for (auto* deco : decos) {
        auto* builtin = deco->As<ast::BuiltinDecoration>();
        if (!builtin) {
          continue;
        }

        builtin_found = true;

        auto attr = builtin_to_attribute(builtin->value());
        if (attr.empty()) {
          diagnostics_.add_error("unknown builtin");
          return false;
        }
        out_ << " [[" << attr << "]]";
      }
      if (!builtin_found) {
        TINT_ICE(diagnostics_) << "Unsupported entry point parameter";
      }
    }
  }

  for (auto data : func_sem->ReferencedUniformVariables()) {
    if (!first) {
      out_ << ", ";
    }
    first = false;

    auto* var = data.first;
    // TODO(dsinclair): We're using the binding to make up the buffer number but
    // we should instead be using a provided mapping that uses both buffer and
    // set. https://bugs.chromium.org/p/tint/issues/detail?id=104
    auto* binding = data.second.binding;
    if (binding == nullptr) {
      diagnostics_.add_error(
          "unable to find binding information for uniform: " +
          program_->Symbols().NameFor(var->Declaration()->symbol()));
      return false;
    }
    // auto* set = data.second.set;

    out_ << "constant ";
    // TODO(dsinclair): Can you have a uniform array? If so, this needs to be
    // updated to handle arrays property.
    if (!EmitType(var->Type()->UnwrapRef(), "")) {
      return false;
    }
    out_ << "& " << program_->Symbols().NameFor(var->Declaration()->symbol())
         << " [[buffer(" << binding->value() << ")]]";
  }

  for (auto data : func_sem->ReferencedStorageBufferVariables()) {
    if (!first) {
      out_ << ", ";
    }
    first = false;

    auto* var = data.first;
    // TODO(dsinclair): We're using the binding to make up the buffer number but
    // we should instead be using a provided mapping that uses both buffer and
    // set. https://bugs.chromium.org/p/tint/issues/detail?id=104
    auto* binding = data.second.binding;
    // auto* set = data.second.set;

    if (var->Access() == ast::Access::kRead) {
      out_ << "const ";
    }

    out_ << "device ";
    if (!EmitType(var->Type()->UnwrapRef(), "")) {
      return false;
    }
    out_ << "& " << program_->Symbols().NameFor(var->Declaration()->symbol())
         << " [[buffer(" << binding->value() << ")]]";
  }

  out_ << ") {" << std::endl;

  increment_indent();

  for (auto* s : *func->body()) {
    if (!EmitStatement(s)) {
      return false;
    }
  }
  auto* last_statement = func->get_last_statement();
  if (last_statement == nullptr ||
      !last_statement->Is<ast::ReturnStatement>()) {
    ast::ReturnStatement ret(ProgramID{}, Source{});
    if (!EmitStatement(&ret)) {
      return false;
    }
  }

  decrement_indent();
  make_indent();
  out_ << "}" << std::endl << std::endl;
  return true;
}

bool GeneratorImpl::EmitIdentifier(ast::IdentifierExpression* expr) {
  out_ << program_->Symbols().NameFor(expr->symbol());
  return true;
}

bool GeneratorImpl::EmitLoop(ast::LoopStatement* stmt) {
  loop_emission_counter_++;

  std::string guard =
      "tint_msl_is_first_" + std::to_string(loop_emission_counter_);

  if (stmt->has_continuing()) {
    make_indent();

    // Continuing variables get their own scope.
    out_ << "{" << std::endl;
    increment_indent();

    make_indent();
    out_ << "bool " << guard << " = true;" << std::endl;

    // A continuing block may use variables declared in the method body. As a
    // first pass, if we have a continuing, we pull all declarations outside
    // the for loop into the continuing scope. Then, the variable declarations
    // will be turned into assignments.
    for (auto* s : *(stmt->body())) {
      if (auto* decl = s->As<ast::VariableDeclStatement>()) {
        if (!EmitVariable(program_->Sem().Get(decl->variable()), true)) {
          return false;
        }
      }
    }
  }

  make_indent();
  out_ << "for(;;) {" << std::endl;
  increment_indent();

  if (stmt->has_continuing()) {
    make_indent();
    out_ << "if (!" << guard << ") ";

    if (!EmitBlockAndNewline(stmt->continuing())) {
      return false;
    }

    make_indent();
    out_ << guard << " = false;" << std::endl;
    out_ << std::endl;
  }

  for (auto* s : *(stmt->body())) {
    // If we have a continuing block we've already emitted the variable
    // declaration before the loop, so treat it as an assignment.
    auto* decl = s->As<ast::VariableDeclStatement>();
    if (decl != nullptr && stmt->has_continuing()) {
      make_indent();

      auto* var = decl->variable();
      out_ << program_->Symbols().NameFor(var->symbol()) << " = ";
      if (var->constructor() != nullptr) {
        if (!EmitExpression(var->constructor())) {
          return false;
        }
      } else {
        auto* type = program_->Sem().Get(var)->Type()->UnwrapRef();
        if (!EmitZeroValue(type)) {
          return false;
        }
      }
      out_ << ";" << std::endl;
      continue;
    }

    if (!EmitStatement(s)) {
      return false;
    }
  }

  decrement_indent();
  make_indent();
  out_ << "}" << std::endl;

  // Close the scope for any continuing variables.
  if (stmt->has_continuing()) {
    decrement_indent();
    make_indent();
    out_ << "}" << std::endl;
  }

  return true;
}

bool GeneratorImpl::EmitDiscard(ast::DiscardStatement*) {
  make_indent();
  // TODO(dsinclair): Verify this is correct when the discard semantics are
  // defined for WGSL (https://github.com/gpuweb/gpuweb/issues/361)
  out_ << "discard_fragment();" << std::endl;
  return true;
}

bool GeneratorImpl::EmitElse(ast::ElseStatement* stmt) {
  if (stmt->HasCondition()) {
    out_ << " else if (";
    if (!EmitExpression(stmt->condition())) {
      return false;
    }
    out_ << ") ";
  } else {
    out_ << " else ";
  }

  return EmitBlock(stmt->body());
}

bool GeneratorImpl::EmitIf(ast::IfStatement* stmt) {
  make_indent();

  out_ << "if (";
  if (!EmitExpression(stmt->condition())) {
    return false;
  }
  out_ << ") ";

  if (!EmitBlock(stmt->body())) {
    return false;
  }

  for (auto* e : stmt->else_statements()) {
    if (!EmitElse(e)) {
      return false;
    }
  }
  out_ << std::endl;

  return true;
}

bool GeneratorImpl::EmitMemberAccessor(ast::MemberAccessorExpression* expr) {
  bool paren_lhs =
      !expr->structure()
           ->IsAnyOf<ast::ArrayAccessorExpression, ast::CallExpression,
                     ast::IdentifierExpression, ast::MemberAccessorExpression,
                     ast::TypeConstructorExpression>();
  if (paren_lhs) {
    out_ << "(";
  }
  if (!EmitExpression(expr->structure())) {
    return false;
  }
  if (paren_lhs) {
    out_ << ")";
  }

  out_ << ".";

  // Swizzles get written out directly
  if (program_->Sem().Get(expr)->Is<sem::Swizzle>()) {
    out_ << program_->Symbols().NameFor(expr->member()->symbol());
  } else if (!EmitExpression(expr->member())) {
    return false;
  }

  return true;
}

bool GeneratorImpl::EmitReturn(ast::ReturnStatement* stmt) {
  make_indent();

  out_ << "return";
  if (stmt->has_value()) {
    out_ << " ";
    if (!EmitExpression(stmt->value())) {
      return false;
    }
  }
  out_ << ";" << std::endl;
  return true;
}

bool GeneratorImpl::EmitBlock(const ast::BlockStatement* stmt) {
  out_ << "{" << std::endl;
  increment_indent();

  for (auto* s : *stmt) {
    if (!EmitStatement(s)) {
      return false;
    }
  }

  decrement_indent();
  make_indent();
  out_ << "}";

  return true;
}

bool GeneratorImpl::EmitBlockAndNewline(const ast::BlockStatement* stmt) {
  const bool result = EmitBlock(stmt);
  if (result) {
    out_ << std::endl;
  }
  return result;
}

bool GeneratorImpl::EmitIndentedBlockAndNewline(ast::BlockStatement* stmt) {
  make_indent();
  const bool result = EmitBlock(stmt);
  if (result) {
    out_ << std::endl;
  }
  return result;
}

bool GeneratorImpl::EmitStatement(ast::Statement* stmt) {
  if (auto* a = stmt->As<ast::AssignmentStatement>()) {
    return EmitAssign(a);
  }
  if (auto* b = stmt->As<ast::BlockStatement>()) {
    return EmitIndentedBlockAndNewline(b);
  }
  if (auto* b = stmt->As<ast::BreakStatement>()) {
    return EmitBreak(b);
  }
  if (auto* c = stmt->As<ast::CallStatement>()) {
    make_indent();
    if (!EmitCall(c->expr())) {
      return false;
    }
    out_ << ";" << std::endl;
    return true;
  }
  if (auto* c = stmt->As<ast::ContinueStatement>()) {
    return EmitContinue(c);
  }
  if (auto* d = stmt->As<ast::DiscardStatement>()) {
    return EmitDiscard(d);
  }
  if (stmt->As<ast::FallthroughStatement>()) {
    make_indent();
    out_ << "/* fallthrough */" << std::endl;
    return true;
  }
  if (auto* i = stmt->As<ast::IfStatement>()) {
    return EmitIf(i);
  }
  if (auto* l = stmt->As<ast::LoopStatement>()) {
    return EmitLoop(l);
  }
  if (auto* r = stmt->As<ast::ReturnStatement>()) {
    return EmitReturn(r);
  }
  if (auto* s = stmt->As<ast::SwitchStatement>()) {
    return EmitSwitch(s);
  }
  if (auto* v = stmt->As<ast::VariableDeclStatement>()) {
    auto* var = program_->Sem().Get(v->variable());
    return EmitVariable(var, false);
  }

  diagnostics_.add_error("unknown statement type: " + program_->str(stmt));
  return false;
}

bool GeneratorImpl::EmitSwitch(ast::SwitchStatement* stmt) {
  make_indent();

  out_ << "switch(";
  if (!EmitExpression(stmt->condition())) {
    return false;
  }
  out_ << ") {" << std::endl;

  increment_indent();

  for (auto* s : stmt->body()) {
    if (!EmitCase(s)) {
      return false;
    }
  }

  decrement_indent();
  make_indent();
  out_ << "}" << std::endl;

  return true;
}

bool GeneratorImpl::EmitType(const sem::Type* type, const std::string& name) {
  if (auto* ary = type->As<sem::Array>()) {
    const sem::Type* base_type = ary;
    std::vector<uint32_t> sizes;
    while (auto* arr = base_type->As<sem::Array>()) {
      if (arr->IsRuntimeSized()) {
        sizes.push_back(1);
      } else {
        sizes.push_back(arr->Count());
      }
      base_type = arr->ElemType();
    }
    if (!EmitType(base_type, "")) {
      return false;
    }
    if (!name.empty()) {
      out_ << " " << name;
    }
    for (uint32_t size : sizes) {
      out_ << "[" << size << "]";
    }
  } else if (type->Is<sem::Bool>()) {
    out_ << "bool";
  } else if (type->Is<sem::F32>()) {
    out_ << "float";
  } else if (type->Is<sem::I32>()) {
    out_ << "int";
  } else if (auto* mat = type->As<sem::Matrix>()) {
    if (!EmitType(mat->type(), "")) {
      return false;
    }
    out_ << mat->columns() << "x" << mat->rows();
  } else if (auto* ptr = type->As<sem::Pointer>()) {
    switch (ptr->StorageClass()) {
      case ast::StorageClass::kFunction:
      case ast::StorageClass::kPrivate:
      case ast::StorageClass::kUniformConstant:
        out_ << "thread ";
        break;
      case ast::StorageClass::kWorkgroup:
        out_ << "threadgroup ";
        break;
      case ast::StorageClass::kStorage:
        out_ << "device ";
        break;
      case ast::StorageClass::kUniform:
        out_ << "constant ";
        break;
      default:
        TINT_ICE(diagnostics_) << "unhandled storage class for pointer";
    }
    if (ptr->StoreType()->Is<sem::Array>()) {
      std::string inner = "(*" + name + ")";
      if (!EmitType(ptr->StoreType(), inner)) {
        return false;
      }
    } else {
      if (!EmitType(ptr->StoreType(), "")) {
        return false;
      }
      out_ << "* " << name;
    }
  } else if (type->Is<sem::Sampler>()) {
    out_ << "sampler";
  } else if (auto* str = type->As<sem::Struct>()) {
    // The struct type emits as just the name. The declaration would be emitted
    // as part of emitting the declared types.
    out_ << program_->Symbols().NameFor(str->Declaration()->name());
  } else if (auto* tex = type->As<sem::Texture>()) {
    if (tex->Is<sem::DepthTexture>()) {
      out_ << "depth";
    } else {
      out_ << "texture";
    }

    switch (tex->dim()) {
      case ast::TextureDimension::k1d:
        out_ << "1d";
        break;
      case ast::TextureDimension::k2d:
        out_ << "2d";
        break;
      case ast::TextureDimension::k2dArray:
        out_ << "2d_array";
        break;
      case ast::TextureDimension::k3d:
        out_ << "3d";
        break;
      case ast::TextureDimension::kCube:
        out_ << "cube";
        break;
      case ast::TextureDimension::kCubeArray:
        out_ << "cube_array";
        break;
      default:
        diagnostics_.add_error("Invalid texture dimensions");
        return false;
    }
    if (tex->Is<sem::MultisampledTexture>()) {
      out_ << "_ms";
    }
    out_ << "<";
    if (tex->Is<sem::DepthTexture>()) {
      out_ << "float, access::sample";
    } else if (auto* storage = tex->As<sem::StorageTexture>()) {
      if (!EmitType(storage->type(), "")) {
        return false;
      }

      std::string access_str;
      if (storage->access() == ast::Access::kRead) {
        out_ << ", access::read";
      } else if (storage->access() == ast::Access::kWrite) {
        out_ << ", access::write";
      } else {
        diagnostics_.add_error("Invalid access control for storage texture");
        return false;
      }
    } else if (auto* ms = tex->As<sem::MultisampledTexture>()) {
      if (!EmitType(ms->type(), "")) {
        return false;
      }
      out_ << ", access::read";
    } else if (auto* sampled = tex->As<sem::SampledTexture>()) {
      if (!EmitType(sampled->type(), "")) {
        return false;
      }
      out_ << ", access::sample";
    } else {
      diagnostics_.add_error("invalid texture type");
      return false;
    }
    out_ << ">";

  } else if (type->Is<sem::U32>()) {
    out_ << "uint";
  } else if (auto* vec = type->As<sem::Vector>()) {
    if (!EmitType(vec->type(), "")) {
      return false;
    }
    out_ << vec->size();
  } else if (type->Is<sem::Void>()) {
    out_ << "void";
  } else {
    diagnostics_.add_error("unknown type in EmitType: " + type->type_name());
    return false;
  }

  return true;
}

bool GeneratorImpl::EmitPackedType(const sem::Type* type,
                                   const std::string& name) {
  if (auto* vec = type->As<sem::Vector>()) {
    out_ << "packed_";
    if (!EmitType(vec->type(), "")) {
      return false;
    }
    out_ << vec->size();
    return true;
  }

  return EmitType(type, name);
}

bool GeneratorImpl::EmitStructType(const sem::Struct* str) {
  out_ << "struct " << program_->Symbols().NameFor(str->Declaration()->name())
       << " {" << std::endl;

  bool is_host_shareable = str->IsHostShareable();

  // Emits a `/* 0xnnnn */` byte offset comment for a struct member.
  auto add_byte_offset_comment = [&](uint32_t offset) {
    std::ios_base::fmtflags saved_flag_state(out_.flags());
    out_ << "/* 0x" << std::hex << std::setfill('0') << std::setw(4) << offset
         << " */ ";
    out_.flags(saved_flag_state);
  };

  uint32_t pad_count = 0;
  auto add_padding = [&](uint32_t size) {
    std::string name;
    do {
      name = "tint_pad_" + std::to_string(pad_count++);
    } while (str->FindMember(program_->Symbols().Get(name)));

    out_ << "int8_t " << name << "[" << size << "];" << std::endl;
  };

  increment_indent();
  uint32_t msl_offset = 0;
  for (auto* mem : str->Members()) {
    make_indent();

    auto name = program_->Symbols().NameFor(mem->Declaration()->symbol());
    auto wgsl_offset = mem->Offset();

    if (is_host_shareable) {
      if (wgsl_offset < msl_offset) {
        // Unimplementable layout
        TINT_ICE(diagnostics_)
            << "Structure member WGSL offset (" << wgsl_offset
            << ") is behind MSL offset (" << msl_offset << ")";
        return false;
      }

      // Generate padding if required
      if (auto padding = wgsl_offset - msl_offset) {
        add_byte_offset_comment(msl_offset);
        add_padding(padding);
        msl_offset += padding;
        make_indent();
      }

      add_byte_offset_comment(msl_offset);

      if (!EmitPackedType(mem->Type(), name)) {
        return false;
      }
    } else {
      if (!EmitType(mem->Type(), name)) {
        return false;
      }
    }

    auto* ty = mem->Type();

    // Array member name will be output with the type
    if (!ty->Is<sem::Array>()) {
      out_ << " " << name;
    }

    // Emit decorations
    for (auto* deco : mem->Declaration()->decorations()) {
      if (auto* builtin = deco->As<ast::BuiltinDecoration>()) {
        auto attr = builtin_to_attribute(builtin->value());
        if (attr.empty()) {
          diagnostics_.add_error("unknown builtin");
          return false;
        }
        out_ << " [[" << attr << "]]";
      } else if (auto* loc = deco->As<ast::LocationDecoration>()) {
        auto& pipeline_stage_uses = str->PipelineStageUses();
        if (pipeline_stage_uses.size() != 1) {
          TINT_ICE(diagnostics_) << "invalid entry point IO struct uses";
        }

        if (pipeline_stage_uses.count(sem::PipelineStageUsage::kVertexInput)) {
          out_ << " [[attribute(" + std::to_string(loc->value()) + ")]]";
        } else if (pipeline_stage_uses.count(
                       sem::PipelineStageUsage::kVertexOutput)) {
          out_ << " [[user(locn" + std::to_string(loc->value()) + ")]]";
        } else if (pipeline_stage_uses.count(
                       sem::PipelineStageUsage::kFragmentInput)) {
          out_ << " [[user(locn" + std::to_string(loc->value()) + ")]]";
        } else if (pipeline_stage_uses.count(
                       sem::PipelineStageUsage::kFragmentOutput)) {
          out_ << " [[color(" + std::to_string(loc->value()) + ")]]";
        } else {
          TINT_ICE(diagnostics_) << "invalid use of location decoration";
        }
      }
    }

    out_ << ";" << std::endl;

    if (is_host_shareable) {
      // Calculate new MSL offset
      auto size_align = MslPackedTypeSizeAndAlign(ty);
      if (msl_offset % size_align.align) {
        TINT_ICE(diagnostics_)
            << "Misaligned MSL structure member "
            << ty->FriendlyName(program_->Symbols()) << " " << name;
        return false;
      }
      msl_offset += size_align.size;
    }
  }

  if (is_host_shareable && str->Size() != msl_offset) {
    make_indent();
    add_byte_offset_comment(msl_offset);
    add_padding(str->Size() - msl_offset);
  }

  decrement_indent();
  make_indent();

  out_ << "};" << std::endl;
  return true;
}

bool GeneratorImpl::EmitUnaryOp(ast::UnaryOpExpression* expr) {
  switch (expr->op()) {
    case ast::UnaryOp::kAddressOf:
      out_ << "&";
      break;
    case ast::UnaryOp::kComplement:
      out_ << "~";
      break;
    case ast::UnaryOp::kIndirection:
      out_ << "*";
      break;
    case ast::UnaryOp::kNot:
      out_ << "!";
      break;
    case ast::UnaryOp::kNegation:
      out_ << "-";
      break;
  }
  out_ << "(";

  if (!EmitExpression(expr->expr())) {
    return false;
  }

  out_ << ")";

  return true;
}

bool GeneratorImpl::EmitVariable(const sem::Variable* var,
                                 bool skip_constructor) {
  make_indent();

  auto* decl = var->Declaration();

  for (auto* deco : decl->decorations()) {
    if (!deco->Is<ast::InternalDecoration>()) {
      TINT_ICE(diagnostics_) << "unexpected variable decoration";
      return false;
    }
  }

  switch (var->StorageClass()) {
    case ast::StorageClass::kFunction:
    case ast::StorageClass::kUniformConstant:
    case ast::StorageClass::kNone:
      break;
    case ast::StorageClass::kPrivate:
      out_ << "thread ";
      break;
    case ast::StorageClass::kWorkgroup:
      out_ << "threadgroup ";
      break;
    default:
      TINT_ICE(diagnostics_) << "unhandled variable storage class";
      return false;
  }

  auto* type = var->Type()->UnwrapRef();

  std::string name = program_->Symbols().NameFor(decl->symbol());
  if (decl->is_const()) {
    name = "const " + name;
  }
  if (!EmitType(type, name)) {
    return false;
  }
  // Variable name is output as part of the type for arrays and pointers.
  if (!type->Is<sem::Array>() && !type->Is<sem::Pointer>()) {
    out_ << " " << name;
  }

  if (!skip_constructor) {
    if (decl->constructor() != nullptr) {
      out_ << " = ";
      if (!EmitExpression(decl->constructor())) {
        return false;
      }
    } else if (var->StorageClass() == ast::StorageClass::kPrivate ||
               var->StorageClass() == ast::StorageClass::kFunction ||
               var->StorageClass() == ast::StorageClass::kNone) {
      out_ << " = ";
      if (!EmitZeroValue(type)) {
        return false;
      }
    }
  }
  out_ << ";" << std::endl;

  return true;
}

bool GeneratorImpl::EmitProgramConstVariable(const ast::Variable* var) {
  make_indent();

  for (auto* d : var->decorations()) {
    if (!d->Is<ast::OverrideDecoration>()) {
      diagnostics_.add_error("Decorated const values not valid");
      return false;
    }
  }
  if (!var->is_const()) {
    diagnostics_.add_error("Expected a const value");
    return false;
  }

  out_ << "constant ";
  auto* type = program_->Sem().Get(var)->Type()->UnwrapRef();
  if (!EmitType(type, program_->Symbols().NameFor(var->symbol()))) {
    return false;
  }
  if (!type->Is<sem::Array>()) {
    out_ << " " << program_->Symbols().NameFor(var->symbol());
  }

  auto* sem_var = program_->Sem().Get(var);
  if (sem_var->IsPipelineConstant()) {
    out_ << " [[function_constant(" << sem_var->ConstantId() << ")]]";
  } else if (var->constructor() != nullptr) {
    out_ << " = ";
    if (!EmitExpression(var->constructor())) {
      return false;
    }
  }
  out_ << ";" << std::endl;

  return true;
}

// TODO(crbug.com/tint/898): We need CTS and / or Dawn e2e tests for this logic.
GeneratorImpl::SizeAndAlign GeneratorImpl::MslPackedTypeSizeAndAlign(
    const sem::Type* ty) {
  if (ty->IsAnyOf<sem::U32, sem::I32, sem::F32>()) {
    // https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf
    // 2.1 Scalar Data Types
    return {4, 4};
  }

  if (auto* vec = ty->As<sem::Vector>()) {
    // https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf
    // 2.2.3 Packed Vector Types
    auto num_els = vec->size();
    auto* el_ty = vec->type();
    if (el_ty->IsAnyOf<sem::U32, sem::I32, sem::F32>()) {
      return SizeAndAlign{num_els * 4, 4};
    }
  }

  if (auto* mat = ty->As<sem::Matrix>()) {
    // https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf
    // 2.3 Matrix Data Types
    auto cols = mat->columns();
    auto rows = mat->rows();
    auto* el_ty = mat->type();
    if (el_ty->IsAnyOf<sem::U32, sem::I32, sem::F32>()) {
      static constexpr SizeAndAlign table[] = {
          /* float2x2 */ {16, 8},
          /* float2x3 */ {32, 16},
          /* float2x4 */ {32, 16},
          /* float3x2 */ {24, 8},
          /* float3x3 */ {48, 16},
          /* float3x4 */ {48, 16},
          /* float4x2 */ {32, 8},
          /* float4x3 */ {64, 16},
          /* float4x4 */ {64, 16},
      };
      if (cols >= 2 && cols <= 4 && rows >= 2 && rows <= 4) {
        return table[(3 * (cols - 2)) + (rows - 2)];
      }
    }
  }

  if (auto* arr = ty->As<sem::Array>()) {
    auto el_size_align = MslPackedTypeSizeAndAlign(arr->ElemType());
    if (!arr->IsStrideImplicit()) {
      TINT_ICE(diagnostics_) << "arrays with explicit strides should have "
                                "removed with the PadArrayElements transform";
      return {};
    }
    auto num_els = std::max<uint32_t>(arr->Count(), 1);
    return SizeAndAlign{el_size_align.size * num_els, el_size_align.align};
  }

  if (auto* str = ty->As<sem::Struct>()) {
    // TODO(crbug.com/tint/650): There's an assumption here that MSL's default
    // structure size and alignment matches WGSL's. We need to confirm this.
    return SizeAndAlign{str->Size(), str->Align()};
  }

  TINT_UNREACHABLE(diagnostics_) << "Unhandled type " << ty->TypeInfo().name;
  return {};
}

}  // namespace msl
}  // namespace writer
}  // namespace tint
