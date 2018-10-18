
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

int main()
{
    {
        Lua lua;
        lua.pushNewUserData<Temp>("Barfing");

        lua.pushFunction( std::bind(&callme, std::placeholders::_1, 5) );
        lua_setglobal(lua, "callme");

        luaL_loadstring(lua, "callme(\"This is a test\")");
        lua_call(lua, 0, 0);
    }

    char c;
    cin >> c;

    return 0;
}
