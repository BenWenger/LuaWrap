
#include <iostream>
#include "luawrap.h"

using namespace luawrap;
using namespace std;

class Temp : public LuaObject
{
public:
    Temp()
    {
        cout << "Object default constructed\n";
    }
    Temp(const char* str)
    {
        cout << "Object constructed with a string:  " << str << "\n";
    }
    virtual ~Temp()
    {
        std::cout << "Object destroyed\n";
    }
};

int callme(Lua& lua, int val)
{
    cout << "callme called with value:  " << val << '\n';
    cout << "and lua param:  " << lua.toString(1,false) << '\n';
    return 0;
}

class Butt
{
public:
    int someFunc(Lua& lua)
    {
        cout << "In someFunc" <<  endl;
        return 0;
    }
};

class SomeObj : public LuaObject
{
public:

protected:
    int indexHandler(Lua& lua) override
    {
        if(lua.toString(-1) == "test")
            lua.pushFunction(this, &SomeObj::test);
        else
            lua_pushnil(lua);
        return 1;
    }

private:
    int test(Lua& lua)
    {
        cout << "In SomeObj::test" << endl;
        return 0;
    }
};

int globalFunc(Lua& lua)
{
    //throw std::runtime_error("Throwing an error");
    return 0;
}

static const char* const errProgram = 
"afunc = function() globalFunc() end\n"
"bfunc = function() afunc() end\n"
"cfunc = function() globalObj.test() end\n"
;

int main()
{
    {
        Lua lua;
        lua.enableStackTrace(true);
        lua.pushFunction(&globalFunc);
        lua_setglobal(lua, "globalFunc");
        lua.pushNewUserData<SomeObj>();
        lua_setglobal(lua, "globalObj");
        luaL_loadstring(lua, errProgram);
        lua.callFunction(0,0);

        try
        {
            lua_getglobal(lua, "cfunc");
            lua.callFunction(0,0);
        }
        catch(std::exception& e)
        {
            std::cout << "Error received:  " << e.what() << std::endl;
        }
    }

    char c;
    cin >> c;

    return 0;
}
