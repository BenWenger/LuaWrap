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
        virtual void                addMetaMethods(Lua& lua)        {};
        virtual int                 indexHandler(Lua& lua)          { return 0;     }


    private:
        friend class Lua;
        LUAWRAP_API void            initialize(Lua& lua, bool needGC);
        static int                  gcHandler(lua_State* L);
        static int                  rawIndexHandler(lua_State* L);
        Lua*                        hostLuaObj;
    };
}

#endif