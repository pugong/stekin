#ifndef __STEKIN_INSTANCE_EXPRESSION_NODES_H__
#define __STEKIN_INSTANCE_EXPRESSION_NODES_H__

#include <vector>
#include <list>

#include <util/pointer.h>
#include <util/sn.h>
#include <misc/platform.h>
#include <misc/pos-type.h>

#include "node-base.h"
#include "address.h"
#include "types.h"

namespace inst {

    struct IntLiteral
        : public Expression
    {
        explicit IntLiteral(platform::int_type v)
            : value(v)
        {}

        void write() const;

        platform::int_type const value;
    };

    struct FloatLiteral
        : public Expression
    {
        explicit FloatLiteral(platform::float_type v)
            : value(v)
        {}

        void write() const;

        platform::float_type const value;
    };

    struct BoolLiteral
        : public Expression
    {
        explicit BoolLiteral(bool v)
            : value(v)
        {}

        void write() const;

        bool const value;
    };

    struct EmptyListLiteral
        : public Expression
    {
        void write() const;
    };

    struct ListLiteral
        : public Expression
    {
        ListLiteral(util::sptr<Type const> mt, std::vector<util::sptr<Expression const>> v)
            : member_type(std::move(mt))
            , value(std::move(v))
        {}

        void write() const;
        void writePipeDef(int level) const;

        util::sptr<Type const> const member_type;
        std::vector<util::sptr<Expression const>> const value;
    };

    struct ListElement
        : public Expression
    {
        void write() const;
    };

    struct ListIndex
        : public Expression
    {
        void write() const;
    };

    struct Reference
        : public Expression
    {
        Reference(util::sptr<Type const> t, Address const& a)
            : type(std::move(t))
            , address(a)
        {}

        void write() const;

        util::sptr<Type const> const type;
        Address address;
    };

    struct Call
        : public Expression
    {
        Call(util::serial_num c, std::vector<util::sptr<Expression const>> a)
            : call_sn(c)
            , args(std::move(a))
        {}

        void write() const;
        void writePipeDef(int level) const;

        util::serial_num const call_sn;
        std::vector<util::sptr<Expression const>> args;
    };

    struct MemberCall
        : public Expression
    {
        MemberCall(util::sptr<Expression const> o
                 , std::string const& n
                 , std::vector<util::sptr<Expression const>> a)
            : object(std::move(o))
            , name(n)
            , args(std::move(a))
        {}

        void write() const;
        void writePipeDef(int level) const;

        util::sptr<Expression const> object;
        std::string const name;
        std::vector<util::sptr<Expression const>> args;
    };

    struct FuncReference
        : public Expression
    {
        struct ArgInfo {
            Address const address;
            util::sptr<Type const> type;
            int const self_offset;

            ArgInfo(ArgInfo&& rhs)
                : address(rhs.address)
                , type(std::move(rhs.type))
                , self_offset(rhs.self_offset)
            {}

            ArgInfo(Address const& a, util::sptr<Type const> t, int soff)
                : address(a)
                , type(std::move(t))
                , self_offset(soff)
            {}

            ArgInfo(ArgInfo const&) = delete;
        };

        FuncReference(int s, std::list<ArgInfo> a)
            : size(s)
            , args(std::move(a))
        {}

        void write() const;

        int const size;
        std::list<ArgInfo> const args;
    };

    struct ListAppend
        : public Expression
    {
        ListAppend(util::sptr<Expression const> l, util::sptr<Expression const> r)
            : lhs(std::move(l))
            , rhs(std::move(r))
        {}

        void write() const;
        void writePipeDef(int level) const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct BinaryOp
        : public Expression
    {
        BinaryOp(util::sptr<Expression const> l
               , std::string const& o
               , util::sptr<Expression const> r)
            : lhs(std::move(l))
            , op(o)
            , rhs(std::move(r))
        {}

        void write() const;
        void writePipeDef(int level) const;

        util::sptr<Expression const> const lhs;
        std::string const op;
        util::sptr<Expression const> const rhs;
    };

    struct PreUnaryOp
        : public Expression
    {
        PreUnaryOp(std::string const& o, util::sptr<Expression const> r)
            : op(o)
            , rhs(std::move(r))
        {}

        void write() const;
        void writePipeDef(int level) const;

        std::string const op;
        util::sptr<Expression const> const rhs;
    };

    struct Conjunction
        : public Expression
    {
        Conjunction(util::sptr<Expression const> l, util::sptr<Expression const> r)
            : lhs(std::move(l))
            , rhs(std::move(r))
        {}

        void write() const;
        void writePipeDef(int level) const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct Disjunction
        : public Expression
    {
        Disjunction(util::sptr<Expression const> l, util::sptr<Expression const> r)
            : lhs(std::move(l))
            , rhs(std::move(r))
        {}

        void write() const;
        void writePipeDef(int level) const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct Negation
        : public Expression
    {
        explicit Negation(util::sptr<Expression const> r)
            : rhs(std::move(r))
        {}

        void write() const;
        void writePipeDef(int level) const;

        util::sptr<Expression const> const rhs;
    };

}

#endif /* __STEKIN_INSTANCE_EXPRESSION_NODES_H__ */
