#ifndef __STEKIN_FLOWCHECK_FUNCTION_H__
#define __STEKIN_FLOWCHECK_FUNCTION_H__

#include <string>
#include <vector>

#include <util/pointer.h>
#include <misc/pos-type.h>

#include "block.h"

namespace flchk {

    struct Function {
        Function(misc::position const& ps
               , std::string const& func_name
               , std::vector<std::string> const& params
               , util::sptr<Filter> func_body)
            : pos(ps)
            , name(func_name)
            , param_names(params)
            , _body(std::move(func_body))
            , _func_proto_or_nul_if_not_compiled(nullptr)
        {}

        util::sref<proto::Function> compile(util::sref<proto::Block> block);
        std::vector<std::string> freeVariables() const;

        misc::position const pos;
        std::string const name;
        std::vector<std::string> const param_names;
    private:
        util::sptr<Filter> const _body;
        util::sref<proto::Function> _func_proto_or_nul_if_not_compiled;
    };

}

#endif /* __STEKIN_FLOWCHECK_FUNCTION_H__ */
