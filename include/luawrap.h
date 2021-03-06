#ifndef LUAWRAP_H_INCLUDED
#define LUAWRAP_H_INCLUDED

#include "luawrap/luawrap_config.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <type_traits>
#include <functional>

#include "luawrap/luaobject.h"
#include "luawrap/luastacksaver.h"

namespace luawrap
{
    class Lua
    {
    public:
        typedef std::function<int(Lua&)>    func_t;

        LUAWRAP_API                 Lua(bool loadlibs = true);
        LUAWRAP_API                 ~Lua();
        LUAWRAP_API                 Lua(Lua&& rhs);
        LUAWRAP_API Lua&            operator = (Lua&& rhs);
                                    Lua(const Lua&) = delete;
                    Lua&            operator = (const Lua&) = delete;

        LUAWRAP_API void            loadFile(const std::string& filename, const char* debugname = nullptr);
        LUAWRAP_API void            loadStream(std::istream& strm, const char* debugname = nullptr);
        LUAWRAP_API void            loadFromString(const std::string& str, const char* debugname = nullptr);
        LUAWRAP_API void            loadFromString(const char* str, const char* debugname = nullptr);

        inline      bool            isStackTraceEnabled() const             { return stackTraceOn;              }
        inline      void            enableStackTrace(bool en)               { stackTraceOn = en;                }

        LUAWRAP_API const char*     pushString(const std::string& s);
        LUAWRAP_API std::string     toString(int idx, bool loopsafe = true);

        LUAWRAP_API operator        lua_State* ()   { return L;     }

        LUAWRAP_API void            pushFunction(const func_t& func);
        
        LUAWRAP_API int             callFunction(int args, int ret);

        template <typename UserType, typename... Args>
        UserType*           pushNewUserData(Args&&... args);

        LUAWRAP_API void            pushLightUserData(LuaObject* obj);

        template <typename UserType>
        UserType*           toUserData(int idx, const char* failMessage = nullptr);

        template <typename T>
        void                pushFunction(T* obj, int (T::*func)(Lua&));

    private:
        typedef const char* (*readerFunc_t)(lua_State*, void*, size_t*);
        lua_State*                  L;
        static int                  funcCallback(lua_State* L);
        bool                        stackTraceOn;

        LUAWRAP_API void            customLoad(readerFunc_t func, void* data, const char* chunkname);
        
        static int                  luaErrHandler(lua_State* L);
    };


    //////////////////////////////////////
    //  Inline functions
    
    template <typename UserType, typename... Args>
    UserType* Lua::pushNewUserData(Args&&... args)
    {
        static_assert(std::is_base_of<LuaObject, UserType>::value, "Lua::pushNewUserData must produce a type derived from LuaObject");

        void* buffer = lua_newuserdata(L, sizeof(UserType));
        if(!buffer)     throw std::bad_alloc();

        UserType* obj = new(buffer) UserType(std::forward<Args>(args)...);
        LuaObject* baseobj = static_cast<LuaObject*>(obj);
        baseobj->initialize(*this, true);
        return obj;
    }

    
    template <typename UserType>
    UserType* Lua::toUserData(int idx, const char* failMessage)
    {
        if(failMessage)
        {
            auto res = toUserData<UserType>(idx, nullptr);
            if(res)     return res;
            else        throw std::runtime_error(failMessage);
        }

        if(lua_type(L, idx) != LUA_TUSERDATA)
            return nullptr;
        LuaObject* baseptr = reinterpret_cast<LuaObject*>(lua_touserdata(L, idx));
        if(!baseptr)
            return nullptr;

        return dynamic_cast<UserType*>(baseptr);
    }
    
    template <typename T>
    void Lua::pushFunction(T* obj, int (T::*func)(Lua&))
    {
        pushFunction( std::bind(func, obj, std::placeholders::_1) );
    }
}

#endif