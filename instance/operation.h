#ifndef __STACKENGING_INSTANCE_OPERATION_H__
#define __STACKENGING_INSTANCE_OPERATION_H__

#include <string>

#include "fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace inst {

    struct operation {
        util::sref<Type const> const ret_type;
        std::string const op_img;

        operation(util::sref<Type const> rt, std::string const& oi)
            : ret_type(rt)
            , op_img(oi)
        {}

        static operation const* queryBinary(misc::position const& pos
                                           , std::string const& op
                                           , util::sref<Type const> lhs
                                           , util::sref<Type const> rhs);
        static operation const* queryPreUnary(misc::position const& pos
                                              , std::string const& op
                                              , util::sref<Type const> rhs);
    };

}

#endif /* __STACKENGING_INSTANCE_OPERATION_H__ */
