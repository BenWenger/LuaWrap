
#include <iostream>
#include "luawrap.h"

using namespace luawrap;
using namespace std;

int main()
{
    Lua lua;

    cout << "whaaaaat" << endl;
    cout << (lua_State*)lua << endl;

    char c;
    cin >> c;

    return 0;
}
