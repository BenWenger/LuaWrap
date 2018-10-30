
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

int main()
{
    /*
    {
        Lua lua;
        lua.pushNewUserData<Temp>("Barfing");

        lua.pushFunction( std::bind(&callme, std::placeholders::_1, 5) );
        lua_setglobal(lua, "callme");

        luaL_loadstring(lua, "callme(\"This is a test\")");
        lua_call(lua, 0, 0);
    }
    */
    {
        const char* const luascript = "run = function() return 1, 2, 3 end";

        Lua lua;
        luaL_loadstring(lua, luascript);
        lua_pcall(lua,0,0,0);

        lua_getglobal(lua, "run");
        int res = lua_pcall(lua, 0, LUA_MULTRET,0);

        int top = lua_gettop(lua);
        int t1 = lua_type(lua, 1);
        int t2 = lua_type(lua, 2);
        int t3 = lua_type(lua, 3);



        cout << "\n\n";
    }

    char c;
    cin >> c;

    return 0;
}
