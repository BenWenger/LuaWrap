
#include "luawrap.h"
#include "luastacksaver.h"

namespace luawrap
{
    LuaStackSaver::LuaStackSaver(Lua& lua, int adj)
    {
        luaObj = &lua;
        stackSize = lua_gettop(lua) + adj;
    }

    LuaStackSaver::~LuaStackSaver()
    {
        if(luaObj)
            lua_settop(*luaObj, stackSize);
    }

    void LuaStackSaver::detach()
    {
        luaObj = nullptr;
    }
}
