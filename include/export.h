//
// Created by user on 07.07.2023.
//

#ifndef PCSERVER_EXPORT_H
#define PCSERVER_EXPORT_H

#ifndef PCS_API
    #ifdef PCSERVER_DLL
        #if defined _WIN32 || defined __CYGWIN__
            #define PCSERVER_HELPER_DLL_EXPORT __declspec(dllexport)
            #define PCSERVER_HELPER_DLL_IMPORT __declspec(dllimport)
        #else
            #if __GNUC__ >= 4
                #define PCSERVER_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
                #define PCSERVER_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
            #else
                #define PCSERVER_HELPER_DLL_EXPORT
                #define PCSERVER_HELPER_DLL_IMPORT
            #endif
        #endif
        #ifdef PCSERVER_EXPORTS
            #define PCS_API PCSERVER_HELPER_DLL_EXPORT
        #else
            #define PCS_API PCSERVER_HELPER_DLL_IMPORT
        #endif
    #else
        #define PCS_API
    #endif
#endif

#endif //PCSERVER_EXPORT_H
