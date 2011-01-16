#ifndef __STACKENING_PROTO_SCOPE_H__
#define __STACKENING_PROTO_SCOPE_H__

#include <string>
#include <vector>
#include <list>

#include "node-base.h"
#include "flow-managers.h"
#include "../misc/pos-type.h"

namespace proto {

    struct symbol_table;
    struct func_templ;

    struct scope {
        explicit scope(util::sref<symbol_table> s)
            : _flow_mgr(new flow_mgr(_stmts))
            , _symbols(s)
        {}
    public:
        util::sptr<expr_base const> make_bool(misc::pos_type const& pos, bool value) const;
        util::sptr<expr_base const> make_int(misc::pos_type const& pos, std::string const& value) const;
        util::sptr<expr_base const> make_float(misc::pos_type const& pos, std::string const& value) const;
        util::sptr<expr_base const> make_ref(misc::pos_type const& pos, std::string const& var_name) const;
        util::sptr<expr_base const> make_call(misc::pos_type const& pos
                                            , std::string const& func_name
                                            , std::vector<util::sptr<expr_base const>> args) const;
        util::sptr<expr_base const> make_binary(misc::pos_type const& pos
                                              , util::sptr<expr_base const> lhs
                                              , std::string const& op
                                              , util::sptr<expr_base const> rhs) const;
        util::sptr<expr_base const> make_pre_unary(misc::pos_type const& pos
                                                 , std::string const& op
                                                 , util::sptr<expr_base const> rhs) const;
        util::sptr<expr_base const> make_conj(misc::pos_type const& pos
                                            , util::sptr<expr_base const> lhs
                                            , util::sptr<expr_base const> rhs) const;
        util::sptr<expr_base const> make_disj(misc::pos_type const& pos
                                            , util::sptr<expr_base const> lhs
                                            , util::sptr<expr_base const> rhs) const;
        util::sptr<expr_base const> make_nega(misc::pos_type const& pos, util::sptr<expr_base const> rhs) const;
    public:
        virtual void add_func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val);
        virtual void add_func_ret_nothing(misc::pos_type const& pos);
        virtual void add_arith(misc::pos_type const& pos, util::sptr<expr_base const> expr);

        virtual void add_branch(misc::pos_type const& pos
                              , util::sptr<expr_base const> condition
                              , util::sptr<stmt_base const> valid
                              , util::sptr<stmt_base const> invalid);
        virtual void add_loop(misc::pos_type const& pos
                            , util::sptr<expr_base const> condition
                            , util::sptr<stmt_base const> body);

        virtual void def_var(misc::pos_type const& pos
                           , std::string const& name
                           , util::sptr<expr_base const> init);
    public:
        util::sptr<scope> create_branch_scope();
        util::sptr<scope> create_loop_scope();
    public:
        util::sref<func_templ> decl_func(misc::pos_type const& pos
                                       , std::string const& name
                                       , std::vector<std::string> const& param_names);
    public:
        util::sptr<stmt_base const> extract_stmts();
    public:
        util::sref<symbol_table> get_symbols() const;
    public:
        static util::sptr<scope> global_scope();
    protected:
        std::list<util::sptr<stmt_base const>> _stmts;
        util::sptr<flow_mgr_base> _flow_mgr;
        util::sref<symbol_table> const _symbols;
    };

    struct sub_scope
        : public scope
    {
        explicit sub_scope(util::sref<symbol_table> s)
            : scope(s)
        {}
    public:
        void def_var(misc::pos_type const& pos
                   , std::string const& name
                   , util::sptr<expr_base const> init);
    };

}

#endif /* __STACKENING_PROTO_SCOPE_H__ */
