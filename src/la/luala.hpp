#ifndef LUAAUDIO_H_
#define LUAAUDIO_H_

struct lua_State;

extern "C"
{
#include "export.h"

EXPORT int luaopen_la(struct lua_State* L);

}

#endif // LUAAUDIO_H_
