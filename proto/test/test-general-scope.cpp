#include <gtest/gtest.h>

#include "test-common.h"
#include "../scope.h"
#include "../symbol-table.h"
#include "../function.h"
#include "../stmt-nodes.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

using namespace test;

struct ScopeTest
    : public ProtoTest
{
    void SetUp()
    {
        ProtoTest::SetUp();
        resetScope();
    }

    void resetScope()
    {
        ext_symbols.reset(new proto::SymbolTable);
        scope.reset(new proto::GeneralScope(*ext_symbols));
        inst_scope.reset(new PhonyFunc);
    }

    util::sptr<proto::Scope> mkscope() const
    {
        return std::move(scope->createBranchScope());
    }

    util::sptr<proto::SymbolTable> ext_symbols;
    util::sptr<proto::GeneralScope> scope;
    util::sptr<inst::Scope> inst_scope;
};

TEST_F(ScopeTest, ExprNodesCreation)
{
    misc::position pos(1);

    scope->makeBool(pos, true)->inst(*inst_scope)->typeof();
    scope->makeBool(pos, false)->inst(*inst_scope)->typeof();
    scope->makeInt(pos, 0)->inst(*inst_scope)->typeof();
    scope->makeInt(pos, 2048)->inst(*inst_scope)->typeof();
    scope->makeFloat(pos, 21.36)->inst(*inst_scope)->typeof();
    scope->makeFloat(pos, 0.0)->inst(*inst_scope)->typeof();
    scope->makeRef(pos, "zero")->inst(*inst_scope)->typeof();
    scope->makeRef(pos, "one")->inst(*inst_scope)->typeof();
    scope->makeBinary(pos
                    , std::move(scope->makeInt(pos, 1))
                    , "+"
                    , std::move(scope->makeBool(pos, true)))
         ->inst(*inst_scope)
         ->typeof();
    scope->makeBinary(pos
                    , std::move(scope->makeInt(pos, 4))
                    , "<="
                    , std::move(scope->makeBool(pos, false)))
         ->inst(*inst_scope)
         ->typeof();
    scope->makePreUnary(pos, "-", std::move(scope->makeFloat(pos, 0.9)))
         ->inst(*inst_scope)
         ->typeof();
    scope->makePreUnary(pos, "+", std::move(scope->makeFloat(pos, 1.6)))
         ->inst(*inst_scope)
         ->typeof();
    scope->makeConj(pos, std::move(scope->makeInt(pos, 25)), std::move(scope->makeBool(pos, true)))
         ->inst(*inst_scope)
         ->typeof();
    scope->makeDisj(pos, std::move(scope->makeBool(pos, false)), std::move(scope->makeInt(pos, 36)))
         ->inst(*inst_scope)
         ->typeof();
    scope->makeNega(pos, std::move(scope->makeInt(pos, 49)))->inst(*inst_scope)->typeof();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (BOOLEAN, "true")
        (BOOLEAN, "false")
        (INTEGER, "0")
        (INTEGER, "2048")
        (FLOATING, "21.36")
        (FLOATING, "0")
        (pos, QUERY_VAR, "zero")
        (REFERENCE)
        (pos, QUERY_VAR, "one")
        (REFERENCE)
            (INTEGER, "1")
            (BOOLEAN, "true")
        (pos, QUERY_BINARY_OP, "+")
        (BINARY_OP)
            (INTEGER, "1")
            (BOOLEAN, "true")

            (INTEGER, "4")
            (BOOLEAN, "false")
        (pos, QUERY_BINARY_OP, "<=")
        (BINARY_OP)
            (INTEGER, "4")
            (BOOLEAN, "false")

            (FLOATING, "0.9")
        (pos, QUERY_PRE_UNARY_OP, "-")
        (PRE_UNARY_OP)
            (FLOATING, "0.9")

            (FLOATING, "1.6")
        (pos, QUERY_PRE_UNARY_OP, "+")
        (PRE_UNARY_OP)
            (FLOATING, "1.6")

        (CONJUNCTION)
            (INTEGER, "25")
            (BOOLEAN, "true")

        (DISJUNCTION)
            (BOOLEAN, "false")
            (INTEGER, "36")

        (NEGATION)
            (INTEGER, "49")
    ;
}

TEST_F(ScopeTest, Symbol)
{
    misc::position pos(2);
    scope->addStmt(std::move(util::mkptr(new proto::Arithmetics(
                                  pos
                                , std::move(scope->makeRef(pos, "four"))))));
    ASSERT_FALSE(error::hasError());
    scope->defVar(pos, "four");
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getInvalidRefs().size());
    ASSERT_EQ(pos, getInvalidRefs()[0].def_pos);
    ASSERT_EQ(1, getInvalidRefs()[0].ref_positions.size());
    ASSERT_EQ(pos, getInvalidRefs()[0].ref_positions[0]);
}
