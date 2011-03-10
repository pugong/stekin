#ifndef __STACKENING_PROTO_GENERAL_SCOPE_H__
#define __STACKENING_PROTO_GENERAL_SCOPE_H__

#include <string>
#include <vector>
#include <list>

#include "scope.h"
#include "symbol-table.h"

namespace proto {

    struct general_scope
        : public scope
    {
        explicit general_scope(util::sref<symbol_table const> ext_symbols)
            : _symbols(ext_symbols)
        {}

        general_scope(general_scope&& rhs)
            : scope(std::move(rhs))
            , _symbols(std::move(rhs._symbols))
        {}
    public:
        util::sptr<expr_base const> make_ref(misc::pos_type const& pos, std::string const& var_name);
        util::sptr<expr_base const> make_call(misc::pos_type const& pos
                                            , std::string const& func_name
                                            , std::vector<util::sptr<expr_base const>> args) const;

        void def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const> init);

        util::sref<function> decl_func(misc::pos_type const& pos
                                     , std::string const& name
                                     , std::vector<std::string> const& param_names);

        util::sptr<scope> create_branch_scope();
    protected:
        symbol_table _symbols;
    protected:
        general_scope() {}
    };

}

#endif /* __STACKENING_PROTO_GENERAL_SCOPE_H__ */