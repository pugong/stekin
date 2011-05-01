#include <algorithm>
#include <list>

#include "function.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"
#include "inst-mediates.h"
#include "../instance/stmt-nodes.h"
#include "../instance/function.h"
#include "../util/map-compare.h"
#include "../report/errors.h"

using namespace proto;

util::sref<inst::Function> Function::inst(
                                        misc::position const& pos
                                      , util::sref<inst::Scope> ext_scope
                                      , std::vector<util::sref<inst::Type const>> const& arg_types)
{
    return inst(ext_scope->level(), bindExternalVars(pos, ext_scope), arg_types);
}

static util::sref<inst::Function> instanceAlreadyDoneOrNulIfNonexist(
                std::map<Function::InstanceInfo, util::sref<inst::Function>> const& instance_cache
              , std::map<std::string, inst::Variable const> const& ext_vars
              , std::vector<util::sref<inst::Type const>> const& arg_types
              , std::string const& name)
{
    auto find_result = instance_cache.find(Function::InstanceInfo(ext_vars, arg_types));
    if (instance_cache.end() != find_result) {
        util::sref<inst::Function> instance = find_result->second;
        while (!instance->isReturnTypeResolved() && instance->hasMorePath()) {
            instance->instNextPath();
        }
        if (!instance->isReturnTypeResolved()) {
            error::returnTypeUnresolvable(name, arg_types.size());
        }
        return instance;
    }
    return util::sref<inst::Function>(NULL);
}

std::list<inst::ArgNameTypeRec> makeArgInfo(
        std::vector<std::string> const& param_names
      , std::vector<util::sref<inst::Type const>> const& arg_types)
{
    std::list<inst::ArgNameTypeRec> args;
    for (unsigned i = 0; i < arg_types.size(); ++i) {
        args.push_back(inst::ArgNameTypeRec(param_names[i], arg_types[i]));
    }
    return args;
}

static util::sref<inst::Function> tryInst(util::sref<inst::Function> instance
                                        , std::list<util::sptr<Statement const>> const& stmts)
{
    BlockMediate body_mediate(stmts, instance);
    instance->instNextPath();
    instance->addStmt(std::move(body_mediate.inst(instance)));
    return instance;
}

util::sref<inst::Function> Function::inst(
                                        int level
                                      , std::map<std::string, inst::Variable const> const& ext_vars
                                      , std::vector<util::sref<inst::Type const>> const& arg_types)
{
    util::sref<inst::Function> result_inst = instanceAlreadyDoneOrNulIfNonexist(_instance_cache
                                                                              , ext_vars
                                                                              , arg_types
                                                                              , name);
    if (bool(result_inst)) {
        return result_inst;
    }

    util::sref<inst::Function> new_inst
                = inst::Function::createInstance(level
                                               , makeArgInfo(param_names, arg_types)
                                               , ext_vars
                                               , hint_void_return);
    _instance_cache.insert(std::make_pair(InstanceInfo(ext_vars, arg_types), new_inst));
    return tryInst(new_inst, _block.getStmts());
}

std::map<std::string, inst::Variable const> Function::bindExternalVars(
                                                  misc::position const& pos
                                                , util::sref<inst::Scope const> ext_scope) const
{
    std::map<std::string, inst::Variable const> result;
    std::for_each(_free_variables.begin()
                , _free_variables.end()
                , [&](std::string const& var_name)
                  {
                      result.insert(std::make_pair(var_name, ext_scope->queryVar(pos, var_name)));
                  });
    return result;
}

void Function::setFreeVariables(std::vector<std::string> const& free_vars)
{
    _free_variables = free_vars;
}

bool Function::InstanceInfo::operator<(Function::InstanceInfo const& rhs) const
{
    if (arg_types != rhs.arg_types) {
        return arg_types < rhs.arg_types;
    }
    return util::map_less(ext_vars, rhs.ext_vars);
}
