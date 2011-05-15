#include <algorithm>
#include <vector>
#include <map>

#include "test-common.h"
#include "../../proto/stmt-nodes.h"
#include "../../proto/expr-nodes.h"
#include "../../proto/function.h"
#include "../../proto/built-in.h"
#include "../../instance/node-base.h"
#include "../../util/string.h"

using namespace test;
using namespace proto;

static std::list<util::sptr<Function>> func_entities;
static util::sptr<inst::Expression const> NUL_INST_EXPR(NULL);
static util::sptr<inst::Statement const> NUL_INST_STMT(NULL);
static util::sref<Type const> NUL_TYPE(NULL);

void Block::addTo(util::sref<FuncInstDraft>) {}
void Block::mediateInst(util::sref<FuncInstDraft>, util::sref<SymbolTable>) {}

void Block::addStmt(util::sptr<Statement> stmt)
{
    _stmts.push_back(std::move(stmt));
}

util::sptr<inst::Statement const> Block::inst(util::sref<FuncInstDraft>, util::sref<SymbolTable>)
{
    DataTree::actualOne()(SCOPE_BEGIN);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](util::sptr<Statement> const& stmt)
                  {
                      stmt->inst(nul_draft, nul_st);
                  });
    DataTree::actualOne()(SCOPE_END);
    return std::move(NUL_INST_STMT);
}

void DirectInst::addTo(util::sref<FuncInstDraft>) {}
void DirectInst::mediateInst(util::sref<FuncInstDraft>, util::sref<SymbolTable>) {}

util::sptr<inst::Statement const> DirectInst::inst(util::sref<FuncInstDraft>
                                                 , util::sref<SymbolTable>)
{
    return _inst(nul_draft, nul_st);
}

util::sptr<Statement> Scope::inst()
{
    return std::move(_block);
}

util::sptr<inst::Statement const> Return::_inst(util::sref<FuncInstDraft>
                                              , util::sref<SymbolTable>) const
{
    DataTree::actualOne()(pos, RETURN);
    ret_val->inst(nul_st);
    return std::move(NUL_INST_STMT);
}

util::sptr<inst::Statement const> ReturnNothing::_inst(util::sref<FuncInstDraft>
                                                     , util::sref<SymbolTable>) const
{
    DataTree::actualOne()(pos, RETURN_NOTHING);
    return std::move(NUL_INST_STMT);
}

util::sptr<inst::Statement const> VarDef::_inst(util::sref<FuncInstDraft>
                                              , util::sref<SymbolTable>) const
{
    DataTree::actualOne()(pos, VAR_DEF, name);
    init->inst(nul_st);
    return std::move(NUL_INST_STMT);
}

void Branch::addTo(util::sref<FuncInstDraft>) {}
void Branch::mediateInst(util::sref<FuncInstDraft>, util::sref<SymbolTable>) {}

util::sptr<inst::Statement const> Branch::inst(util::sref<FuncInstDraft>, util::sref<SymbolTable>)
{
    DataTree::actualOne()(pos, BRANCH);
    predicate->inst(nul_st);
    _consequence_stmt->inst(nul_draft, nul_st);
    _alternative_stmt->inst(nul_draft, nul_st);
    return std::move(NUL_INST_STMT);
}

util::sptr<inst::Statement const> Arithmetics::_inst(util::sref<FuncInstDraft>
                                                   , util::sref<SymbolTable>) const
{
    DataTree::actualOne()(pos, ARITHMETICS);
    expr->inst(nul_st);
    return std::move(NUL_INST_STMT);
}

util::sptr<inst::Expression const> BoolLiteral::inst(util::sref<SymbolTable const>) const
{
    DataTree::actualOne()(pos, BOOLEAN, util::str(value));
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> IntLiteral::inst(util::sref<SymbolTable const>) const
{
    DataTree::actualOne()(pos, INTEGER, util::str(value));
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> FloatLiteral::inst(util::sref<SymbolTable const>) const
{
    DataTree::actualOne()(pos, FLOATING, util::str(value));
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> Reference::inst(util::sref<SymbolTable const>) const
{
    DataTree::actualOne()(pos, REFERENCE, name);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> Call::inst(util::sref<SymbolTable const>) const
{
    DataTree::actualOne()(pos, CALL, _func->name, _args.size(), false);
    std::for_each(_args.begin()
                , _args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg->inst(nul_st);
                  });
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> FuncReference::inst(util::sref<SymbolTable const>) const
{
    DataTree::actualOne()(pos, FUNC_REFERENCE, _func->name, _func->param_names.size(), false);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> Functor::inst(util::sref<SymbolTable const>) const
{
    DataTree::actualOne()(pos, FUNCTOR, name, _args.size(), false);
    std::for_each(_args.begin()
                , _args.end()
                , [&](util::sptr<Expression const> const& arg)
                  {
                      arg->inst(nul_st);
                  });
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> BinaryOp::inst(util::sref<SymbolTable const>) const
{
    DataTree::actualOne()(pos, BINARY_OP, op);
    lhs->inst(nul_st);
    rhs->inst(nul_st);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> PreUnaryOp::inst(util::sref<SymbolTable const>) const
{
    DataTree::actualOne()(pos, PRE_UNARY_OP, op);
    rhs->inst(nul_st);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> Conjunction::inst(util::sref<SymbolTable const>) const
{
    DataTree::actualOne()(pos, BINARY_OP, "&&");
    lhs->inst(nul_st);
    rhs->inst(nul_st);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> Disjunction::inst(util::sref<SymbolTable const>) const
{
    DataTree::actualOne()(pos, BINARY_OP, "||");
    lhs->inst(nul_st);
    rhs->inst(nul_st);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> Negation::inst(util::sref<SymbolTable const>) const
{
    DataTree::actualOne()(pos, PRE_UNARY_OP, "!");
    rhs->inst(nul_st);
    return std::move(NUL_INST_EXPR);
}

util::sptr<inst::Expression const> WriteExpr::inst(util::sref<SymbolTable const>) const
{
    return std::move(NUL_INST_EXPR);
}

void Scope::addStmt(util::sptr<Statement> stmt)
{
    _block->addStmt(std::move(stmt));
}

util::sref<Function> Scope::declare(misc::position const& pos
                                  , std::string const& name
                                  , std::vector<std::string> const& param_names
                                  , bool hint_return_void)
{
    DataTree::actualOne()(pos, FUNC_DECL, name, param_names.size(), hint_return_void);
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param)
                  {
                      DataTree::actualOne()(pos, PARAMETER, param);
                  });
    func_entities.push_back(util::mkptr(new Function(pos, name, param_names, hint_return_void)));
    return *func_entities.back();
}

void Function::setFreeVariables(std::vector<std::string> const&) {}

util::sref<Type const> BoolLiteral::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

util::sref<Type const> IntLiteral::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

util::sref<Type const> FloatLiteral::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

util::sref<Type const> Reference::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

util::sref<Type const> Call::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

util::sref<Type const> Functor::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

util::sref<Type const> FuncReference::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

util::sref<Type const> BinaryOp::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

util::sref<Type const> PreUnaryOp::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

util::sref<Type const> Conjunction::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

util::sref<Type const> Disjunction::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

util::sref<Type const> Negation::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

util::sref<Type const> WriteExpr::type(util::sref<SymbolTable const>) const
{
    return NUL_TYPE;
}

void Type::checkCondType(misc::position const&) const {}

bool Type::eqAsBuiltIn(Type const&) const
{
    return false;
}

bool Type::eqAsFuncReference(util::sref<Function>
                           , std::map<std::string, Variable const> const&) const
{
    return false;
}

std::string FuncReferenceType::exportedName() const
{
    return "";
}

std::string FuncReferenceType::name() const
{
    return "";
}

util::sref<FuncInstDraft> FuncReferenceType::call(misc::position const&
                                                , int
                                                , int
                                                , std::vector<util::sref<Type const>> const&) const
{
    return nul_draft;
}

bool FuncReferenceType::operator==(Type const&) const
{
    return false;
}

bool FuncReferenceType::operator<(Type const&) const
{
    return false;
}

bool FuncReferenceType::eqAsFuncReference(util::sref<Function>
                                        , std::map<std::string, Variable const> const&) const
{
    return false;
}

bool FuncReferenceType::ltAsFuncReference(util::sref<Function>
                                        , std::map<std::string, Variable const> const&) const
{
    return false;
}

bool FuncReferenceType::ltAsBuiltIn(Type const&) const
{
    return false;
}
