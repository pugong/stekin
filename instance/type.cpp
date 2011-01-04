#include "type.h"

using namespace inst;

static type const BAD_TYPE_DEF("*BAD STACKENING TYPE*", 0);
static type const VOID("void", 0);
static type const BOOL("bool", 1);
static type const INT("int", 4);
static type const FLOAT("float", 8);

type const* const type::BAD_TYPE = &BAD_TYPE_DEF;
type const* const type::BIT_VOID = &VOID;
type const* const type::BIT_BOOL = &BOOL;
type const* const type::BIT_INT = &INT;
type const* const type::BIT_FLOAT = &FLOAT;
