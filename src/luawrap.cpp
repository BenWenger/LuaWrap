
#include "luawrap.h"
#include <stdexcept>

namespace luawrap
{
    Lua::Lua()
    {
        L = luaL_newstate();
        if(!L)
            throw std::bad_alloc();
    }

    Lua::Lua(Lua&& rhs)
    {
        L = rhs.L;
        rhs.L = nullptr;
    }

    Lua& Lua::operator = (Lua&& rhs)
    {
        if(&rhs != this)
        {
            if(L)       lua_close(L);
            L = rhs.L;
            rhs.L = nullptr;
        }
        return *this;
    }

    Lua::~Lua()
    {
        if(L)
            lua_close(L);
        L = nullptr;
    }

    //////////////////////////////////////////////////

    const char* Lua::pushString(const std::string& s)
    {
        return lua_pushlstring(L, s.data(), s.size());
    }

    std::string Lua::toString(int idx, bool loopsafe)
    {
        // TODO - preserve stack
        if(loopsafe)
        {
            lua_pushvalue(L, idx);
            idx = -1;
        }

        size_t len;
        const char* s = lua_tolstring(L, idx, &len);
        return std::string(s, len);
    }
}