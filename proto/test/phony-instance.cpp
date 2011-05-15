#include "test-common.h"
#include "../../instance/block.h"
#include "../../instance/expr-nodes.h"
#include "../../instance/stmt-nodes.h"
#include "../../instance/built-in.h"
#include "../../output/func-writer.h"
#include "../../output/name-mangler.h"
#include "../../util/string.h"

using namespace test;
using namespace inst;

void Block::addStmt(util::sptr<Statement const> stmt)
{
    _stmts.push_back(std::move(stmt));
}

void Block::write() const
{
    DataTree::actualOne()(BLOCK_BEGIN);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement const> const& stmt)
                  {
                      stmt->write();
                  });
    DataTree::actualOne()(BLOCK_END);
}

void Arithmetics::write() const
{
    DataTree::actualOne()(ARITHMETICS);
}

void Branch::write() const
{
    DataTree::actualOne()(BRANCH);
    predicate->write();
    consequence->write();
    alternative->write();
}

void Initialization::write() const
{
    DataTree::actualOne()(INITIALIZATION, util::str(offset));
    init->write();
}

void Return::write() const
{
    DataTree::actualOne()(RETURN);
    ret_val->write();
}

void ReturnNothing::write() const
{
    DataTree::actualOne()(RETURN_NOTHING);
}

void IntLiteral::write() const
{
    DataTree::actualOne()(INTEGER, util::str(value));
}

void FloatLiteral::write() const
{
    DataTree::actualOne()(FLOATING, util::str(value));
}

void BoolLiteral::write() const
{
    DataTree::actualOne()(BOOLEAN, util::str(value));
}

void Reference::write() const
{
    DataTree::actualOne()(REFERENCE);
}

void Call::write() const
{
    DataTree::actualOne()(CALL, util::str(int(args.size())));
}

void FuncReference::write() const
{
    DataTree::actualOne()(FUNC_REFERENCE);
}

void BinaryOp::write() const
{
    DataTree::actualOne()(BINARY_OP, op);
    lhs->write();
    rhs->write();
}

void PreUnaryOp::write() const
{
    DataTree::actualOne()(PRE_UNARY_OP, op);
    rhs->write();
}

void Conjunction::write() const
{
    DataTree::actualOne()(CONJUNCTION);
    lhs->write();
    rhs->write();
}

void Disjunction::write() const
{
    DataTree::actualOne()(DISJUNCTION);
    lhs->write();
    rhs->write();
}

void Negation::write() const
{
    DataTree::actualOne()(NEGATION);
    rhs->write();
}

void WriteExpr::write() const
{
    DataTree::actualOne()(WRITE);
}

void output::writeFuncDecl(std::string const&, util::id, std::list<StackVarRec> const&, int, int) {}
void output::writeFuncImpl(std::string const&, util::id) {}

std::string output::formType(std::string const& type)
{
    return type;
}

std::string output::formFuncReferenceType(int size)
{
    return util::str(size);
}
