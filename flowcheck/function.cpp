#include <algorithm>

#include <proto/node-base.h>
#include <proto/function.h>
#include <proto/variable.h>

#include "function.h"
#include "node-base.h"
#include "filter.h"
#include "symbol-table.h"

using namespace flchk;

util::sref<proto::Function> Function::compile(util::sref<proto::Block> block)
{
    if (_func_proto_or_nul_if_not_compiled.nul()) {
        _func_proto_or_nul_if_not_compiled
                = block->declare(pos, name, param_names, _body->hintReturnVoid());
        std::for_each(param_names.begin()
                    , param_names.end()
                    , [&](std::string const& param)
                      {
                          _body->getSymbols()->defVar(pos, param);
                      });
        _body->compile(_func_proto_or_nul_if_not_compiled->block());
        _func_proto_or_nul_if_not_compiled->setFreeVariables(_body->getSymbols()->freeVariables());
    }
    return _func_proto_or_nul_if_not_compiled;
}

std::vector<std::string> Function::freeVariables() const
{
    return _body->getSymbols()->freeVariables();
}
