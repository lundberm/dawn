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

#include "src/ast/function.h"

#include "src/ast/builtin_decoration.h"
#include "src/ast/decorated_variable.h"
#include "src/ast/discard_statement.h"
#include "src/ast/location_decoration.h"
#include "src/ast/pipeline_stage.h"
#include "src/ast/test_helper.h"
#include "src/ast/type/f32_type.h"
#include "src/ast/type/i32_type.h"
#include "src/ast/type/void_type.h"
#include "src/ast/variable.h"
#include "src/ast/workgroup_decoration.h"

namespace tint {
namespace ast {
namespace {

using FunctionTest = TestHelper;

TEST_F(FunctionTest, Creation) {
  type::Void void_type;
  type::I32 i32;

  VariableList params;
  params.push_back(
      create<Variable>(Source{}, "var", StorageClass::kNone, &i32));
  auto* var = params[0];

  Function f(Source{}, "func", params, &void_type, create<BlockStatement>(),
             FunctionDecorationList{});
  EXPECT_EQ(f.name(), "func");
  ASSERT_EQ(f.params().size(), 1u);
  EXPECT_EQ(f.return_type(), &void_type);
  EXPECT_EQ(f.params()[0], var);
}

TEST_F(FunctionTest, Creation_WithSource) {
  type::Void void_type;
  type::I32 i32;

  VariableList params;
  params.push_back(
      create<Variable>(Source{}, "var", StorageClass::kNone, &i32));

  Function f(Source{Source::Location{20, 2}}, "func", params, &void_type,
             create<BlockStatement>(), FunctionDecorationList{});
  auto src = f.source();
  EXPECT_EQ(src.range.begin.line, 20u);
  EXPECT_EQ(src.range.begin.column, 2u);
}

TEST_F(FunctionTest, AddDuplicateReferencedVariables) {
  type::Void void_type;
  type::I32 i32;

  Variable v(Source{}, "var", StorageClass::kInput, &i32);
  Function f(Source{}, "func", VariableList{}, &void_type,
             create<BlockStatement>(), FunctionDecorationList{});

  f.add_referenced_module_variable(&v);
  ASSERT_EQ(f.referenced_module_variables().size(), 1u);
  EXPECT_EQ(f.referenced_module_variables()[0], &v);

  f.add_referenced_module_variable(&v);
  ASSERT_EQ(f.referenced_module_variables().size(), 1u);

  Variable v2(Source{}, "var2", StorageClass::kOutput, &i32);
  f.add_referenced_module_variable(&v2);
  ASSERT_EQ(f.referenced_module_variables().size(), 2u);
  EXPECT_EQ(f.referenced_module_variables()[1], &v2);
}

TEST_F(FunctionTest, GetReferenceLocations) {
  type::Void void_type;
  type::I32 i32;

  DecoratedVariable loc1(
      create<Variable>(Source{}, "loc1", StorageClass::kInput, &i32));
  loc1.set_decorations({create<LocationDecoration>(0, Source{})});

  DecoratedVariable loc2(
      create<Variable>(Source{}, "loc2", StorageClass::kInput, &i32));
  loc2.set_decorations({create<LocationDecoration>(1, Source{})});

  DecoratedVariable builtin1(
      create<Variable>(Source{}, "builtin1", StorageClass::kInput, &i32));
  builtin1.set_decorations(
      {create<BuiltinDecoration>(Builtin::kPosition, Source{})});

  DecoratedVariable builtin2(
      create<Variable>(Source{}, "builtin2", StorageClass::kInput, &i32));
  builtin2.set_decorations(
      {create<BuiltinDecoration>(Builtin::kFragDepth, Source{})});

  Function f(Source{}, "func", VariableList{}, &void_type,
             create<BlockStatement>(), FunctionDecorationList{});

  f.add_referenced_module_variable(&loc1);
  f.add_referenced_module_variable(&builtin1);
  f.add_referenced_module_variable(&loc2);
  f.add_referenced_module_variable(&builtin2);
  ASSERT_EQ(f.referenced_module_variables().size(), 4u);

  auto ref_locs = f.referenced_location_variables();
  ASSERT_EQ(ref_locs.size(), 2u);
  EXPECT_EQ(ref_locs[0].first, &loc1);
  EXPECT_EQ(ref_locs[0].second->value(), 0u);
  EXPECT_EQ(ref_locs[1].first, &loc2);
  EXPECT_EQ(ref_locs[1].second->value(), 1u);
}

TEST_F(FunctionTest, GetReferenceBuiltins) {
  type::Void void_type;
  type::I32 i32;

  DecoratedVariable loc1(
      create<Variable>(Source{}, "loc1", StorageClass::kInput, &i32));
  loc1.set_decorations({create<LocationDecoration>(0, Source{})});

  DecoratedVariable loc2(
      create<Variable>(Source{}, "loc2", StorageClass::kInput, &i32));
  loc2.set_decorations({create<LocationDecoration>(1, Source{})});

  DecoratedVariable builtin1(
      create<Variable>(Source{}, "builtin1", StorageClass::kInput, &i32));
  builtin1.set_decorations(
      {create<BuiltinDecoration>(Builtin::kPosition, Source{})});

  DecoratedVariable builtin2(
      create<Variable>(Source{}, "builtin2", StorageClass::kInput, &i32));
  builtin2.set_decorations(
      {create<BuiltinDecoration>(Builtin::kFragDepth, Source{})});

  Function f(Source{}, "func", VariableList{}, &void_type,
             create<BlockStatement>(), FunctionDecorationList{});

  f.add_referenced_module_variable(&loc1);
  f.add_referenced_module_variable(&builtin1);
  f.add_referenced_module_variable(&loc2);
  f.add_referenced_module_variable(&builtin2);
  ASSERT_EQ(f.referenced_module_variables().size(), 4u);

  auto ref_locs = f.referenced_builtin_variables();
  ASSERT_EQ(ref_locs.size(), 2u);
  EXPECT_EQ(ref_locs[0].first, &builtin1);
  EXPECT_EQ(ref_locs[0].second->value(), Builtin::kPosition);
  EXPECT_EQ(ref_locs[1].first, &builtin2);
  EXPECT_EQ(ref_locs[1].second->value(), Builtin::kFragDepth);
}

TEST_F(FunctionTest, AddDuplicateEntryPoints) {
  type::Void void_type;
  Function f(Source{}, "func", VariableList{}, &void_type,
             create<BlockStatement>(), FunctionDecorationList{});

  f.add_ancestor_entry_point("main");
  ASSERT_EQ(1u, f.ancestor_entry_points().size());
  EXPECT_EQ("main", f.ancestor_entry_points()[0]);

  f.add_ancestor_entry_point("main");
  ASSERT_EQ(1u, f.ancestor_entry_points().size());
  EXPECT_EQ("main", f.ancestor_entry_points()[0]);
}

TEST_F(FunctionTest, IsValid) {
  type::Void void_type;
  type::I32 i32;

  VariableList params;
  params.push_back(
      create<Variable>(Source{}, "var", StorageClass::kNone, &i32));

  auto* body = create<BlockStatement>();
  body->append(create<DiscardStatement>());

  Function f(Source{}, "func", params, &void_type, body,
             FunctionDecorationList{});
  EXPECT_TRUE(f.IsValid());
}

TEST_F(FunctionTest, IsValid_EmptyName) {
  type::Void void_type;
  type::I32 i32;

  VariableList params;
  params.push_back(
      create<Variable>(Source{}, "var", StorageClass::kNone, &i32));

  Function f(Source{}, "", params, &void_type, create<BlockStatement>(),
             FunctionDecorationList{});
  EXPECT_FALSE(f.IsValid());
}

TEST_F(FunctionTest, IsValid_MissingReturnType) {
  type::I32 i32;

  VariableList params;
  params.push_back(
      create<Variable>(Source{}, "var", StorageClass::kNone, &i32));

  Function f(Source{}, "func", params, nullptr, create<BlockStatement>(),
             FunctionDecorationList{});
  EXPECT_FALSE(f.IsValid());
}

TEST_F(FunctionTest, IsValid_NullParam) {
  type::Void void_type;
  type::I32 i32;

  VariableList params;
  params.push_back(
      create<Variable>(Source{}, "var", StorageClass::kNone, &i32));
  params.push_back(nullptr);

  Function f(Source{}, "func", params, &void_type, create<BlockStatement>(),
             FunctionDecorationList{});
  EXPECT_FALSE(f.IsValid());
}

TEST_F(FunctionTest, IsValid_InvalidParam) {
  type::Void void_type;

  VariableList params;
  params.push_back(
      create<Variable>(Source{}, "var", StorageClass::kNone, nullptr));

  Function f(Source{}, "func", params, &void_type, create<BlockStatement>(),
             FunctionDecorationList{});
  EXPECT_FALSE(f.IsValid());
}

TEST_F(FunctionTest, IsValid_NullBodyStatement) {
  type::Void void_type;
  type::I32 i32;

  VariableList params;
  params.push_back(
      create<Variable>(Source{}, "var", StorageClass::kNone, &i32));

  auto* body = create<BlockStatement>();
  body->append(create<DiscardStatement>());
  body->append(nullptr);

  Function f(Source{}, "func", params, &void_type, body,
             FunctionDecorationList{});

  EXPECT_FALSE(f.IsValid());
}

TEST_F(FunctionTest, IsValid_InvalidBodyStatement) {
  type::Void void_type;
  type::I32 i32;

  VariableList params;
  params.push_back(
      create<Variable>(Source{}, "var", StorageClass::kNone, &i32));

  auto* body = create<BlockStatement>();
  body->append(create<DiscardStatement>());
  body->append(nullptr);

  Function f(Source{}, "func", params, &void_type, body,
             FunctionDecorationList{});
  EXPECT_FALSE(f.IsValid());
}

TEST_F(FunctionTest, ToStr) {
  type::Void void_type;
  type::I32 i32;

  auto* body = create<BlockStatement>();
  body->append(create<DiscardStatement>());

  Function f(Source{}, "func", {}, &void_type, body, FunctionDecorationList{});

  std::ostringstream out;
  f.to_str(out, 2);
  EXPECT_EQ(out.str(), R"(  Function func -> __void
  ()
  {
    Discard{}
  }
)");
}

TEST_F(FunctionTest, ToStr_WithDecoration) {
  type::Void void_type;
  type::I32 i32;

  auto* body = create<BlockStatement>();
  body->append(create<DiscardStatement>());

  Function f(
      Source{}, "func", {}, &void_type, body,
      FunctionDecorationList{create<WorkgroupDecoration>(2, 4, 6, Source{})});

  std::ostringstream out;
  f.to_str(out, 2);
  EXPECT_EQ(out.str(), R"(  Function func -> __void
  WorkgroupDecoration{2 4 6}
  ()
  {
    Discard{}
  }
)");
}

TEST_F(FunctionTest, ToStr_WithParams) {
  type::Void void_type;
  type::I32 i32;

  VariableList params;
  params.push_back(
      create<Variable>(Source{}, "var", StorageClass::kNone, &i32));

  auto* body = create<BlockStatement>();
  body->append(create<DiscardStatement>());

  Function f(Source{}, "func", params, &void_type, body,
             FunctionDecorationList{});

  std::ostringstream out;
  f.to_str(out, 2);
  EXPECT_EQ(out.str(), R"(  Function func -> __void
  (
    Variable{
      var
      none
      __i32
    }
  )
  {
    Discard{}
  }
)");
}

TEST_F(FunctionTest, TypeName) {
  type::Void void_type;

  Function f(Source{}, "func", {}, &void_type, create<BlockStatement>(),
             FunctionDecorationList{});
  EXPECT_EQ(f.type_name(), "__func__void");
}

TEST_F(FunctionTest, TypeName_WithParams) {
  type::Void void_type;
  type::I32 i32;
  type::F32 f32;

  VariableList params;
  params.push_back(
      create<Variable>(Source{}, "var1", StorageClass::kNone, &i32));
  params.push_back(
      create<Variable>(Source{}, "var2", StorageClass::kNone, &f32));

  Function f(Source{}, "func", params, &void_type, create<BlockStatement>(),
             FunctionDecorationList{});
  EXPECT_EQ(f.type_name(), "__func__void__i32__f32");
}

TEST_F(FunctionTest, GetLastStatement) {
  type::Void void_type;

  VariableList params;
  auto* body = create<BlockStatement>();
  auto* stmt = create<DiscardStatement>();
  body->append(stmt);
  Function f(Source{}, "func", params, &void_type, body,
             FunctionDecorationList{});

  EXPECT_EQ(f.get_last_statement(), stmt);
}

TEST_F(FunctionTest, GetLastStatement_nullptr) {
  type::Void void_type;

  VariableList params;
  auto* body = create<BlockStatement>();
  Function f(Source{}, "func", params, &void_type, body,
             FunctionDecorationList{});

  EXPECT_EQ(f.get_last_statement(), nullptr);
}

TEST_F(FunctionTest, WorkgroupSize_NoneSet) {
  type::Void void_type;
  Function f(Source{}, "f", {}, &void_type, create<BlockStatement>(),
             FunctionDecorationList{});
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t z = 0;
  std::tie(x, y, z) = f.workgroup_size();
  EXPECT_EQ(x, 1u);
  EXPECT_EQ(y, 1u);
  EXPECT_EQ(z, 1u);
}

TEST_F(FunctionTest, WorkgroupSize) {
  type::Void void_type;
  Function f(Source{}, "f", {}, &void_type, create<BlockStatement>(),
             {create<WorkgroupDecoration>(2u, 4u, 6u, Source{})});

  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t z = 0;
  std::tie(x, y, z) = f.workgroup_size();
  EXPECT_EQ(x, 2u);
  EXPECT_EQ(y, 4u);
  EXPECT_EQ(z, 6u);
}

}  // namespace
}  // namespace ast
}  // namespace tint
