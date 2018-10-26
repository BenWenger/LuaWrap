
#include "luawrap.h"
#include "luafunction.h"
#include <stdexcept>
#include <dshfs.h>
#include <sstream>

namespace luawrap
{
    Lua::Lua(bool loadlibs)
    {
        L = luaL_newstate();
        if(!L)
            throw std::bad_alloc();

        if(loadlibs)
            luaL_openlibs(L);
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
    
    void Lua::loadFile(const std::string& filename)
    {
        dshfs::FileStream stream(filename);
        loadFromStream(stream);
    }

    void Lua::loadFromStream(std::istream& stream)
    {
        std::stringstream sstr;
        sstr << stream.rdbuf();
        loadFromString(sstr.str().c_str());
    }

    void Lua::loadFromString(const char* str)
    {
        auto res = luaL_loadstring(L, str);
        // TODO handle errors here
    }

    //////////////////////////////////////////////////

    const char* Lua::pushString(const std::string& s)
    {
        return lua_pushlstring(L, s.data(), s.size());
    }

    std::string Lua::toString(int idx, bool loopsafe)
    {
        LuaStackSaver stk(*this);

        if(loopsafe)
        {
            lua_pushvalue(L, idx);
            idx = -1;
        }

        size_t len;
        const char* s = lua_tolstring(L, idx, &len);
        return std::string(s, len);
    }

    /////////////////////////////////////////
    void Lua::pushFunction(const func_t& func)
    {
        lua_pushlightuserdata(L, this);
        pushNewUserData<LuaFunction>(func);
        lua_pushcclosure(L, &Lua::funcCallback, 2);
    }

    int Lua::funcCallback(lua_State* L)
    {
        try
        {
            if(lua_type(L, lua_upvalueindex(1)) != LUA_TLIGHTUSERDATA)
                throw std::runtime_error("Unknown error in Lua::funcCallback:  1st upvalue was not light user data");
            Lua* obj = reinterpret_cast<Lua*>(lua_touserdata(L, lua_upvalueindex(1)));
            if(!obj || obj->L != L)
                throw std::runtime_error("Unknown error in Lua::funcCallback:  1st upvalue was not the right Lua object");

            auto func = obj->toUserData<LuaFunction>(lua_upvalueindex(2), "Unknown Error in Lua::funcCallback:  2nd upvalue was not a LuaFunction object");
            return func->call(*obj);
        }
        catch(std::exception& e)
        {
            luaL_error(L, e.what());
        }
        return 0;       // shouldn't reach here
    }

    
    /////////////////////////////////////////
    int Lua::callFunction(int args, int rets)
    {
        return 0;
    }
}