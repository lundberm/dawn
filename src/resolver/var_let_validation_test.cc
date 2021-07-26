// Copyright 2021 The Tint Authors.
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

#include "src/ast/struct_block_decoration.h"
#include "src/resolver/resolver.h"
#include "src/resolver/resolver_test_helper.h"

#include "gmock/gmock.h"

namespace tint {
namespace resolver {
namespace {

struct ResolverVarLetValidationTest : public resolver::TestHelper,
                                      public testing::Test {};

TEST_F(ResolverVarLetValidationTest, LetNoInitializer) {
  // let a : i32;
  WrapInFunction(Const(Source{{12, 34}}, "a", ty.i32(), nullptr));

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(r()->error(),
            "12:34 error: let declaration must have an initializer");
}

TEST_F(ResolverVarLetValidationTest, GlobalLetNoInitializer) {
  // let a : i32;
  GlobalConst(Source{{12, 34}}, "a", ty.i32(), nullptr);

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(r()->error(),
            "12:34 error: let declaration must have an initializer");
}

TEST_F(ResolverVarLetValidationTest, VarNoInitializerNoType) {
  // var a;
  WrapInFunction(Var(Source{{12, 34}}, "a", nullptr));

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(r()->error(),
            "12:34 error: function scope var declaration requires a type or "
            "initializer");
}

TEST_F(ResolverVarLetValidationTest, GlobalVarNoInitializerNoType) {
  // var a;
  Global(Source{{12, 34}}, "a", nullptr);

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(r()->error(),
            "12:34 error: module scope var declaration requires a type and "
            "initializer");
}

TEST_F(ResolverVarLetValidationTest, VarTypeNotStorable) {
  // var i : i32;
  // var p : pointer<function, i32> = &v;
  auto* i = Var("i", ty.i32(), ast::StorageClass::kNone);
  auto* p =
      Var(Source{{56, 78}}, "a", ty.pointer<i32>(ast::StorageClass::kFunction),
          ast::StorageClass::kNone, AddressOf(Source{{12, 34}}, "i"));
  WrapInFunction(i, p);

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(r()->error(),
            "56:78 error: ptr<function, i32, read_write> cannot be used as the "
            "type of a var");
}

TEST_F(ResolverVarLetValidationTest, LetTypeNotConstructible) {
  // [[group(0), binding(0)]] var t1 : texture_2d<f32>;
  // let t2 : t1;
  auto* t1 =
      Global("t1", ty.sampled_texture(ast::TextureDimension::k2d, ty.f32()),
             GroupAndBinding(0, 0));
  auto* t2 = Const(Source{{56, 78}}, "t2", nullptr, Expr(t1));
  WrapInFunction(t2);

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(r()->error(),
            "56:78 error: texture_2d<f32> cannot be used as the type of a let");
}

TEST_F(ResolverVarLetValidationTest, LetConstructorWrongType) {
  // var v : i32 = 2u
  WrapInFunction(Const(Source{{3, 3}}, "v", ty.i32(), Expr(2u)));

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(
      r()->error(),
      R"(3:3 error: cannot initialize let of type 'i32' with value of type 'u32')");
}

TEST_F(ResolverVarLetValidationTest, VarConstructorWrongType) {
  // var v : i32 = 2u
  WrapInFunction(
      Var(Source{{3, 3}}, "v", ty.i32(), ast::StorageClass::kNone, Expr(2u)));

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(
      r()->error(),
      R"(3:3 error: cannot initialize var of type 'i32' with value of type 'u32')");
}

TEST_F(ResolverVarLetValidationTest, LetConstructorWrongTypeViaAlias) {
  auto* a = Alias("I32", ty.i32());
  WrapInFunction(Const(Source{{3, 3}}, "v", ty.Of(a), Expr(2u)));

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(
      r()->error(),
      R"(3:3 error: cannot initialize let of type 'I32' with value of type 'u32')");
}

TEST_F(ResolverVarLetValidationTest, VarConstructorWrongTypeViaAlias) {
  auto* a = Alias("I32", ty.i32());
  WrapInFunction(
      Var(Source{{3, 3}}, "v", ty.Of(a), ast::StorageClass::kNone, Expr(2u)));

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(
      r()->error(),
      R"(3:3 error: cannot initialize var of type 'I32' with value of type 'u32')");
}

TEST_F(ResolverVarLetValidationTest, LetOfPtrConstructedWithRef) {
  // var a : f32;
  // let b : ptr<function,f32> = a;
  const auto priv = ast::StorageClass::kFunction;
  auto* var_a = Var("a", ty.f32(), priv);
  auto* var_b =
      Const(Source{{12, 34}}, "b", ty.pointer<float>(priv), Expr("a"), {});
  WrapInFunction(var_a, var_b);

  ASSERT_FALSE(r()->Resolve());

  EXPECT_EQ(
      r()->error(),
      R"(12:34 error: cannot initialize let of type 'ptr<function, f32>' with value of type 'f32')");
}

TEST_F(ResolverVarLetValidationTest, LocalVarRedeclared) {
  // var v : f32;
  // var v : i32;
  auto* v1 = Var("v", ty.f32(), ast::StorageClass::kNone);
  auto* v2 = Var(Source{{12, 34}}, "v", ty.i32(), ast::StorageClass::kNone);
  WrapInFunction(v1, v2);

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(
      r()->error(),
      "12:34 error: redefinition of 'v'\nnote: previous definition is here");
}

TEST_F(ResolverVarLetValidationTest, LocalLetRedeclared) {
  // let l : f32 = 1.;
  // let l : i32 = 0;
  auto* l1 = Const("l", ty.f32(), Expr(1.f));
  auto* l2 = Const(Source{{12, 34}}, "l", ty.i32(), Expr(0));
  WrapInFunction(l1, l2);

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(
      r()->error(),
      "12:34 error: redefinition of 'l'\nnote: previous definition is here");
}

TEST_F(ResolverVarLetValidationTest, GlobalVarRedeclared) {
  // var v : f32;
  // var v : i32;
  Global("v", ty.f32(), ast::StorageClass::kPrivate);
  Global(Source{{12, 34}}, "v", ty.i32(), ast::StorageClass::kPrivate);

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(
      r()->error(),
      "12:34 error: redefinition of 'v'\nnote: previous definition is here");
}

TEST_F(ResolverVarLetValidationTest, GlobalLetRedeclared) {
  // let l : f32 = 0.1;
  // let l : i32 = 0;
  GlobalConst("l", ty.f32(), Expr(0.1f));
  GlobalConst(Source{{12, 34}}, "l", ty.i32(), Expr(0));

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(
      r()->error(),
      "12:34 error: redefinition of 'l'\nnote: previous definition is here");
}

TEST_F(ResolverVarLetValidationTest, GlobalVarRedeclaredAsLocal) {
  // var v : f32 = 2.1;
  // fn my_func() {
  //   var v : f32 = 2.0;
  //   return 0;
  // }

  Global("v", ty.f32(), ast::StorageClass::kPrivate, Expr(2.1f));

  WrapInFunction(Var(Source{{12, 34}}, "v", ty.f32(), ast::StorageClass::kNone,
                     Expr(2.0f)));

  EXPECT_FALSE(r()->Resolve()) << r()->error();
  EXPECT_EQ(
      r()->error(),
      "12:34 error: redefinition of 'v'\nnote: previous definition is here");
}

TEST_F(ResolverVarLetValidationTest, VarRedeclaredInInnerBlock) {
  // {
  //  var v : f32;
  //  { var v : f32; }
  // }
  auto* var_outer = Var("v", ty.f32(), ast::StorageClass::kNone);
  auto* var_inner =
      Var(Source{{12, 34}}, "v", ty.f32(), ast::StorageClass::kNone);
  auto* inner = Block(Decl(var_inner));
  auto* outer_body = Block(Decl(var_outer), inner);

  WrapInFunction(outer_body);

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(
      r()->error(),
      "12:34 error: redefinition of 'v'\nnote: previous definition is here");
}

TEST_F(ResolverVarLetValidationTest, VarRedeclaredInIfBlock) {
  // {
  //   var v : f32 = 3.14;
  //   if (true) { var v : f32 = 2.0; }
  // }
  auto* var_a_float = Var("v", ty.f32(), ast::StorageClass::kNone, Expr(3.1f));

  auto* var = Var(Source{{12, 34}}, "v", ty.f32(), ast::StorageClass::kNone,
                  Expr(2.0f));

  auto* cond = Expr(true);
  auto* body = Block(Decl(var));

  auto* outer_body =
      Block(Decl(var_a_float),
            create<ast::IfStatement>(cond, body, ast::ElseStatementList{}));

  WrapInFunction(outer_body);

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(
      r()->error(),
      "12:34 error: redefinition of 'v'\nnote: previous definition is here");
}

TEST_F(ResolverVarLetValidationTest, InferredPtrStorageAccessMismatch) {
  // struct Inner {
  //    arr: array<i32, 4>;
  // }
  // [[block]] struct S {
  //    inner: Inner;
  // }
  // [[group(0), binding(0)]] var<storage> s : S;
  // fn f() {
  //   let p : pointer<storage, i32, read_write> = &s.inner.arr[2];
  // }
  auto* inner = Structure("Inner", {Member("arr", ty.array<i32, 4>())});
  auto* buf = Structure("S", {Member("inner", ty.Of(inner))},
                        {create<ast::StructBlockDecoration>()});
  auto* storage = Global("s", ty.Of(buf), ast::StorageClass::kStorage,
                         ast::DecorationList{
                             create<ast::BindingDecoration>(0),
                             create<ast::GroupDecoration>(0),
                         });

  auto* expr =
      IndexAccessor(MemberAccessor(MemberAccessor(storage, "inner"), "arr"), 4);
  auto* ptr = Const(
      Source{{12, 34}}, "p",
      ty.pointer<i32>(ast::StorageClass::kStorage, ast::Access::kReadWrite),
      AddressOf(expr));

  WrapInFunction(ptr);

  EXPECT_FALSE(r()->Resolve());
  EXPECT_EQ(r()->error(),
            "12:34 error: cannot initialize let of type "
            "'ptr<storage, i32, read_write>' with value of type "
            "'ptr<storage, i32, read>'");
}

}  // namespace
}  // namespace resolver
}  // namespace tint
