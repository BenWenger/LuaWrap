#ifndef LUAOBJECT_H_INCLUDED
#define LUAOBJECT_H_INCLUDED

#include "luawrap_config.h"

namespace luawrap
{
    class Lua;

    class LuaObject
    {
    public:
        LUAWRAP_API virtual         ~LuaObject();
        virtual void                addMetaMethods(Lua& lua) {};

    private:
        friend class Lua;
        LUAWRAP_API void            buildMetatable(Lua& lua);
        static int                  gcHandler(lua_State* L);
    };
}

#endif