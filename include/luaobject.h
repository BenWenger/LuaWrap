#ifndef LUAOBJECT_H_INCLUDED
#define LUAOBJECT_H_INCLUDED

#include "luawrap_config.h"

namespace luawrap
{
    class LuaObject
    {
    public:
        LUAWRAP_API virtual         ~LuaObject();
    };
}

#endif