#ifndef __STEKIN_INSTANCE_BUILT_IN_H__
#define __STEKIN_INSTANCE_BUILT_IN_H__

#include "node-base.h"
#include "../util/pointer.h"

namespace inst {

    struct WriteExpr
        : public Expression
    {
        explicit WriteExpr(util::sptr<inst::Expression const> e)
            : expr(std::move(e))
        {}

        void write() const;

        util::sptr<Expression const> const expr;
    };

}

#endif /* __STEKIN_INSTANCE_BUILT_IN_H__ */
