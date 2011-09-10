#include <stdlib.h>
#include "luala.hpp"
#include "la.hpp"
#include "export.h"
extern "C"
{

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

float laL_ParseNumber(lua_State* L, int i, const char* field)
{
    float f;
    lua_getfield(L, i, field);
    f = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return f;
}

la_Event* laL_ParseEvent(lua_State* L, int i)
{
    if (lua_istable(L, i))
    {
        la_Event* event = (la_Event*)calloc(1, sizeof(la_Event));
        event->tone = laL_ParseNumber(L, i, "tone");
        event->beat = laL_ParseNumber(L, i, "beat");
        event->volume = laL_ParseNumber(L, i, "volume");
        event->wavetype = (la_WaveType)laL_ParseNumber(L, i, "wavetype");
        event->length = laL_ParseNumber(L, i, "length");
        return event;
    }
    return NULL;
}

static int laL_Play(lua_State* L)
{
    la_Play();
    return 0;
}

static int laL_Pause(lua_State* L)
{
    la_Pause();
    return 0;
}

static int laL_Stop(lua_State* L)
{
    la_Stop();
    return 0;
}

static int laL_AddEvent(lua_State* L)
{
    int top = lua_gettop(L);
    int i;
    for (i = 2; i <= top; i++)
    {
        la_Event* event = laL_ParseEvent(L, i);
        la_AddEvent(event);
    }
    return 0;
}

static int laL_GetBeat(lua_State* L)
{
    lua_pushnumber(L, la_GetBeat());
    return 1;
}

static int laL_SetTempo(lua_State* L)
{
    la_SetTempo(lua_tonumber(L, 1));
    return 0;
}

static luaL_reg laL_table[] =
{
    { "AddEvent", laL_AddEvent },
    { "Play", laL_Play },
    { "Stop", laL_Stop },
    { "Pause", laL_Pause },
    { "GetBeat", laL_GetBeat },
    { "SetTempo", laL_SetTempo },
    { NULL, NULL }
};

EXPORT int luaopen_la(lua_State* L)
{
    luaL_register(L, "la", laL_table);
    la_Init();
    return 0;
}

}
