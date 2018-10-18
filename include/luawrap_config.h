
#ifndef LUAWRAP_CONFIG_H_INCLUDED
#define LUAWRAP_CONFIG_H_INCLUDED


#ifdef BUILD_LUAWRAP_DLL
    #define LUAWRAP_API     __declspec(dllexport)
#else
    #define LUAWRAP_API     __declspec(dllimport)
#endif

#ifdef _MSC_VER
    #ifdef _DEBUG
        #pragma comment(lib,"lua_d.lib")
        #ifndef BUILD_LUAWRAP_DLL
            #pragma comment(lib,"luawrap_d.lib")
        #endif
    #else
        #pragma comment(lib,"lua.lib")
        #ifndef BUILD_LUAWRAP_DLL
            #pragma comment(lib,"luawrap.lib")
        #endif
    #endif
#endif


#endif