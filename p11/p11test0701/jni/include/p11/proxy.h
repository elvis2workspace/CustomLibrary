#pragma once

#ifdef ___CRYPTOKI_H_INC___
#error MUST include proxy.h before include cryptoki.h, or use proxy.h instead of cryptoki.h
#else
#define _CLIENT_ //USE IN libPKCS11 for compatible on x64
#include "cryptoki.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef void* (*NotifyFunc)(unsigned int); //If SCS is crash, this function will be called to notify the proxy to reconnect

const char* StrErr(unsigned int errnum); //Get the error text of the error number
CK_RV Proxy_Init();
//Input the pointer of notify function to register
CK_RV Register_Exception_Notify_Callback(NotifyFunc notifyfunc); //The function should be called after Proxy_Init(), it's optional
CK_RV Proxy_Final();
CK_RV Monopolize_Enable();
CK_RV Monopolize_Disable();

#ifdef __cplusplus
};
#endif