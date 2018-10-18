#ifndef LUAWRAP_H_INCLUDED
#define LUAWRAP_H_INCLUDED

#include "luawrap_config.h"

#include <string>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

namespace luawrap
{
    class Lua
    {
    public:

        LUAWRAP_API                 Lua();
        LUAWRAP_API                 ~Lua();
        LUAWRAP_API                 Lua(Lua&& rhs);
        LUAWRAP_API Lua&            operator = (Lua&& rhs);
                                    Lua(const Lua&) = delete;
                    Lua&            operator = (const Lua&) = delete;
        LUAWRAP_API const char*     pushString(const std::string& s);
        LUAWRAP_API std::string     toString(int idx, bool loopsafe = true);

        LUAWRAP_API operator        lua_State* ()   { return L;     }

    private:
        lua_State*                  L;
    };
}

#endif