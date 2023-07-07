//
// Created by user on 07.07.2023.
//

#ifndef PROTOSCLOUDSERVER_EXPORT_H
#define PROTOSCLOUDSERVER_EXPORT_H

#ifndef PROTOSCLOUDSERVER_API
#ifdef PROTOSCLOUDSERVER_DLL
#if defined _WIN32 || defined __CYGWIN__
            #define PROTOSCLOUDSERVER_HELPER_DLL_EXPORT __declspec(dllexport)
            #define PROTOSCLOUDSERVER_HELPER_DLL_IMPORT __declspec(dllimport)
        #else
            #if __GNUC__ >= 4
                #define PROTOSCLOUDSERVER_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
                #define PROTOSCLOUDSERVER_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
            #else
                #define PROTOSCLOUDSERVER_HELPER_DLL_EXPORT
                #define PROTOSCLOUDSERVER_HELPER_DLL_IMPORT
            #endif
        #endif
        #ifdef PROTOSCLOUDSERVER_EXPORTS
            #define PROTOSCLOUDSERVER_API TGBOT_HELPER_DLL_EXPORT
        #else
            #define PROTOSCLOUDSERVER_API TGBOT_HELPER_DLL_IMPORT
        #endif
#else
#define PROTOSCLOUDSERVER_API
#endif
#endif

#endif //PROTOSCLOUDSERVER_EXPORT_H
