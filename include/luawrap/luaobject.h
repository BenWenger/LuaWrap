#ifndef LUAOBJECT_H_INCLUDED
#define LUAOBJECT_H_INCLUDED

#include <string>
#include <functional>
#include "luawrap_config.h"

namespace luawrap
{
    class Lua;

    class LuaObject
    {
    public:
        LUAWRAP_API virtual         ~LuaObject();

    protected:
        virtual void                addLuaMetaMethods(Lua& lua)     {}


    private:
        friend class Lua;
        LUAWRAP_API void            initialize(Lua& lua, bool needGC);
        static int                  gcHandler(lua_State* L);
        static int                  rawIndexHandler(lua_State* L);
        static int                  rawNewIndexHandler(lua_State* L);
        Lua*                        hostLuaObj;
    };
}

#endif