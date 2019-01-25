
#include "luawrap.h"
#include "luawrap/luaobject.h"

namespace luawrap
{
    LuaObject::~LuaObject()
    {
    }

    void LuaObject::initialize(Lua& lua, bool needGC)
    {
        hostLuaObj = &lua;
        LuaStackSaver stk(lua);
                                                            // Bottom  ---->    Top
                                                            // obj                   <-  upon entry
        lua_createtable(lua, 0, 2);                         // obj, table
        if(needGC) {
            lua_pushliteral(lua, "__gc");                       // obj, table, "__gc"
            lua_pushcfunction(lua, &LuaObject::gcHandler);      // obj, table, "__gc", func
            lua_settable(lua, -3);                              // obj, table
        }
        lua_pushliteral(lua, "__index");                    // obj, table, "__index"
        lua_pushcfunction(lua, &LuaObject::rawIndexHandler);// obj, table, "__index", func
        lua_settable(lua, -3);                              // obj, table

        auto top = lua_gettop(lua);
        addMetaMethods(lua);                                // obj, table
        if(top != lua_gettop(lua))
            throw std::runtime_error("Lua stack size disrupted by call to LuaObject::addMetaMethods");

        lua_setmetatable(lua, -2);                          // obj                  <- upon exit
    }

    int LuaObject::gcHandler(lua_State* L)
    {
        if(lua_type(L, -1) != LUA_TUSERDATA)
            luaL_error(L, "Unknown error - LuaObject gcHandler function called with a value that is not a userdata object");
        LuaObject* ptr = reinterpret_cast<LuaObject*>(lua_touserdata(L, -1));
        ptr->~LuaObject();
        return 0;
    }

    int LuaObject::rawIndexHandler(lua_State* L)
    {
        if(!lua_isuserdata(L, -2))
            luaL_error(L, "Unknown error - LuaObject indexHandler function called with a value that is not a userdata object");

        LuaObject* ptr = reinterpret_cast<LuaObject*>(lua_touserdata(L, -2));

        return ptr->indexHandler(*ptr->hostLuaObj);
    }
}
