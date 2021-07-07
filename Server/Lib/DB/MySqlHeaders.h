
#pragma once

#include "SFTypedefs.h"

#if !ANDROID
#pragma warning(push)

#pragma warning(disable: 4702) // unreachable code

#include <mysqlx/xdevapi.h>

#pragma warning(pop)
#endif
