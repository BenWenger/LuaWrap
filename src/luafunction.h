#ifndef LUAFUNCTION_H_INCLUDED
#define LUAFUNCTION_H_INCLUDED

#include "luawrap.h"
#include "luawrap/luaobject.h"

namespace luawrap
{
    class LuaFunction : public LuaObject
    {
    public:
        typedef Lua::func_t         func_t;
        LuaFunction(const func_t& v)
            : func(v)
        {}

        int call(Lua& lua)
        {
            return func(lua);
        }

    private:
        func_t      func;
    };
}

#endif