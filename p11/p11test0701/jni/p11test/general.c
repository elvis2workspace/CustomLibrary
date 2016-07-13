#include "Utils_c.h"
#include <assert.h>
#include "general.h"
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <dlfcn.h>
#endif
#include <errno.h>

typedef unsigned int (*F)();
//全局变量
char czAppName[32] = {'\0'};
int ilevel = 0;

CK_UTF8CHAR userpin[MAX_PIN_LEN+1]={0};
CK_UTF8CHAR adminpin[MAX_PIN_LEN+1]={0};

#ifndef _WIN32
#define HINSTANCE void* 
#endif
#ifndef _LINUX_DYNAMIC_LIBRARY
HINSTANCE     hDll=NULL_PTR;  //动态库句柄
HINSTANCE     hDll_self=NULL_PTR;
HINSTANCE	hDllOTP=NULL_PTR;
#endif //_LINUX_DYNAMIC_LIBRARY
#ifdef _WIN32
HINSTANCE	  hDllToken=NULL;//Win32下的Token动态库句柄

#endif


unsigned short CardVersionData = 0x0000; 
//当flag为1时判定版本为412，默认不是412

int nItemNumb = 0;
int nResultLength = 0;

char Testresult[100][10000] = { 0 };
int Testchoose[100] = { 0 };
int Test[100] = { 0 };

#ifdef __cplusplus
extern "C" {
#endif

CK_C_Proxy_Final            pC_Proxy_Final = NULL;
CK_C_Proxy_Init             pC_Proxy_Init = NULL;
CK_C_Monopolize_Enable      pC_Monopolize_Enable = NULL;
CK_C_Monopolize_Disable     pC_Monopolize_Disable = NULL;
CK_C_GetFunctionList        pC_GetFunctionList=NULL;
CK_C_Initialize             pC_Initialize=NULL;
CK_C_Finalize				pC_Finalize=NULL;
CK_C_GetInfo				pC_GetInfo=NULL;
CK_C_GetSlotList			pC_GetSlotList=NULL;
CK_C_GetSlotInfo			pC_GetSlotInfo=NULL;
CK_C_GetTokenInfo			pC_GetTokenInfo=NULL;
CK_C_GetMechanismList		pC_GetMechanismList=NULL;
CK_C_GetMechanismInfo		pC_GetMechanismInfo=NULL;
CK_C_InitToken				pC_InitToken=NULL;
CK_C_InitPIN				pC_InitPIN=NULL;
CK_C_SetPIN					pC_SetPIN=NULL;
CK_C_OpenSession			pC_OpenSession=NULL;
CK_C_CloseSession			pC_CloseSession=NULL;
CK_C_CloseAllSessions		pC_CloseAllSessions=NULL;
CK_C_GetSessionInfo			pC_GetSessionInfo=NULL;	
CK_C_GetOperationState		pC_GetOperationState=NULL;
CK_C_SetOperationState		pC_SetOperationState=NULL;
CK_C_Login					pC_Login=NULL;
CK_C_Logout					pC_Logout=NULL;
CK_C_CreateObject			pC_CreateObject=NULL;
CK_C_CopyObject				pC_CopyObject=NULL;
CK_C_DestroyObject			pC_DestroyObject=NULL;
CK_C_GetObjectSize			pC_GetObjectSize=NULL;
CK_C_GetAttributeValue		pC_GetAttributeValue=NULL;
CK_C_SetAttributeValue		pC_SetAttributeValue=NULL;
CK_C_FindObjectsInit		pC_FindObjectsInit=NULL;
CK_C_FindObjects			pC_FindObjects=NULL;
CK_C_FindObjectsFinal		pC_FindObjectsFinal=NULL;
CK_C_EncryptInit			pC_EncryptInit=NULL;
CK_C_Encrypt				pC_Encrypt=NULL;
CK_C_EncryptUpdate			pC_EncryptUpdate=NULL;
CK_C_EncryptFinal			pC_EncryptFinal=NULL;
CK_C_DecryptInit			pC_DecryptInit=NULL;
CK_C_Decrypt				pC_Decrypt=NULL;
CK_C_DecryptUpdate			pC_DecryptUpdate=NULL;
CK_C_DecryptFinal			pC_DecryptFinal=NULL;
CK_C_DigestInit				pC_DigestInit=NULL;
CK_C_Digest					pC_Digest=NULL;
CK_C_DigestUpdate			pC_DigestUpdate=NULL;
CK_C_DigestKey				pC_DigestKey=NULL;
CK_C_DigestFinal			pC_DigestFinal=NULL;
CK_C_SignInit				pC_SignInit=NULL;
CK_C_Sign					pC_Sign=NULL;
CK_C_SignUpdate				pC_SignUpdate=NULL;
CK_C_SignFinal				pC_SignFinal=NULL;
CK_C_SignRecoverInit		pC_SignRecoverInit=NULL;
CK_C_SignRecover			pC_SignRecover=NULL;
CK_C_VerifyInit				pC_VerifyInit=NULL;
CK_C_Verify					pC_Verify=NULL;
CK_C_VerifyUpdate			pC_VerifyUpdate=NULL;
CK_C_VerifyFinal			pC_VerifyFinal=NULL;
CK_C_VerifyRecoverInit		pC_VerifyRecoverInit=NULL;
CK_C_VerifyRecover			pC_VerifyRecover=NULL;
CK_C_DigestEncryptUpdate	pC_DigestEncryptUpdate=NULL;
CK_C_DecryptDigestUpdate	pC_DecryptDigestUpdate=NULL;
CK_C_SignEncryptUpdate		pC_SignEncryptUpdate=NULL;
CK_C_DecryptVerifyUpdate	pC_DecryptVerifyUpdate=NULL;
CK_C_GenerateKey			pC_GenerateKey=NULL;
CK_C_GenerateKeyPair		pC_GenerateKeyPair=NULL;
CK_C_WrapKey				pC_WrapKey=NULL;
CK_C_UnwrapKey				pC_UnwrapKey=NULL;
CK_C_DeriveKey				pC_DeriveKey=NULL;
CK_C_SeedRandom				pC_SeedRandom=NULL;
CK_C_GenerateRandom			pC_GenerateRandom=NULL;
CK_C_GetFunctionStatus		pC_GetFunctionStatus=NULL;
CK_C_CancelFunction			pC_CancelFunction=NULL;
CK_C_WaitForSlotEvent		pC_WaitForSlotEvent=NULL;
CK_C_CryptoExtend			pC_CryptoExtend = NULL;
CK_C_StrErr                 pC_StrErr = NULL;
CK_T_ReloadOTPPin_GenCipher	pC_ReloadOTPPin_GenCipher=NULL;

CK_SESSION_HANDLE hSession=NULL_PTR;
CK_SLOT_ID slotID=0;
CK_FUNCTION_LIST *  pFunctionList=NULL;

#ifdef __cplusplus
}
#endif

#define ISBIGENDIAN (1!=*(short *)(unsigned char[]){1, 0})
void testbiger()
{
	printf("%x\n",ISBIGENDIAN);
}
/*装载动态链接库*/
CK_RV test_loadlibrary()
{
	CK_RV                  brtn=FALSE;
	CK_RV                 rv=0;
	int ret = 0;
	testbiger();
	printf("enter test_loadlibrary.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "enter test_loadlibrary.<br>") ;

#ifdef USE_STATIC_LIB_OPEN

	brtn = test_getstatic();
	if(brtn!=TRUE)
	{
		printf("test_getstatic FAILED\n");
	}
	return TRUE;
#endif

#ifndef _LINUX_DYNAMIC_LIBRARY
#ifdef _WIN32
	hDll = LoadLibrary(dllname);
	hDllToken = LoadLibrary(dllname_token);
	
	
#else
	//hDll = (HINSTANCE)dlopen(dllname, RTLD_LAZY );
	printf("enter dlopen%s\n",pathname);
	hDll_self = dlopen(pathname, RTLD_LAZY);
	if(hDll_self == NULL_PTR)
	//if(hDll_self == NULL_PTR)
	{
		printf("dlopen - %sn\n", dlerror());
		printf("failed 1\n");
		hDll_self = dlopen(dllname_my_absolute, RTLD_LAZY);
		if(hDll_self == NULL_PTR)
		{
			printf("failed 2\n");
			hDll_self = dlopen(dllname_my_local, RTLD_LAZY);
			if (hDll_self == NULL_PTR)
			{
				printf("test_loadlibrary failed,open LoadLibrary.\n");
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "test_loadlibrary failed,open LoadLibrary.<br>") ;
				return FALSE;
			}
		}
	}
#endif // _WIN32

#endif // _LINUX_DYNAMIC_LIBRARY

#ifndef _LINUX_DYNAMIC_LIBRARY
	if(hDll_self==NULL_PTR)
	{
#ifdef _WIN32
		printf("test_loadlibrary failed,LoadLibrary. ErrCode=0x%x.\n",GetLastError());
#else
        printf("test_loadlibrary failed,LoadLibrary. %s.\n",dlerror());
#endif // _WIN32
		return FALSE;
	}
#endif // _LINUX_DYNAMIC_LIBRARY
	
	//取函数列表
#ifndef _LINUX_DYNAMIC_LIBRARY

#ifdef _WIN32
	pC_GetFunctionList=(CK_C_GetFunctionList)GetProcAddress(hDll,"C_GetFunctionList");
#else

#ifdef USE_PROXY_OPEN

	pC_StrErr = (CK_C_StrErr)dlsym((void*)hDll_self,"StrErr");
	if(!pC_StrErr)
	{
		printf("Load pC_StrErr false\n");
	    nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Load pC_StrErr false<br>") ;
	}

	ret = 0;
	pC_Proxy_Init = (CK_C_Proxy_Init)dlsym((void*)hDll_self,"Proxy_Init");
	if(!pC_Proxy_Init)
	{
		printf("Load pC_Proxy_Init false\n");
	    nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Load pC_Proxy_Init false<br>") ;
	}
	else
	{
		if((rv = pC_Proxy_Init())!=0)
		{
			printf("test_loadlibrary failed,pC_Proxy_Init. ErrCode=0x%08x:%s.\n",rv,pC_StrErr(rv));
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_loadlibrary failed,pC_Proxy_Init. ErrCode=0x%08x:%s.<br>",rv,pC_StrErr(rv)) ;
	
			brtn = dlclose((void*)hDll_self);
			hDll_self = NULL_PTR;
			return FALSE;
		}	
	}

	pC_Proxy_Final = (CK_C_Proxy_Final)dlsym((void*)hDll_self,"Proxy_Final");
	if(!pC_Proxy_Final)
	{
		printf("Load pC_Proxy_Final false\n");
	    nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Load pC_Proxy_Final false<br>") ;
	}

	pC_Monopolize_Enable = (CK_C_Monopolize_Enable)dlsym((void*)hDll_self,"Monopolize_Enable");
	if (pC_Monopolize_Enable == NULL)
	{
		printf("Load pC_Monopolize_Enable false.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Load pC_Monopolize_Enable false.<br>") ;
		brtn = dlclose((void*)hDll_self);
		if(brtn!=0)
		{
			printf("test_loadlibrary failed,FreeLibrary. %s.\n",dlerror());
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_loadlibrary failed,FreeLibrary. %s.<br>",dlerror()) ;
		}
	}

	pC_Monopolize_Disable =(CK_C_Monopolize_Disable)dlsym((void*)hDll_self,"Monopolize_Disable");
	if (pC_Monopolize_Disable == NULL)
	{
		printf("Load pC_Monopolize_Disable error.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Load pC_Monopolize_Disable error.<br>") ;
		brtn = dlclose((void*)hDll_self);
		if(brtn!=0)
		{
			printf("test_loadlibrary failed,FreeLibrary. %s.\n",dlerror());
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_loadlibrary failed,FreeLibrary. %s.<br>",dlerror()) ;
		}
	}
#endif
	pC_GetFunctionList = (CK_C_GetFunctionList)dlsym((void*)hDll_self,"C_GetFunctionList");
#endif	// _WIN32
	if(pC_GetFunctionList==NULL)
	{
#ifdef _WIN32
		printf("test_loadlibrary failed,GetProcAddress. ErrCode=0x%x.\n",GetLastError());
		brtn = FreeLibrary(hDll);
		if(brtn==FALSE)
		{
			printf("test_loadlibrary failed,FreeLibrary. ErrCode=0x%x.\n",GetLastError());
		}
#else
        printf("test_loadlibrary failed,GetProcAddress. %s.\n",dlerror());
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_loadlibrary failed,GetProcAddress. %s.<br>",dlerror()) ;
		brtn = dlclose((void*)hDll_self);
		if(brtn!=0)
		{
			printf("test_loadlibrary failed,FreeLibrary. %s.\n",dlerror());
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_loadlibrary failed,FreeLibrary. %s.<br>",dlerror()) ;
		}
#endif // _WIN32		
		return FALSE;
	}
#endif  //_LINUX_DYNAMIC_LIBRARY
	printf("进入pFunctionList \n");
#ifndef _LINUX_DYNAMIC_LIBRARY
	rv=(*pC_GetFunctionList)((CK_FUNCTION_LIST_PTR_PTR)&pFunctionList);	
#else
	rv = C_GetFunctionList(&pFunctionList);
#endif //_LINUX_DYNAMIC_LIBRARY	
	if((rv!=CKR_OK)||(pFunctionList==NULL))
	{
#ifndef _LINUX_DYNAMIC_LIBRARY
		printf("test_loadlibrary failed,pC_GetFunctionList.rv=%08x\n",rv);
#ifdef _WIN32
		brtn = FreeLibrary(hDll);
		if(brtn==FALSE)
		{
			printf("test_loadlibrary failed,FreeLibrary. ErrCode=0x%x.\n",GetLastError());
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_loadlibrary failed,FreeLibrary. ErrCode=0x%x.<br>",GetLastError()) ;
		}
#else
	#ifdef USE_PROXY_OPEN
		pC_Proxy_Final();
		pC_Proxy_Final = NULL;
		pC_Proxy_Init = NULL;
	#endif

		brtn = dlclose((void*)hDll_self);
		if(brtn!=0)
		{
			hDll_self = NULL_PTR;
			printf("test_loadlibrary failed,FreeLibrary. %s.\n",dlerror());
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_loadlibrary failed,FreeLibrary. %s.<br>",dlerror()) ;
		}
		hDll_self = NULL_PTR;
#endif // _WIN32

#else
		printf("test_loadlibrary failed,pfC_GetFunctionList. Static lib version\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_loadlibrary failed,pfC_GetFunctionList. Static lib version.<br>") ;

#endif // _LINUX_DYNAMIC_LIBRARY
		
		return FALSE;
	}

	printf("version:%x.%x\n",pFunctionList->version.major,pFunctionList->version.minor);

	//初始化
	pC_Initialize=pFunctionList->C_Initialize;
	pC_Finalize=pFunctionList->C_Finalize;
	pC_GetInfo=pFunctionList->C_GetInfo;
	pC_GetSlotList=pFunctionList->C_GetSlotList;
	pC_GetSlotInfo=pFunctionList->C_GetSlotInfo;
	pC_GetTokenInfo=pFunctionList->C_GetTokenInfo;
	pC_GetMechanismList=pFunctionList->C_GetMechanismList;
	pC_GetMechanismInfo=pFunctionList->C_GetMechanismInfo;
	pC_InitToken=pFunctionList->C_InitToken;
	pC_InitPIN=pFunctionList->C_InitPIN;
	pC_SetPIN=pFunctionList->C_SetPIN;
	pC_OpenSession=pFunctionList->C_OpenSession;
	pC_CloseSession=pFunctionList->C_CloseSession;
	pC_CloseAllSessions=pFunctionList->C_CloseAllSessions;
	pC_GetSessionInfo=pFunctionList->C_GetSessionInfo;
	pC_GetOperationState=pFunctionList->C_GetOperationState;
	pC_SetOperationState=pFunctionList->C_SetOperationState;
	pC_Login=pFunctionList->C_Login;
	pC_Logout=pFunctionList->C_Logout;
	pC_CreateObject=pFunctionList->C_CreateObject;
	pC_CopyObject=pFunctionList->C_CopyObject;
	pC_DestroyObject=pFunctionList->C_DestroyObject;
	pC_GetObjectSize=pFunctionList->C_GetObjectSize;
	pC_GetAttributeValue=pFunctionList->C_GetAttributeValue;
	pC_SetAttributeValue=pFunctionList->C_SetAttributeValue;
	pC_FindObjectsInit=pFunctionList->C_FindObjectsInit;
	pC_FindObjects=pFunctionList->C_FindObjects;
	pC_FindObjectsFinal=pFunctionList->C_FindObjectsFinal;
	pC_EncryptInit=pFunctionList->C_EncryptInit;
	pC_Encrypt=pFunctionList->C_Encrypt;
	pC_EncryptUpdate=pFunctionList->C_EncryptUpdate;
	pC_EncryptFinal=pFunctionList->C_EncryptFinal;
	pC_DecryptInit=pFunctionList->C_DecryptInit;
	pC_Decrypt=pFunctionList->C_Decrypt;
	pC_DecryptUpdate=pFunctionList->C_DecryptUpdate;
	pC_DecryptFinal=pFunctionList->C_DecryptFinal;
	pC_DigestInit=pFunctionList->C_DigestInit;
	pC_Digest=pFunctionList->C_Digest;
	pC_DigestUpdate=pFunctionList->C_DigestUpdate;
	pC_DigestKey=pFunctionList->C_DigestKey;
	pC_DigestFinal=pFunctionList->C_DigestFinal;
	pC_SignInit=pFunctionList->C_SignInit;
	pC_Sign=pFunctionList->C_Sign;
	pC_SignUpdate=pFunctionList->C_SignUpdate;
	pC_SignFinal=pFunctionList->C_SignFinal;
	pC_SignRecoverInit=pFunctionList->C_SignRecoverInit;
	pC_SignRecover=pFunctionList->C_SignRecover;
	pC_VerifyInit=pFunctionList->C_VerifyInit;
	pC_Verify=pFunctionList->C_Verify;
	pC_VerifyUpdate=pFunctionList->C_VerifyUpdate;
	pC_VerifyFinal=pFunctionList->C_VerifyFinal;
	pC_VerifyRecoverInit=pFunctionList->C_VerifyRecoverInit;
	pC_VerifyRecover=pFunctionList->C_VerifyRecover;
	pC_DigestEncryptUpdate=pFunctionList->C_DigestEncryptUpdate;
	pC_DecryptDigestUpdate=pFunctionList->C_DecryptDigestUpdate;
	pC_SignEncryptUpdate=pFunctionList->C_SignEncryptUpdate;
	pC_DecryptVerifyUpdate=pFunctionList->C_DecryptVerifyUpdate;
	pC_GenerateKey=pFunctionList->C_GenerateKey;
	pC_GenerateKeyPair=pFunctionList->C_GenerateKeyPair;
	pC_WrapKey=pFunctionList->C_WrapKey;
	pC_UnwrapKey=pFunctionList->C_UnwrapKey;
	pC_DeriveKey=pFunctionList->C_DeriveKey;
	pC_SeedRandom=pFunctionList->C_SeedRandom;
	pC_GenerateRandom=pFunctionList->C_GenerateRandom;
	pC_GetFunctionStatus=pFunctionList->C_GetFunctionStatus;
	pC_CancelFunction=pFunctionList->C_CancelFunction;
	pC_WaitForSlotEvent=pFunctionList->C_WaitForSlotEvent;
	pC_CryptoExtend=pFunctionList->C_CryptoExtend;

	printf("Leave test_loadlibrary.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Leave test_loadlibrary.<br>") ;
	return TRUE;
}

/*卸载动态链接库*/
CK_RV test_freelibrary()
{
	CK_RV  brtn=FALSE;

	printf("enter test_freelibrary.\n");


#ifdef USE_STATIC_LIB_OPEN

	brtn = test_freestatic();
	if(brtn!=TRUE)
	{
		printf("test_getstatic FAILED\n");
	}
	return TRUE;
#endif


	pC_GetFunctionList=NULL;
	pC_Initialize=NULL;
	pC_Finalize=NULL;
	pC_GetInfo=NULL;
	pC_GetSlotList=NULL;
	pC_GetSlotInfo=NULL;
	pC_GetTokenInfo=NULL;
	pC_GetMechanismList=NULL;
	pC_GetMechanismInfo=NULL;
	pC_InitToken=NULL;
	pC_InitPIN=NULL;
	pC_SetPIN=NULL;
	pC_OpenSession=NULL;
	pC_CloseSession=NULL;
	pC_CloseAllSessions=NULL;
	pC_GetSessionInfo=NULL;	
	pC_GetOperationState=NULL;
	pC_SetOperationState=NULL;
	pC_Login=NULL;
	pC_Logout=NULL;
	pC_CreateObject=NULL;
	pC_CopyObject=NULL;
	pC_DestroyObject=NULL;
	pC_GetObjectSize=NULL;
	pC_GetAttributeValue=NULL;
	pC_SetAttributeValue=NULL;
	pC_FindObjectsInit=NULL;
	pC_FindObjects=NULL;
	pC_FindObjectsFinal=NULL;
	pC_EncryptInit=NULL;
	pC_Encrypt=NULL;
	pC_EncryptUpdate=NULL;
	pC_EncryptFinal=NULL;
	pC_DecryptInit=NULL;
	pC_Decrypt=NULL;
	pC_DecryptUpdate=NULL;
	pC_DecryptFinal=NULL;
	pC_DigestInit=NULL;
	pC_Digest=NULL;
	pC_DigestUpdate=NULL;
	pC_DigestKey=NULL;
	pC_DigestFinal=NULL;
	pC_SignInit=NULL;
	pC_Sign=NULL;
	pC_SignUpdate=NULL;
	pC_SignFinal=NULL;
	pC_SignRecoverInit=NULL;
	pC_SignRecover=NULL;
	pC_VerifyInit=NULL;
	pC_Verify=NULL;
	pC_VerifyUpdate=NULL;
	pC_VerifyFinal=NULL;
	pC_VerifyRecoverInit=NULL;
	pC_VerifyRecover=NULL;
	pC_DigestEncryptUpdate=NULL;
	pC_DecryptDigestUpdate=NULL;
	pC_SignEncryptUpdate=NULL;
	pC_DecryptVerifyUpdate=NULL;
	pC_GenerateKey=NULL;
	pC_GenerateKeyPair=NULL;
	pC_WrapKey=NULL;
	pC_UnwrapKey=NULL;
	pC_DeriveKey=NULL;
	pC_SeedRandom=NULL;
	pC_GenerateRandom=NULL;
	pC_GetFunctionStatus=NULL;
	pC_CancelFunction=NULL;
	pC_WaitForSlotEvent=NULL;
	
#ifndef  _LINUX_DYNAMIC_LIBRARY
#ifdef  _WIN32
	brtn=FreeLibrary(hDll);
	hDll=NULL;
	if(brtn==FALSE)
	{
		printf("test_freelibrary failed,FreeLibrary. ErrCode=0x%x.\n",GetLastError());
		return FALSE;
	}
#else
	//	brtn=dlclose((void*)hDll);
	//	hDll=NULL;
	//	if(brtn!=0)
	//	{
	//		printf("test_freelibrary failed,FreeLibrary. %s.\n",dlerror());
	//		return FALSE;
	//	}
	printf("test_freelibrary \n");
	if(pC_Proxy_Final)
	{
		pC_Proxy_Final();
		pC_Proxy_Final = NULL;
	}
	printf("test_freelibrary01 \n");
	if(hDll_self)
	{
		brtn=dlclose((void*)hDll_self);
		hDll_self=NULL_PTR;
		if(brtn!=0)
		{
			printf("test_freelibrary self failed,FreeLibrary. %s.\n",dlerror());
			return FALSE;
		}
	}
#endif // _WIN32
#endif //_LINUX_DYNAMIC_LIBRARY
	
	
	return TRUE;
}

/*通过静态方式获取函数列表*/
CK_RV test_getstatic()
{
	CK_RV                  brtn=FALSE;
	CK_RV                 rv=0;

	printf("enter test_getstatic.\n");
	//取函数列表
	rv = C_GetFunctionList(&pFunctionList);
	if((rv!=CKR_OK)||(pFunctionList==NULL))
	{
		printf("test_loadlibrary failed,pfC_GetFunctionList. Static lib version");
		return FALSE;
	}

	//初始化
	pC_Initialize=pFunctionList->C_Initialize;
	pC_Finalize=pFunctionList->C_Finalize;
	pC_GetInfo=pFunctionList->C_GetInfo;
	pC_GetSlotList=pFunctionList->C_GetSlotList;
	pC_GetSlotInfo=pFunctionList->C_GetSlotInfo;
	pC_GetTokenInfo=pFunctionList->C_GetTokenInfo;
	pC_GetMechanismList=pFunctionList->C_GetMechanismList;
	pC_GetMechanismInfo=pFunctionList->C_GetMechanismInfo;
	pC_InitToken=pFunctionList->C_InitToken;
	pC_InitPIN=pFunctionList->C_InitPIN;
	pC_SetPIN=pFunctionList->C_SetPIN;
	pC_OpenSession=pFunctionList->C_OpenSession;
	pC_CloseSession=pFunctionList->C_CloseSession;
	pC_CloseAllSessions=pFunctionList->C_CloseAllSessions;
	pC_GetSessionInfo=pFunctionList->C_GetSessionInfo;
	pC_GetOperationState=pFunctionList->C_GetOperationState;
	pC_SetOperationState=pFunctionList->C_SetOperationState;
	pC_Login=pFunctionList->C_Login;
	pC_Logout=pFunctionList->C_Logout;
	pC_CreateObject=pFunctionList->C_CreateObject;
	pC_CopyObject=pFunctionList->C_CopyObject;
	pC_DestroyObject=pFunctionList->C_DestroyObject;
	pC_GetObjectSize=pFunctionList->C_GetObjectSize;
	pC_GetAttributeValue=pFunctionList->C_GetAttributeValue;
	pC_SetAttributeValue=pFunctionList->C_SetAttributeValue;
	pC_FindObjectsInit=pFunctionList->C_FindObjectsInit;
	pC_FindObjects=pFunctionList->C_FindObjects;
	pC_FindObjectsFinal=pFunctionList->C_FindObjectsFinal;
	pC_EncryptInit=pFunctionList->C_EncryptInit;
	pC_Encrypt=pFunctionList->C_Encrypt;
	pC_EncryptUpdate=pFunctionList->C_EncryptUpdate;
	pC_EncryptFinal=pFunctionList->C_EncryptFinal;
	pC_DecryptInit=pFunctionList->C_DecryptInit;
	pC_Decrypt=pFunctionList->C_Decrypt;
	pC_DecryptUpdate=pFunctionList->C_DecryptUpdate;
	pC_DecryptFinal=pFunctionList->C_DecryptFinal;
	pC_DigestInit=pFunctionList->C_DigestInit;
	pC_Digest=pFunctionList->C_Digest;
	pC_DigestUpdate=pFunctionList->C_DigestUpdate;
	pC_DigestKey=pFunctionList->C_DigestKey;
	pC_DigestFinal=pFunctionList->C_DigestFinal;
	pC_SignInit=pFunctionList->C_SignInit;
	pC_Sign=pFunctionList->C_Sign;
	pC_SignUpdate=pFunctionList->C_SignUpdate;
	pC_SignFinal=pFunctionList->C_SignFinal;
	pC_SignRecoverInit=pFunctionList->C_SignRecoverInit;
	pC_SignRecover=pFunctionList->C_SignRecover;
	pC_VerifyInit=pFunctionList->C_VerifyInit;
	pC_Verify=pFunctionList->C_Verify;
	pC_VerifyUpdate=pFunctionList->C_VerifyUpdate;
	pC_VerifyFinal=pFunctionList->C_VerifyFinal;
	pC_VerifyRecoverInit=pFunctionList->C_VerifyRecoverInit;
	pC_VerifyRecover=pFunctionList->C_VerifyRecover;
	pC_DigestEncryptUpdate=pFunctionList->C_DigestEncryptUpdate;
	pC_DecryptDigestUpdate=pFunctionList->C_DecryptDigestUpdate;
	pC_SignEncryptUpdate=pFunctionList->C_SignEncryptUpdate;
	pC_DecryptVerifyUpdate=pFunctionList->C_DecryptVerifyUpdate;
	pC_GenerateKey=pFunctionList->C_GenerateKey;
	pC_GenerateKeyPair=pFunctionList->C_GenerateKeyPair;
	pC_WrapKey=pFunctionList->C_WrapKey;
	pC_UnwrapKey=pFunctionList->C_UnwrapKey;
	pC_DeriveKey=pFunctionList->C_DeriveKey;
	pC_SeedRandom=pFunctionList->C_SeedRandom;
	pC_GenerateRandom=pFunctionList->C_GenerateRandom;
	pC_GetFunctionStatus=pFunctionList->C_GetFunctionStatus;
	pC_CancelFunction=pFunctionList->C_CancelFunction;
	pC_WaitForSlotEvent=pFunctionList->C_WaitForSlotEvent;
	
	pC_CryptoExtend=pFunctionList->C_CryptoExtend;
	pC_StrErr = StrErr;
	pC_Proxy_Init = Proxy_Init;
	pC_Proxy_Final = Proxy_Final;
	pC_Monopolize_Enable = Monopolize_Enable;
	pC_Monopolize_Disable = Monopolize_Disable;


	#ifdef USE_PROXY_OPEN
		pC_Proxy_Init();
	#endif

	return TRUE;
}

CK_RV test_freestatic()
{

	printf("enter test_freestatic.\n");

	pC_Proxy_Final();

	pC_GetFunctionList=NULL;
	pC_Initialize=NULL;
	pC_Finalize=NULL;
	pC_GetInfo=NULL;
	pC_GetSlotList=NULL;
	pC_GetSlotInfo=NULL;
	pC_GetTokenInfo=NULL;
	pC_GetMechanismList=NULL;
	pC_GetMechanismInfo=NULL;
	pC_InitToken=NULL;
	pC_InitPIN=NULL;
	pC_SetPIN=NULL;
	pC_OpenSession=NULL;
	pC_CloseSession=NULL;
	pC_CloseAllSessions=NULL;
	pC_GetSessionInfo=NULL;	
	pC_GetOperationState=NULL;
	pC_SetOperationState=NULL;
	pC_Login=NULL;
	pC_Logout=NULL;
	pC_CreateObject=NULL;
	pC_CopyObject=NULL;
	pC_DestroyObject=NULL;
	pC_GetObjectSize=NULL;
	pC_GetAttributeValue=NULL;
	pC_SetAttributeValue=NULL;
	pC_FindObjectsInit=NULL;
	pC_FindObjects=NULL;
	pC_FindObjectsFinal=NULL;
	pC_EncryptInit=NULL;
	pC_Encrypt=NULL;
	pC_EncryptUpdate=NULL;
	pC_EncryptFinal=NULL;
	pC_DecryptInit=NULL;
	pC_Decrypt=NULL;
	pC_DecryptUpdate=NULL;
	pC_DecryptFinal=NULL;
	pC_DigestInit=NULL;
	pC_Digest=NULL;
	pC_DigestUpdate=NULL;
	pC_DigestKey=NULL;
	pC_DigestFinal=NULL;
	pC_SignInit=NULL;
	pC_Sign=NULL;
	pC_SignUpdate=NULL;
	pC_SignFinal=NULL;
	pC_SignRecoverInit=NULL;
	pC_SignRecover=NULL;
	pC_VerifyInit=NULL;
	pC_Verify=NULL;
	pC_VerifyUpdate=NULL;
	pC_VerifyFinal=NULL;
	pC_VerifyRecoverInit=NULL;
	pC_VerifyRecover=NULL;
	pC_DigestEncryptUpdate=NULL;
	pC_DecryptDigestUpdate=NULL;
	pC_SignEncryptUpdate=NULL;
	pC_DecryptVerifyUpdate=NULL;
	pC_GenerateKey=NULL;
	pC_GenerateKeyPair=NULL;
	pC_WrapKey=NULL;
	pC_UnwrapKey=NULL;
	pC_DeriveKey=NULL;
	pC_SeedRandom=NULL;
	pC_GenerateRandom=NULL;
	pC_GetFunctionStatus=NULL;
	pC_CancelFunction=NULL;
	pC_WaitForSlotEvent=NULL;
	
	pC_CryptoExtend=NULL;
	pC_StrErr = NULL;
	pC_Proxy_Init = NULL;
	pC_Proxy_Final = NULL;
	pC_Monopolize_Enable = NULL;
	pC_Monopolize_Disable =NULL;

	return TRUE;
}

/*打开设备*/
CK_RV test_opendevice()
{
	CK_RV rv=0;
	CK_SLOT_ID_PTR pSlotList=NULL_PTR;
	CK_ULONG ulSlotCount=0;	
	CK_UTF8CHAR lable[]="mytoken";
	CK_BBOOL bInited=CK_FALSE;
	CK_SLOT_INFO slotInfo;
	CK_TOKEN_INFO token_info;
	char t_msg[256]={0};
	int iSlotSel = 0;
	int j = 0;
	unsigned char pAdmPin[] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38};
	int iAdmPinLen = sizeof(pAdmPin);
	unsigned char pucOTPPin[] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
	int iOTPPinLen = sizeof(pucOTPPin);
	unsigned char pbRandom[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
	unsigned char pbCipherData[256]={0x00};
	unsigned int CipherDataLen=256;
	int xxx=0;
	printf("enter test_opendevice.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"enter test_opendevice.<br>") ;

	pSlotList=(CK_SLOT_ID_PTR)malloc(10*sizeof(CK_SLOT_ID));
	if(pSlotList==NULL_PTR)
	{
	#ifndef _LINUX_DYNAMIC_LIBRARY
	
	#ifdef _WIN32
		printf("test_opendevice failed,malloc. ErrCode=0x%x.\n",GetLastError());
	#else
	    printf("test_opendevice failed,malloc. %s.\n",dlerror());
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_opendevice failed,malloc. %s.<br>",dlerror()) ;
	#endif // _WIN32
	
	#else
		printf("test_opendevice failed,malloc.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"%s","test_opendevice failed,malloc.<br>") ;
	
	#endif //_LINUX_DYNAMIC_LIBRARY
		return FALSE;
	}

	rv = (*pC_Initialize)(NULL_PTR);
	if(rv!=CKR_OK)
	{
		printf("test_opendevice failed, C_Initialize rv=0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_opendevice failed, C_Initialize rv=0x%08x.<br>",rv) ;
		goto ERR;
	}
	bInited=CK_TRUE;

	for (j=0; j<1; j++)
	{
		rv=(*pC_GetSlotList)(CK_TRUE,NULL_PTR,&ulSlotCount);
		if(rv!=CKR_OK)
		{
			printf("test_opendevice failed, C_GetSlotList rv=0x%08x.\n",rv);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_opendevice failed, C_GetSlotList rv=0x%08x.<br>",rv) ;
			goto ERR;
		}

		rv=(*pC_GetSlotList)(CK_TRUE,pSlotList,&ulSlotCount);
		if(rv!=CKR_OK)
		{
			printf("test_opendevice failed,pC_GetSlotList. rv=0x%08x.\n",rv);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_opendevice failed,pC_GetSlotList. rv=0x%08x.<br>",rv) ;
			goto ERR;
		}
	}

	
	for(j=0;j<ulSlotCount;j++)
	{
		rv=(*pC_GetSlotInfo)(pSlotList[j],&slotInfo);
		rv=(*pC_GetSlotInfo)(pSlotList[j],&slotInfo);
		if(rv!=CKR_OK)
		{
			printf("test_opendevice failed,C_GetSlotInfo. rv=0x%08x.\n",rv);
			//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_opendevice failed,C_GetSlotInfo. rv=0x%08x.<br>",rv) ;
			goto ERR;
		}

		printf("pC_GetSlotInfo ok\n");

		rv=(*pC_GetTokenInfo)(pSlotList[j],&token_info);
		if(rv==CKR_OK)
		{
			memset(t_msg,0,sizeof(t_msg));
			memcpy(t_msg,token_info.serialNumber,sizeof(token_info.serialNumber));
			printf(" Token[%d] Info serialNumber=%s.\n", j, t_msg);
			//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Token[%d] Info serialNumber=%s.<br>", j, t_msg) ;
			memset(t_msg,0,sizeof(t_msg));
			memcpy(t_msg,token_info.label,sizeof(token_info.label));
			printf(" Token[%d] Info label=%s.\n", j, t_msg);
			//nResultLength += sprintf(Testresult[nItemNumb]+ nResultLength, "Token[%d] Info serialNumber=%s.<br>", j, t_msg) ;

			printf(" Token[%d] Info hardwareVersion=%d.%d.\n", j, token_info.hardwareVersion.major, token_info.hardwareVersion.minor);
			//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Token[%d] Info hardwareVersion=%d.%d.<br>", j, token_info.hardwareVersion.major, token_info.hardwareVersion.minor) ;
			printf(" Token[%d] Info firmwareVersion=%d.%d.\n", j, token_info.firmwareVersion.major, token_info.firmwareVersion.minor);
			//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Token[%d] Info firmwareVersion=%d.%d.<br>", j, token_info.firmwareVersion.major, token_info.firmwareVersion.minor) ;
		
			CardVersionData = 0x0000;
			CardVersionData += token_info.firmwareVersion.major; 
			CardVersionData = CardVersionData<<8;
			CardVersionData += token_info.firmwareVersion.minor;

			printf("CardVersionData=%x\n",CardVersionData);
			if(CardVersionData <= CARD_VERSION_413)
			{
				printf("Test Version 4.x\n");
				//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Test Version 4.x<br>") ;
			}
			else if(CardVersionData <= CARD_VERSION_501)
			{
				printf("Test Version 5.1\n");
				//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Test Version 5.1<br>") ;
			}
			else
			{
				printf("CardVersionData <%x> Error.\n",CardVersionData);
				//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"CardVersionData <%x> Error.<br>",CardVersionData) ;
				goto ERR;
			}

			printf("Token[%d]:ulTotalPublicMemory=%d ,ulFreePublicMemory=%d.\n", j, token_info.ulTotalPublicMemory, token_info.ulFreePublicMemory);
			//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Token[%d]:ulTotalPublicMemory=%d ,ulFreePublicMemory=%d.<br>", j, token_info.ulTotalPublicMemory, token_info.ulFreePublicMemory) ;

			printf("Token[%d] ulTotalPrivateMemory=%d ,ulFreePrivateMemory=%d.\n", j, token_info.ulTotalPrivateMemory, token_info.ulFreePrivateMemory);
			//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Token[%d]:ulTotalPrivateMemory=%d ,ulFreePrivateMemory=%d.<br>", j, token_info.ulTotalPrivateMemory, token_info.ulFreePrivateMemory) ;
		}
		else
		{
			printf("test_opendevice failed,pC_GetTokenInfo. rv=0x%08x.\n",rv);
			//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_opendevice failed,pC_GetTokenInfo. rv=0x%08x.<br>",rv) ;
			goto ERR;
		}
	}
	
	if (ulSlotCount > 1)
	{
		printf("Please select a slot (%d ~ %d): ", 0, ulSlotCount-1);
		scanf("%d", &iSlotSel);
	}
	else
	{
		iSlotSel = 0;
	}
	slotID=pSlotList[iSlotSel];	

	rv=(*pC_OpenSession)(slotID,CKF_SERIAL_SESSION|CKF_RW_SESSION,NULL_PTR,NULL_PTR,&hSession);
	if(rv!=CKR_OK)
	{
		printf("test_opendevice failed,C_OpenSession. rv=0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_opendevice failed,C_OpenSession. rv=0x%08x.<br>",rv) ;
		goto ERR;
	}

	free(pSlotList);
	pSlotList=NULL_PTR;

	return TRUE;
	printf("leave test_opendevice.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"%s","leave test_opendevice.<br>") ;

ERR:
	if(pSlotList)
		free(pSlotList);
	pSlotList=NULL_PTR;
	if(bInited)
	{
		rv=(*pC_Finalize)(NULL_PTR);
		if(rv!=CKR_OK)
			printf("test_opendevice failed,C_Finalize. rv=0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_opendevice failed,C_Finalize. rv=0x%08x.<br>",rv) ;
	}
	return FALSE;
}


CK_RV test_get_slotinfo_kokeninfo()
{
	CK_RV rv=0;
	CK_TOKEN_INFO token_info;
	CK_SLOT_INFO slotInfo;
	char t_msg[256]={0};

	rv=(*pC_GetSlotInfo)(slotID,&slotInfo);
	if(rv!=CKR_OK)
	{
		printf("test_opendevice failed,C_GetSlotInfo. rv=0x%08x.\n",rv);

		return FALSE;
	}
	printf("Info: slotInfo.flags=0x%x.\n", slotInfo.flags);


	rv=(*pC_GetTokenInfo)(slotID,&token_info);
	if(rv==CKR_OK)
	{
		memset(t_msg,0,sizeof(t_msg));
		memcpy(t_msg,token_info.serialNumber,sizeof(token_info.serialNumber));
		printf("Info serialNumber=%s.\n",t_msg);

		memset(t_msg,0,sizeof(t_msg));
		memcpy(t_msg,token_info.label,sizeof(token_info.label));
		printf("Info label=%s.\n",t_msg);

		printf("Info hardwareVersion=%d.%d.\n",token_info.hardwareVersion.major, token_info.hardwareVersion.minor);
		printf("Info firmwareVersion=%d.%d.\n",token_info.firmwareVersion.major, token_info.firmwareVersion.minor);

		printf("ulTotalPublicMemory=%d ,ulFreePublicMemory=%d.\n",token_info.ulTotalPublicMemory, token_info.ulFreePublicMemory);
		printf("ulTotalPrivateMemory=%d ,ulFreePrivateMemory=%d.\n",token_info.ulTotalPrivateMemory, token_info.ulFreePrivateMemory);
	}
	else
	{
		printf("test_opendevice failed,pC_GetTokenInfo. rv=0x%08x.\n",rv);
		return FALSE;
	}

	return TRUE;
}

/*关闭设备*/
CK_RV test_closedevice()
{
	CK_RV rv=0;
	CK_RV brtn=TRUE;

	printf("enter test_closedevice.\n");

	rv=(*pC_CloseAllSessions)(slotID);
	if(rv!=CKR_OK)
	{
		printf(" test_closedevice failed,pC_CloseAllSessions. rv=0x%08x.\n",rv);
		brtn=FALSE;
	}

	rv=(*pC_Finalize)(NULL_PTR);
	if(rv!=CKR_OK)
	{
		printf(" test_closedevice failed,pC_Finalize. rv=0x%08x.\n",rv);
		brtn=FALSE;
	}

	printf("leave test_closedevice.\n");
	return brtn;
}

/*获取各种信息*/
CK_RV test_geteveryinfo()
{
	CK_RV rv=0;
	CK_INFO info;
	CK_SLOT_INFO slotInfo;
	CK_TOKEN_INFO TokenInfo;
	CK_MECHANISM_TYPE MechanismList[50];
	CK_ULONG ulCount=50;
	CK_MECHANISM_INFO MechanismInfo;
	CK_SESSION_INFO SessionInfo;

	rv=(*pC_GetInfo)(&info);
	if(rv!=CKR_OK)
	{
		printf("test_geteveryinfo failed,pC_GetInfo. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv=(*pC_GetSlotInfo)(slotID,&slotInfo);
	if(rv!=CKR_OK)
	{
		printf("test_geteveryinfo failed,pC_GetSlotInfo. rv=0x%08x.\n",rv);
		return FALSE;

	}
	
	rv=(*pC_GetTokenInfo)(slotID,&TokenInfo);
	if(rv!=CKR_OK)
	{
		printf("test_geteveryinfo failed,pC_GetTokenInfo. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv=(*pC_GetMechanismList)(slotID,MechanismList,&ulCount);
	if(rv!=CKR_OK)
	{
		printf("test_geteveryinfo failed,pC_GetMechanismList. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv=(*pC_GetMechanismInfo)(slotID,MechanismList[0],&MechanismInfo);
	if(rv!=CKR_OK)
	{
		printf("test_geteveryinfo failed,pC_GetMechanismInfo. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv=(*pC_GetSessionInfo)(hSession,&SessionInfo);
	if(rv!=CKR_OK)
	{
		printf("test_geteveryinfo failed,pC_GetSessionInfo. rv=0x%08x.\n",rv);
		return FALSE;
	}

	return TRUE;
}

CK_RV test_keystate()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;
	int i=0;

	//for verify pin
	CK_EXTEND_IN ExtIn_VerifyPin = {CK_EXTEND_VERIFYPIN, (void*)"88888888",8};
	CK_EXTEND_OUT ExtOut_VerifyPin = {CK_EXTEND_VERIFYPIN, NULL, 0};

	//for get sn
	CK_EXTEND_IN ExtIn_GetSN = {CK_EXTEND_GETSN, NULL, 0};
	CK_EXTEND_OUT ExtOut_GetSN = {CK_EXTEND_GETSN, NULL, 0};

	//for get pin try time
	int iPinTime = 0;
	CK_EXTEND_IN ExtIn_GetPinTime = {CK_EXTEND_GETPINTIME, NULL, 0};
	CK_EXTEND_OUT ExtOut_GetPinTime = {CK_EXTEND_GETPINTIME, &iPinTime, sizeof(iPinTime)};

	////for is key valid
	//CK_EXTEND_IN ExtIn_IsKeyValid = {CK_EXTEND_ISKEYVALID, NULL, 0};
	//CK_EXTEND_OUT ExtOut_IsKeyValid = {CK_EXTEND_ISKEYVALID, NULL, 0};

	CK_EXTEND_IN ExtIn_SDStatus = {CK_EXTEND_GETSDSTATUS, NULL, 0};
	CK_EXTEND_OUT ExtOut_SDStatus = {CK_EXTEND_GETSDSTATUS, NULL, 0};

	////for is raycom key
	//CK_EXTEND_IN ExtIn_IsRayComKey = {CK_EXTEND_ISRAYCOMKEY, NULL, 0};
	//CK_EXTEND_OUT ExtOut_IsRayComKey = {CK_EXTEND_ISRAYCOMKEY, NULL, 0};

	//for get login state
	int iLoginState = 0;
	char* Logsate[] = {"NOT LOGIN", "LOGGED IN"};
	CK_EXTEND_IN ExtIn_GetLoginState = {CK_EXTEND_GETLOGINSTATE, NULL, 0};
	CK_EXTEND_OUT ExtOut_GetLoginState = {CK_EXTEND_GETLOGINSTATE, &iLoginState, sizeof(iLoginState)};


	//verify pin
	//rv = (*pC_CryptoExtend)(hSession, &ExtIn_VerifyPin, &ExtOut_VerifyPin, NULL);
	//if (rv == CKR_OK)
	//{
	//	printf("Verify Pin OK.\n");
	//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","Verify Pin OK.\n");
	//}
	//else
	//{
	//	printf("Verify Pin Fail.rv=%08x\n",rv);
	//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","Verify Pin Fail.\n");
	//	bRtn = FALSE;
	//	goto END;
	//}

	//for get sn
	//rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetSN, &ExtOut_GetSN, NULL);
	//if (rv == CKR_OK)
	//{
	//	printf("Get SN 1 OK.\n");
	//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","Get SN 1 OK.\n");
	//}
	//else
	//{
	//	printf("Get SN 1 Fail.\n");
	//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","Get SN 1 Fail.\n");
	//	bRtn = FALSE;
	//	goto END;
	//}

	//ExtOut_GetSN.pParameter = (void*)malloc(ExtOut_GetSN.ulParameterLen);
	//memset(ExtOut_GetSN.pParameter, 0, ExtOut_GetSN.ulParameterLen);

	//rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetSN, &ExtOut_GetSN, NULL);
	//if (rv == CKR_OK)
	//{
	//	printf("Get SN 2 OK, SN: %s.\n", (char*)ExtOut_GetSN.pParameter);
	//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Get SN 2 OK, SN: %s.\n", (char*)ExtOut_GetSN.pParameter);
	//}
	//else
	//{
	//	printf("Get SN 2 Fail.\n");
	//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","Get SN 2 Fail.\n");
	//	bRtn = FALSE;
	//	goto END;
	//}

	//for get pin try time
	//rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetPinTime, &ExtOut_GetPinTime, NULL);
	//if (rv == CKR_OK)
	//{
	//	printf("Get Pin Time OK, Pin Time: %d.\n", *(int *)(ExtOut_GetPinTime.pParameter));
	//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Get Pin Time OK, Pin Time: %d.\n", *(int *)(ExtOut_GetPinTime.pParameter));
	//}
	//else
	//{
	//	printf("Get Pin Time Fail.\n");
	//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","Get Pin Time Fail.\n");
	//	bRtn = FALSE;
	//	goto END;
	//}

	////for is key valid
	//rv = (*pC_CryptoExtend)(hSession, &ExtIn_IsKeyValid, &ExtOut_IsKeyValid, NULL);
	//if (rv == CKR_OK)
	//{
	//	printf("Key Valid OK.\n");
	//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","Key Valid OK.\n");
	//}
	//else
	//{
	//	printf("Key Valid Fail.\n");
	//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","Key Valid Fail.\n");
	//	bRtn = FALSE;
	//	goto END;
	//}

	////for is raycom key
	//rv = (*pC_CryptoExtend)(hSession, &ExtIn_IsRayComKey, &ExtOut_IsRayComKey, NULL);
	//if (rv == CKR_OK)
	//{
	//	printf("Is RayCom Key OK.\n");
	//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","Is RayCom Key OK.\n");
	//}
	//else
	//{
	//	printf("Is RayCom Key Fail.\n");
	//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","Is RayCom Key Fail.\n");
	//	bRtn = FALSE;
	//	goto END;
	//}
	//测试新的扩展接口
	//sleep(60);
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SDStatus, &ExtOut_SDStatus, NULL);
	if (rv == CKR_OK)
	{
		printf("Get SD State OK.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Get KEY State OK.<br>");
	}
	else
	{
		printf("Get SD  State Fail.RV = %0x\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Get KEY State Fail.<br>");
		bRtn = FALSE;
		goto END;
	}

	//for get login state
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetLoginState, &ExtOut_GetLoginState, NULL);
	if (rv == CKR_OK)
	{
		printf("Get Login State OK,  Login State: %s.\n",Logsate[iLoginState]);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Get Login State OK,  Login State: %s.<br>",Logsate[iLoginState]);
	}
	else
	{
		printf("Get Login State Fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","Get Login State Fail.<br>");
		bRtn = FALSE;
		goto END;
	}


	bRtn = TRUE;
END:
	if (ExtOut_GetSN.pParameter)
	{
		free(ExtOut_GetSN.pParameter);
		ExtOut_GetSN.pParameter = NULL;
	}
	return bRtn;
}

CK_RV test_InitToken()
{

	CK_RV rv=0;
	CK_UTF8CHAR labelToken[32]={' '};

	printf("enter test_InitToken.\n");
	//slotID=0x03;
	//先关闭所有的会话
	rv=(*pC_CloseAllSessions)(slotID);
	if(rv!=CKR_OK)
	{
		printf("test_InitToken failed,pC_CloseAllSessions. rv=0x%08x.\n",rv);
		return FALSE;
	}

	printf("After C_CloseAllSessions.\n");
	
	memcpy(labelToken,"label",5);
	printf("test_InitToken adminpin=%s.\n",adminpin);
	rv=(*pC_InitToken)(slotID,adminpin,Util_GetUTF8CharLen(adminpin),labelToken);
	//rv=(*pC_InitToken)(slotID,NULL,0,labelToken);
	if(rv!=CKR_OK)
	{
		printf("test_InitToken failed,pC_InitToken. rv=0x%08x.\n",rv);
		return FALSE;
	}

	return TRUE;
}

CK_RV test_destroykey()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;

	//for set destroy key
	unsigned char pbDestroyKey[64] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, \
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};
	char *pbSOPin = (char*)adminpin;
	unsigned char pbBuffer[128] = {0};
	CK_EXTEND_IN ExtIn_SetDestroyKey = {CK_EXTEND_SETDESTROYKEY, (void*)pbBuffer, 0};
	CK_EXTEND_OUT ExtOut_SetDestroyKey = {CK_EXTEND_SETDESTROYKEY, NULL, 0};

	//for do destroy
	unsigned char pbUpDestroyKey[32] = {0};
	unsigned char pbDownDestroyKey[32] = {0};
	CK_EXTEND_IN ExtIn_DoDestroy = {CK_EXTEND_DODESTROY, (void*)pbUpDestroyKey, 32};
	CK_EXTEND_OUT ExtOut_DoDestroy = {CK_EXTEND_DODESTROY, (void*)pbDownDestroyKey, 32};

	//set destroy key 1
	memcpy(ExtIn_SetDestroyKey.pParameter, pbDestroyKey, 64);
	memcpy(((unsigned char*)(ExtIn_SetDestroyKey.pParameter)) + 64, pbSOPin, strlen(pbSOPin));
	ExtIn_SetDestroyKey.ulParameterLen = 64 + strlen(pbSOPin);
	((unsigned char*)ExtIn_SetDestroyKey.pParameter)[0] += 1;
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetDestroyKey, &ExtOut_SetDestroyKey, NULL);
	if (rv == CKR_OK)
	{
		printf("Set Destroy Key 1 OK.\n");
	}
	else
	{
		printf("Set Destroy Key 1 Fail.\n");
		bRtn = FALSE;
		goto END;
	}

	//set destroy key 2
	memcpy(ExtIn_SetDestroyKey.pParameter, pbDestroyKey, 64);
	memcpy(((unsigned char*)(ExtIn_SetDestroyKey.pParameter)) + 64, pbSOPin, strlen(pbSOPin));
	ExtIn_SetDestroyKey.ulParameterLen = 64 + strlen(pbSOPin);
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetDestroyKey, &ExtOut_SetDestroyKey, NULL);
	if (rv == CKR_OK)
	{
		printf("Set Destroy Key 2 OK.\n");
	}
	else
	{
		printf("Set Destroy Key 2 Fail.\n");
		bRtn = FALSE;
		goto END;
	}

	//do destroy 1
	memcpy(ExtIn_DoDestroy.pParameter, pbDestroyKey, 32);
	ExtIn_DoDestroy.ulParameterLen = 32;
	memset(ExtOut_DoDestroy.pParameter, 0, 32);
	ExtOut_DoDestroy.ulParameterLen = 32;
	((unsigned char*)ExtIn_DoDestroy.pParameter)[0] += 1;
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_DoDestroy, &ExtOut_DoDestroy, NULL);
	if (rv == CKR_OK)
	{
		printf("Do Destroy 1 Should Be Fail.\n");
		bRtn = FALSE;
		goto END;
	}
	else
	{
		printf("Do Destroy 1 Failed OK.\n");
	}

	//do destroy 2
	memcpy(ExtIn_DoDestroy.pParameter, pbDestroyKey, 32);
	ExtIn_DoDestroy.ulParameterLen = 32;
	memset(ExtOut_DoDestroy.pParameter, 0, 32);
	ExtOut_DoDestroy.ulParameterLen = 32;
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_DoDestroy, &ExtOut_DoDestroy, NULL);
	if (rv == CKR_OK)
	{
		printf("Do Destroy 2 OK.\n");
	}
	else
	{
		printf("Do Destroy 2 Fail.\n");
		bRtn = FALSE;
		goto END;
	}

	bRtn = TRUE;
END:
	return bRtn;
}


CK_ULONG Util_GetUTF8CharLen(CK_UTF8CHAR_PTR pChar)
{
	char *p=(char*)pChar;
	return (CK_ULONG)strlen(p);
}

CK_RV xtest_TokenStorageKeyPairVerify()
{
	CK_RV rv=0;
	CK_INFO info;
	CK_SLOT_INFO slotInfo;
	CK_TOKEN_INFO TokenInfo;
	CK_MECHANISM_TYPE MechanismList[50];
	CK_ULONG ulCount=50;
	CK_MECHANISM_INFO MechanismInfo;
	CK_SESSION_INFO SessionInfo;
	char t_msg[256]={0};
	int totalpubkeyspace=0,freepubkeyspace=0,totalprikeyspace=0,freeprikeyspace=0;

	totalpubkeyspace=0;
	freepubkeyspace=0;
	totalprikeyspace=0;
	freeprikeyspace=0;
	rv=(*pC_GetTokenInfo)(slotID,&TokenInfo);
	if(rv==CKR_OK)
	{
		totalpubkeyspace=TokenInfo.ulTotalPublicMemory;
		printf(" TotalPublicMemory=%d Bytes.\n", totalpubkeyspace);

		freepubkeyspace=TokenInfo.ulFreePublicMemory;
		printf(" FreePublicMemory=%d Bytes.\n", freepubkeyspace);

		totalprikeyspace=TokenInfo.ulTotalPrivateMemory;
		printf(" TotalPrivateMemory=%d Bytes.\n", totalprikeyspace);

		freeprikeyspace=TokenInfo.ulFreePrivateMemory;
		printf(" FreePrivateMemory=%d Bytes.\n", freeprikeyspace);

	}
	else
	{
		printf("xtest_TokenStorageKeyPairVerify failed,pC_GetTokenInfo. rv=0x%08x.\n",rv);
		return FALSE;
	}
	return TRUE;
}

void RandomGenerate(unsigned char* dataaddress, unsigned int cnt)
{
	int i,randt,randmaxx=0x7FFFFFFF-0x7FFFFFFF%0xFF;
	for(i=0;i<cnt;i++)
	{
		randt = rand();
		while( randt > randmaxx ) 
		{
			randt = rand();
		}
		dataaddress[i] = (unsigned char)(randt % 0x100); // 符合要求的随机数
	}
}


CK_RV test_getinfo()
{
	CK_RV rv=0;
	CK_SLOT_ID_PTR pSlotList=NULL_PTR;
	CK_ULONG ulSlotCount=0;	
	CK_UTF8CHAR lable[]="mytoken";
	CK_BBOOL bInited=CK_FALSE;
	CK_SLOT_INFO slotInfo;
	CK_TOKEN_INFO token_info;
	char t_msg[256]={0};
	int iSlotSel = 0;
	int nResultLength =0;
	int i=0,j=0;


	printf("Enter test_getinfo.\n");
	nResultLength += sprintf(Testresult[nItemNumb] +nResultLength, "Enter test_getinfo.<br>");

	pSlotList=(CK_SLOT_ID_PTR)malloc(10*sizeof(CK_SLOT_ID));
	if(pSlotList==NULL_PTR)
	{
#ifndef _LINUX_DYNAMIC_LIBRARY

#ifdef _WIN32
		//printf("test_opendevice failed,malloc. ErrCode=0x%x.\n",GetLastError());
#else
		// printf("test_opendevice failed,malloc. %s.\n",dlerror());
#endif // _WIN32

#else
		printf("test_opendevice failed,malloc.\n");
		sprintf(Testresult[nItemNumb] +nResultLength, "test_opendevice failed,malloc.<br>");

#endif //_LINUX_DYNAMIC_LIBRARY
		return FALSE;
	}

	//printf("pC_Initilize 1 0x%x\n", pC_Initialize);
	//printf("pC_Initilize 2 0x%x\n", dlsym((void*)hDll_self,"C_Initialize"));


	bInited=CK_TRUE;

#ifndef _WIN32
	//为p11的so文件设置路径
	//(*pImportValidDir)(NULL, NULL, HDZB_SD_PATH);
	//为p11test的HD_Token设置路径
	//ImportValidDir(NULL, NULL, HDZB_SD_PATH);
#endif

	for (j=0; j<1; j++)
	{
		printf("before getsolotlist1, count=%d\n", j);
		nResultLength += sprintf(Testresult[nItemNumb] +nResultLength,"before getsolotlist1, count=%d<br>", j);
		rv=(*pC_GetSlotList)(CK_TRUE,NULL_PTR,&ulSlotCount);
		if(rv!=CKR_OK)
		{
			printf("test_opendevice failed, C_GetSlotList rv=0x%08x.\n",rv);
			nResultLength += sprintf(Testresult[nItemNumb] +nResultLength,"test_opendevice failed, C_GetSlotList rv=0x%08x.<br>",rv);
			goto ERR;
		}
		printf("after getsolotlist1, count=%d\n", j);
		nResultLength += sprintf(Testresult[nItemNumb] +nResultLength,"after getsolotlist1, count=%d<br>", j);
		printf(" C_GetSlotList ulSlotCount=0x%08x.\n", ulSlotCount);
		nResultLength += sprintf(Testresult[nItemNumb] +nResultLength," C_GetSlotList ulSlotCount=0x%08x.<br>", ulSlotCount);

		printf("before getsolotlist2, count=%d\n", j);
		nResultLength += sprintf(Testresult[nItemNumb] +nResultLength,"before getsolotlist2, count=%d<br>", j);
		rv=(*pC_GetSlotList)(CK_TRUE,pSlotList,&ulSlotCount);
		if(rv!=CKR_OK)
		{
			printf("test_opendevice failed,pC_GetSlotList. rv=0x%08x.\n",rv);
			nResultLength += sprintf(Testresult[nItemNumb] +nResultLength,"test_opendevice failed,pC_GetSlotList. rv=0x%08x.<br>",rv);
			goto ERR;
		}
		printf("after getsolotlist2, count=%d\n", j);
		nResultLength += sprintf(Testresult[nItemNumb] +nResultLength,"after getsolotlist2, count=%d<br>", j);
	}


	for(j=0;j<ulSlotCount;j++)
	{
		rv=(*pC_GetSlotInfo)(pSlotList[j],&slotInfo);
		if(rv!=CKR_OK)
		{
			printf("test_opendevice failed,C_GetSlotInfo. rv=0x%08x.\n",rv);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_opendevice failed,C_GetSlotInfo. rv=0x%08x.<br>",rv);
			goto ERR;
		}
		printf(" Slot[%d] Info: slotInfo.flags=0x%x.\n", j, slotInfo.flags);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Slot[%d] Info: slotInfo.flags=0x%x.<br>", j, slotInfo.flags);

		rv=(*pC_GetTokenInfo)(pSlotList[j],&token_info);
		if(rv==CKR_OK)
		{
			memset(t_msg,0,sizeof(t_msg));
			memcpy(t_msg,token_info.serialNumber,sizeof(token_info.serialNumber));
			printf("Token[%d] Info serialNumber=%s.\n", j, t_msg);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Token[%d] Info serialNumber=%s.<br>", j, t_msg);
			
			memset(t_msg,0,sizeof(t_msg));
			memcpy(t_msg,token_info.label,sizeof(token_info.label));
			printf("Token[%d] Info label=%s.\n", j, t_msg);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Token[%d] Info label=%s.<br>", j, t_msg);

			printf(" Token[%d] Info hardwareVersion=%d.%d.\n", j, token_info.hardwareVersion.major, token_info.hardwareVersion.minor);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Token[%d]  Info hardwareVersion=%d.%d.<br>",j, token_info.hardwareVersion.major, token_info.hardwareVersion.minor);
					
			printf(" Token[%d] Info firmwareVersion=%d.%d.\n", j, token_info.firmwareVersion.major, token_info.firmwareVersion.minor);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Token[%d]  Info firmwareVersion=%d.%d.<br>",j, token_info.firmwareVersion.major, token_info.firmwareVersion.minor);
		}
		else
		{
			printf("test_opendevice failed,pC_GetTokenInfo. rv=0x%08x.\n",rv);
			nResultLength += sprintf(Testresult[nItemNumb] +nResultLength,"test_opendevice failed,pC_GetTokenInfo. rv=0x%08x.<br>",rv);
			goto ERR;
		}
	}

	printf(" Test the first device. \n");
	if (ulSlotCount > 1)
	{
		printf("Please select a slot (%d ~ %d): ", 0, ulSlotCount-1);
		scanf("%d", &iSlotSel);
	}
	else
	{
		iSlotSel = 0;
	}


	free(pSlotList);
	pSlotList=NULL_PTR;

	printf("Leave test_getinfo.\n");
	nResultLength += sprintf(Testresult[nItemNumb] ,"Leave test_getinfo.<br>");

	return TRUE;


ERR:
	if(pSlotList)
		free(pSlotList);
	pSlotList=NULL_PTR;
	if(bInited)
	{
		rv=(*pC_Finalize)(NULL_PTR);
		if(rv!=CKR_OK)
			printf("test_opendevice failed,C_Finalize. rv=0x%08x.\n",rv);
	}
	return FALSE;
}



CK_RV test_otp()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;

	int i = 0;
	int MaxTryTime = 5, MaxUsableTime = 3;

	unsigned char *pbBuffer = NULL;

	const char* userpinset = "88888888";

	//for Reset User Pin
	CK_EXTEND_IN ExtIn_ResetUserPin = {CK_EXTEND_RESET_USERPIN, NULL, 0};
	CK_EXTEND_OUT ExtOut_ResetUserPin = {CK_EXTEND_RESET_USERPIN, NULL, 0};

	//for Reset OTP Pin
	unsigned char pbDefaultOTPPin[9] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,0x39};
	unsigned char nOTPPinLen = sizeof(pbDefaultOTPPin);
	CK_EXTEND_IN ExtIn_ResetOTPPin = {CK_EXTEND_RESET_OTPPIN, NULL, 0};
	CK_EXTEND_OUT ExtOut_ResetOTPPin = {CK_EXTEND_RESET_OTPPIN, NULL, 0};

	//for get otp pin time usable
	unsigned char nOTPPinUsableTime = 0;
	CK_EXTEND_IN ExtIn_GetOTPPinTime_Usable = {CK_EXTEND_GETOTPTIME_USABLE, NULL, 0};
	CK_EXTEND_OUT ExtOut_GetOTPPinTime_Usable = {CK_EXTEND_GETOTPTIME_USABLE, (void*)&nOTPPinUsableTime, 1};

	//for get otp pin time try
	unsigned char nOTPPinTryTime = 0;
	CK_EXTEND_IN ExtIn_GetOTPPinTime_Try = {CK_EXTEND_GETOTPTIME_TRY, NULL, 0};
	CK_EXTEND_OUT ExtOut_GetOTPPinTime_Try = {CK_EXTEND_GETOTPTIME_TRY, (void*)&nOTPPinTryTime, 1};

	//for(i = 0; i < 7; ++i)
	//{
	//	rv=(*pC_Login)(hSession,CKU_USER,"12345678999",11);
	//	printf("login rv=0x%08x.\n",rv);
	//}

	pbBuffer = (unsigned char *)malloc(MAX_PIN_LEN*2 + 2);
	if (!pbBuffer)
	{
		bRtn = FALSE;
		goto END;
	}
	ExtIn_ResetUserPin.pParameter = pbBuffer;
	ExtIn_ResetOTPPin.pParameter = pbBuffer;

	printf("so pin =%s\n",adminpin);
	//Reset OTP Pin
	*pbBuffer = strlen((const char*)adminpin);
	memcpy(pbBuffer + 1, adminpin, strlen((const char*)adminpin));
	*(pbBuffer + 1 + strlen((const char*)adminpin)) = nOTPPinLen;
	memcpy(pbBuffer + 2 + strlen((const char*)adminpin), pbDefaultOTPPin, nOTPPinLen);
	ExtIn_ResetOTPPin.ulParameterLen = 2 + nOTPPinLen + strlen((const char*)adminpin);
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_ResetOTPPin, &ExtOut_ResetOTPPin, NULL);
	if (rv == CKR_OK)
	{
		printf("Reset OTP Pin First Time OK.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Reset OTP Pin First Time OK.<br>") ;
	}
	else
	{
		
		printf("Reset OTP Pin First Time Fail. rv=0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Reset OTP Pin First Time Fail.<br>") ;
		bRtn = FALSE;
		goto END;
	}

	//get otp pin time usable
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Usable, &ExtOut_GetOTPPinTime_Usable, NULL);
	if (rv == CKR_OK)
	{
		printf("get otp pin time usable OK, time = %d.\n", nOTPPinUsableTime);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "get otp pin time usable OK, time = %d.<br>", nOTPPinUsableTime) ;
	}
	else
	{
		printf("get otp pin time usable Fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "get otp pin time usable Fail.<br>") ;
		bRtn = FALSE;
		goto END;
	}

	//get otp pin time try
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Try, &ExtOut_GetOTPPinTime_Try, NULL);
	if (rv == CKR_OK)
	{
		printf("get otp pin time try OK, time = %d.\n", nOTPPinTryTime);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "get otp pin time try OK, time = %d.<br>", nOTPPinTryTime) ;
	}
	else
	{
		printf("get otp pin time try Fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "get otp pin time try Fail.<br>") ;
		bRtn = FALSE;
		goto END;
	}

	//Reset User Pin Sucess for n time
	*pbBuffer = nOTPPinLen;
	memcpy(pbBuffer + 1, pbDefaultOTPPin, nOTPPinLen);
	*(pbBuffer + 1 + nOTPPinLen) = strlen((const char*)userpinset);
	memcpy(pbBuffer + 2 + nOTPPinLen, userpinset, strlen((const char*)userpinset));
	ExtIn_ResetUserPin.ulParameterLen = 2 + nOTPPinLen + strlen((const char*)userpinset);
	printf("Set UserPin is %s\n",userpinset);
	for (i=0; i<=MaxUsableTime; i++)
	{
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_ResetUserPin, &ExtOut_ResetUserPin, NULL);
		if (i < MaxUsableTime)
		{
			if (rv == CKR_OK)
			{
				printf("Reset User Pin OK.\n");
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Reset User Pin OK.<br>") ;
			}
			else
			{
				printf("Reset User Pin Fail.\n");
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Reset User Pin Fail.<br>") ;
				bRtn = FALSE;
				goto END;
			}
		}
		else
		{
			if (rv != CKR_OK)
			{
				printf("Reset User Pin OK, NEED BE ERROR.\n");
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Reset User Pin OK, NEED BE ERROR.<br>") ;
			}
			else
			{
				printf("Reset User Pin Fail, SHOULD BE ERROR.\n");
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Reset User Pin Fail, SHOULD BE ERROR.<br>") ;
				bRtn = FALSE;
				goto END;
			}
		}
		

		//get otp pin time usable
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Usable, &ExtOut_GetOTPPinTime_Usable, NULL);
		if (rv == CKR_OK)
		{
			printf("get otp pin time usable OK, time = %d.\n", nOTPPinUsableTime);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "get otp pin time usable OK, time = %d.<br>", nOTPPinUsableTime) ;
		}
		else
		{
			printf("get otp pin time usable Fail.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "get otp pin time usable Fail.<br>") ;
			bRtn = FALSE;
			goto END;
		}

		//get otp pin time try
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Try, &ExtOut_GetOTPPinTime_Try, NULL);
		if (rv == CKR_OK)
		{
			printf("get otp pin try time usable OK, time = %d.\n", nOTPPinTryTime);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"get otp pin try time usable OK, time = %d.<br>", nOTPPinTryTime) ;
		}
		else
		{
			printf("get otp pin try time usable Fail.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"get otp pin try time usable Fail.<br>") ;
			bRtn = FALSE;
			goto END;
		}
	}

	//after reset userpin successful, then execute login
	strcpy((char *)userpin,userpinset);
	rv=(*pC_Login)(hSession,CKU_USER,userpin,strlen(userpin));
	if(rv!=CKR_OK)
	{
		if(rv==0x00000100)
		{
			printf("Already logged in,go on.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Already logged in, go on.<br>") ;
		}
		else
		{
			printf("UserPin login failed,pC_Login. rv=0x%08x.\n",rv);	
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"UserPin login Fail.<br>") ;
			bRtn = FALSE;
			goto END;
		}
	}
	//end

	//Reset OTP Pin
	*pbBuffer = strlen((const char*)adminpin);
	memcpy(pbBuffer + 1, adminpin, strlen((const char*)adminpin));
	*(pbBuffer + 1 + strlen((const char*)adminpin)) = nOTPPinLen;
	memcpy(pbBuffer + 2 + strlen((const char*)adminpin), pbDefaultOTPPin, nOTPPinLen);
	ExtIn_ResetOTPPin.ulParameterLen = 2 + nOTPPinLen + strlen((const char*)adminpin);
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_ResetOTPPin, &ExtOut_ResetOTPPin, NULL);
	if (rv == CKR_OK)
	{
		printf("Reset OTP Pin OK.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Reset OTP Pin OK.<br>") ;
	}
	else
	{
		printf("Reset OTP Pin Fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Reset OTP Pin Fail.<br>") ;
		printf("rv=0x%08x: %s.\n",rv,pC_StrErr(rv));
		bRtn = FALSE;
		goto END;
	}

	//Reset User Pin Fail for n time
	*pbBuffer = nOTPPinLen;
	memcpy(pbBuffer + 1, pbDefaultOTPPin, nOTPPinLen);
	*(pbBuffer+1) += 1;//uncorrect pin
	*(pbBuffer + 1 + nOTPPinLen) = strlen((const char*)userpinset);
	memcpy(pbBuffer + 2 + nOTPPinLen, userpinset, strlen((const char*)userpinset));
	ExtIn_ResetUserPin.ulParameterLen = 2 + nOTPPinLen + strlen((const char*)userpinset);
	for (i=0; i<=MaxTryTime; i++)
	{
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_ResetUserPin, &ExtOut_ResetUserPin, NULL);
		if (rv != CKR_OK)
		{
			printf("Reset User Pin OK, NEED BE ERROR.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Reset User Pin OK, NEED BE ERROR.<br>") ;
		}
		else
		{
			printf("Reset User Pin Fail, SHOULD BE ERROR.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Reset User Pin Fail, SHOULD BE ERROR.<br>") ;
			bRtn = FALSE;
			goto END;
		}

		//get otp pin time usable
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Usable, &ExtOut_GetOTPPinTime_Usable, NULL);
		if (rv == CKR_OK)
		{
			printf("get otp pin time usable OK, time = %d.\n", nOTPPinUsableTime);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"get otp pin time usable OK, time = %d.<br>", nOTPPinUsableTime) ;
		}
		else
		{
			printf("get otp pin time usable Fail.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"get otp pin time usable Fail.<br>") ;
			bRtn = FALSE;
			goto END;
		}

		//get otp pin time try
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Try, &ExtOut_GetOTPPinTime_Try, NULL);
		if (rv == CKR_OK)
		{
			printf("get otp pin try time usable OK, time = %d.\n", nOTPPinTryTime);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"get otp pin try time usable OK, time = %d.<br>", nOTPPinTryTime) ;
		}
		else
		{
			printf("get otp pin try time usable Fail.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"get otp pin try time usable Fail.<br>") ;
			bRtn = FALSE;
			goto END;
		}
	}

	//Reset OTP Pin
	*pbBuffer = strlen((const char*)adminpin);
	memcpy(pbBuffer + 1, adminpin, strlen((const char*)adminpin));
	*(pbBuffer + 1 + strlen((const char*)adminpin)) = nOTPPinLen;
	memcpy(pbBuffer + 2 + strlen((const char*)adminpin), pbDefaultOTPPin, nOTPPinLen);
	ExtIn_ResetOTPPin.ulParameterLen = 2 + nOTPPinLen + strlen((const char*)adminpin);
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_ResetOTPPin, &ExtOut_ResetOTPPin, NULL);
	if (rv == CKR_OK)
	{
		printf("Reset OTP Pin OK.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Reset OTP Pin OK.<br>") ;
	}
	else
	{
		printf("Reset OTP Pin Fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Reset OTP Pin Fail.<br>") ;
		bRtn = FALSE;
		goto END;
	}

	bRtn = TRUE;
END:
	if (pbBuffer)
	{
		free(pbBuffer);
		pbBuffer = NULL;
	}
	return bRtn;
}

CK_RV test_otp_reset_user()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;

	int i = 0;
	int MaxTryTime = 5, MaxUsableTime = 3;

	unsigned char *pbBuffer = NULL;

	const char* userpinset = "88888888";

	//for Reset User Pin
	CK_EXTEND_IN ExtIn_ResetUserPin = {CK_EXTEND_RESET_USERPIN, NULL, 0};
	CK_EXTEND_OUT ExtOut_ResetUserPin = {CK_EXTEND_RESET_USERPIN, NULL, 0};


	unsigned char pbDefaultOTPPin[9] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,0x39};
	unsigned char nOTPPinLen = sizeof(pbDefaultOTPPin);

	//for get otp pin time usable
	unsigned char nOTPPinUsableTime = 0;
	CK_EXTEND_IN ExtIn_GetOTPPinTime_Usable = {CK_EXTEND_GETOTPTIME_USABLE, NULL, 0};
	CK_EXTEND_OUT ExtOut_GetOTPPinTime_Usable = {CK_EXTEND_GETOTPTIME_USABLE, (void*)&nOTPPinUsableTime, 1};

	//for get otp pin time try
	unsigned char nOTPPinTryTime = 0;
	CK_EXTEND_IN ExtIn_GetOTPPinTime_Try = {CK_EXTEND_GETOTPTIME_TRY, NULL, 0};
	CK_EXTEND_OUT ExtOut_GetOTPPinTime_Try = {CK_EXTEND_GETOTPTIME_TRY, (void*)&nOTPPinTryTime, 1};

	pbBuffer = (unsigned char *)malloc(MAX_PIN_LEN*2 + 2);
	if (!pbBuffer)
	{
		bRtn = FALSE;
		goto END;
	}
	ExtIn_ResetUserPin.pParameter = pbBuffer;
	//ExtIn_ResetOTPPin.pParameter = pbBuffer;

	//get otp pin time usable
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Usable, &ExtOut_GetOTPPinTime_Usable, NULL);
	if (rv == CKR_OK)
	{
		printf("get otp pin time usable OK, time = %d.\n", nOTPPinUsableTime);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "get otp pin time usable OK, time = %d.<br>", nOTPPinUsableTime) ;
	}
	else
	{
		printf("get otp pin time usable Fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "get otp pin time usable Fail.<br>") ;
		bRtn = FALSE;
		goto END;
	}

	//get otp pin time try
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Try, &ExtOut_GetOTPPinTime_Try, NULL);
	if (rv == CKR_OK)
	{
		printf("get otp pin time try OK, time = %d.\n", nOTPPinTryTime);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "get otp pin time try OK, time = %d.<br>", nOTPPinTryTime) ;
	}
	else
	{
		printf("get otp pin time try Fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "get otp pin time try Fail.<br>") ;
		bRtn = FALSE;
		goto END;
	}

	//Reset User Pin Sucess for n time
	*pbBuffer = nOTPPinLen;
	memcpy(pbBuffer + 1, pbDefaultOTPPin, nOTPPinLen);
	*(pbBuffer + 1 + nOTPPinLen) = strlen((const char*)userpinset);
	memcpy(pbBuffer + 2 + nOTPPinLen, userpinset, strlen((const char*)userpinset));
	ExtIn_ResetUserPin.ulParameterLen = 2 + nOTPPinLen + strlen((const char*)userpinset);
	printf("Set UserPin is %s\n",userpinset);
	for (i=0; i<=MaxUsableTime; i++)
	{
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_ResetUserPin, &ExtOut_ResetUserPin, NULL);
		if (i < MaxUsableTime)
		{
			if (rv == CKR_OK)
			{
				printf("Reset User Pin OK.\n");
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Reset User Pin OK.<br>") ;
			}
			else
			{
				printf("Reset User Pin Fail rv=%08x.\n",rv);
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Reset User Pin Fail.<br>") ;
				bRtn = FALSE;
				goto END;
			}
		}
		else
		{
			if (rv != CKR_OK)
			{
				printf("Reset User Pin OK, NEED BE ERROR.\n");
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Reset User Pin OK, NEED BE ERROR.<br>") ;
			}
			else
			{
				printf("Reset User Pin Fail, SHOULD BE ERROR.\n");
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Reset User Pin Fail, SHOULD BE ERROR.<br>") ;
				bRtn = FALSE;
				goto END;
			}
		}
		

		//get otp pin time usable
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Usable, &ExtOut_GetOTPPinTime_Usable, NULL);
		if (rv == CKR_OK)
		{
			printf("get otp pin time usable OK, time = %d.\n", nOTPPinUsableTime);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "get otp pin time usable OK, time = %d.<br>", nOTPPinUsableTime) ;
		}
		else
		{
			printf("get otp pin time usable Fail.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "get otp pin time usable Fail.<br>") ;
			bRtn = FALSE;
			goto END;
		}

		//get otp pin time try
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Try, &ExtOut_GetOTPPinTime_Try, NULL);
		if (rv == CKR_OK)
		{
			printf("get otp pin try time usable OK, time = %d.\n", nOTPPinTryTime);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"get otp pin try time usable OK, time = %d.<br>", nOTPPinTryTime) ;
		}
		else
		{
			printf("get otp pin try time usable Fail.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"get otp pin try time usable Fail.<br>") ;
			bRtn = FALSE;
			goto END;
		}
	}


	//Reset User Pin Fail for n time
	*pbBuffer = nOTPPinLen;
	memcpy(pbBuffer + 1, pbDefaultOTPPin, nOTPPinLen);
	*(pbBuffer+1) += 1;//uncorrect pin
	*(pbBuffer + 1 + nOTPPinLen) = strlen((const char*)userpinset);
	memcpy(pbBuffer + 2 + nOTPPinLen, userpinset, strlen((const char*)userpinset));
	ExtIn_ResetUserPin.ulParameterLen = 2 + nOTPPinLen + strlen((const char*)userpinset);
	for (i=0; i<=MaxTryTime; i++)
	{
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_ResetUserPin, &ExtOut_ResetUserPin, NULL);
		if (rv != CKR_OK)
		{
			printf("Reset User Pin OK, NEED BE ERROR.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Reset User Pin OK, NEED BE ERROR.<br>") ;
		}
		else
		{
			printf("Reset User Pin Fail, SHOULD BE ERROR.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Reset User Pin Fail, SHOULD BE ERROR.<br>") ;
			bRtn = FALSE;
			goto END;
		}

		//get otp pin time usable
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Usable, &ExtOut_GetOTPPinTime_Usable, NULL);
		if (rv == CKR_OK)
		{
			printf("get otp pin time usable OK, time = %d.\n", nOTPPinUsableTime);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"get otp pin time usable OK, time = %d.<br>", nOTPPinUsableTime) ;
		}
		else
		{
			printf("get otp pin time usable Fail.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"get otp pin time usable Fail.<br>") ;
			bRtn = FALSE;
			goto END;
		}

		//get otp pin time try
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Try, &ExtOut_GetOTPPinTime_Try, NULL);
		if (rv == CKR_OK)
		{
			printf("get otp pin try time usable OK, time = %d.\n", nOTPPinTryTime);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"get otp pin try time usable OK, time = %d.<br>", nOTPPinTryTime) ;
		}
		else
		{
			printf("get otp pin try time usable Fail.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"get otp pin try time usable Fail.<br>") ;
			bRtn = FALSE;
			goto END;
		}
	}


	bRtn = TRUE;
END:
	if (pbBuffer)
	{
		free(pbBuffer);
		pbBuffer = NULL;
	}
	return bRtn;
}
