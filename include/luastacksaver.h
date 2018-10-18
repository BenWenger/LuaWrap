#ifndef LUASTACKSAVER_H_INCLUDED
#define LUASTACKSAVER_H_INCLUDED

#include "luawrap_config.h"

namespace luawrap
{
    class Lua;
    
    class LuaStackSaver
    {
    public:
        LUAWRAP_API                 LuaStackSaver(Lua& lua, int adj = 0);
        LUAWRAP_API                 ~LuaStackSaver();
        LUAWRAP_API void            detach();

    private:
        Lua*                        luaObj;
        int                         stackSize;

    private:
                                    LuaStackSaver(const LuaStackSaver&) = delete;
                    LuaStackSaver&  operator = (const LuaStackSaver&) = delete;
                                    LuaStackSaver(LuaStackSaver&&) = delete;
                    LuaStackSaver&  operator = (LuaStackSaver&&) = delete;
    };
}

#endif