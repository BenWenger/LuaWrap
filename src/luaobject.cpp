
#include "luawrap.h"
#include "luawrap/luaobject.h"

namespace luawrap
{
    LuaObject::~LuaObject()
    {
    }

    void LuaObject::buildMetatable(Lua& lua)
    {
        LuaStackSaver stk(lua);
                                                        // Bottom  ---->    Top
                                                        // obj                   <-  upon entry
        lua_createtable(lua, 0, 1);                     // obj, table
        lua_pushliteral(lua, "__gc");                   // obj, table, "__gc"
        lua_pushcfunction(lua, &LuaObject::gcHandler);  // obj, table, "__gc", func
        lua_settable(lua, -3);                          // obj, table

        auto top = lua_gettop(lua);
        addMetaMethods(lua);
        if(top != lua_gettop(lua))
            throw std::runtime_error("Lua stack size disrupted by call to LuaObject::addMetaMethods");

        lua_setmetatable(lua, -2);                      // obj                  <- upon exit
    }

    int LuaObject::gcHandler(lua_State* L)
    {
        if(lua_type(L, -1) != LUA_TUSERDATA)
            luaL_error(L, "Unknown error - LuaObject gcHandler function called with a value that is not a userdata object");
        LuaObject* ptr = reinterpret_cast<LuaObject*>(lua_touserdata(L, -1));
        ptr->~LuaObject();
        return 0;
    }
}
