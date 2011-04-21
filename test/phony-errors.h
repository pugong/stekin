#ifndef __STEKIN_TEST_PHONY_ERRORS_H__
#define __STEKIN_TEST_PHONY_ERRORS_H__

#include <vector>

#include "../report/errors.h"

namespace test {

    struct TabAsIndRec {
        explicit TabAsIndRec(misc::position const& ps)
            : pos(ps)
        {}

        misc::position const pos;
    };

    struct BadIndentRec {
        explicit BadIndentRec(misc::position const& ps)
            : pos(ps)
        {}

        misc::position const pos;
    };

    struct InvCharRec {
        InvCharRec(misc::position const& ps, int ch)
            : pos(ps)
            , character(ch)
        {}

        misc::position const pos;
        int const character;
    };

    struct ElseNotMatchRec {
        misc::position const pos;

        explicit ElseNotMatchRec(misc::position const& ps)
            : pos(ps)
        {}
    };

    struct IfMatchedRec {
        misc::position const prev_pos;
        misc::position const this_pos;

        IfMatchedRec(misc::position const& ppos, misc::position const& tpos)
            : prev_pos(ppos)
            , this_pos(tpos)
        {}
    };

    struct ExcessIndRec {
        misc::position const pos;

        explicit ExcessIndRec(misc::position const& ps)
            : pos(ps)
        {}
    };

    struct flow_terminated_rec {
        misc::position const this_pos;
        misc::position const prev_pos;

        flow_terminated_rec(misc::position const tpos, misc::position const& ppos)
            : this_pos(tpos)
            , prev_pos(ppos)
        {}
    };

    struct func_forbidden_rec {
        misc::position const pos;
        std::string const name;

        func_forbidden_rec(misc::position const& ps, std::string const& n)
            : pos(ps)
            , name(n)
        {}
    };

    struct var_redef_rec {
        misc::position const prev_pos;
        misc::position const this_pos;
        std::string const name;

        var_redef_rec(misc::position const& ppos, misc::position const& tpos, std::string const& n)
            : prev_pos(ppos)
            , this_pos(tpos)
            , name(n)
        {}
    };

    struct invalid_ref_rec {
        std::vector<misc::position> const ref_positions;
        misc::position const def_pos;
        std::string const name;

        template <typename _LineNoIterator>
        invalid_ref_rec(_LineNoIterator pos_begin
                      , _LineNoIterator pos_end
                      , misc::position def_ps
                      , std::string const& n)
            : ref_positions(pos_begin, pos_end)
            , def_pos(def_ps)
            , name(n)
        {}
    };

    struct func_ref_ambiguous_rec {
        misc::position const ref_pos;
        std::string const name;

        func_ref_ambiguous_rec(misc::position const& rpos, std::string const& n)
            : ref_pos(rpos)
            , name(n)
        {}
    };

    struct func_redef_rec {
        misc::position const prev_def_pos;
        misc::position const this_def_pos;
        std::string const name;
        int const param_count;

        func_redef_rec(misc::position prev_def_ps
                     , misc::position this_def_ps
                     , std::string const& n
                     , int pc)
            : prev_def_pos(prev_def_ps)
            , this_def_pos(this_def_ps)
            , name(n)
            , param_count(pc)
        {}
    };

    struct func_nondef_rec {
        misc::position const call_pos;
        std::string const name;
        int const param_count;

        func_nondef_rec(misc::position call_ps, std::string const& n, int pc)
            : call_pos(call_ps)
            , name(n)
            , param_count(pc)
        {}
    };

    struct ForbidDefRec {
        misc::position const pos;
        std::string const name;

        ForbidDefRec(misc::position ps, std::string const& n)
            : pos(ps)
            , name(n)
        {}
    };

    struct var_nondef_rec {
        misc::position const ref_pos;
        std::string const name;

        var_nondef_rec(misc::position const& rpos, std::string const& n)
            : ref_pos(rpos)
            , name(n)
        {}
    };

    struct NABinaryOpRec {
        misc::position const pos;
        std::string const op_img;
        std::string const lhst_name;
        std::string const rhst_name;

        NABinaryOpRec(misc::position const& ps
                       , std::string const& op
                       , std::string const& lhst
                       , std::string const& rhst)
            : pos(ps)
            , op_img(op)
            , lhst_name(lhst)
            , rhst_name(rhst)
        {}
    };

    struct NAPreUnaryOpRec {
        misc::position const pos;
        std::string const op_img;
        std::string const rhst_name;

        NAPreUnaryOpRec(misc::position const& ps, std::string const& op, std::string const& rhst)
            : pos(ps)
            , op_img(op)
            , rhst_name(rhst)
        {}
    };

    struct ret_type_conflict_rec {
        misc::position const this_pos;
        std::string const prev_type_name;
        std::string const this_type_name;

        ret_type_conflict_rec(misc::position const& tpos
                            , std::string const& prev_ret_type_name
                            , std::string const& this_ret_type_name)
            : this_pos(tpos)
            , prev_type_name(prev_ret_type_name)
            , this_type_name(this_ret_type_name)
        {}
    };

    struct RetTypeUnresolvableRec {
        std::string const name;
        int const arg_count;

        RetTypeUnresolvableRec(std::string const& n, int ac)
            : name(n)
            , arg_count(ac)
        {}
    };

    struct cond_not_bool_rec {
        misc::position const pos;
        std::string const type_name;

        cond_not_bool_rec(misc::position const& ps, std::string const& tname)
            : pos(ps)
            , type_name(tname)
        {}
    };

    struct variable_not_callable_rec {
        misc::position const call_pos;

        explicit variable_not_callable_rec(misc::position const& cp)
            : call_pos(cp)
        {}
    };

    void clear_err();

    std::vector<TabAsIndRec> getTabAsIndents();
    std::vector<BadIndentRec> getBadIndents();
    std::vector<InvCharRec> getInvCharRecs();
    std::vector<ElseNotMatchRec> getElseNotMatches();
    std::vector<IfMatchedRec> getIfMatchedRecs();
    std::vector<ExcessIndRec> get_excess_inds();
    std::vector<flow_terminated_rec> get_flow_terminated_recs();
    std::vector<func_forbidden_rec> get_forbidden_funcs();
    std::vector<ForbidDefRec> getForbidVarDefs();
    std::vector<var_redef_rec> get_local_redefs();
    std::vector<invalid_ref_rec> get_invalid_refs();
    std::vector<func_ref_ambiguous_rec> get_ambiguous_refs();
    std::vector<func_redef_rec> get_func_redefs();
    std::vector<func_nondef_rec> get_func_nondefs();
    std::vector<var_nondef_rec> get_nondefs();
    std::vector<NABinaryOpRec> getNABinaryOps();
    std::vector<NAPreUnaryOpRec> getNAPreUnaryOps();
    std::vector<ret_type_conflict_rec> get_ret_type_conflicts();
    std::vector<RetTypeUnresolvableRec> getRetTypeUnresolvables();
    std::vector<cond_not_bool_rec> get_cond_not_bools();
    std::vector<variable_not_callable_rec> get_variable_not_callables();

}

#endif /* __STEKIN_TEST_PHONY_ERRORS_H__ */
