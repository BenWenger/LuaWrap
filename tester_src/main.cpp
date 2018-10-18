
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

int main()
{
    {
        Lua lua;
        lua.pushNewUserData<Temp>("This is a test");

        cout << "whaaaaat" << endl;
        cout << (lua_State*)lua << endl;
    }

    char c;
    cin >> c;

    return 0;
}
