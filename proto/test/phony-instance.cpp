#include "test-common.h"
#include "../../instance/function.h"
#include "../../instance/expr-nodes.h"
#include "../../util/string.h"

using namespace test;
using namespace inst;

type const* const type::BIT_VOID = &test::PROTO_TEST_VOID;
static phony_func func;
static bool test_func_inst_resolved = false;
static int path_count = 0;

void function::set_return_type(misc::pos_type const& pos, type const* t)
{
    test_func_inst_resolved = true;
    data_tree::actual_one()(pos, t == type::BIT_VOID ? SET_RETURN_TYPE_VOID : SET_RETURN_TYPE);
}

type const* function::get_return_type() const
{
    data_tree::actual_one()(GET_RETURN_TYPE);
    return &PROTO_TEST_TYPE;
}

bool function::is_return_type_resolved() const
{
    data_tree::actual_one()(QUERY_RETURN_TYPE_RESOLVE_STATUS);
    return test_func_inst_resolved ;
}

void function::add_path(util::sref<mediate_base>)
{
    data_tree::actual_one()(ADD_PATH);
    ++path_count;
}

void function::inst_next_path()
{
    data_tree::actual_one()(NEXT_PATH);
    --path_count;
}

bool function::has_more_path() const
{
    return path_count > 0;
}

variable function::def_var(misc::pos_type const& pos, type const* vtype, std::string const&)
{
    return variable(pos, vtype, 0, 0);
}

variable function::query_var(misc::pos_type const& pos, std::string const& name) const
{
    data_tree::actual_one()(pos, QUERY_VAR, name);
    return variable(misc::pos_type(0), &test::PROTO_TEST_TYPE, 0, 0);
}

operation const* scope::query_binary(misc::pos_type const& pos
                                   , std::string const& op
                                   , type const*
                                   , type const*) const
{
    data_tree::actual_one()(pos, QUERY_BINARY_OP, op);
    return NULL;
}

operation const* scope::query_pre_unary(misc::pos_type const& pos, std::string const& op, type const*) const
{
    data_tree::actual_one()(pos, QUERY_PRE_UNARY_OP, op);
    return NULL;
}

void scope::add_stmt(util::sptr<stmt_base const> stmt)
{
    data_tree::actual_one()(ADD_STMT_TO_SCOPE);
}

int function::level() const
{
    return 0;
}

symbol_table::symbol_table(
            int, std::list<arg_name_type_pair> const&, std::map<std::string, variable const> const&)
    : level(0)
    , _ss_used(0)
{}

util::sref<function> function::create_instance(int ext_level
                                             , std::list<arg_name_type_pair> const& args
                                             , std::map<std::string, variable const> const& extvars
                                             , bool has_void_returns)
{
    test_func_inst_resolved = false;
    path_count = 0;
    if (has_void_returns) {
        test_func_inst_resolved = true;
        data_tree::actual_one()(INIT_AS_VOID_RET);
    }
    return util::mkref(func);
}

void block::add_stmt(util::sptr<stmt_base const>)
{
    data_tree::actual_one()(ADD_STMT_TO_BLOCK);
}

type const* int_literal::typeof() const
{
    data_tree::actual_one()(INTEGER, util::str(value));
    return &PROTO_TEST_TYPE;
}

type const* float_literal::typeof() const
{
    data_tree::actual_one()(FLOATING, util::str(value));
    return &PROTO_TEST_TYPE;
}

type const* bool_literal::typeof() const
{
    data_tree::actual_one()(BOOLEAN, value ? "true" : "false");
    return &PROTO_TEST_TYPE;
}

type const* reference::typeof() const
{
    data_tree::actual_one()(REFERENCE);
    return &PROTO_TEST_TYPE;
}

type const* call::typeof() const
{
    data_tree::actual_one()(CALL, util::str(int(args.size())));
    return &PROTO_TEST_TYPE;
}

type const* binary_op::typeof() const
{
    data_tree::actual_one()(BINARY_OP);
    lhs->typeof();
    rhs->typeof();
    return &PROTO_TEST_TYPE;
}

type const* pre_unary_op::typeof() const
{
    data_tree::actual_one()(PRE_UNARY_OP);
    rhs->typeof();
    return &PROTO_TEST_TYPE;
}

type const* conjunction::typeof() const
{
    data_tree::actual_one()(CONJUNCTION);
    lhs->typeof();
    rhs->typeof();
    return &PROTO_TEST_TYPE;
}

type const* disjunction::typeof() const
{
    data_tree::actual_one()(DISJUNCTION);
    lhs->typeof();
    rhs->typeof();
    return &PROTO_TEST_TYPE;
}

type const* negation::typeof() const
{
    data_tree::actual_one()(NEGATION);
    rhs->typeof();
    return &PROTO_TEST_TYPE;
}