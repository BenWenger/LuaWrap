
#include "luawrap.h"
#include "luafunction.h"
#include <stdexcept>
#include <dshfs.h>

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

    namespace
    {
        struct StringReadDat
        {
            const char*         buffer;
            size_t              size;
        };
        const char* reader_String(lua_State* L, void* data, size_t* size)
        {
            auto dat = reinterpret_cast<StringReadDat*>(data);
            *size = dat->size;
            return dat->buffer;
        }

        struct StreamReadDat
        {
            std::istream*       stream;
            char                buffer[1024];
        };
        const char* reader_Stream(lua_State* L, void* data, size_t* size)
        {
            auto dat = reinterpret_cast<StreamReadDat*>(data);
            dat->stream->read(dat->buffer, 1024);
            *size = static_cast<size_t>(dat->stream->gcount());
            return dat->buffer;
        }
    }

    void Lua::customLoad(readerFunc_t func, void* data, const char* chunkname)
    {
        auto res = lua_load(L, func, data, chunkname ? chunkname : "<unknown chunk>", nullptr);
        switch(res)
        {
        case LUA_OK:
            break;
        case LUA_ERRSYNTAX:
            throw std::runtime_error("Lua Syntax Error:  " + toString(-1,false));
        case LUA_ERRMEM:
            throw std::bad_alloc();
        default:
            throw std::runtime_error("Unknown Lua error occurred when trying to load");
        }
    }
    
    void Lua::loadFile(const std::string& filename, const char* debugname)
    {
        dshfs::FileStream stream(filename);
        loadStream(stream, debugname ? debugname : filename.c_str());
    }
    void Lua::loadStream(std::istream& strm, const char* debugname)
    {
        StreamReadDat dat;
        dat.stream = &strm;
        customLoad( &reader_Stream, &dat, debugname );
    }
    void Lua::loadFromString(const std::string& str, const char* debugname)
    {
        StringReadDat dat;
        dat.buffer = str.c_str();
        dat.size = str.size();
        customLoad( &reader_String, &dat, debugname );
    }
    void Lua::loadFromString(const char* str, const char* debugname)
    {
        StringReadDat dat;
        dat.buffer = str;
        dat.size = std::strlen(str);
        customLoad( &reader_String, &dat, debugname );
    }

    //////////////////////////////////////////////////

    const char* Lua::pushString(const std::string& s)
    {
        return lua_pushlstring(L, s.data(), s.size());
    }

    std::string Lua::toString(int idx, bool loopsafe)
    {
        LuaStackSaver stk(*this);

        if(loopsafe && (lua_type(L, idx) != LUA_TSTRING))
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
        if(rets < 0)        rets = LUA_MULTRET;

        auto top = lua_gettop(L) - args - 1;
        int err = lua_pcall( L, args, rets, 0 );        // TODO error handling and all that shiznit

        if(err != LUA_OK)
        {
            // TODO do something better with this
            throw std::runtime_error("Error in the Lua -- TODO I need to make a better message here");
        }
        return lua_gettop(L) - top;
    }
}