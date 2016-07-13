
#include "general.h"
//#include "Utils.h"
#include "Utils_c.h"
#include "GeneralCall.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "sm2.h"


#ifdef __ANDROID__
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#endif


//Set all buffer data to zero
#define BUFFER_REFRESH_ZERO(data1,data1len,data2,data2len)\
	do\
{\
	data1len = sizeof(data1);\
	memset(data1, 0, data1len);\
	data2len=sizeof(data2);\
	memset(data2, 0, data2len);\
}while(0)

//EnterAndLeaveFunction
//必须成对使用，第一次输出enter，第二次输出leave
int enterorleaveflag=0;
#define ENTER_LEAVE_FUNCTION(_storage_start,_storage_address,_Flag)\
	do\
{\
	if(enterorleaveflag==0)\
{\
	printf("Enter %s.\n", #_Flag);\
	*_storage_address+=sprintf(_storage_start + *_storage_address, "Enter %s.<br>", #_Flag);\
	enterorleaveflag=1;\
}\
	else\
{\
	printf("Leave %s.\n", #_Flag);\
	*_storage_address+=sprintf(_storage_start + *_storage_address, "Leave %s.<br>", #_Flag);\
	enterorleaveflag=0;\
}\
} while(0)

//&&(_rtn!=0xA0001108)
//Judgement
#define RV_NOT_OK_RETURN_FALSE(_storage_start,_storage_address,_func_name,_rtn)\
	do\
{\
	if((_rtn!=CKR_OK)&&(_rtn!=0x90000000))\
{\
	printf("Error: %s ,rv=0x%08x.\n", #_func_name, (unsigned int)_rtn);\
	*_storage_address+=sprintf(_storage_start + *_storage_address, "Error: %s ,rv=0x%08x.<br>", #_func_name, (unsigned int)_rtn);\
	bRtn=false;\
	goto END;\
}\
} while(0)

#ifdef SHOW_ERROR_TEST
#define RV_NOT_OK_RETURN_TRUE(_storage_start,_storage_address,_func_name,_rtn)\
	do\
{\
	if((_rtn!=CKR_OK)&&(_rtn!=0x90000000))\
{\
	printf("Should failed: %s ,rv=0x%08x.\n", #_func_name, (unsigned int)_rtn);\
	*_storage_address+=sprintf(_storage_start + *_storage_address, "Should failed: %s ,rv=0x%08x.<br>", #_func_name, (unsigned int)_rtn);\
	bRtn=true;\
}\
else\
{\
	printf("May succeed?: %s ,rv=0x%08x.\n", #_func_name, (unsigned int)_rtn);\
	*_storage_address+=sprintf(_storage_start + *_storage_address, "May succeed?: %s ,rv=0x%08x.<br>", #_func_name, (unsigned int)_rtn);\
	bRtn=false;\
	goto END;\
}\
} while(0)
#else
#define RV_NOT_OK_RETURN_TRUE(_storage_start,_storage_address,_func_name,_rtn)\
	do\
{\
	if((_rtn!=CKR_OK)&&(_rtn!=0x90000000))\
{\
	bRtn=true;\
}\
else\
{\
	printf("May succeed?: %s ,rv=0x%08x.\n", #_func_name, (unsigned int)_rtn);\
	*_storage_address+=sprintf(_storage_start + *_storage_address, "May succeed?: %s ,rv=0x%08x.<br>", #_func_name, (unsigned int)_rtn);\
	bRtn=false;\
	goto END;\
}\
} while(0)
#endif


#define RV_NOT_OK(_storage_start,_storage_address,_func_name,_rtn)\
	do\
{\
	if(_rtn!=CKR_OK)\
{\
	printf("Error: %s ,rv=0x%08x.\n", #_func_name, (unsigned int)_rtn);\
	*_storage_address+=sprintf(_storage_start + *_storage_address, "Error: %s ,rv=0x%08x.\n", #_func_name, (unsigned int)_rtn);\
	bRtn=false;\
}\
} while(0)


//Assertion
#define TEST_ASSERT_EQ(_func_name,_rtn,_val) \
	do\
{\
	if(_rtn!=_val)\
{\
	printf("Error: %s return 0x%x.\n", #_func_name,(unsigned int)_rtn);\
	goto END;\
}\
} while(0)

#define TEST_ASSERT_EQ_EX(_func_name,_rtn,_val,_err_num) \
	do\
{\
	if(_rtn!=_val)\
{\
	printf("Error: %s return 0x%x.\n", #_func_name, (unsigned int)_rtn);\
	bRtn=_err_num;\
	goto END;\
}\
} while(0)

#define TEST_ASSERT_NEQ(_func_name,_rtn,_val) \
	do\
{\
	if(_rtn==_val)\
{\
	printf("Error: %s return 0x%x.\n", #_func_name,(unsigned int)_rtn);\
	goto END;\
}\
} while(0)

#define TEST_ASSERT_NEQ_EX(_func_name,_rtn,_val,_err_num) \
	do\
{\
	if(_rtn==_val)\
{\
	printf("Error: %s return 0x%x.\n", #_func_name, (unsigned int)_rtn);\
	bRtn=_err_num;\
	goto END;\
}\
} while(0)

#define RTN_OK 0x9000



/*祖冲之算法加解密功能与性能*/
bool xtest_ZUCEncrypt()
{
	bool bRtn = false;
	CK_RV rv=0;
	unsigned int Value;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_BYTE nSessKeyID_Dec = CK_SESSKEY_PRESET_ID0;
	//加密参数
	unsigned char	ZUCkeyVal_Enc[]={0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};

	unsigned char	ZUCplain_Enc[100] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d};//, \
		0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03};
	unsigned char	ZUCcipher_Enc[100] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, 0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01};//, \
		0x9b, 0x48, 0xac, 0xd1, 0xfe, 0xf3, 0x01, 0x1f, 0x5e, 0x22, 0xd2, 0x97, 0x75, 0xcc, 0x94, 0xc2, 0xca, 0x6f, 0xb0, 0x9c, 0x2d, 0x86, 0xd6, 0xd0, 0x18, 0x3c, 0x24, 0x34, 0x09, 0x73, 0x5b, 0xad, 0x57, 0x5e, 0x9f, 0x91};

	unsigned char	ZUCiv_Enc[] = {0x00, 0x05, 0x68, 0x23, 0x38};
	unsigned char	ididididi1[] = {0x00, 0x05, 0x68, 0x23, 0x38};
	unsigned char	ididididi2[] = {0x20, 0x25, 0x28, 0x23, 0x28};
	CK_ATTRIBUTE ZUCkeyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(ZUCkeyType)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,ZUCkeyVal_Enc,sizeof(ZUCkeyVal_Enc)},
		//{CKA_VALUE,ZUCkeyVal_Enc,sizeof(ZUCkeyVal_Enc)},
		//{CKA_ID, ididididi1,sizeof(ididididi1)},
		//{CKA_ID, ididididi2,sizeof(ididididi2)},		
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(nSessKeyID_Dec)}
	};

	CK_MECHANISM ZUCmechanism_Enc = {CKM_ZUC_CALC, ZUCiv_Enc, sizeof(ZUCiv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);


	CK_BYTE getValue[16]={0};
	//CK_ATTRIBUTE secFindKeyTemplate[] = {
	//	{CKA_CLASS, &getValue, sizeof(keyClass)}
	//};

	CK_ATTRIBUTE secGetKeyTemplate[] = {
		{CKA_ID, getValue, sizeof(getValue)}
	};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCEncrypt);

	//对称加密初始化
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Enc, sizeof(ZUCkeyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	indatalen = sizeof(indata);
	memset(indata, 0, indatalen);

	//将IV拼接到明文之前
	memcpy(indata, ZUCiv_Enc, sizeof(ZUCiv_Enc));
	memcpy(indata+sizeof(ZUCiv_Enc), ZUCplain_Enc, sizeof(ZUCplain_Enc));
	indatalen = sizeof(ZUCiv_Enc) + sizeof(ZUCplain_Enc);

	//加密运算
	rv = (*pC_Encrypt)(hSession, indata, indatalen, outdata, &outdatalen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Encrypt,rv);	

	if ((outdatalen != sizeof(ZUCcipher_Enc)) || (memcmp(outdata, ZUCcipher_Enc, outdatalen)))
	{
		printf("Error: ZUC Encrypt Not Match.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: ZUC Encrypt Not Match.<br>"); 
		bRtn = false;
		goto END;
	}

	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);	

	indatalen = sizeof(indata);
	memset(indata, 0, indatalen);

	//将IV拼接到明文之前
	memcpy(indata, ZUCiv_Enc, sizeof(ZUCiv_Enc));
	memcpy(indata+sizeof(ZUCiv_Enc), ZUCplain_Enc, sizeof(ZUCplain_Enc));
	indatalen = sizeof(ZUCiv_Enc) + sizeof(ZUCplain_Enc);
	
	rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);

	if ((outdatalen != sizeof(ZUCcipher_Enc)) || (memcmp(outdata, ZUCcipher_Enc, outdatalen)))
	{
		printf("Error: ZUC Encrypt Not Match.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: ZUC Encrypt Not Match.<br>"); 
		bRtn = false;
		goto END;
	}

	rv = (*pC_EncryptFinal)(hSession, outdata, &outdatalen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);	

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);	

	bRtn = true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCEncrypt);
	
	return bRtn;
}

bool xtest_ZUCEncrypt0607()
{
	bool bRtn = false;
	CK_RV rv=0;
	unsigned int Value;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_BYTE nSessKeyID_Dec = CK_SESSKEY_PRESET_ID0;
	//加密参数
	unsigned char	ZUCkeyVal_Enc[]={
	0xe5,0xbd,0x3e,0xa0,
	0xeb,0x55,0xad,0xe8,
	0x66,0xc6,0xac,0x58,
	0xbd,0x54,0x30,0x2a};

	unsigned char	ZUCplain_Enc[100] = {0x14,0xa8,0xef,0x69,		0x3d,0x67,0x85,0x07,		0xbb,0xe7,0x27,0x0a,		0x7f,0x67,0xff,0x50,		0x06,0xc3,0x52,0x5b,		0x98,0x07,0xe4,0x67,		0xc4,0xe5,0x60,0x00,		0xba,0x33,0x8f,0x5d,		0x42,0x95,0x59,0x03,		0x67,0x51,0x82,0x22,		0x46,0xc8,0x0d,0x3b,		0x38,0xf0,0x7f,0x4b,		0xe2,0xd8,0xff,0x58,		0x05,0xf5,0x13,0x22,		0x29,0xbd,0xe9,0x3b,		0xbb,0xdc,0xaf,0x38,		0x2b,0xf1,0xee,0x97,		0x2f,0xbf,0x99,0x77,		0xba,0xda,0x89,0x45,		0x84,0x7a,0x2a,0x6c,		0x9a,0xd3,0x4a,0x66,		0x75,0x54,0xe0,0x4d,		0x1f,0x7f,0xa2,0xc3,		0x32,0x41,0xbd,0x8f,		0x01,0xba,0x22,0x0d};
	unsigned char	ZUCcipher_Enc[100] = {0x13,0x1d,0x43,0xe0,		0xde,0xa1,0xbe,0x5c,		0x5a,0x1b,0xfd,0x97,		0x1d,0x85,0x2c,0xbf,		0x71,0x2d,0x7b,0x4f,		0x57,0x96,0x1f,0xea,		0x32,0x08,0xaf,0xa8,		0xbc,0xa4,0x33,0xf4,		0x56,0xad,0x09,0xc7,		0x41,0x7e,0x58,0xbc,		0x69,0xcf,0x88,0x66,		0xd1,0x35,0x3f,0x74,		0x86,0x5e,0x80,0x78,		0x1d,0x20,0x2d,0xfb,		0x3e,0xcf,0xf7,0xfc,		0xbc,0x3b,0x19,0x0f,		0xe8,0x2a,0x20,0x4e,		0xd0,0xe3,0x50,0xfc,		0x0f,0x6f,0x26,0x13,		0xb2,0xf2,0xbc,0xa6,		0xdf,0x5a,0x47,0x3a,		0x57,0xa4,0xa0,0x0d,		0x98,0x5e,0xba,0xd8,		0x80,0xd6,0xf2,0x38,		0x64,0xa0,0x7b,0x01};

	unsigned char	ZUCiv_Enc[] = {0x00, 0x05, 0x68, 0x23, 0x38};
	unsigned char	ididididi1[] = {0x00, 0x05, 0x68, 0x23, 0x38};
	unsigned char	ididididi2[] = {0x20, 0x25, 0x28, 0x23, 0x28};
	CK_ATTRIBUTE ZUCkeyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(ZUCkeyType)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,ZUCkeyVal_Enc,sizeof(ZUCkeyVal_Enc)},
		//{CKA_VALUE,ZUCkeyVal_Enc,sizeof(ZUCkeyVal_Enc)},
		//{CKA_ID, ididididi1,sizeof(ididididi1)},
		//{CKA_ID, ididididi2,sizeof(ididididi2)},		
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(nSessKeyID_Dec)}
	};

	CK_MECHANISM ZUCmechanism_Enc = {CKM_ZUC_CALC, ZUCiv_Enc, sizeof(ZUCiv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);


	CK_BYTE getValue[16]={0};
	//CK_ATTRIBUTE secFindKeyTemplate[] = {
	//	{CKA_CLASS, &getValue, sizeof(keyClass)}
	//};

	CK_ATTRIBUTE secGetKeyTemplate[] = {
		{CKA_ID, getValue, sizeof(getValue)}
	};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCEncrypt);

	//对称加密初始化
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Enc, sizeof(ZUCkeyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	indatalen = sizeof(indata);
	memset(indata, 0, indatalen);

	//将IV拼接到明文之前
	memcpy(indata, ZUCiv_Enc, sizeof(ZUCiv_Enc));
	memcpy(indata+sizeof(ZUCiv_Enc), ZUCplain_Enc, sizeof(ZUCplain_Enc));
	indatalen = sizeof(ZUCiv_Enc) + sizeof(ZUCplain_Enc);

	//加密运算
	rv = (*pC_Encrypt)(hSession, indata, indatalen, outdata, &outdatalen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Encrypt,rv);	

	if ((outdatalen != sizeof(ZUCcipher_Enc)) || (memcmp(outdata, ZUCcipher_Enc, outdatalen)))
	{
		printf("Error: ZUC Encrypt Not Match.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: ZUC Encrypt Not Match.<br>"); 
		bRtn = false;
		goto END;
	}

	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);	

	indatalen = sizeof(indata);
	memset(indata, 0, indatalen);

	//将IV拼接到明文之前
	memcpy(indata, ZUCiv_Enc, sizeof(ZUCiv_Enc));
	memcpy(indata+sizeof(ZUCiv_Enc), ZUCplain_Enc, sizeof(ZUCplain_Enc));
	indatalen = sizeof(ZUCiv_Enc) + sizeof(ZUCplain_Enc);
	
	rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);

	if ((outdatalen != sizeof(ZUCcipher_Enc)) || (memcmp(outdata, ZUCcipher_Enc, outdatalen)))
	{
		printf("Error: ZUC Encrypt Not Match.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: ZUC Encrypt Not Match.<br>"); 
		bRtn = false;
		goto END;
	}

	rv = (*pC_EncryptFinal)(hSession, outdata, &outdatalen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);	

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);	

	bRtn = true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCEncrypt);
	
	return bRtn;
}

bool xtest_ZUCDecrypt()
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_BYTE nSessKeyID_Dec = CK_SESSKEY_PRESET_ID1;
	//解密参数
	unsigned char	ZUCkeyVal_Dec[]={0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};

	unsigned char	ZUCplain_Dec[100] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d};//, \
		0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03};
	unsigned char	ZUCcipher_Dec[100] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, 0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01};//, \
		0x9b, 0x48, 0xac, 0xd1, 0xfe, 0xf3, 0x01, 0x1f, 0x5e, 0x22, 0xd2, 0x97, 0x75, 0xcc, 0x94, 0xc2, 0xca, 0x6f, 0xb0, 0x9c, 0x2d, 0x86, 0xd6, 0xd0, 0x18, 0x3c, 0x24, 0x34, 0x09, 0x73, 0x5b, 0xad, 0x57, 0x5e, 0x9f, 0x91};

	unsigned char	ZUCiv_Dec[]={0x00, 0x05, 0x68, 0x23, 0x38};

	CK_ATTRIBUTE ZUCkeyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,ZUCkeyVal_Dec,sizeof(ZUCkeyVal_Dec)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(CK_BYTE)}
	};
	CK_MECHANISM ZUCmechanism_Dec = {CKM_ZUC_CALC, ZUCiv_Dec, sizeof(ZUCiv_Dec)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;

	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCDecrypt);

	//对称解密初始化
	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Dec, sizeof(ZUCkeyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);	

	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	indatalen1 = sizeof(indata1);
	memset(indata1, 0, indatalen1);

	//将IV拼接到密文之前
	memcpy(indata1, ZUCiv_Dec, sizeof(ZUCiv_Dec));
	memcpy(indata1+sizeof(ZUCiv_Dec), ZUCcipher_Dec, sizeof(ZUCcipher_Dec));
	indatalen1 = sizeof(ZUCiv_Dec) + sizeof(ZUCcipher_Dec);

	rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);		
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);	

	if ((outdatalen1 != sizeof(ZUCplain_Dec)) || (memcmp(outdata1, ZUCplain_Dec, outdatalen1)))
	{
		printf("Error: ZUC Decrypt.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: ZUC Decrypt.<br>");
		bRtn = false;
		goto END;
	}
	
	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);	
	
	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);	
	bRtn = true;

END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCDecrypt);
	
	return bRtn;
}


bool xtest_ZUCPerformance(int looptime,int datalen)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_BYTE nSessKeyID_Enc = CK_SESSKEY_PRESET_ID1;
	CK_BYTE nSessKeyID_Dec = CK_SESSKEY_PRESET_ID0;
	//加密参数
	unsigned char	ZUCkeyVal_Enc[]={0xe5, 0xbd, 0x38, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};
	//unsigned char	ZUCkeyVal_Enc[]={0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};
	unsigned char	ZUCplain_Enc[128] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, \
	 		0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, 0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00};
	unsigned char	ZUCcipher_Enc[128] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, \
	 		0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01, 0x9b, 0x48, 0xac, 0xd1, 0xfe, 0xf3, 0x01, 0x1f, 0x5e, 0x22, 0xd2, 0x97, 0x75, 0xcc, 0x94, 0xc2, 0xca, 0x6f, 0xb0, 0x9c, 0x2d, 0x86, 0xd6, 0xd0, 0x18, 0x3c, 0x24, 0x34};	
	unsigned char	ZUCiv_Enc[] = {0x00, 0x05, 0x68, 0x23, 0x38};

	CK_ATTRIBUTE ZUCkeyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,ZUCkeyVal_Enc,sizeof(ZUCkeyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)}
	};
	CK_MECHANISM ZUCmechanism_Enc = {CKM_ZUC_CALC, ZUCiv_Enc, sizeof(ZUCiv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	//解密参数
	unsigned char	ZUCkeyVal_Dec[]={0xe5, 0xbd, 0x38, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};
	//unsigned char	ZUCkeyVal_Dec[]={0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};
	unsigned char	ZUCplain_Dec[128] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, \
	 		0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, 0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00};
	unsigned char	ZUCcipher_Dec[128] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, \
			0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01, 0x9b, 0x48, 0xac, 0xd1, 0xfe, 0xf3, 0x01, 0x1f, 0x5e, 0x22, 0xd2, 0x97, 0x75, 0xcc, 0x94, 0xc2, 0xca, 0x6f, 0xb0, 0x9c, 0x2d, 0x86, 0xd6, 0xd0, 0x18, 0x3c, 0x24, 0x34};

	unsigned char	ZUCiv_Dec[]={0x00, 0x05, 0x68, 0x23, 0x38};

	CK_ATTRIBUTE ZUCkeyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,ZUCkeyVal_Dec,sizeof(ZUCkeyVal_Dec)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(CK_BYTE)}
	};
	CK_MECHANISM ZUCmechanism_Dec = {CKM_ZUC_CALC, ZUCiv_Dec, sizeof(ZUCiv_Dec)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);

	unsigned int i = 0,j=0;
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCPerformance);

	srand( (unsigned)time( NULL ) );//随机数初始化

	//对称加密初始化
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Enc, sizeof(ZUCkeyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject_Enc,rv);

	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Dec, sizeof(ZUCkeyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject_Dec,rv);

	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	printf("Datalen=%d.\n",datalen);
	nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);
	for (i=0; i<looptime; i++)
	{
		
		//生成随机数明文	
		RandomGenerate(ZUCplain_Enc,datalen);
		//生成随机数初始向量
		RandomGenerate(ZUCiv_Enc,5);
		memcpy(ZUCiv_Dec,ZUCiv_Enc,5);

		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv_Enc, sizeof(ZUCiv_Enc));
		memcpy(indata+sizeof(ZUCiv_Enc), ZUCplain_Enc, datalen);
		indatalen = sizeof(ZUCiv_Enc) + datalen;
		
		/*******************加密过程**********************/
		Utilsgettime(&ttc1);
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);
		//tt2.printn("ZUC Encrypt", i);

		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv_Dec, sizeof(ZUCiv_Dec));
		memcpy(indata1+sizeof(ZUCiv_Dec), outdata, outdatalen);
		indatalen1 = sizeof(ZUCiv_Dec) + outdatalen;

		/******************解密过程***********************/
		Utilsgettime(&ttc1);
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);
		//tt2.printn("ZUC Decrypt", i);

		if ((outdatalen1 != datalen) || (memcmp(outdata1, ZUCplain_Enc, outdatalen1)))
		{
			printf("Error: ZUCPerformance.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: ZUCPerformance.<br>");
			bRtn = false;
			goto END;
		}

	}
	rv = (*pC_EncryptFinal)(hSession, outdata, &outdatalen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);

	Utilsprint(&ttc3,"ZUC Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"ZUC Encrypt", looptime);
	Utilsprint(&ttc4,"ZUC Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"ZUC Decrypt", looptime);

	bRtn = true;	
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCPerformance);
	return bRtn;
}


bool xtest_ZUCHashPerformance_withkey(int looptime,int datalen)
{
	CK_RV rv=0;
	bool bRtn=false;
	int i=0;

	CK_BYTE	ZUCkeyVal[]={0xc9, 0xe6, 0xce, 0xc4, 0x60, 0x7c, 0x72, 0xdb, 0x00, 0x0a, 0xef, 0xa8, 0x83, 0x85, 0xab, 0x0a};
	CK_BYTE	ZUCiv[]={0xa9, 0x40, 0x59, 0xda, 0x2a};

	CK_BYTE digData[32],digData1[32];
	CK_ULONG ulDigLen=sizeof(digData);
	CK_ULONG ulDigLen1=sizeof(digData1);

	//CK_MECHANISM mechanism={CKM_HASH_ZUC_CALC,NULL_PTR,0};	
	CK_MECHANISM_PTR pPmechanism = (CK_MECHANISM_PTR)malloc(sizeof(CK_MECHANISM));
	memset(pPmechanism, 0 , sizeof(CK_MECHANISM));
	pPmechanism->mechanism = CKM_HASH_ZUC_CALC;
	pPmechanism->pParameter = (CK_BYTE_PTR)malloc(sizeof(CK_BYTE)*(16+5));
	memcpy(pPmechanism->pParameter,ZUCkeyVal,sizeof(CK_BYTE)*16);
	memcpy((CK_BYTE_PTR)pPmechanism->pParameter+sizeof(CK_BYTE)*16,ZUCiv,sizeof(CK_BYTE)*5);
	pPmechanism->ulParameterLen = 16+5;
	//UtilsPrintData(VNAME(pPmechanism->pParameter),21,0);

	CK_BYTE srcData[240] ={0x98, 0x3b, 0x41, 0xd4, 0x7d, 0x78, 0x0c, 0x9e, 0x1a, 0xd1, 0x1d, 0x7e, 0xb7, 0x03, 0x91, 0xb1};
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};
	srand( (unsigned)time( NULL ) );//随机数初始化
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCHashPerformanceWithKey);

	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);

	for(i=0;i<looptime;i++)
	{
		RandomGenerate(srcData,datalen);
		Utilsgettime(&ttc1);
		rv=(*pC_DigestInit)(hSession,pPmechanism);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestInit,rv);

		ulDigLen=sizeof(digData);		
		rv=(*pC_Digest)(hSession,srcData,datalen,digData,&ulDigLen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Digest,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);

		Utilsgettime(&ttc1);
		rv=(*pC_DigestInit)(hSession,pPmechanism);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestInit,rv);
		rv=(*pC_DigestUpdate)(hSession,srcData,datalen/2);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestUpdate,rv);
		rv=(*pC_DigestUpdate)(hSession,srcData+datalen/2,datalen-datalen/2);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestUpdate,rv);			
		ulDigLen1=sizeof(digData1);
		rv=(*pC_DigestFinal)(hSession,digData1,&ulDigLen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestFinal,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);

		if (ulDigLen!=ulDigLen1)
		{
			printf("test_digest failed.两次长度不一致\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength , "test_digest failed.两次长度不一致<br>");
			bRtn=false;
			goto END;
		}
		if (memcmp(digData, digData1, ulDigLen))
		{
			printf("test_digest failed.两次加密数据不一样\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength , "test_digest failed.两次加密数据不一样<br>");
			bRtn=false;
			goto END;
		}	
		//UtilsPrintData(VNAME(digData1),sizeof(digData1),0);	
	}
	Utilsprint(&ttc3,"ZUC Hash Digest", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"ZUC Hash Digest", looptime);
	Utilsprint(&ttc4,"ZUC Hash Update", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"ZUC Hash Update", looptime);
	bRtn=true;
END:
	free(pPmechanism->pParameter);
	free(pPmechanism);
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCHashPerformanceWithKey);
	return bRtn;
}


bool xtest_SM3Encrypt()
{
	bool bRtn = false;
	CK_RV rv = 0;
	int i = 0;
	CK_BYTE srcData[64] = {0};
	for(i = 0;i < sizeof(srcData)/4;i++)
	{
		memcpy(&srcData[i*4],"abcd",4);
	}
	unsigned char pszCorrectResult_SM3[]={0xde, 0xbe, 0x9f, 0xf9, 0x22, 0x75, 0xb8, 0xa1, 0x38, 0x60, 0x48, 0x89, 0xc1, 0x8e, 0x5a, 0x4d, 0x6f, 0xdb, 0x70, 0xe5, 0x38, 0x7e, 0x57, 0x65, 0x29, 0x3d, 0xcb, 0xa3, 0x9c, 0x0c, 0x57, 0x32};
	CK_BYTE digData[100];
	CK_ULONG ulDigLen=sizeof(digData);

	CK_MECHANISM mechanism={CKM_HASH_SM3,NULL_PTR,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM3Encrypt);

	rv=(*pC_DigestInit)(hSession,&mechanism);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestInit,rv);
		
	ulDigLen=sizeof(digData);
	rv=(*pC_Digest)(hSession,srcData,sizeof(srcData),digData,&ulDigLen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Digest,rv);

	if (memcmp(pszCorrectResult_SM3, digData, ulDigLen))
	{
		printf("test_digest failed.SM3 Result is wrong1!!! \n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "test_digest failed.SM3 Result is wrong1! <br>");
		bRtn= false;
		goto END;
	}
	
	rv=(*pC_DigestInit)(hSession,&mechanism);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestInit,rv);

	rv=(*pC_DigestUpdate)(hSession,srcData,sizeof(srcData)/2);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestUpdate1,rv);

	rv=(*pC_DigestUpdate)(hSession,srcData+sizeof(srcData)/2,sizeof(srcData)-sizeof(srcData)/2);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestUpdate2,rv);
		
	ulDigLen=sizeof(digData);
	rv=(*pC_DigestFinal)(hSession,digData,&ulDigLen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestFinal,rv);
	
	if (memcmp(pszCorrectResult_SM3, digData, ulDigLen))
	{
		printf("test_digest failed.SM3 Result is wrong2!!! \n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "test_digest failed.SM3 Result is wrong2! <br>");
		bRtn= false;
		goto END;
	}

	bRtn=true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM3Encrypt);

	
	return bRtn;
}


bool xtest_SM3Performance(int looptime,int datalen)
{
	CK_RV rv=0;
	bool bRtn=false;
	int i=0;
	CK_ULONG ulSlotCountXX=0;	

	CK_BYTE digData[100],digData1[100];
	CK_ULONG ulDigLen=sizeof(digData);
	CK_ULONG ulDigLen1=sizeof(digData1);

	CK_MECHANISM mechanism={CKM_HASH_SM3,NULL_PTR,0};	

	CK_BYTE srcData[4096]={0};
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM3Performance);
	
	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);
	for(i=0;i<looptime;i++)
	{
			srand( (unsigned)time( NULL ) );//随机数初始化
			//生成随机数明文	
			RandomGenerate(srcData,datalen);
			Utilsgettime(&ttc1);
			rv=(*pC_DigestInit)(hSession,&mechanism);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestInit1,rv);
			ulDigLen=sizeof(digData);

			
			rv=(*pC_Digest)(hSession,srcData,datalen,digData,&ulDigLen);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Digest,rv);
			Utilsgettime(&ttc2);			
			UtilsTimeSubstracted(&ttc2,&ttc1);			
			UtilsTimeAdded(&ttc3,&ttc2);

			Utilsgettime(&ttc1);
			rv=(*pC_DigestInit)(hSession,&mechanism);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestInit2,rv);

			rv=(*pC_DigestUpdate)(hSession,srcData,datalen/2);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestUpdate1,rv);

			rv=(*pC_DigestUpdate)(hSession,srcData+datalen/2,datalen-datalen/2);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestUpdate2,rv);

			rv=(*pC_DigestFinal)(hSession,digData1,&ulDigLen1);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DigestFinal,rv);
			Utilsgettime(&ttc2);
			UtilsTimeSubstracted(&ttc2,&ttc1);
			UtilsTimeAdded(&ttc4,&ttc2);
			if (ulDigLen!=ulDigLen1)
			{
				printf("test_digest failed.两次长度不一致\n");
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength , "test_digest failed.两次长度不一致<br>");
				bRtn = false;
				goto END;
			}
			if (memcmp(digData, digData1, ulDigLen))
			{
				printf("test_digest failed.两次加密数据不一样\n");
				nResultLength += sprintf(Testresult[nItemNumb]+nResultLength , "test_digest failed.两次加密数据不一样<br>");
				bRtn = false;
				goto END;
			}	
	}

	Utilsprint(&ttc3,"Digest", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"Digest", looptime);
	Utilsprint(&ttc4,"Digest", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"Digest", looptime);
	

	//printf("after pC_DigestFinal ok sleep\n");
	//sleep(30);

	//pC_Proxy_Final();

	//rv=pC_Proxy_Init();
	//RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Proxy_Init,rv);

	//rv = (*pC_Initialize)(NULL_PTR);
	//RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Initialize,rv);

	//rv=(*pC_GetSlotList)(CK_TRUE,NULL_PTR,&ulSlotCountXX);
	//RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetSlotList,rv);

	//rv=(*pC_GetSlotList)(CK_TRUE,pSlotList,&ulSlotCount);

	bRtn=true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM3Performance);
	return bRtn;
}


bool xtest_SM2GenPerformance(int looptime,int datalen)
{
	CK_RV rv=0;
	bool bRtn=false;
	CK_OBJECT_HANDLE hPublicKey, hPrivateKey;
	CK_OBJECT_HANDLE * phPublicKey = &hPublicKey, *phPrivateKey = &hPrivateKey;

	int i=0,j=0;
	CK_BYTE pData[128]={0};

	CK_MECHANISM      mechanism = {CKM_ECC_KEY_PAIR_GEN, NULL_PTR, 0};
	CK_UINT     modulusBits = 256;
	CK_BYTE      subject[] = {0x00,0x01,0x02};
	CK_BYTE      idid[] = {0x00,0x01,0x02,0x03};
	CK_BBOOL     ttrue = CK_TRUE, ffalse = CK_FALSE;
	CK_KEY_TYPE  keyType=CKK_ECC;
	CK_ATTRIBUTE publicKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_SUBJECT, subject, sizeof(subject)},
		//{CKA_ID, idid, sizeof(idid)},
		{CKA_LABEL, idid, sizeof(idid)},
		//{CKA_WRAP, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)}
	};
	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_SUBJECT, subject, sizeof(subject)},
		//{CKA_ID, idid, sizeof(idid)},
		{CKA_LABEL, idid, sizeof(idid)},
		{CKA_SENSITIVE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		//{CKA_UNWRAP, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)}
	};

	CK_MECHANISM  mechanismcalc = {CKM_ECC_CALC, NULL_PTR, 0};
	CK_BYTE       pOutData[256];   
	CK_ULONG      ulOutDataLen=sizeof(pOutData);
	CK_BYTE       pOutData1[256];   
	CK_ULONG      ulOutDataLen1=sizeof(pOutData1);

	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0},ttc5={0,0},ttc6={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2GenPerformance);

	//产生密钥对
	Utilsgettime(&ttc1);
	rv = (*pC_GenerateKeyPair)(hSession, &mechanism,
						  publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  phPublicKey, phPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPair,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"GenKeyPair",1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb] + nResultLength,"GenKeyPair",1);

	srand( (unsigned)time( NULL ) );//随机数初始化		
	printf("Datalen=%d.\n",datalen);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Datalen=%d.<br>",datalen);
	for(j = 0; j < looptime; j++)
	{
		RandomGenerate(pData,datalen);		
		memset(pOutData,0,ulOutDataLen);
		//公钥加密	
		Utilsgettime(&ttc1);
		rv=(*pC_EncryptInit)(hSession,&mechanismcalc,hPublicKey);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

		rv=(*pC_Encrypt)(hSession,pData,datalen,pOutData,&ulOutDataLen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Encrypt,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);

		//私钥解密
		Utilsgettime(&ttc1);
		rv=(*pC_DecryptInit)(hSession,&mechanismcalc,hPrivateKey);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);
		
		rv=(*pC_Decrypt)(hSession,pOutData,ulOutDataLen,pOutData1,&ulOutDataLen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Decrypt,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);

		if((ulOutDataLen1!=datalen)||(memcmp(pOutData1,pData,datalen)!=0))
		{
			printf("test_ECC_pubpricalc failed,解密数据与原始数据不一致.");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength , "test_ECC_pubpricalc failed,解密数据与原始数据不一致.<br>");
			bRtn = false;
			goto END;
		}

		//签名验签使用的是SM3的哈希值，固定为32字节
		if(datalen==32)
		{
				//私钥签名
				ulOutDataLen=sizeof(pOutData);
				memset(pOutData,0,ulOutDataLen);

				Utilsgettime(&ttc1);
				rv=(*pC_SignInit)(hSession,&mechanismcalc,hPrivateKey);
				RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_SignInit,rv);

				rv=(*pC_Sign)(hSession,pData,datalen,pOutData,&ulOutDataLen);
				RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Sign,rv);
				Utilsgettime(&ttc2);
				UtilsTimeSubstracted(&ttc2,&ttc1);
				UtilsTimeAdded(&ttc5,&ttc2);

				//公钥验签
				Utilsgettime(&ttc1);
				rv=(*pC_VerifyInit)(hSession,&mechanismcalc,hPublicKey);
				RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_VerifyInit,rv);
	
				rv=(*pC_Verify)(hSession,pData,datalen,pOutData,ulOutDataLen);
				RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Verify,rv);
				Utilsgettime(&ttc2);
				UtilsTimeSubstracted(&ttc2,&ttc1);
				UtilsTimeAdded(&ttc6,&ttc2);
		}

	}

	Utilsprint(&ttc3,"Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"Encrypt", looptime);
	Utilsprint(&ttc4,"Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"Decrypt", looptime);


	if(datalen==32)
	{
		Utilsprint(&ttc5,"Sign", looptime);
		nResultLength += Utilssprint(&ttc5,Testresult[nItemNumb]+ nResultLength,"Sign", looptime);
		Utilsprint(&ttc6,"Verify", looptime);
		nResultLength += Utilssprint(&ttc6,Testresult[nItemNumb]+ nResultLength,"Verify", looptime);
	}
	//rv = pC_DestroyObject(hSession,hPublicKey);
	//RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);

	//rv = pC_DestroyObject(hSession,hPrivateKey);
	//RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);
	bRtn=true;
END:
    ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2GenPerformance);
	return bRtn;
}


bool xtest_SM2ImportPerformance(int looptime,int datalen)
{
	CK_RV rv=0;
	bool bRtn=false;
	CK_OBJECT_HANDLE hPublicKey, hPrivateKey;
	CK_OBJECT_HANDLE * phPublicKey=&hPublicKey,*phPrivateKey=&hPrivateKey;

	int i=0;	
	CK_BYTE pData[256]={0};

	CK_BYTE     temp_pub_x[32]={
		0xec,0x91,0x81,0x8d,0xe0,0xb7,0x01,0x21,0x73,0xf5,0x1c,0x33,0x75,0x43,0x6e,0x43,
			0xb6,0xa9,0xa2,0x6a,0xbd,0x6d,0xbc,0xb7,0x9f,0x85,0x1c,0xde,0xaf,0x7a,0x0f,0x6c
	};
	CK_BYTE     temp_pub_y[32]={
		0xcb,0xf4,0xb5,0xa1,0x5f,0xb8,0x7e,0x60,0xfc,0x0b,0x3a,0x92,0x3d,0x12,0xe8,0x66,
			0x36,0x4a,0x93,0x5f,0xfb,0x30,0x84,0x2b,0xc9,0x13,0x9e,0xbd,0x2d,0xdc,0xe9,0x61
	};
	CK_BYTE     temp_prv[32]={
		0xc5,0x6a,0x2b,0x58,0xa0,0x94,0xef,0x24,0x41,0x03,0x79,0x45,0xba,0xb1,0x39,0x8c,
			0xc0,0xdf,0x9f,0xc4,0xf9,0x9e,0x9a,0x60,0x2c,0xd8,0x6f,0xc2,0xc3,0x88,0xad,0x0c
	};
	
	CK_BBOOL     ttrue = CK_TRUE;
	CK_KEY_TYPE  keyType=CKK_ECC;
	CK_OBJECT_CLASS pubclass=CKO_PUBLIC_KEY,priclass=CKO_PRIVATE_KEY;
	CK_UINT     modulusBits = 256;
	CK_BYTE     id[] = {0x01,0x01,0x02,0x03};

	CK_ATTRIBUTE publicKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, temp_pub_x, sizeof(temp_pub_x)},
		{CKA_ECC_Y_COORDINATE, temp_pub_y, sizeof(temp_pub_y)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_PRIVATE, temp_prv, sizeof(temp_prv)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE pubFindKeyTemplate[] = {
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE prvFindKeyTemplate[] = {
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_BYTE get_pub_x[32] = {0};
	CK_BYTE get_pub_y[32] = {0};
	CK_ATTRIBUTE pubGetKeyTemplate[] = {
		{CKA_ECC_X_COORDINATE, get_pub_x, sizeof(get_pub_x)},
		{CKA_ECC_Y_COORDINATE, get_pub_y, sizeof(get_pub_y)}
	};

	CK_BYTE get_prv[32] = {0};
	CK_ATTRIBUTE prvGetKeyTemplate[] = {
		{CKA_ECC_PRIVATE, get_prv, sizeof(get_prv)}
	};

	CK_OBJECT_HANDLE hObject = NULL_PTR;
	CK_ULONG ulObjectCount = 0;

	CK_MECHANISM  mechanismcalc = {CKM_ECC_CALC, NULL_PTR, 0};
	CK_BYTE       pOutData[256];   
	CK_ULONG      ulOutDataLen=sizeof(pOutData);
	CK_BYTE       pOutData1[256];   
	CK_ULONG      ulOutDataLen1=sizeof(pOutData1);
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0},ttc5={0,0},ttc6={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2ImportPerformance);

	//创建公钥对象
	Utilsgettime(&ttc1);
	rv = (*pC_CreateObject)(hSession, 
						  publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  phPublicKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject1,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"Create Public Key",1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb]+ nResultLength,"Create Public Key",1);

	//创建私钥对象
	Utilsgettime(&ttc1);
	rv = (*pC_CreateObject)(hSession, 
						  privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  phPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject2,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"Create Private Key",1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb]+ nResultLength,"Create Private Key",1);

	//查找私钥对象	
	hObject = NULL_PTR;
	ulObjectCount = 0;
	rv = (*pC_FindObjectsInit)(hSession, prvFindKeyTemplate, sizeof(prvFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit1,rv);

	rv = (*pC_FindObjects)(hSession, &hObject, 1, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects1,rv);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal1,rv);

	//查找公钥对象，并读取公钥对象属性
	hObject = NULL_PTR;
	ulObjectCount = 0;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate, sizeof(pubFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit2,rv);

	rv = (*pC_FindObjects)(hSession, &hObject, 1, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects2,rv);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal2,rv);

	rv = (*pC_GetAttributeValue)(hSession, hObject, pubGetKeyTemplate, sizeof(pubGetKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValue2,rv);

	srand( (unsigned)time( NULL ) );//随机数初始化
	printf("Datalen=%d.\n",datalen);
	nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);
	for(i=0;i<looptime;i++)
	{
		RandomGenerate(pData,datalen);		
		memset(pOutData,0,ulOutDataLen);
		//公钥加密	
		Utilsgettime(&ttc1);
		rv=(*pC_EncryptInit)(hSession,&mechanismcalc,hPublicKey);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

		rv=(*pC_Encrypt)(hSession,pData,datalen,pOutData,&ulOutDataLen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Encrypt,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);

		//私钥解密
		Utilsgettime(&ttc1);
		rv=(*pC_DecryptInit)(hSession,&mechanismcalc,hPrivateKey);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);
	
		rv=(*pC_Decrypt)(hSession,pOutData,ulOutDataLen,pOutData1,&ulOutDataLen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Decrypt,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);

		if((ulOutDataLen1!=datalen)||(memcmp(pOutData1,pData,datalen)!=0))
		{
				printf("test_ECC_pubpricalc failed,解密数据与原始数据不一致.");
				nResultLength += sprintf(Testresult[nItemNumb]+nResultLength , "test_ECC_pubpricalc failed,解密数据与原始数据不一致.<br>");
				bRtn = false;
				goto END;
		}
		//签名验签使用的是SM3的哈希值，固定为32字节
		if(datalen==32)
		{
				//私钥签名
				ulOutDataLen=sizeof(pOutData);
				memset(pOutData,0,ulOutDataLen);

				Utilsgettime(&ttc1);
				rv=(*pC_SignInit)(hSession,&mechanismcalc,hPrivateKey);
				RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_SignInit,rv);

				rv=(*pC_Sign)(hSession,pData,datalen,pOutData,&ulOutDataLen);
				RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Sign,rv);
				Utilsgettime(&ttc2);
				UtilsTimeSubstracted(&ttc2,&ttc1);
				UtilsTimeAdded(&ttc5,&ttc2);
				//公钥验签
				Utilsgettime(&ttc1);
				rv=(*pC_VerifyInit)(hSession,&mechanismcalc,hPublicKey);
				RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_VerifyInit,rv);

				rv=(*pC_Verify)(hSession,pData,datalen,pOutData,ulOutDataLen);
				RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Verify,rv);
				Utilsgettime(&ttc2);
				UtilsTimeSubstracted(&ttc2,&ttc1);
				UtilsTimeAdded(&ttc6,&ttc2);
		}
	}

	Utilsprint(&ttc3,"Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"Encrypt", looptime);
	Utilsprint(&ttc4,"Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"Decrypt", looptime);

	if(datalen==32)
	{
		Utilsprint(&ttc5,"Sign", looptime);
		nResultLength += Utilssprint(&ttc5,Testresult[nItemNumb]+ nResultLength,"Sign", looptime);
		Utilsprint(&ttc6,"Verify", looptime);
		nResultLength += Utilssprint(&ttc6,Testresult[nItemNumb]+ nResultLength,"Verify", looptime);
	}
	rv = pC_DestroyObject(hSession,hPublicKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);

	rv = pC_DestroyObject(hSession,hPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);

	bRtn = true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2ImportPerformance);
	return bRtn;
}


bool xtest_SM2ImportDataVerify0606()
{
	CK_RV rv=0;
	bool bRtn=false;
	CK_OBJECT_HANDLE hPublicKey, hPrivateKey;
	CK_OBJECT_HANDLE * phPublicKey=&hPublicKey,*phPrivateKey=&hPrivateKey;

	int i=0;	
	CK_BYTE pData[256]={0};

	CK_BYTE     temp_pub_x[32]={
		0x1A,0x3B,0xB4,0x7B,
		0xD5,0x80,0x8C,0x7D,
		0x3C,0xF9,0x91,0xF3,
		0xDA,0xCC,0x08,0x0E,
		0xDC,0xEF,0x7F,0xD6,
		0x8F,0x33,0xCE,0x34,
		0x44,0x83,0x19,0x42,
		0x06,0xBD,0x31,0x6A,
	};
	CK_BYTE     temp_pub_y[32]={
		0xED,0x6A,0x6E,0x92,
		0x14,0x71,0x57,0xB7,
		0x09,0xB3,0x67,0x9D,
		0x3C,0xFE,0xF8,0x58,
		0x78,0x26,0x4D,0x8C,
		0xE0,0x0E,0xCF,0xE4,
		0xF4,0xE0,0x05,0x3D,
		0x1C,0x8B,0x4C,0x7C 
	};
	CK_BYTE     temp_prv[32]={
		0xF6,0x39,0xEA,0x67,
		0xD9,0xB2,0x54,0x7C,
		0x94,0xAC,0xBD,0x12,
		0x80,0xC7,0x60,0x2C,
		0x79,0x7E,0x12,0x89,
		0x41,0xD1,0x01,0xE1,
		0x9A,0x4B,0x0C,0xA9,
		0xE6,0x57,0xCD,0xDB
	};
	
	CK_BBOOL     ttrue = CK_TRUE;
	CK_KEY_TYPE  keyType=CKK_ECC;
	CK_OBJECT_CLASS pubclass=CKO_PUBLIC_KEY,priclass=CKO_PRIVATE_KEY;
	CK_UINT     modulusBits = 256;
	CK_BYTE     id[] = {0x01,0x01,0x02,0x03};

	CK_ATTRIBUTE publicKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, temp_pub_x, sizeof(temp_pub_x)},
		{CKA_ECC_Y_COORDINATE, temp_pub_y, sizeof(temp_pub_y)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_PRIVATE, temp_prv, sizeof(temp_prv)},
		{CKA_ID, id, sizeof(id)}
	};

	unsigned char ccccc[128] = 
	{
		0xC5,0x4C,0x64,0x62,
		0xA4,0xF2,0xE1,0xF6,
		0x80,0xA8,0x88,0x47,
		0x07,0x67,0x6C,0xD5,
		0xA4,0x8C,0xC7,0xF9,
		0x84,0x32,0x2B,0x3A,
		0xA7,0x64,0x12,0x62,
		0x0B,0x1E,0xC0,0x00,
		0x33,0x02,0x0A,0x54,
		0xAD,0xDF,0x95,0x30,
		0x5D,0x08,0x6C,0x66,
		0x76,0x97,0x32,0x6A,
		0xEA,0x08,0x2B,0x14,
		0x76,0x9F,0x0F,0xBC,
		0x6A,0xBB,0xAF,0x91,
		0x02,0xEF,0x29,0x4B,
		
		0x92,0xD3,0xE2,0x88,
		0x23,0xFF,0x52,0x3A,			
		0xDD,0x4F,0x08,0xC9,
		0xA5,0x31,0x6D,0xCF,
		0x79,0x7F,0x48,0xB8,
		0xDF,0x98,0xC0,0x86,
		0x1B,0x1E,0x67,0xCA,
		0xCB,0xE3,0x66,0x79,

		
		0x40,0x80,0x39,0x6D,
		0x05,0x07,0xA6,0xBF,
		0x30,0xD0,0x48,0x8E,
		0xB9,0xCC,0x59,0x87,
		0x76,0xE2,0x7E,0xB9,
		0x73,0x45,0xCB,0x77,
		0xD0,0x15,0x96,0x7E,
		0x64,0x55,0xFE,0x0B
		
	};

	CK_OBJECT_HANDLE hObject = NULL_PTR;
	CK_ULONG ulObjectCount = 0;

	CK_MECHANISM  mechanismcalc = {CKM_ECC_CALC, NULL_PTR, 0};
	CK_BYTE       pOutData[256];   
	CK_ULONG      ulOutDataLen=sizeof(pOutData);
	CK_BYTE       pOutData1[256];   
	CK_ULONG      ulOutDataLen1=sizeof(pOutData1);

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2ImportPerformance);


	ulOutDataLen1=32;

	if (SM2Init() != SM2_OK)
	{
		printf("SM2InitB fail\n");
	}

	if (SM2Decrypt(ccccc, 128, temp_prv, 32, pOutData1, &ulOutDataLen1) != SM2_OK)
	{
		printf("SM2Decrypt fail\n");
	}

	UtilsPrintData(VNAME(pOutData1),ulOutDataLen1,0);

	rv = (*pC_CreateObject)(hSession, 
						  privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  phPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject2,rv);

	
	//私钥解密
	rv=(*pC_DecryptInit)(hSession,&mechanismcalc,hPrivateKey);	
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);	
	rv=(*pC_Decrypt)(hSession,ccccc,128,pOutData1,&ulOutDataLen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Decrypt,rv);
	
	UtilsPrintData(VNAME(pOutData1),ulOutDataLen1,0);
	rv = pC_DestroyObject(hSession,hPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);

	bRtn = true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2ImportPerformance);
	return bRtn;
}


bool xtest_SM2PointMul1()
{

	bool	bRtn = false;
	CK_RV	rv=0;
	unsigned int i = 0, j = 0,k=0;
	
	unsigned char	ECCPriv1[32] = {0x65, 0x83, 0x2D, 0x7C, 0x7B, 0x27, 0x2E, 0x98, 0x57, 0xD1, 0x08, 0x4F, 0x3A, 0x75, 0x54, 0x0D, 0x40, 0xC2, 0x04, 0x23, 0x3A, 0x6A, 0x65, 0x6A, 0x15, 0x9D, 0x59, 0x52, 0x2D, 0x68, 0x4E, 0x3E};
	unsigned char	ECCGX1[32] = {0xD6, 0xA7, 0x78, 0x8C, 0x41, 0xC3, 0xA3, 0x66, 0x18, 0xE8, 0x02, 0x1E, 0xE1, 0xEB, 0x27, 0x2F, 0xA7, 0x24, 0x7F, 0x77, 0xF4, 0x7F, 0x40, 0xBE, 0x6A, 0x31, 0x02, 0x48, 0x41, 0xA0, 0x14, 0xC6};
	unsigned char	ECCGY1[32] = {0x39, 0xE2, 0xF0, 0xA4, 0x69, 0x22, 0xD3, 0x56, 0xBD, 0xB0, 0x39, 0x12, 0x26, 0xA9, 0xD1, 0x47, 0x40, 0x84, 0x3D, 0xFE, 0x57, 0x68, 0x0D, 0x28, 0x87, 0x44, 0xB7, 0xD8, 0x20, 0xE0, 0x32, 0xBB};

	unsigned char	ECCPriv2[32] = {0x6E, 0xDD, 0x1A, 0xD0, 0x2D, 0x5F, 0x4A, 0x8C, 0x10, 0x25, 0x42, 0xF2, 0x56, 0xCC, 0x30, 0xFC, 0x6C, 0x88, 0x67, 0xEB, 0x59, 0xC4, 0x67, 0xEC, 0x02, 0x36, 0x03, 0x5D, 0x2E, 0x17, 0x06, 0x9D};
	unsigned char	ECCGX2[32] = {0xD6, 0x5C, 0xA2, 0xDA, 0x1C, 0x05, 0x8E, 0xFA, 0xFD, 0x7E, 0x98, 0x4B, 0x86, 0x08, 0xF6, 0x24, 0x0D, 0xE8, 0x78, 0x03, 0x87, 0x98, 0x2D, 0x71, 0xF6, 0xFC, 0xB7, 0x87, 0x6E, 0xD2, 0x36, 0x58};
	unsigned char	ECCGY2[32] = {0x0B, 0xEB, 0x90, 0x5B, 0x18, 0x52, 0x3E, 0xDB, 0xE2, 0x46, 0x45, 0xCD, 0x08, 0x8F, 0x7B, 0x02, 0x5C, 0xCE, 0xE4, 0xBF, 0xD8, 0x46, 0x8A, 0xA5, 0x0F, 0x02, 0x2C, 0x55, 0x32, 0xEF, 0x88, 0xB5};

	unsigned char	ECCPriv3[32] = {0x3D, 0x73, 0x7C, 0x01, 0x52, 0x36, 0x79, 0xB0, 0x39, 0x95, 0x60, 0xD1, 0x27, 0x4E, 0x77, 0x7C, 0x59, 0xD0, 0x40, 0xBF, 0x28, 0x38, 0x0E, 0x33, 0x1F, 0xFA, 0x66, 0x79, 0x66, 0xF2, 0x02, 0x89};
	unsigned char	ECCGX3[32] = {0x38, 0xCD, 0xAD, 0xDD, 0xDA, 0x1C, 0x6C, 0x51, 0x35, 0x23, 0xCC, 0xB3, 0x2E, 0x0E, 0x28, 0x01, 0x26, 0x2B, 0x7F, 0x95, 0xF2, 0x5E, 0x85, 0x3C, 0x86, 0xE1, 0x01, 0x28, 0x6C, 0x82, 0xD7, 0x18};
	unsigned char	ECCGY3[32] = {0x6A, 0x1C, 0x8A, 0x61, 0xC4, 0xD3, 0xED, 0x61, 0x9D, 0xFA, 0xA0, 0x33, 0x20, 0x5C, 0x5F, 0x76, 0x9B, 0x3B, 0xE1, 0x2C, 0x8F, 0x15, 0x7C, 0x2F, 0x55, 0x75, 0xFF, 0xEE, 0xFB, 0x42, 0xA0, 0x64};

	unsigned char	ECCPriv4[32] = {0x23, 0xEA, 0x2D, 0x20, 0x4E, 0x69, 0x41, 0xD0, 0x50, 0x37, 0x01, 0xAB, 0x6A, 0xA7, 0x5F, 0x86, 0x2B, 0x1E, 0x7B, 0x11, 0x13, 0x61, 0x43, 0x68, 0x24, 0xDC, 0x74, 0xCA, 0x39, 0x86, 0x11, 0x5B};
	unsigned char	ECCGX4[32] = {0xC0, 0x54, 0x13, 0x71, 0xDA, 0xA7, 0x29, 0x6E, 0x04, 0xDF, 0x21, 0x90, 0xF6, 0xF3, 0x91, 0xCF, 0xBF, 0x3C, 0x4C, 0x2E, 0x42, 0xA2, 0x8B, 0x63, 0xBE, 0x9F, 0xE4, 0x0C, 0xFD, 0x9B, 0x11, 0x9E};
	unsigned char	ECCGY4[32] = {0x9B, 0xAA, 0x6A, 0xFF, 0xEB, 0x03, 0x2D, 0x7A, 0x82, 0xD9, 0x90, 0xA8, 0x36, 0x8D, 0x0B, 0x82, 0x35, 0x5A, 0x56, 0xD1, 0xBC, 0x14, 0xEA, 0xA2, 0xB4, 0x7E, 0x69, 0xF7, 0xB6, 0x21, 0x1E, 0xC3};

	unsigned char	ECCPriv5[32] = {0x71, 0x26, 0x15, 0x7E, 0x30, 0xF6, 0x29, 0xF2, 0x38, 0x5F, 0x0C, 0x82, 0x15, 0xC4, 0x7E, 0x53, 0x17, 0x37, 0x46, 0x91, 0x0B, 0x1E, 0x6B, 0xF3, 0x47, 0x12, 0x1F, 0xAE, 0x15, 0xA0, 0x57, 0xAC};
	unsigned char	ECCGX5[32] = {0xA9, 0x80, 0xD7, 0x8E, 0xEE, 0x50, 0x52, 0x84, 0x3D, 0xA8, 0x35, 0x45, 0x13, 0x1B, 0x5C, 0xDA, 0xE6, 0xD2, 0xE4, 0x6D, 0x2A, 0x9E, 0x6D, 0xA4, 0x46, 0x73, 0xE1, 0xAF, 0x97, 0x8E, 0x54, 0xF5};
	unsigned char	ECCGY5[32] = {0x6A, 0x97, 0xA2, 0x54, 0xC4, 0x6F, 0x92, 0xC2, 0x45, 0xD1, 0x71, 0x1D, 0xEE, 0x94, 0xEA, 0x2D, 0x21, 0xEB, 0xE2, 0x2E, 0x22, 0xC8, 0x05, 0x57, 0xA1, 0x4F, 0x90, 0x15, 0x79, 0x31, 0xCD, 0x56};

	unsigned char	ECCPriv6[32] = {0x40, 0x4D, 0x37, 0xAC, 0x03, 0x19, 0x4A, 0x61, 0x0E, 0xA2, 0x3F, 0x97, 0x23, 0xD3, 0x56, 0x5D, 0x39, 0x20, 0x26, 0x31, 0x51, 0x8A, 0x75, 0x7F, 0x0D, 0x11, 0x67, 0xC8, 0x49, 0x4C, 0x5F, 0x54};
	unsigned char	ECCGX6[32] = {0xD8, 0x67, 0xE2, 0x55, 0xF4, 0x60, 0x80, 0x04, 0xCD, 0xCF, 0xBB, 0x0B, 0xC4, 0x6C, 0xB3, 0x97, 0xF4, 0xB2, 0xBA, 0x73, 0x4D, 0x70, 0xEE, 0x89, 0x76, 0xFC, 0x0D, 0x86, 0x4B, 0xC8, 0x32, 0x86};
	unsigned char	ECCGY6[32] = {0x5B, 0xE8, 0x39, 0xF9, 0x23, 0x13, 0x47, 0x63, 0xA6, 0x5A, 0x11, 0xFB, 0xD6, 0xC4, 0x5A, 0xAB, 0x10, 0x0D, 0x5A, 0x78, 0x87, 0xC4, 0x00, 0x09, 0x19, 0xE2, 0x85, 0xFA, 0x6C, 0xB7, 0xB7, 0x60};

	unsigned char	ECCPriv7[32] = {0x48, 0xC4, 0x41, 0x7B, 0x18, 0x50, 0x5C, 0xA6, 0x77, 0xD6, 0x40, 0x6B, 0x66, 0x40, 0x67, 0x5C, 0x60, 0x1E, 0x00, 0x22, 0x4B, 0x04, 0x66, 0xF4, 0x1D, 0x8E, 0x6C, 0xF8, 0x50, 0xD8, 0x27, 0x6D};
	unsigned char	ECCGX7[32] = {0x7D, 0x64, 0xDE, 0x59, 0x4B, 0x7D, 0xAA, 0x94, 0xD0, 0xB0, 0xEA, 0x57, 0x8C, 0x70, 0x98, 0x27, 0xFB, 0x94, 0x17, 0xF6, 0x91, 0xDB, 0x0B, 0x6A, 0x72, 0x93, 0x33, 0x05, 0x6C, 0x1C, 0x7F, 0x9B};
	unsigned char	ECCGY7[32] = {0xC8, 0x2A, 0x65, 0x82, 0x44, 0xBD, 0xFF, 0x22, 0x43, 0x0C, 0x5E, 0xAE, 0x11, 0x0A, 0x48, 0x59, 0xF3, 0xB8, 0x53, 0x83, 0x9D, 0xAA, 0xE8, 0x8B, 0x35, 0xA8, 0x99, 0x53, 0xD1, 0xD5, 0x54, 0x40};

	unsigned char	ECCPriv8[32] = {0x2E, 0x2F, 0x1B, 0xFC, 0x5E, 0x58, 0x4B, 0x89, 0x71, 0x0F, 0x2B, 0xBE, 0x54, 0xB8, 0x3E, 0x49, 0x5F, 0xB6, 0x2D, 0xD4, 0x4E, 0x27, 0x70, 0x7A, 0x0F, 0x7E, 0x7E, 0x60, 0x26, 0xD1, 0x34, 0x50};
	unsigned char	ECCGX8[32] = {0xDD, 0x91, 0xDA, 0xF8, 0xAD, 0x1A, 0x57, 0xFF, 0x4B, 0xDD, 0xA8, 0x12, 0xB8, 0x61, 0x19, 0x10, 0x7D, 0x88, 0x04, 0xBE, 0x4E, 0x01, 0xCD, 0xAC, 0x0A, 0x30, 0x44, 0xB7, 0xB5, 0x35, 0x51, 0x93};
	unsigned char	ECCGY8[32] = {0x0A, 0xC2, 0x86, 0xB7, 0x8B, 0x06, 0xBA, 0xD9, 0x2A, 0xE7, 0x89, 0x43, 0xDC, 0xDB, 0x4C, 0xA5, 0x7E, 0x84, 0xA8, 0xCF, 0x46, 0xEC, 0x00, 0xBD, 0xB1, 0x66, 0xB9, 0xD9, 0x3E, 0xF8, 0x1B, 0x45};

	unsigned char	ECCPriv9[32] = {0x50, 0x74, 0x7B, 0x80, 0x2D, 0x2C, 0x43, 0x13, 0x1F, 0xA3, 0x25, 0x93, 0x5D, 0x28, 0x05, 0x5D, 0x5F, 0xAE, 0x0B, 0xF8, 0x73, 0xD2, 0x7B, 0x7C, 0x3A, 0x16, 0x2A, 0x60, 0x14, 0x03, 0x1F, 0x96};
	unsigned char	ECCGX9[32] = {0xFD, 0xD0, 0x3B, 0x0C, 0xA6, 0x6C, 0x69, 0x9C, 0x60, 0x7D, 0x1B, 0x12, 0x41, 0x81, 0x92, 0xA8, 0x36, 0xE1, 0x39, 0x44, 0x7B, 0xD1, 0x2B, 0x57, 0x51, 0x8A, 0xA7, 0xE5, 0xA7, 0xB4, 0x04, 0xBF};
	unsigned char	ECCGY9[32] = {0xDE, 0x38, 0xFF, 0x33, 0x2F, 0xDB, 0xDA, 0xC3, 0xA2, 0xA8, 0x72, 0xD1, 0x26, 0xC0, 0x26, 0xCD, 0xC6, 0x93, 0x2E, 0x70, 0x40, 0x8A, 0x5A, 0x82, 0x09, 0xEB, 0xCB, 0x8F, 0x3C, 0xFA, 0xE6, 0x5E};

	unsigned char	ECCPriv10[32] = {0x1B, 0xB8, 0x6F, 0x99, 0x17, 0x0C, 0x40, 0x8E, 0x21, 0x26, 0x69, 0x29, 0x33, 0xBE, 0x14, 0x11, 0x2C, 0x0A, 0x0A, 0x81, 0x67, 0x21, 0x3A, 0xA1, 0x04, 0xCB, 0x4E, 0x99, 0x7F, 0x7D, 0x28, 0x17};
	unsigned char	ECCGX10[32] = {0x12, 0x25, 0xEE, 0xDA, 0x41, 0x62, 0x03, 0xAD, 0xFE, 0xDC, 0x1A, 0x81, 0x50, 0x09, 0x9A, 0xC7, 0xB5, 0x5B, 0x83, 0x78, 0xDC, 0x9A, 0x40, 0xAF, 0xBF, 0x73, 0xE3, 0x71, 0xDB, 0x54, 0x17, 0xA8};
	unsigned char	ECCGY10[32] = {0xB8, 0x90, 0x41, 0xC8, 0x97, 0xEC, 0xF0, 0xD9, 0xC7, 0x78, 0x65, 0x24, 0x52, 0x86, 0xA5, 0x88, 0x85, 0xBD, 0xCA, 0x76, 0xDF, 0x8A, 0xA6, 0x9B, 0x25, 0x29, 0x0B, 0xE4, 0x6B, 0x5D, 0xAC, 0x55};

	unsigned char	ECCOUTX1[32] = {0};
	unsigned char	ECCOUTY1[32] = {0};
	unsigned char	ECCOUTX2[32] = {0};
	unsigned char	ECCOUTY2[32] = {0};

	unsigned char	*pECCPrivData[10] = {ECCPriv1, ECCPriv2, ECCPriv3, ECCPriv4, ECCPriv5, ECCPriv6, ECCPriv7, ECCPriv8, ECCPriv9, ECCPriv10};
	unsigned char	*pECCPubDataX[10] = {ECCGX1, ECCGX2, ECCGX3, ECCGX4, ECCGX5, ECCGX6, ECCGX7, ECCGX8, ECCGX9, ECCGX10};
	unsigned char	*pECCPubDataY[10] = {ECCGY1, ECCGY2, ECCGY3, ECCGY4, ECCGY5, ECCGY6, ECCGY7, ECCGY8, ECCGY9, ECCGY10};

	unsigned char *pMechanismParameter1 = NULL;
	CK_MECHANISM PointMultMechanism1 = {CKM_DERIVE_SM2_POINTMUL_1, NULL, 0};

	unsigned char *pMechanismParameter2 = NULL;
	CK_MECHANISM PointMultMechanism2 = {CKM_DERIVE_SM2_POINTMUL_1, NULL, 0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2PointMul1WithData);

	pMechanismParameter1 = (unsigned char*)malloc(32+64+64);
	memset(pMechanismParameter1, 0, 32+64+64);
	PointMultMechanism1.pParameter = pMechanismParameter1;
	PointMultMechanism1.ulParameterLen = 32+64+64;

	pMechanismParameter2 = (unsigned char*)malloc(32+64+64);
	memset(pMechanismParameter2, 0, 32+64+64);
	PointMultMechanism2.pParameter = pMechanismParameter2;
	PointMultMechanism2.ulParameterLen = 32+64+64;

	unsigned char RepeatCount[32] ={0}; 

	for (i=0; i<10; i++)
	{
		for (j=i+1; j<10; j++)
		{
			memset(ECCOUTX1, 0, sizeof(ECCOUTX1));
			memset(ECCOUTY1, 0, sizeof(ECCOUTY1));

			memset(ECCOUTX2, 0, sizeof(ECCOUTX2));
			memset(ECCOUTY2, 0, sizeof(ECCOUTY2));

			/******************设置机制参数**************/
			memcpy(pMechanismParameter1, pECCPrivData[i], 32);
			memcpy(pMechanismParameter1 + 32, pECCPubDataX[j], 32);
			memcpy(pMechanismParameter1 + 64, pECCPubDataY[j], 32);

			memcpy(pMechanismParameter2, pECCPrivData[j], 32);
			memcpy(pMechanismParameter2 + 32, pECCPubDataX[i], 32);
			memcpy(pMechanismParameter2 + 64, pECCPubDataY[i], 32);

			//点乘

			rv = (*pC_DeriveKey)(hSession, &PointMultMechanism1, NULL_PTR, NULL, 0, NULL);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey1,rv);

			memcpy(ECCOUTX1, (unsigned char*)PointMultMechanism1.pParameter + 32 + 64, 32);
			memcpy(ECCOUTY1, (unsigned char*)PointMultMechanism1.pParameter + 32 + 96, 32);

			rv = (*pC_DeriveKey)(hSession, &PointMultMechanism2, NULL_PTR, NULL, 0, NULL);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey2,rv);

			memcpy(ECCOUTX2, (unsigned char*)PointMultMechanism2.pParameter + 32 + 64, 32);
			memcpy(ECCOUTY2, (unsigned char*)PointMultMechanism2.pParameter + 32 + 96, 32);

			
			memset(RepeatCount, 0, 32);
			for(k=1;k<32;k++)
			{
				if(ECCOUTX1[k]==ECCOUTX1[k-1])
				{
					RepeatCount[k]=RepeatCount[k-1]+1;
					if(RepeatCount[k]>=(32/4))
					{
						printf("Error:DataX repeat times>=%d.\n",RepeatCount[k]);
						nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Error:DataX repeat times>=%d.<br>",RepeatCount[k] ); 
						bRtn = false;
						goto END;
					}
				}
			}
			memset(RepeatCount, 0, 32);
			for(k=1;k<32;k++)
			{
				if(ECCOUTY1[k]==ECCOUTY1[k-1])
				{
					RepeatCount[k]=RepeatCount[k-1]+1;
					if(RepeatCount[k]>=(32/4))
					{
						printf("Error:DataY repeat times>=%d.\n",RepeatCount[k]);
						nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error:DataY repeat times>=%d.<br>",RepeatCount[k]); 
						bRtn = false;
						goto END;
					}
				}
			}
			if (memcmp(ECCOUTX1, ECCOUTX2, sizeof(ECCOUTX1)) || memcmp(ECCOUTY1, ECCOUTY2, sizeof(ECCOUTY1)))
			{
				printf("Error: ECCPointMul, i=%d, j=%d.\n", i, j);
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Error: ECCPointMul, i=%d, j=%d.<br>", i, j); 
				bRtn = false;
				goto END;
			}
			//else
			//{
			printf("%*c\r",79,0x20);
			printf("OK: ECCPointMul, i=%d, j=%d.\r", i, j);
			fflush(stdout);
				//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"OK: ECCPointMul, i=%d, j=%d.\r", i, j); 
			//}
		}
	}
	bRtn = true;
END:
	if (pMechanismParameter1)
	{
		free(pMechanismParameter1);
		pMechanismParameter1 = NULL;
	}
	if (pMechanismParameter2)
	{
		free(pMechanismParameter2);
		pMechanismParameter2 = NULL;
	}

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2PointMulWithData);

	
	return bRtn;
}


bool xtest_SM2PointMul_speed(int looptime)
{

	bool	bRtn = false;
	CK_RV	rv=0;
	unsigned int i = 0, j = 0,k=0;
	
	unsigned char	ECCPriv1[32] = {0x65, 0x83, 0x2D, 0x7C, 0x7B, 0x27, 0x2E, 0x98, 0x57, 0xD1, 0x08, 0x4F, 0x3A, 0x75, 0x54, 0x0D, 0x40, 0xC2, 0x04, 0x23, 0x3A, 0x6A, 0x65, 0x6A, 0x15, 0x9D, 0x59, 0x52, 0x2D, 0x68, 0x4E, 0x3E};
	unsigned char	ECCGX1[32] = {0xD6, 0xA7, 0x78, 0x8C, 0x41, 0xC3, 0xA3, 0x66, 0x18, 0xE8, 0x02, 0x1E, 0xE1, 0xEB, 0x27, 0x2F, 0xA7, 0x24, 0x7F, 0x77, 0xF4, 0x7F, 0x40, 0xBE, 0x6A, 0x31, 0x02, 0x48, 0x41, 0xA0, 0x14, 0xC6};
	unsigned char	ECCGY1[32] = {0x39, 0xE2, 0xF0, 0xA4, 0x69, 0x22, 0xD3, 0x56, 0xBD, 0xB0, 0x39, 0x12, 0x26, 0xA9, 0xD1, 0x47, 0x40, 0x84, 0x3D, 0xFE, 0x57, 0x68, 0x0D, 0x28, 0x87, 0x44, 0xB7, 0xD8, 0x20, 0xE0, 0x32, 0xBB};

	unsigned char	ECCPriv2[32] = {0x6E, 0xDD, 0x1A, 0xD0, 0x2D, 0x5F, 0x4A, 0x8C, 0x10, 0x25, 0x42, 0xF2, 0x56, 0xCC, 0x30, 0xFC, 0x6C, 0x88, 0x67, 0xEB, 0x59, 0xC4, 0x67, 0xEC, 0x02, 0x36, 0x03, 0x5D, 0x2E, 0x17, 0x06, 0x9D};
	unsigned char	ECCGX2[32] = {0xD6, 0x5C, 0xA2, 0xDA, 0x1C, 0x05, 0x8E, 0xFA, 0xFD, 0x7E, 0x98, 0x4B, 0x86, 0x08, 0xF6, 0x24, 0x0D, 0xE8, 0x78, 0x03, 0x87, 0x98, 0x2D, 0x71, 0xF6, 0xFC, 0xB7, 0x87, 0x6E, 0xD2, 0x36, 0x58};
	unsigned char	ECCGY2[32] = {0x0B, 0xEB, 0x90, 0x5B, 0x18, 0x52, 0x3E, 0xDB, 0xE2, 0x46, 0x45, 0xCD, 0x08, 0x8F, 0x7B, 0x02, 0x5C, 0xCE, 0xE4, 0xBF, 0xD8, 0x46, 0x8A, 0xA5, 0x0F, 0x02, 0x2C, 0x55, 0x32, 0xEF, 0x88, 0xB5};

	unsigned char	ECCPriv3[32] = {0x3D, 0x73, 0x7C, 0x01, 0x52, 0x36, 0x79, 0xB0, 0x39, 0x95, 0x60, 0xD1, 0x27, 0x4E, 0x77, 0x7C, 0x59, 0xD0, 0x40, 0xBF, 0x28, 0x38, 0x0E, 0x33, 0x1F, 0xFA, 0x66, 0x79, 0x66, 0xF2, 0x02, 0x89};
	unsigned char	ECCGX3[32] = {0x38, 0xCD, 0xAD, 0xDD, 0xDA, 0x1C, 0x6C, 0x51, 0x35, 0x23, 0xCC, 0xB3, 0x2E, 0x0E, 0x28, 0x01, 0x26, 0x2B, 0x7F, 0x95, 0xF2, 0x5E, 0x85, 0x3C, 0x86, 0xE1, 0x01, 0x28, 0x6C, 0x82, 0xD7, 0x18};
	unsigned char	ECCGY3[32] = {0x6A, 0x1C, 0x8A, 0x61, 0xC4, 0xD3, 0xED, 0x61, 0x9D, 0xFA, 0xA0, 0x33, 0x20, 0x5C, 0x5F, 0x76, 0x9B, 0x3B, 0xE1, 0x2C, 0x8F, 0x15, 0x7C, 0x2F, 0x55, 0x75, 0xFF, 0xEE, 0xFB, 0x42, 0xA0, 0x64};

	unsigned char	ECCPriv4[32] = {0x23, 0xEA, 0x2D, 0x20, 0x4E, 0x69, 0x41, 0xD0, 0x50, 0x37, 0x01, 0xAB, 0x6A, 0xA7, 0x5F, 0x86, 0x2B, 0x1E, 0x7B, 0x11, 0x13, 0x61, 0x43, 0x68, 0x24, 0xDC, 0x74, 0xCA, 0x39, 0x86, 0x11, 0x5B};
	unsigned char	ECCGX4[32] = {0xC0, 0x54, 0x13, 0x71, 0xDA, 0xA7, 0x29, 0x6E, 0x04, 0xDF, 0x21, 0x90, 0xF6, 0xF3, 0x91, 0xCF, 0xBF, 0x3C, 0x4C, 0x2E, 0x42, 0xA2, 0x8B, 0x63, 0xBE, 0x9F, 0xE4, 0x0C, 0xFD, 0x9B, 0x11, 0x9E};
	unsigned char	ECCGY4[32] = {0x9B, 0xAA, 0x6A, 0xFF, 0xEB, 0x03, 0x2D, 0x7A, 0x82, 0xD9, 0x90, 0xA8, 0x36, 0x8D, 0x0B, 0x82, 0x35, 0x5A, 0x56, 0xD1, 0xBC, 0x14, 0xEA, 0xA2, 0xB4, 0x7E, 0x69, 0xF7, 0xB6, 0x21, 0x1E, 0xC3};

	unsigned char	ECCPriv5[32] = {0x71, 0x26, 0x15, 0x7E, 0x30, 0xF6, 0x29, 0xF2, 0x38, 0x5F, 0x0C, 0x82, 0x15, 0xC4, 0x7E, 0x53, 0x17, 0x37, 0x46, 0x91, 0x0B, 0x1E, 0x6B, 0xF3, 0x47, 0x12, 0x1F, 0xAE, 0x15, 0xA0, 0x57, 0xAC};
	unsigned char	ECCGX5[32] = {0xA9, 0x80, 0xD7, 0x8E, 0xEE, 0x50, 0x52, 0x84, 0x3D, 0xA8, 0x35, 0x45, 0x13, 0x1B, 0x5C, 0xDA, 0xE6, 0xD2, 0xE4, 0x6D, 0x2A, 0x9E, 0x6D, 0xA4, 0x46, 0x73, 0xE1, 0xAF, 0x97, 0x8E, 0x54, 0xF5};
	unsigned char	ECCGY5[32] = {0x6A, 0x97, 0xA2, 0x54, 0xC4, 0x6F, 0x92, 0xC2, 0x45, 0xD1, 0x71, 0x1D, 0xEE, 0x94, 0xEA, 0x2D, 0x21, 0xEB, 0xE2, 0x2E, 0x22, 0xC8, 0x05, 0x57, 0xA1, 0x4F, 0x90, 0x15, 0x79, 0x31, 0xCD, 0x56};

	unsigned char	ECCPriv6[32] = {0x40, 0x4D, 0x37, 0xAC, 0x03, 0x19, 0x4A, 0x61, 0x0E, 0xA2, 0x3F, 0x97, 0x23, 0xD3, 0x56, 0x5D, 0x39, 0x20, 0x26, 0x31, 0x51, 0x8A, 0x75, 0x7F, 0x0D, 0x11, 0x67, 0xC8, 0x49, 0x4C, 0x5F, 0x54};
	unsigned char	ECCGX6[32] = {0xD8, 0x67, 0xE2, 0x55, 0xF4, 0x60, 0x80, 0x04, 0xCD, 0xCF, 0xBB, 0x0B, 0xC4, 0x6C, 0xB3, 0x97, 0xF4, 0xB2, 0xBA, 0x73, 0x4D, 0x70, 0xEE, 0x89, 0x76, 0xFC, 0x0D, 0x86, 0x4B, 0xC8, 0x32, 0x86};
	unsigned char	ECCGY6[32] = {0x5B, 0xE8, 0x39, 0xF9, 0x23, 0x13, 0x47, 0x63, 0xA6, 0x5A, 0x11, 0xFB, 0xD6, 0xC4, 0x5A, 0xAB, 0x10, 0x0D, 0x5A, 0x78, 0x87, 0xC4, 0x00, 0x09, 0x19, 0xE2, 0x85, 0xFA, 0x6C, 0xB7, 0xB7, 0x60};

	unsigned char	ECCPriv7[32] = {0x48, 0xC4, 0x41, 0x7B, 0x18, 0x50, 0x5C, 0xA6, 0x77, 0xD6, 0x40, 0x6B, 0x66, 0x40, 0x67, 0x5C, 0x60, 0x1E, 0x00, 0x22, 0x4B, 0x04, 0x66, 0xF4, 0x1D, 0x8E, 0x6C, 0xF8, 0x50, 0xD8, 0x27, 0x6D};
	unsigned char	ECCGX7[32] = {0x7D, 0x64, 0xDE, 0x59, 0x4B, 0x7D, 0xAA, 0x94, 0xD0, 0xB0, 0xEA, 0x57, 0x8C, 0x70, 0x98, 0x27, 0xFB, 0x94, 0x17, 0xF6, 0x91, 0xDB, 0x0B, 0x6A, 0x72, 0x93, 0x33, 0x05, 0x6C, 0x1C, 0x7F, 0x9B};
	unsigned char	ECCGY7[32] = {0xC8, 0x2A, 0x65, 0x82, 0x44, 0xBD, 0xFF, 0x22, 0x43, 0x0C, 0x5E, 0xAE, 0x11, 0x0A, 0x48, 0x59, 0xF3, 0xB8, 0x53, 0x83, 0x9D, 0xAA, 0xE8, 0x8B, 0x35, 0xA8, 0x99, 0x53, 0xD1, 0xD5, 0x54, 0x40};

	unsigned char	ECCPriv8[32] = {0x2E, 0x2F, 0x1B, 0xFC, 0x5E, 0x58, 0x4B, 0x89, 0x71, 0x0F, 0x2B, 0xBE, 0x54, 0xB8, 0x3E, 0x49, 0x5F, 0xB6, 0x2D, 0xD4, 0x4E, 0x27, 0x70, 0x7A, 0x0F, 0x7E, 0x7E, 0x60, 0x26, 0xD1, 0x34, 0x50};
	unsigned char	ECCGX8[32] = {0xDD, 0x91, 0xDA, 0xF8, 0xAD, 0x1A, 0x57, 0xFF, 0x4B, 0xDD, 0xA8, 0x12, 0xB8, 0x61, 0x19, 0x10, 0x7D, 0x88, 0x04, 0xBE, 0x4E, 0x01, 0xCD, 0xAC, 0x0A, 0x30, 0x44, 0xB7, 0xB5, 0x35, 0x51, 0x93};
	unsigned char	ECCGY8[32] = {0x0A, 0xC2, 0x86, 0xB7, 0x8B, 0x06, 0xBA, 0xD9, 0x2A, 0xE7, 0x89, 0x43, 0xDC, 0xDB, 0x4C, 0xA5, 0x7E, 0x84, 0xA8, 0xCF, 0x46, 0xEC, 0x00, 0xBD, 0xB1, 0x66, 0xB9, 0xD9, 0x3E, 0xF8, 0x1B, 0x45};

	unsigned char	ECCPriv9[32] = {0x50, 0x74, 0x7B, 0x80, 0x2D, 0x2C, 0x43, 0x13, 0x1F, 0xA3, 0x25, 0x93, 0x5D, 0x28, 0x05, 0x5D, 0x5F, 0xAE, 0x0B, 0xF8, 0x73, 0xD2, 0x7B, 0x7C, 0x3A, 0x16, 0x2A, 0x60, 0x14, 0x03, 0x1F, 0x96};
	unsigned char	ECCGX9[32] = {0xFD, 0xD0, 0x3B, 0x0C, 0xA6, 0x6C, 0x69, 0x9C, 0x60, 0x7D, 0x1B, 0x12, 0x41, 0x81, 0x92, 0xA8, 0x36, 0xE1, 0x39, 0x44, 0x7B, 0xD1, 0x2B, 0x57, 0x51, 0x8A, 0xA7, 0xE5, 0xA7, 0xB4, 0x04, 0xBF};
	unsigned char	ECCGY9[32] = {0xDE, 0x38, 0xFF, 0x33, 0x2F, 0xDB, 0xDA, 0xC3, 0xA2, 0xA8, 0x72, 0xD1, 0x26, 0xC0, 0x26, 0xCD, 0xC6, 0x93, 0x2E, 0x70, 0x40, 0x8A, 0x5A, 0x82, 0x09, 0xEB, 0xCB, 0x8F, 0x3C, 0xFA, 0xE6, 0x5E};

	unsigned char	ECCPriv10[32] = {0x1B, 0xB8, 0x6F, 0x99, 0x17, 0x0C, 0x40, 0x8E, 0x21, 0x26, 0x69, 0x29, 0x33, 0xBE, 0x14, 0x11, 0x2C, 0x0A, 0x0A, 0x81, 0x67, 0x21, 0x3A, 0xA1, 0x04, 0xCB, 0x4E, 0x99, 0x7F, 0x7D, 0x28, 0x17};
	unsigned char	ECCGX10[32] = {0x12, 0x25, 0xEE, 0xDA, 0x41, 0x62, 0x03, 0xAD, 0xFE, 0xDC, 0x1A, 0x81, 0x50, 0x09, 0x9A, 0xC7, 0xB5, 0x5B, 0x83, 0x78, 0xDC, 0x9A, 0x40, 0xAF, 0xBF, 0x73, 0xE3, 0x71, 0xDB, 0x54, 0x17, 0xA8};
	unsigned char	ECCGY10[32] = {0xB8, 0x90, 0x41, 0xC8, 0x97, 0xEC, 0xF0, 0xD9, 0xC7, 0x78, 0x65, 0x24, 0x52, 0x86, 0xA5, 0x88, 0x85, 0xBD, 0xCA, 0x76, 0xDF, 0x8A, 0xA6, 0x9B, 0x25, 0x29, 0x0B, 0xE4, 0x6B, 0x5D, 0xAC, 0x55};

	unsigned char	ECCOUTX1[32] = {0};
	unsigned char	ECCOUTY1[32] = {0};
	unsigned char	ECCOUTX2[32] = {0};
	unsigned char	ECCOUTY2[32] = {0};

	unsigned char	*pECCPrivData[10] = {ECCPriv1, ECCPriv2, ECCPriv3, ECCPriv4, ECCPriv5, ECCPriv6, ECCPriv7, ECCPriv8, ECCPriv9, ECCPriv10};
	unsigned char	*pECCPubDataX[10] = {ECCGX1, ECCGX2, ECCGX3, ECCGX4, ECCGX5, ECCGX6, ECCGX7, ECCGX8, ECCGX9, ECCGX10};
	unsigned char	*pECCPubDataY[10] = {ECCGY1, ECCGY2, ECCGY3, ECCGY4, ECCGY5, ECCGY6, ECCGY7, ECCGY8, ECCGY9, ECCGY10};

	unsigned char *pMechanismParameter1 = NULL;
	CK_MECHANISM PointMultMechanism1 = {CKM_DERIVE_SM2_POINTMUL_1, NULL, 0};

	unsigned char *pMechanismParameter2 = NULL;
	CK_MECHANISM PointMultMechanism2 = {CKM_DERIVE_SM2_POINTMUL_1, NULL, 0};
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2PointMulPerformance);

	pMechanismParameter1 = (unsigned char*)malloc(32+64+64);
	memset(pMechanismParameter1, 0, 32+64+64);
	PointMultMechanism1.pParameter = pMechanismParameter1;
	PointMultMechanism1.ulParameterLen = 32+64+64;

	pMechanismParameter2 = (unsigned char*)malloc(32+64+64);
	memset(pMechanismParameter2, 0, 32+64+64);
	PointMultMechanism2.pParameter = pMechanismParameter2;
	PointMultMechanism2.ulParameterLen = 32+64+64;

	unsigned char *RepeatCount = (unsigned char*)malloc(32);
	looptime=0;
	for (i=0; i<10; i++)
	{
		for (j=i+1; j<10; j++)
		{
			memset(ECCOUTX1, 0, sizeof(ECCOUTX1));
			memset(ECCOUTY1, 0, sizeof(ECCOUTY1));

			memset(ECCOUTX2, 0, sizeof(ECCOUTX2));
			memset(ECCOUTY2, 0, sizeof(ECCOUTY2));

			/******************设置机制参数**************/
			memcpy(pMechanismParameter1, pECCPrivData[i], 32);
			memcpy(pMechanismParameter1 + 32, pECCPubDataX[j], 32);
			memcpy(pMechanismParameter1 + 64, pECCPubDataY[j], 32);

			memcpy(pMechanismParameter2, pECCPrivData[j], 32);
			memcpy(pMechanismParameter2 + 32, pECCPubDataX[i], 32);
			memcpy(pMechanismParameter2 + 64, pECCPubDataY[i], 32);

			//点乘
			Utilsgettime(&ttc1);
			rv = (*pC_DeriveKey)(hSession, &PointMultMechanism1, NULL_PTR, NULL, 0, NULL);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey1,rv);
			Utilsgettime(&ttc2);
			UtilsTimeSubstracted(&ttc2,&ttc1);
			UtilsTimeAdded(&ttc3,&ttc2);

			memcpy(ECCOUTX1, (unsigned char*)PointMultMechanism1.pParameter + 32 + 64, 32);
			memcpy(ECCOUTY1, (unsigned char*)PointMultMechanism1.pParameter + 32 + 96, 32);

			Utilsgettime(&ttc1);
			rv = (*pC_DeriveKey)(hSession, &PointMultMechanism2, NULL_PTR, NULL, 0, NULL);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey2,rv);
			Utilsgettime(&ttc2);
			UtilsTimeSubstracted(&ttc2,&ttc1);
			UtilsTimeAdded(&ttc4,&ttc2);

			memcpy(ECCOUTX2, (unsigned char*)PointMultMechanism2.pParameter + 32 + 64, 32);
			memcpy(ECCOUTY2, (unsigned char*)PointMultMechanism2.pParameter + 32 + 96, 32);

			memset(RepeatCount, 0, 32);
			for(k=1;k<32;k++)
			{
				if(ECCOUTX1[k]==ECCOUTX1[k-1])
				{
					RepeatCount[k]=RepeatCount[k-1]+1;
					if(RepeatCount[k]>=(32/4))
					{
						printf("Error:DataX repeat times>=%d.\n",RepeatCount[k]);
						nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Error:DataX repeat times>=%d.<br>",RepeatCount[k]); 
						bRtn = false;
						goto END;
					}
				}
			}
			memset(RepeatCount, 0, 32);
			for(k=1;k<32;k++)
			{
				if(ECCOUTY1[k]==ECCOUTY1[k-1])
				{
					RepeatCount[k]=RepeatCount[k-1]+1;
					if(RepeatCount[k]>=(32/4))
					{
						printf("Error:DataY repeat times>=%d.\n",RepeatCount[k]);
						nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error:DataY repeat times>=%d.<br>",RepeatCount[k]); 
						bRtn = false;
						goto END;
					}
				}
			}
			if (memcmp(ECCOUTX1, ECCOUTX2, sizeof(ECCOUTX1)) || memcmp(ECCOUTY1, ECCOUTY2, sizeof(ECCOUTY1)))
			{
				printf("Error: ECCPointMul, i=%d, j=%d.\n", i, j);
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Error: ECCPointMul, i=%d, j=%d.<br>", i, j); 
				bRtn = false;
				goto END;
			}
			//else
			//{
			//	printf("OK: ECCPointMul, i=%d, j=%d.\n", i, j);
			//	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"OK: ECCPointMul, i=%d, j=%d.\n", i, j); 
			//}
			looptime++;
		}
	}
	Utilsprint(&ttc3,"ECCPointMul DeriveKey1", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"ECCPointMul DeriveKey1", looptime);
	Utilsprint(&ttc4,"ECCPointMul DeriveKey2", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"ECCPointMul DeriveKey2", looptime);

	bRtn = true;
END:
	if (pMechanismParameter1)
	{
		free(pMechanismParameter1);
		pMechanismParameter1 = NULL;
	}
	if (pMechanismParameter2)
	{
		free(pMechanismParameter2);
		pMechanismParameter2 = NULL;
	}
	free(RepeatCount);
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2PointMulPerformance);
	return bRtn;
}


bool xtest_SM2PointMul2()
{
	bool	bRtn = false;
	CK_RV	rv=0;
	unsigned int i = 0, j = 0,k=0;

	unsigned char	ECCPriv1[32] = {0x65, 0x83, 0x2D, 0x7C, 0x7B, 0x27, 0x2E, 0x98, 0x57, 0xD1, 0x08, 0x4F, 0x3A, 0x75, 0x54, 0x0D, 0x40, 0xC2, 0x04, 0x23, 0x3A, 0x6A, 0x65, 0x6A, 0x15, 0x9D, 0x59, 0x52, 0x2D, 0x68, 0x4E, 0x3E};
	unsigned char	ECCGX1[32] = {0xD6, 0xA7, 0x78, 0x8C, 0x41, 0xC3, 0xA3, 0x66, 0x18, 0xE8, 0x02, 0x1E, 0xE1, 0xEB, 0x27, 0x2F, 0xA7, 0x24, 0x7F, 0x77, 0xF4, 0x7F, 0x40, 0xBE, 0x6A, 0x31, 0x02, 0x48, 0x41, 0xA0, 0x14, 0xC6};
	unsigned char	ECCGY1[32] = {0x39, 0xE2, 0xF0, 0xA4, 0x69, 0x22, 0xD3, 0x56, 0xBD, 0xB0, 0x39, 0x12, 0x26, 0xA9, 0xD1, 0x47, 0x40, 0x84, 0x3D, 0xFE, 0x57, 0x68, 0x0D, 0x28, 0x87, 0x44, 0xB7, 0xD8, 0x20, 0xE0, 0x32, 0xBB};

	unsigned char	ECCPriv2[32] = {0x6E, 0xDD, 0x1A, 0xD0, 0x2D, 0x5F, 0x4A, 0x8C, 0x10, 0x25, 0x42, 0xF2, 0x56, 0xCC, 0x30, 0xFC, 0x6C, 0x88, 0x67, 0xEB, 0x59, 0xC4, 0x67, 0xEC, 0x02, 0x36, 0x03, 0x5D, 0x2E, 0x17, 0x06, 0x9D};
	unsigned char	ECCGX2[32] = {0xD6, 0x5C, 0xA2, 0xDA, 0x1C, 0x05, 0x8E, 0xFA, 0xFD, 0x7E, 0x98, 0x4B, 0x86, 0x08, 0xF6, 0x24, 0x0D, 0xE8, 0x78, 0x03, 0x87, 0x98, 0x2D, 0x71, 0xF6, 0xFC, 0xB7, 0x87, 0x6E, 0xD2, 0x36, 0x58};
	unsigned char	ECCGY2[32] = {0x0B, 0xEB, 0x90, 0x5B, 0x18, 0x52, 0x3E, 0xDB, 0xE2, 0x46, 0x45, 0xCD, 0x08, 0x8F, 0x7B, 0x02, 0x5C, 0xCE, 0xE4, 0xBF, 0xD8, 0x46, 0x8A, 0xA5, 0x0F, 0x02, 0x2C, 0x55, 0x32, 0xEF, 0x88, 0xB5};

	unsigned char	ECCPriv3[32] = {0x3D, 0x73, 0x7C, 0x01, 0x52, 0x36, 0x79, 0xB0, 0x39, 0x95, 0x60, 0xD1, 0x27, 0x4E, 0x77, 0x7C, 0x59, 0xD0, 0x40, 0xBF, 0x28, 0x38, 0x0E, 0x33, 0x1F, 0xFA, 0x66, 0x79, 0x66, 0xF2, 0x02, 0x89};
	unsigned char	ECCGX3[32] = {0x38, 0xCD, 0xAD, 0xDD, 0xDA, 0x1C, 0x6C, 0x51, 0x35, 0x23, 0xCC, 0xB3, 0x2E, 0x0E, 0x28, 0x01, 0x26, 0x2B, 0x7F, 0x95, 0xF2, 0x5E, 0x85, 0x3C, 0x86, 0xE1, 0x01, 0x28, 0x6C, 0x82, 0xD7, 0x18};
	unsigned char	ECCGY3[32] = {0x6A, 0x1C, 0x8A, 0x61, 0xC4, 0xD3, 0xED, 0x61, 0x9D, 0xFA, 0xA0, 0x33, 0x20, 0x5C, 0x5F, 0x76, 0x9B, 0x3B, 0xE1, 0x2C, 0x8F, 0x15, 0x7C, 0x2F, 0x55, 0x75, 0xFF, 0xEE, 0xFB, 0x42, 0xA0, 0x64};

	unsigned char	ECCPriv4[32] = {0x23, 0xEA, 0x2D, 0x20, 0x4E, 0x69, 0x41, 0xD0, 0x50, 0x37, 0x01, 0xAB, 0x6A, 0xA7, 0x5F, 0x86, 0x2B, 0x1E, 0x7B, 0x11, 0x13, 0x61, 0x43, 0x68, 0x24, 0xDC, 0x74, 0xCA, 0x39, 0x86, 0x11, 0x5B};
	unsigned char	ECCGX4[32] = {0xC0, 0x54, 0x13, 0x71, 0xDA, 0xA7, 0x29, 0x6E, 0x04, 0xDF, 0x21, 0x90, 0xF6, 0xF3, 0x91, 0xCF, 0xBF, 0x3C, 0x4C, 0x2E, 0x42, 0xA2, 0x8B, 0x63, 0xBE, 0x9F, 0xE4, 0x0C, 0xFD, 0x9B, 0x11, 0x9E};
	unsigned char	ECCGY4[32] = {0x9B, 0xAA, 0x6A, 0xFF, 0xEB, 0x03, 0x2D, 0x7A, 0x82, 0xD9, 0x90, 0xA8, 0x36, 0x8D, 0x0B, 0x82, 0x35, 0x5A, 0x56, 0xD1, 0xBC, 0x14, 0xEA, 0xA2, 0xB4, 0x7E, 0x69, 0xF7, 0xB6, 0x21, 0x1E, 0xC3};

	unsigned char	ECCPriv5[32] = {0x71, 0x26, 0x15, 0x7E, 0x30, 0xF6, 0x29, 0xF2, 0x38, 0x5F, 0x0C, 0x82, 0x15, 0xC4, 0x7E, 0x53, 0x17, 0x37, 0x46, 0x91, 0x0B, 0x1E, 0x6B, 0xF3, 0x47, 0x12, 0x1F, 0xAE, 0x15, 0xA0, 0x57, 0xAC};
	unsigned char	ECCGX5[32] = {0xA9, 0x80, 0xD7, 0x8E, 0xEE, 0x50, 0x52, 0x84, 0x3D, 0xA8, 0x35, 0x45, 0x13, 0x1B, 0x5C, 0xDA, 0xE6, 0xD2, 0xE4, 0x6D, 0x2A, 0x9E, 0x6D, 0xA4, 0x46, 0x73, 0xE1, 0xAF, 0x97, 0x8E, 0x54, 0xF5};
	unsigned char	ECCGY5[32] = {0x6A, 0x97, 0xA2, 0x54, 0xC4, 0x6F, 0x92, 0xC2, 0x45, 0xD1, 0x71, 0x1D, 0xEE, 0x94, 0xEA, 0x2D, 0x21, 0xEB, 0xE2, 0x2E, 0x22, 0xC8, 0x05, 0x57, 0xA1, 0x4F, 0x90, 0x15, 0x79, 0x31, 0xCD, 0x56};

	unsigned char	ECCPriv6[32] = {0x40, 0x4D, 0x37, 0xAC, 0x03, 0x19, 0x4A, 0x61, 0x0E, 0xA2, 0x3F, 0x97, 0x23, 0xD3, 0x56, 0x5D, 0x39, 0x20, 0x26, 0x31, 0x51, 0x8A, 0x75, 0x7F, 0x0D, 0x11, 0x67, 0xC8, 0x49, 0x4C, 0x5F, 0x54};
	unsigned char	ECCGX6[32] = {0xD8, 0x67, 0xE2, 0x55, 0xF4, 0x60, 0x80, 0x04, 0xCD, 0xCF, 0xBB, 0x0B, 0xC4, 0x6C, 0xB3, 0x97, 0xF4, 0xB2, 0xBA, 0x73, 0x4D, 0x70, 0xEE, 0x89, 0x76, 0xFC, 0x0D, 0x86, 0x4B, 0xC8, 0x32, 0x86};
	unsigned char	ECCGY6[32] = {0x5B, 0xE8, 0x39, 0xF9, 0x23, 0x13, 0x47, 0x63, 0xA6, 0x5A, 0x11, 0xFB, 0xD6, 0xC4, 0x5A, 0xAB, 0x10, 0x0D, 0x5A, 0x78, 0x87, 0xC4, 0x00, 0x09, 0x19, 0xE2, 0x85, 0xFA, 0x6C, 0xB7, 0xB7, 0x60};

	unsigned char	ECCPriv7[32] = {0x48, 0xC4, 0x41, 0x7B, 0x18, 0x50, 0x5C, 0xA6, 0x77, 0xD6, 0x40, 0x6B, 0x66, 0x40, 0x67, 0x5C, 0x60, 0x1E, 0x00, 0x22, 0x4B, 0x04, 0x66, 0xF4, 0x1D, 0x8E, 0x6C, 0xF8, 0x50, 0xD8, 0x27, 0x6D};
	unsigned char	ECCGX7[32] = {0x7D, 0x64, 0xDE, 0x59, 0x4B, 0x7D, 0xAA, 0x94, 0xD0, 0xB0, 0xEA, 0x57, 0x8C, 0x70, 0x98, 0x27, 0xFB, 0x94, 0x17, 0xF6, 0x91, 0xDB, 0x0B, 0x6A, 0x72, 0x93, 0x33, 0x05, 0x6C, 0x1C, 0x7F, 0x9B};
	unsigned char	ECCGY7[32] = {0xC8, 0x2A, 0x65, 0x82, 0x44, 0xBD, 0xFF, 0x22, 0x43, 0x0C, 0x5E, 0xAE, 0x11, 0x0A, 0x48, 0x59, 0xF3, 0xB8, 0x53, 0x83, 0x9D, 0xAA, 0xE8, 0x8B, 0x35, 0xA8, 0x99, 0x53, 0xD1, 0xD5, 0x54, 0x40};

	unsigned char	ECCPriv8[32] = {0x2E, 0x2F, 0x1B, 0xFC, 0x5E, 0x58, 0x4B, 0x89, 0x71, 0x0F, 0x2B, 0xBE, 0x54, 0xB8, 0x3E, 0x49, 0x5F, 0xB6, 0x2D, 0xD4, 0x4E, 0x27, 0x70, 0x7A, 0x0F, 0x7E, 0x7E, 0x60, 0x26, 0xD1, 0x34, 0x50};
	unsigned char	ECCGX8[32] = {0xDD, 0x91, 0xDA, 0xF8, 0xAD, 0x1A, 0x57, 0xFF, 0x4B, 0xDD, 0xA8, 0x12, 0xB8, 0x61, 0x19, 0x10, 0x7D, 0x88, 0x04, 0xBE, 0x4E, 0x01, 0xCD, 0xAC, 0x0A, 0x30, 0x44, 0xB7, 0xB5, 0x35, 0x51, 0x93};
	unsigned char	ECCGY8[32] = {0x0A, 0xC2, 0x86, 0xB7, 0x8B, 0x06, 0xBA, 0xD9, 0x2A, 0xE7, 0x89, 0x43, 0xDC, 0xDB, 0x4C, 0xA5, 0x7E, 0x84, 0xA8, 0xCF, 0x46, 0xEC, 0x00, 0xBD, 0xB1, 0x66, 0xB9, 0xD9, 0x3E, 0xF8, 0x1B, 0x45};

	unsigned char	ECCPriv9[32] = {0x50, 0x74, 0x7B, 0x80, 0x2D, 0x2C, 0x43, 0x13, 0x1F, 0xA3, 0x25, 0x93, 0x5D, 0x28, 0x05, 0x5D, 0x5F, 0xAE, 0x0B, 0xF8, 0x73, 0xD2, 0x7B, 0x7C, 0x3A, 0x16, 0x2A, 0x60, 0x14, 0x03, 0x1F, 0x96};
	unsigned char	ECCGX9[32] = {0xFD, 0xD0, 0x3B, 0x0C, 0xA6, 0x6C, 0x69, 0x9C, 0x60, 0x7D, 0x1B, 0x12, 0x41, 0x81, 0x92, 0xA8, 0x36, 0xE1, 0x39, 0x44, 0x7B, 0xD1, 0x2B, 0x57, 0x51, 0x8A, 0xA7, 0xE5, 0xA7, 0xB4, 0x04, 0xBF};
	unsigned char	ECCGY9[32] = {0xDE, 0x38, 0xFF, 0x33, 0x2F, 0xDB, 0xDA, 0xC3, 0xA2, 0xA8, 0x72, 0xD1, 0x26, 0xC0, 0x26, 0xCD, 0xC6, 0x93, 0x2E, 0x70, 0x40, 0x8A, 0x5A, 0x82, 0x09, 0xEB, 0xCB, 0x8F, 0x3C, 0xFA, 0xE6, 0x5E};

	unsigned char	ECCPriv10[32] = {0x1B, 0xB8, 0x6F, 0x99, 0x17, 0x0C, 0x40, 0x8E, 0x21, 0x26, 0x69, 0x29, 0x33, 0xBE, 0x14, 0x11, 0x2C, 0x0A, 0x0A, 0x81, 0x67, 0x21, 0x3A, 0xA1, 0x04, 0xCB, 0x4E, 0x99, 0x7F, 0x7D, 0x28, 0x17};
	unsigned char	ECCGX10[32] = {0x12, 0x25, 0xEE, 0xDA, 0x41, 0x62, 0x03, 0xAD, 0xFE, 0xDC, 0x1A, 0x81, 0x50, 0x09, 0x9A, 0xC7, 0xB5, 0x5B, 0x83, 0x78, 0xDC, 0x9A, 0x40, 0xAF, 0xBF, 0x73, 0xE3, 0x71, 0xDB, 0x54, 0x17, 0xA8};
	unsigned char	ECCGY10[32] = {0xB8, 0x90, 0x41, 0xC8, 0x97, 0xEC, 0xF0, 0xD9, 0xC7, 0x78, 0x65, 0x24, 0x52, 0x86, 0xA5, 0x88, 0x85, 0xBD, 0xCA, 0x76, 0xDF, 0x8A, 0xA6, 0x9B, 0x25, 0x29, 0x0B, 0xE4, 0x6B, 0x5D, 0xAC, 0x55};

	unsigned char	ECCOUTX1[32] = {0};
	unsigned char	ECCOUTY1[32] = {0};
	unsigned char	ECCOUTX2[32] = {0};
	unsigned char	ECCOUTY2[32] = {0};

	unsigned char	*pECCPrivData[10] = {ECCPriv1, ECCPriv2, ECCPriv3, ECCPriv4, ECCPriv5, ECCPriv6, ECCPriv7, ECCPriv8, ECCPriv9, ECCPriv10};
	unsigned char	*pECCPubDataX[10] = {ECCGX1, ECCGX2, ECCGX3, ECCGX4, ECCGX5, ECCGX6, ECCGX7, ECCGX8, ECCGX9, ECCGX10};
	unsigned char	*pECCPubDataY[10] = {ECCGY1, ECCGY2, ECCGY3, ECCGY4, ECCGY5, ECCGY6, ECCGY7, ECCGY8, ECCGY9, ECCGY10};

	CK_BBOOL	ttrue = CK_TRUE;
	CK_KEY_TYPE	keyType=CKK_ECC;
	CK_OBJECT_CLASS	pubclass=CKO_PUBLIC_KEY, priclass=CKO_PRIVATE_KEY;
	CK_UINT	modulusBits = 256;

	CK_BYTE	id1[] = {0x01,0x01,0x02,0x03}, id2[] = {0x01,0x01,0x02,0x04};

	CK_OBJECT_HANDLE hPrivateKey1 = NULL_PTR, hPrivateKey2 = NULL_PTR;

	CK_ATTRIBUTE privateKeyTemplate1[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(priclass)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(modulusBits)},
		{CKA_ECC_PRIVATE, pECCPrivData[0], 32},
		{CKA_ID, id1, sizeof(id1)}
	};

	CK_ATTRIBUTE privateKeyTemplate2[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(priclass)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(modulusBits)},
		{CKA_ECC_PRIVATE, pECCPrivData[1], 32},
		{CKA_ID, id2, sizeof(id2)}
	};

	CK_OBJECT_HANDLE hObject = NULL_PTR;
	CK_ULONG ulObjectCount = 0;

	unsigned char *pMechanismParameter1 = NULL;
	CK_MECHANISM PointMultMechanism1 = {CKM_DERIVE_SM2_POINTMUL_2, NULL, 0};

	unsigned char *pMechanismParameter2 = NULL;
	CK_MECHANISM PointMultMechanism2 = {CKM_DERIVE_SM2_POINTMUL_2, NULL, 0};

	unsigned char *RepeatCount=NULL;

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2PointMul2WithHandle);

	pMechanismParameter1 = (unsigned char*)malloc(sizeof(CK_OBJECT_HANDLE)+64+64);
	memset(pMechanismParameter1, 0, sizeof(CK_OBJECT_HANDLE)+64+64);
	PointMultMechanism1.pParameter = pMechanismParameter1;
	PointMultMechanism1.ulParameterLen = sizeof(CK_OBJECT_HANDLE)+64+64;

	pMechanismParameter2 = (unsigned char*)malloc(sizeof(CK_OBJECT_HANDLE)+64+64);
	memset(pMechanismParameter2, 0, sizeof(CK_OBJECT_HANDLE)+64+64);
	PointMultMechanism2.pParameter = pMechanismParameter2;
	PointMultMechanism2.ulParameterLen = sizeof(CK_OBJECT_HANDLE)+64+64;

	for (i=0; i<10; i++)
	{
		for (j=i+1; j<10; j++)
		{
			memset(ECCOUTX1, 0, sizeof(ECCOUTX1));
			memset(ECCOUTY1, 0, sizeof(ECCOUTY1));

			memset(ECCOUTX2, 0, sizeof(ECCOUTX2));
			memset(ECCOUTY2, 0, sizeof(ECCOUTY2));

			//创建私钥对象1
			privateKeyTemplate1[7].pValue = pECCPrivData[i];
			rv = (*pC_CreateObject)(hSession,
				privateKeyTemplate1, sizeof(privateKeyTemplate1)/sizeof(CK_ATTRIBUTE),
				&hPrivateKey1);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject1,rv);

			//创建私钥对象2
			privateKeyTemplate2[7].pValue = pECCPrivData[j];
			rv = (*pC_CreateObject)(hSession,
				privateKeyTemplate2, sizeof(privateKeyTemplate2)/sizeof(CK_ATTRIBUTE),
				&hPrivateKey2);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject2,rv);

			//设置机制参数
			(*(CK_OBJECT_HANDLE *)pMechanismParameter1) = hPrivateKey1;
			memcpy(pMechanismParameter1 + sizeof(CK_OBJECT_HANDLE), pECCPubDataX[j], 32);
			memcpy(pMechanismParameter1 + sizeof(CK_OBJECT_HANDLE) + 32, pECCPubDataY[j], 32);

			(*(CK_OBJECT_HANDLE *)pMechanismParameter2) = hPrivateKey2;
			memcpy(pMechanismParameter2 + sizeof(CK_OBJECT_HANDLE), pECCPubDataX[i], 32);
			memcpy(pMechanismParameter2 + sizeof(CK_OBJECT_HANDLE) + 32, pECCPubDataY[i], 32);

			//点乘
			rv = (*pC_DeriveKey)(hSession, &PointMultMechanism1, NULL_PTR, NULL, 0, NULL);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey1,rv);
			TEST_ASSERT_EQ_EX(pC_DeriveKey, rv, CKR_OK, false);

			memcpy(ECCOUTX1, (unsigned char*)PointMultMechanism1.pParameter + sizeof(CK_OBJECT_HANDLE) + 64, 32);
			memcpy(ECCOUTY1, (unsigned char*)PointMultMechanism1.pParameter + sizeof(CK_OBJECT_HANDLE) + 96, 32);

			rv = (*pC_DeriveKey)(hSession, &PointMultMechanism2, NULL_PTR, NULL, 0, NULL);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey2,rv);

			memcpy(ECCOUTX2, (unsigned char*)PointMultMechanism2.pParameter + sizeof(CK_OBJECT_HANDLE) + 64, 32);
			memcpy(ECCOUTY2, (unsigned char*)PointMultMechanism2.pParameter + sizeof(CK_OBJECT_HANDLE) + 96, 32);

			RepeatCount = (unsigned char*)malloc(32);
			memset(RepeatCount, 0, 32);
			for(k=1;k<32;k++)
			{
				if(ECCOUTX1[k]==ECCOUTX1[k-1])
				{
					RepeatCount[k]=RepeatCount[k-1]+1;
					if(RepeatCount[k]>=(32/4))
					{
						printf("Error:DataX repeat times>=%d.\n",RepeatCount[k]);
						nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error:DataX repeat times>=%d.<br>",RepeatCount[k]);
						return false;
					}
				}
			}
			memset(RepeatCount, 0, 32);
			for(k=1;k<32;k++)
			{
				if(ECCOUTY1[k]==ECCOUTY1[k-1])
				{
					RepeatCount[k]=RepeatCount[k-1]+1;
					if(RepeatCount[k]>=(32/4))
					{
						printf("Error:DataY repeat times>=%d.\n",RepeatCount[k]);
						nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error:DataY repeat times>=%d.<br>",RepeatCount[k]);
						bRtn = false;
						goto END;
					}
				}
			}
			free(RepeatCount);
			if (memcmp(ECCOUTX1, ECCOUTX2, sizeof(ECCOUTX1)) || memcmp(ECCOUTY1, ECCOUTY2, sizeof(ECCOUTY1)))
			{
				printf("Error: ECCPointMul2, i=%d, j=%d.\n", i, j);
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: ECCPointMul2, i=%d, j=%d.<br>", i, j);
				bRtn = false;
				goto END;
			}
			//else
			//{
			printf("%*c\r",79,0x20);
				printf("OK: ECCPointMul2, i=%d, j=%d.\r", i, j);
				fflush(stdout);
				//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "OK: ECCPointMul2, i=%d, j=%d.\n", i, j);
			//}
	//printf("OK: xxxECCPointMul2, i=%d, j=%d.\n", i, j);
			rv = (*pC_DestroyObject)(hSession, hPrivateKey1);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);
			hPrivateKey1 = NULL_PTR;

			rv = (*pC_DestroyObject)(hSession, hPrivateKey2);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);
			hPrivateKey2 = NULL_PTR;			
		}
	}
	bRtn = true;
END:
	if (pMechanismParameter1)
	{
		free(pMechanismParameter1);
		pMechanismParameter1 = NULL;
	}
	if (pMechanismParameter2)
	{
		free(pMechanismParameter2);
		pMechanismParameter2 = NULL;
	}
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2PointMul2WithHandle);

	
	return bRtn;
}



#define _SM2_KEYEX_GENKEY 
bool xtest_SM2KeyCoordinate()
{
	bool	bRtn = false;
	CK_RV	rv = 0;
	unsigned char *pSponsorMechanismParameter = NULL;
	CK_MECHANISM sm2KeyExSponsorMech = {CKM_DERIVE_SM2KEYEX, NULL, 0};
	unsigned char *pReceiverMechanismParameter = NULL;
	CK_MECHANISM sm2KeyExReceiverMech = {CKM_DERIVE_SM2KEYEX, NULL, 0};

	CK_BYTE	nSponsorSessKeyID = CK_SESSKEY_ID0;
	CK_BYTE	nReceiverSessKeyID = CK_SESSKEY_ID0+1;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_ATTRIBUTE keySponsorAttrib[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSponsorSessKeyID, sizeof(CK_BYTE)}
	};


	CK_ATTRIBUTE keyReceiverAttrib[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nReceiverSessKeyID, sizeof(CK_BYTE)}
	};
	CK_OBJECT_HANDLE hSponsorKey = NULL_PTR, hReceiverKey = NULL_PTR;
	CK_OBJECT_HANDLE hSponsorPublicKey = NULL_PTR, hSponsorPrivateKey = NULL_PTR, hSponsorTempPublicKey = NULL_PTR, hSponsorTempPrivateKey = NULL_PTR;
	CK_OBJECT_HANDLE hReceiverPublicKey = NULL_PTR, hReceiverPrivateKey = NULL_PTR, hReceiverTempPublicKey = NULL_PTR, hReceiverTempPrivateKey = NULL_PTR;

	//for zuc test
	unsigned char	ZUCplain[136] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, \
		0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03};
	unsigned char	ZUCiv[] = {0x00, 0x05, 0x68, 0x23, 0x38};
	unsigned char	ZUCCipher[136] = {0x39, 0x93, 0x18, 0xB9, 0x68, 0x17, 0xD4, 0x34, 0xD3, 0x9C, 0x0A, 0x6A, 0x14, 0xBB, 0xA0, 0xCD, 0x2A, 0xB3, 0x13, 0x64, 0x42, 0xE5, 0x5D, 0x6E, 0xE3, 0xED, 0x7B, 0x15, 0x30, 0x37, 0x57, 0x28, 0x2F, 0x7F, 0xE3, 0xA7, 0x6C, 0x3A, 0x14, 0x34, 0xD3, 0x95, 0xB0, 0x11, 0xE4, 0x92, 0x0A, 0x0F, 0x85, 0x33, 0xCA, 0x8C, 0x2B, 0x63, 0x15, 0x81, 0xB3, 0x24, 0x6E, 0x4B, 0x34, 0xF5, 0xAF, 0xCA, 0x38, 0x5E, 0x05, 0xD2, 0xD7, 0x0E, 0x96, 0x51, 0xE2, 0xC8, 0x7C, 0x07, 0x95, 0xE6, 0x83, 0x82, 0x21, 0xF6, 0x37, 0xC9, 0x93, 0x47, 0x97, 0xA5, 0x69, 0x3B, 0xB8, 0xB8, 0x77, 0xD5, 0x6C, 0x8F, 0x66, 0xFC, 0x8D, 0x28, \
		0x9F, 0xF3, 0xF8, 0xAC, 0xE6, 0x1A, 0x02, 0xF3, 0x98, 0x44, 0x85, 0xE6, 0x58, 0xC0, 0x63, 0xF2, 0xEB, 0x5B, 0x1E, 0x09, 0xD8, 0xC6, 0x11, 0x00, 0xB2, 0x85, 0x83, 0x90, 0x0F, 0xD5, 0xA2, 0xAA, 0xBF, 0x95, 0x57, 0xB1};
	CK_MECHANISM ZUCmechanism = {CKM_ZUC_CALC, ZUCiv, sizeof(ZUCiv)};

	unsigned int i = 0,j=0, loopTime = 10;
	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);
	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2KeyCoordinate);

#ifdef _SM2_KEYEX_GENKEY
	CK_MECHANISM      mechanism = {CKM_ECC_KEY_PAIR_GEN, NULL_PTR, 0};
	CK_UINT     modulusBits = 256;
	CK_BYTE      subject[] = {0x00,0x01,0x02};
	CK_BYTE      id[] = {0x00,0x01,0x02,0x03};
	CK_KEY_TYPE  keyType=CKK_ECC;
	CK_ATTRIBUTE publicKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_SUBJECT, subject, sizeof(subject)},
		{CKA_ID, id, sizeof(id)},
		//{CKA_WRAP, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)}
	};



	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_SUBJECT, subject, sizeof(subject)},
		{CKA_ID, id, sizeof(id)},
		{CKA_SENSITIVE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		//{CKA_UNWRAP, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)}
	};

	CK_BYTE pub_x[32] = {0};
	CK_BYTE pub_y[32] = {0};
	CK_ATTRIBUTE pubGetSponsorKeyTemplate[] = {
		{CKA_ECC_X_COORDINATE, pub_x, sizeof(pub_x)},
		{CKA_ECC_Y_COORDINATE, pub_y, sizeof(pub_y)}
	};

	CK_BYTE temp_pub_x[32] = {0};
	CK_BYTE temp_pub_y[32] = {0};
	CK_ATTRIBUTE pubGetSponsorTmpKeyTemplate[] = {
		{CKA_ECC_X_COORDINATE, temp_pub_x, sizeof(pub_x)},
		{CKA_ECC_Y_COORDINATE, temp_pub_y, sizeof(pub_y)}
	};

	CK_BYTE pbOtherPublicKeyX[32] = {0};
	CK_BYTE pbOtherPublicKeyY[32] = {0};
	CK_ATTRIBUTE pubGetReceiverKeyTemplate[] = {
		{CKA_ECC_X_COORDINATE, pbOtherPublicKeyX, sizeof(pub_x)},
		{CKA_ECC_Y_COORDINATE, pbOtherPublicKeyY, sizeof(pub_y)}
	};

	CK_BYTE pbOtherTempPublicKeyX[32] = {0};
	CK_BYTE pbOtherTempPublicKeyY[32] = {0};
	CK_ATTRIBUTE pubGetReceiverTmpKeyTemplate[] = {
		{CKA_ECC_X_COORDINATE, pbOtherTempPublicKeyX, sizeof(pub_x)},
		{CKA_ECC_Y_COORDINATE, pbOtherTempPublicKeyY, sizeof(pub_y)}
	};

	//生成发送端密钥对
	//产生永久密钥对
	rv = (*pC_GenerateKeyPair)(hSession, &mechanism,
		publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hSponsorPublicKey, &hSponsorPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPairT1,rv);
	//获取永久公钥
	rv = (*pC_GetAttributeValue)(hSession, hSponsorPublicKey, pubGetSponsorKeyTemplate, sizeof(pubGetSponsorKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValueT1,rv);

	//产生临时密钥对
	rv = (*pC_GenerateKeyPair)(hSession, &mechanism,
		publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hSponsorTempPublicKey, &hSponsorTempPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPairT2,rv);
	//获取临时公钥
	rv = (*pC_GetAttributeValue)(hSession, hSponsorTempPublicKey, pubGetSponsorTmpKeyTemplate, sizeof(pubGetSponsorTmpKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValueT2,rv);

	//生成接收端密钥对
	//产生永久密钥对
	rv = (*pC_GenerateKeyPair)(hSession, &mechanism,
		publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hReceiverPublicKey, &hReceiverPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPairR1,rv);
	//获取永久公钥
	rv = (*pC_GetAttributeValue)(hSession, hReceiverPublicKey, pubGetReceiverKeyTemplate, sizeof(pubGetReceiverKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValueR1,rv);

	//产生临时密钥对
	rv = (*pC_GenerateKeyPair)(hSession, &mechanism,
		publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hReceiverTempPublicKey, &hReceiverTempPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPairR2,rv);
	//获取临时公钥
	rv = (*pC_GetAttributeValue)(hSession, hReceiverTempPublicKey, pubGetReceiverTmpKeyTemplate, sizeof(pubGetReceiverTmpKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValueR2,rv);

#else //_SM2_KEYEX_GENKEY
	CK_BYTE     prv[32]={
		0x65, 0x83, 0x2D, 0x7C, 0x7B, 0x27, 0x2E, 0x98, 0x57, 0xD1, 0x08, 0x4F, 0x3A, 0x75, 0x54, 0x0D, 
		0x40, 0xC2, 0x04, 0x23, 0x3A, 0x6A, 0x65, 0x6A, 0x15, 0x9D, 0x59, 0x52, 0x2D, 0x68, 0x4E, 0x3E
	};
	CK_BYTE     pub_x[32]={
		0xD6, 0xA7, 0x78, 0x8C, 0x41, 0xC3, 0xA3, 0x66, 0x18, 0xE8, 0x02, 0x1E, 0xE1, 0xEB, 0x27, 0x2F, 
		0xA7, 0x24, 0x7F, 0x77, 0xF4, 0x7F, 0x40, 0xBE, 0x6A, 0x31, 0x02, 0x48, 0x41, 0xA0, 0x14, 0xC6
	};
	CK_BYTE     pub_y[32]={
		0x39, 0xE2, 0xF0, 0xA4, 0x69, 0x22, 0xD3, 0x56, 0xBD, 0xB0, 0x39, 0x12, 0x26, 0xA9, 0xD1, 0x47, 
		0x40, 0x84, 0x3D, 0xFE, 0x57, 0x68, 0x0D, 0x28, 0x87, 0x44, 0xB7, 0xD8, 0x20, 0xE0, 0x32, 0xBB
	};

	CK_BYTE     temp_prv[32] = {
		0x3D, 0x73, 0x7C, 0x01, 0x52, 0x36, 0x79, 0xB0, 0x39, 0x95, 0x60, 0xD1, 0x27, 0x4E, 0x77, 0x7C, 
		0x59, 0xD0, 0x40, 0xBF, 0x28, 0x38, 0x0E, 0x33, 0x1F, 0xFA, 0x66, 0x79, 0x66, 0xF2, 0x02, 0x89
	};
	CK_BYTE     temp_pub_x[32] = {
		0x38, 0xCD, 0xAD, 0xDD, 0xDA, 0x1C, 0x6C, 0x51, 0x35, 0x23, 0xCC, 0xB3, 0x2E, 0x0E, 0x28, 0x01, 
		0x26, 0x2B, 0x7F, 0x95, 0xF2, 0x5E, 0x85, 0x3C, 0x86, 0xE1, 0x01, 0x28, 0x6C, 0x82, 0xD7, 0x18
	};
	CK_BYTE     temp_pub_y[32] = {
		0x6A, 0x1C, 0x8A, 0x61, 0xC4, 0xD3, 0xED, 0x61, 0x9D, 0xFA, 0xA0, 0x33, 0x20, 0x5C, 0x5F, 0x76, 
		0x9B, 0x3B, 0xE1, 0x2C, 0x8F, 0x15, 0x7C, 0x2F, 0x55, 0x75, 0xFF, 0xEE, 0xFB, 0x42, 0xA0, 0x64
	};

	unsigned char pbOtherPublicKeyX[32] = {
		0xC0, 0x54, 0x13, 0x71, 0xDA, 0xA7, 0x29, 0x6E, 0x04, 0xDF, 0x21, 0x90, 0xF6, 0xF3, 0x91, 0xCF, 
		0xBF, 0x3C, 0x4C, 0x2E, 0x42, 0xA2, 0x8B, 0x63, 0xBE, 0x9F, 0xE4, 0x0C, 0xFD, 0x9B, 0x11, 0x9E
	};
	unsigned char pbOtherPublicKeyY[32] = {
		0x9B, 0xAA, 0x6A, 0xFF, 0xEB, 0x03, 0x2D, 0x7A, 0x82, 0xD9, 0x90, 0xA8, 0x36, 0x8D, 0x0B, 0x82, 
		0x35, 0x5A, 0x56, 0xD1, 0xBC, 0x14, 0xEA, 0xA2, 0xB4, 0x7E, 0x69, 0xF7, 0xB6, 0x21, 0x1E, 0xC3
	};
	unsigned char pbOtherPrivateKey[32] = {
		0x23, 0xEA, 0x2D, 0x20, 0x4E, 0x69, 0x41, 0xD0, 0x50, 0x37, 0x01, 0xAB, 0x6A, 0xA7, 0x5F, 0x86, 
		0x2B, 0x1E, 0x7B, 0x11, 0x13, 0x61, 0x43, 0x68, 0x24, 0xDC, 0x74, 0xCA, 0x39, 0x86, 0x11, 0x5B
	};
	unsigned char pbOtherTempPublicKeyX[32] = {
		0xD6, 0x5C, 0xA2, 0xDA, 0x1C, 0x05, 0x8E, 0xFA, 0xFD, 0x7E, 0x98, 0x4B, 0x86, 0x08, 0xF6, 0x24, 
		0x0D, 0xE8, 0x78, 0x03, 0x87, 0x98, 0x2D, 0x71, 0xF6, 0xFC, 0xB7, 0x87, 0x6E, 0xD2, 0x36, 0x58
	};
	unsigned char pbOtherTempPublicKeyY[32] = {
		0x0B, 0xEB, 0x90, 0x5B, 0x18, 0x52, 0x3E, 0xDB, 0xE2, 0x46, 0x45, 0xCD, 0x08, 0x8F, 0x7B, 0x02, 
		0x5C, 0xCE, 0xE4, 0xBF, 0xD8, 0x46, 0x8A, 0xA5, 0x0F, 0x02, 0x2C, 0x55, 0x32, 0xEF, 0x88, 0xB5
	};
	unsigned char pbOtherTempPrivateKey[32] = {
		0x6E, 0xDD, 0x1A, 0xD0, 0x2D, 0x5F, 0x4A, 0x8C, 0x10, 0x25, 0x42, 0xF2, 0x56, 0xCC, 0x30, 0xFC, 
		0x6C, 0x88, 0x67, 0xEB, 0x59, 0xC4, 0x67, 0xEC, 0x02, 0x36, 0x03, 0x5D, 0x2E, 0x17, 0x06, 0x9D
	};

	CK_KEY_TYPE  keyType=CKK_ECC;
	CK_OBJECT_CLASS pubclass=CKO_PUBLIC_KEY,priclass=CKO_PRIVATE_KEY;
	CK_UINT     modulusBits = 256;

	CK_BYTE      id[] = {0x01,0x01,0x02,0x03};

	CK_ATTRIBUTE publicKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, pub_x, sizeof(temp_pub_x)},
		{CKA_ECC_Y_COORDINATE, pub_y, sizeof(temp_pub_y)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_PRIVATE, prv, sizeof(temp_prv)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE temp_publicKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, temp_pub_x, sizeof(temp_pub_x)},
		{CKA_ECC_Y_COORDINATE, temp_pub_y, sizeof(temp_pub_y)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE temp_privateKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_PRIVATE, temp_prv, sizeof(temp_prv)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE publicOtherKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, pbOtherPublicKeyX, sizeof(pbOtherPublicKeyX)},
		{CKA_ECC_Y_COORDINATE, pbOtherPublicKeyY, sizeof(pbOtherPublicKeyY)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE privateOtherKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_PRIVATE, pbOtherPrivateKey, sizeof(pbOtherPrivateKey)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE temp_publicOtherKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, pbOtherTempPublicKeyX, sizeof(pbOtherTempPublicKeyX)},
		{CKA_ECC_Y_COORDINATE, pbOtherTempPublicKeyY, sizeof(pbOtherTempPublicKeyY)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE temp_privateOtherKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_PRIVATE, pbOtherTempPrivateKey, sizeof(pbOtherTempPrivateKey)},
		{CKA_ID, id, sizeof(id)}
	};

	//创建公钥对象
	rv = (*pC_CreateObject)(hSession, 
		publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hSponsorPublicKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectT1,rv);

	//创建私钥对象
	rv = (*pC_CreateObject)(hSession, 
		privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hSponsorPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectT2,rv);

	//创建临时公钥对象
	rv = (*pC_CreateObject)(hSession, 
		temp_publicKeyTemplate, sizeof(temp_publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hSponsorTempPublicKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectT3,rv);

	//创建临时私钥对象
	rv = (*pC_CreateObject)(hSession, 
		temp_privateKeyTemplate, sizeof(temp_privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hSponsorTempPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectT4,rv);

	//创建对方公钥对象
	rv = (*pC_CreateObject)(hSession, 
		publicOtherKeyTemplate, sizeof(publicOtherKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hReceiverPublicKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectR1,rv);

	//创建对方私钥对象
	rv = (*pC_CreateObject)(hSession, 
		privateOtherKeyTemplate, sizeof(privateOtherKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hReceiverPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectR2,rv);

	//创建对方临时公钥对象
	rv = (*pC_CreateObject)(hSession, 
		temp_publicOtherKeyTemplate, sizeof(temp_publicOtherKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hReceiverTempPublicKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectR3,rv);

	//创建对方临时私钥对象
	rv = (*pC_CreateObject)(hSession, 
		temp_privateOtherKeyTemplate, sizeof(temp_privateOtherKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hReceiverTempPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectR4,rv);
#endif //_SM2_KEYEX_GENKEY

	//sponsor
	sm2KeyExSponsorMech.ulParameterLen = sizeof(CK_OBJECT_HANDLE)*4 + sizeof(CK_UINT) + 64 + 64 + sizeof(CK_UINT);
	pSponsorMechanismParameter = (unsigned char *)malloc(sm2KeyExSponsorMech.ulParameterLen);
	sm2KeyExSponsorMech.pParameter = pSponsorMechanismParameter;

	(*(CK_OBJECT_HANDLE *)pSponsorMechanismParameter) = hSponsorPublicKey;
	(*(CK_OBJECT_HANDLE*)((unsigned char*)pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE))) = hSponsorPrivateKey;
	(*(CK_OBJECT_HANDLE*)((unsigned char*)pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 2)) = hSponsorTempPublicKey;
	(*(CK_OBJECT_HANDLE*)((unsigned char*)pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 3)) = hSponsorTempPrivateKey;
	(*(CK_UINT*)((unsigned char*)pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4)) = 16;
	memcpy(pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT), pbOtherPublicKeyX, 32);
	memcpy(pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT) + 32, pbOtherPublicKeyY, 32);
	memcpy(pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT) + 64, pbOtherTempPublicKeyX, 32);
	memcpy(pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT) + 96, pbOtherTempPublicKeyY, 32);
	(*(CK_UINT*)((unsigned char*)pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4  + sizeof(CK_UINT) + 32 * 4)) = 1;

	rv = (*pC_DeriveKey)(hSession, &sm2KeyExSponsorMech, NULL_PTR, keySponsorAttrib, sizeof(keySponsorAttrib)/sizeof(CK_ATTRIBUTE), &hSponsorKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey1,rv);

	//receiver
	sm2KeyExReceiverMech.ulParameterLen = sizeof(CK_OBJECT_HANDLE)*4 + sizeof(CK_UINT) + 64 + 64 + sizeof(CK_UINT);
	pReceiverMechanismParameter = (unsigned char *)malloc(sm2KeyExReceiverMech.ulParameterLen);
	sm2KeyExReceiverMech.pParameter = pReceiverMechanismParameter;

	(*(CK_OBJECT_HANDLE *)pReceiverMechanismParameter) = hReceiverPublicKey;
	(*(CK_OBJECT_HANDLE*)((unsigned char*)pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE))) = hReceiverPrivateKey;
	(*(CK_OBJECT_HANDLE*)((unsigned char*)pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 2)) = hReceiverTempPublicKey;
	(*(CK_OBJECT_HANDLE*)((unsigned char*)pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 3)) = hReceiverTempPrivateKey;
	(*(CK_UINT*)((unsigned char*)pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4)) = 16;
	memcpy(pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT), pub_x, 32);
	memcpy(pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT) + 32, pub_y, 32);
	memcpy(pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT) + 64, temp_pub_x, 32);
	memcpy(pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT) + 96, temp_pub_y, 32);
	(*(CK_UINT*)((unsigned char*)pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4  + sizeof(CK_UINT) + 32 * 4)) = 0;

	rv = (*pC_DeriveKey)(hSession, &sm2KeyExReceiverMech, NULL_PTR, keyReceiverAttrib, sizeof(keyReceiverAttrib)/sizeof(CK_ATTRIBUTE), &hReceiverKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey2,rv);

	//对称加密初始化
	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism, hSponsorKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);
	//对称解密初始化
	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism, hReceiverKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	for (i=0; i<loopTime; i++)
	{
		indatalen = sizeof(indata);
		memset(indata, 0, indatalen);
		outdatalen=sizeof(outdata);
		memset(outdata, 0, outdatalen);

		indatalen1 = sizeof(indata1);
		memset(indata1, 0, indatalen1);
		outdatalen1=sizeof(outdata1);
		memset(outdata1, 0, outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv, sizeof(ZUCiv));
		memcpy(indata+sizeof(ZUCiv), ZUCplain, sizeof(ZUCplain));
		indatalen = sizeof(ZUCiv) + sizeof(ZUCplain);

		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);

#ifndef _SM2_KEYEX_GENKEY
		if ((outdatalen != sizeof(ZUCCipher)) || (memcmp(outdata, ZUCCipher, outdatalen)))
		{
			printf("Error: test_SM2KeyCoordinate.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Error: test_SM2KeyCoordinate.<br>");
			bRtn=false;
			goto END;
		}
#endif
		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv, sizeof(ZUCiv));
		memcpy(indata1+sizeof(ZUCiv), outdata, outdatalen);
		indatalen1 = sizeof(ZUCiv) + outdatalen;

		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);

		if ((outdatalen1 != sizeof(ZUCplain)) || (memcmp(outdata1, ZUCplain, outdatalen1)))
		{
			printf("Error: test_SM2KeyCoordinate.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Error: test_SM2KeyCoordinate.<br>");
			bRtn=false;
			goto END;
		}	
		RandomGenerate(ZUCplain,136);
		RandomGenerate(ZUCiv,5);
	}
	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	bRtn = true;
END:
	if (hSponsorPublicKey)
	{
		rv = pC_DestroyObject(hSession, hSponsorPublicKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);
	}
	if (hSponsorPrivateKey)
	{
		rv = pC_DestroyObject(hSession, hSponsorPrivateKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);
	}
	if (hSponsorTempPublicKey)
	{
		rv = pC_DestroyObject(hSession, hSponsorTempPublicKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject3,rv);
	}
	if (hSponsorTempPrivateKey)
	{
		rv = pC_DestroyObject(hSession, hSponsorTempPrivateKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject4,rv);
	}
	if (hSponsorKey)
	{
		rv = pC_DestroyObject(hSession, hSponsorKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject5,rv);
	}
	if (hReceiverPublicKey)
	{
		rv = pC_DestroyObject(hSession, hReceiverPublicKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject6,rv);
	}
	if (hReceiverPrivateKey)
	{
		rv = pC_DestroyObject(hSession, hReceiverPrivateKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject7,rv);
	}
	if (hReceiverTempPublicKey)
	{
		rv = pC_DestroyObject(hSession, hReceiverTempPublicKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject8,rv);
	}
	if (hReceiverTempPrivateKey)
	{
		rv = pC_DestroyObject(hSession, hReceiverTempPrivateKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject9,rv);
	}
	if (hReceiverKey)
	{
		rv = pC_DestroyObject(hSession, hReceiverKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject10,rv);
	}

	if (sm2KeyExReceiverMech.pParameter)
	{
		free(sm2KeyExReceiverMech.pParameter);
	}
	if (sm2KeyExSponsorMech.pParameter)
	{
		free(sm2KeyExSponsorMech.pParameter);
	}
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2KeyCoordinate);

	
	return bRtn;
}

bool xtest_SM2KeyCoordinate_Performace()
{
	bool	bRtn = false;
	CK_RV	rv = 0;
	unsigned char *pSponsorMechanismParameter = NULL;
	CK_MECHANISM sm2KeyExSponsorMech = {CKM_DERIVE_SM2KEYEX, NULL, 0};
	unsigned char *pReceiverMechanismParameter = NULL;
	CK_MECHANISM sm2KeyExReceiverMech = {CKM_DERIVE_SM2KEYEX, NULL, 0};

	CK_BYTE	nSponsorSessKeyID = CK_SESSKEY_ID0;
	CK_BYTE	nReceiverSessKeyID = CK_SESSKEY_ID0+1;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_ATTRIBUTE keySponsorAttrib[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSponsorSessKeyID, sizeof(CK_BYTE)}
	};
	CK_ATTRIBUTE keyReceiverAttrib[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nReceiverSessKeyID, sizeof(CK_BYTE)}
	};
	CK_OBJECT_HANDLE hSponsorKey = NULL_PTR, hReceiverKey = NULL_PTR;
	CK_OBJECT_HANDLE hSponsorPublicKey = NULL_PTR, hSponsorPrivateKey = NULL_PTR, hSponsorTempPublicKey = NULL_PTR, hSponsorTempPrivateKey = NULL_PTR;
	CK_OBJECT_HANDLE hReceiverPublicKey = NULL_PTR, hReceiverPrivateKey = NULL_PTR, hReceiverTempPublicKey = NULL_PTR, hReceiverTempPrivateKey = NULL_PTR;

	//for zuc test
	unsigned char	ZUCplain[136] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, \
		0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03};
	unsigned char	ZUCiv[] = {0x00, 0x05, 0x68, 0x23, 0x38};
	unsigned char	ZUCCipher[136] = {0x39, 0x93, 0x18, 0xB9, 0x68, 0x17, 0xD4, 0x34, 0xD3, 0x9C, 0x0A, 0x6A, 0x14, 0xBB, 0xA0, 0xCD, 0x2A, 0xB3, 0x13, 0x64, 0x42, 0xE5, 0x5D, 0x6E, 0xE3, 0xED, 0x7B, 0x15, 0x30, 0x37, 0x57, 0x28, 0x2F, 0x7F, 0xE3, 0xA7, 0x6C, 0x3A, 0x14, 0x34, 0xD3, 0x95, 0xB0, 0x11, 0xE4, 0x92, 0x0A, 0x0F, 0x85, 0x33, 0xCA, 0x8C, 0x2B, 0x63, 0x15, 0x81, 0xB3, 0x24, 0x6E, 0x4B, 0x34, 0xF5, 0xAF, 0xCA, 0x38, 0x5E, 0x05, 0xD2, 0xD7, 0x0E, 0x96, 0x51, 0xE2, 0xC8, 0x7C, 0x07, 0x95, 0xE6, 0x83, 0x82, 0x21, 0xF6, 0x37, 0xC9, 0x93, 0x47, 0x97, 0xA5, 0x69, 0x3B, 0xB8, 0xB8, 0x77, 0xD5, 0x6C, 0x8F, 0x66, 0xFC, 0x8D, 0x28, \
		0x9F, 0xF3, 0xF8, 0xAC, 0xE6, 0x1A, 0x02, 0xF3, 0x98, 0x44, 0x85, 0xE6, 0x58, 0xC0, 0x63, 0xF2, 0xEB, 0x5B, 0x1E, 0x09, 0xD8, 0xC6, 0x11, 0x00, 0xB2, 0x85, 0x83, 0x90, 0x0F, 0xD5, 0xA2, 0xAA, 0xBF, 0x95, 0x57, 0xB1};
	CK_MECHANISM ZUCmechanism = {CKM_ZUC_CALC, ZUCiv, sizeof(ZUCiv)};

	unsigned int i = 0,j=0, loopTime = 10;
	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);
	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	UtilscTime ttc1={0, 0},ttc2={0,0};//,ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2KeyCoordinatePerformace);

#ifdef _SM2_KEYEX_GENKEY
	CK_MECHANISM      mechanism = {CKM_ECC_KEY_PAIR_GEN, NULL_PTR, 0};
	CK_UINT     modulusBits = 256;
	CK_BYTE      subject[] = {0x00,0x01,0x02};
	CK_BYTE      id[] = {0x00,0x01,0x02,0x03};
	CK_KEY_TYPE  keyType=CKK_ECC;
	CK_ATTRIBUTE publicKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_SUBJECT, subject, sizeof(subject)},
		{CKA_ID, id, sizeof(id)},
		//{CKA_WRAP, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)}
	};
	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_SUBJECT, subject, sizeof(subject)},
		{CKA_ID, id, sizeof(id)},
		{CKA_SENSITIVE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		//{CKA_UNWRAP, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)}
	};

	CK_BYTE pub_x[32] = {0};
	CK_BYTE pub_y[32] = {0};
	CK_ATTRIBUTE pubGetSponsorKeyTemplate[] = {
		{CKA_ECC_X_COORDINATE, pub_x, sizeof(pub_x)},
		{CKA_ECC_Y_COORDINATE, pub_y, sizeof(pub_y)}
	};

	CK_BYTE temp_pub_x[32] = {0};
	CK_BYTE temp_pub_y[32] = {0};
	CK_ATTRIBUTE pubGetSponsorTmpKeyTemplate[] = {
		{CKA_ECC_X_COORDINATE, temp_pub_x, sizeof(pub_x)},
		{CKA_ECC_Y_COORDINATE, temp_pub_y, sizeof(pub_y)}
	};

	CK_BYTE pbOtherPublicKeyX[32] = {0};
	CK_BYTE pbOtherPublicKeyY[32] = {0};
	CK_ATTRIBUTE pubGetReceiverKeyTemplate[] = {
		{CKA_ECC_X_COORDINATE, pbOtherPublicKeyX, sizeof(pub_x)},
		{CKA_ECC_Y_COORDINATE, pbOtherPublicKeyY, sizeof(pub_y)}
	};

	CK_BYTE pbOtherTempPublicKeyX[32] = {0};
	CK_BYTE pbOtherTempPublicKeyY[32] = {0};
	CK_ATTRIBUTE pubGetReceiverTmpKeyTemplate[] = {
		{CKA_ECC_X_COORDINATE, pbOtherTempPublicKeyX, sizeof(pub_x)},
		{CKA_ECC_Y_COORDINATE, pbOtherTempPublicKeyY, sizeof(pub_y)}
	};

	//生成发送端密钥对
	//产生永久密钥对
	rv = (*pC_GenerateKeyPair)(hSession, &mechanism,
		publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hSponsorPublicKey, &hSponsorPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPairT1,rv);
	//获取永久公钥
	rv = (*pC_GetAttributeValue)(hSession, hSponsorPublicKey, pubGetSponsorKeyTemplate, sizeof(pubGetSponsorKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValueT1,rv);

	//产生临时密钥对
	rv = (*pC_GenerateKeyPair)(hSession, &mechanism,
		publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hSponsorTempPublicKey, &hSponsorTempPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPairT2,rv);
	//获取临时公钥
	rv = (*pC_GetAttributeValue)(hSession, hSponsorTempPublicKey, pubGetSponsorTmpKeyTemplate, sizeof(pubGetSponsorTmpKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValueT2,rv);

	//生成接收端密钥对
	//产生永久密钥对
	rv = (*pC_GenerateKeyPair)(hSession, &mechanism,
		publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hReceiverPublicKey, &hReceiverPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPairR1,rv);
	//获取永久公钥
	rv = (*pC_GetAttributeValue)(hSession, hReceiverPublicKey, pubGetReceiverKeyTemplate, sizeof(pubGetReceiverKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValueR1,rv);

	//产生临时密钥对
	rv = (*pC_GenerateKeyPair)(hSession, &mechanism,
		publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hReceiverTempPublicKey, &hReceiverTempPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPairR2,rv);
	//获取临时公钥
	rv = (*pC_GetAttributeValue)(hSession, hReceiverTempPublicKey, pubGetReceiverTmpKeyTemplate, sizeof(pubGetReceiverTmpKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValueR2,rv);

#else //_SM2_KEYEX_GENKEY
	CK_BYTE     prv[32]={
		0x65, 0x83, 0x2D, 0x7C, 0x7B, 0x27, 0x2E, 0x98, 0x57, 0xD1, 0x08, 0x4F, 0x3A, 0x75, 0x54, 0x0D, 
		0x40, 0xC2, 0x04, 0x23, 0x3A, 0x6A, 0x65, 0x6A, 0x15, 0x9D, 0x59, 0x52, 0x2D, 0x68, 0x4E, 0x3E
	};
	CK_BYTE     pub_x[32]={
		0xD6, 0xA7, 0x78, 0x8C, 0x41, 0xC3, 0xA3, 0x66, 0x18, 0xE8, 0x02, 0x1E, 0xE1, 0xEB, 0x27, 0x2F, 
		0xA7, 0x24, 0x7F, 0x77, 0xF4, 0x7F, 0x40, 0xBE, 0x6A, 0x31, 0x02, 0x48, 0x41, 0xA0, 0x14, 0xC6
	};
	CK_BYTE     pub_y[32]={
		0x39, 0xE2, 0xF0, 0xA4, 0x69, 0x22, 0xD3, 0x56, 0xBD, 0xB0, 0x39, 0x12, 0x26, 0xA9, 0xD1, 0x47, 
		0x40, 0x84, 0x3D, 0xFE, 0x57, 0x68, 0x0D, 0x28, 0x87, 0x44, 0xB7, 0xD8, 0x20, 0xE0, 0x32, 0xBB
	};

	CK_BYTE     temp_prv[32] = {
		0x3D, 0x73, 0x7C, 0x01, 0x52, 0x36, 0x79, 0xB0, 0x39, 0x95, 0x60, 0xD1, 0x27, 0x4E, 0x77, 0x7C, 
		0x59, 0xD0, 0x40, 0xBF, 0x28, 0x38, 0x0E, 0x33, 0x1F, 0xFA, 0x66, 0x79, 0x66, 0xF2, 0x02, 0x89
	};
	CK_BYTE     temp_pub_x[32] = {
		0x38, 0xCD, 0xAD, 0xDD, 0xDA, 0x1C, 0x6C, 0x51, 0x35, 0x23, 0xCC, 0xB3, 0x2E, 0x0E, 0x28, 0x01, 
		0x26, 0x2B, 0x7F, 0x95, 0xF2, 0x5E, 0x85, 0x3C, 0x86, 0xE1, 0x01, 0x28, 0x6C, 0x82, 0xD7, 0x18
	};
	CK_BYTE     temp_pub_y[32] = {
		0x6A, 0x1C, 0x8A, 0x61, 0xC4, 0xD3, 0xED, 0x61, 0x9D, 0xFA, 0xA0, 0x33, 0x20, 0x5C, 0x5F, 0x76, 
		0x9B, 0x3B, 0xE1, 0x2C, 0x8F, 0x15, 0x7C, 0x2F, 0x55, 0x75, 0xFF, 0xEE, 0xFB, 0x42, 0xA0, 0x64
	};

	unsigned char pbOtherPublicKeyX[32] = {
		0xC0, 0x54, 0x13, 0x71, 0xDA, 0xA7, 0x29, 0x6E, 0x04, 0xDF, 0x21, 0x90, 0xF6, 0xF3, 0x91, 0xCF, 
		0xBF, 0x3C, 0x4C, 0x2E, 0x42, 0xA2, 0x8B, 0x63, 0xBE, 0x9F, 0xE4, 0x0C, 0xFD, 0x9B, 0x11, 0x9E
	};
	unsigned char pbOtherPublicKeyY[32] = {
		0x9B, 0xAA, 0x6A, 0xFF, 0xEB, 0x03, 0x2D, 0x7A, 0x82, 0xD9, 0x90, 0xA8, 0x36, 0x8D, 0x0B, 0x82, 
		0x35, 0x5A, 0x56, 0xD1, 0xBC, 0x14, 0xEA, 0xA2, 0xB4, 0x7E, 0x69, 0xF7, 0xB6, 0x21, 0x1E, 0xC3
	};
	unsigned char pbOtherPrivateKey[32] = {
		0x23, 0xEA, 0x2D, 0x20, 0x4E, 0x69, 0x41, 0xD0, 0x50, 0x37, 0x01, 0xAB, 0x6A, 0xA7, 0x5F, 0x86, 
		0x2B, 0x1E, 0x7B, 0x11, 0x13, 0x61, 0x43, 0x68, 0x24, 0xDC, 0x74, 0xCA, 0x39, 0x86, 0x11, 0x5B
	};
	unsigned char pbOtherTempPublicKeyX[32] = {
		0xD6, 0x5C, 0xA2, 0xDA, 0x1C, 0x05, 0x8E, 0xFA, 0xFD, 0x7E, 0x98, 0x4B, 0x86, 0x08, 0xF6, 0x24, 
		0x0D, 0xE8, 0x78, 0x03, 0x87, 0x98, 0x2D, 0x71, 0xF6, 0xFC, 0xB7, 0x87, 0x6E, 0xD2, 0x36, 0x58
	};
	unsigned char pbOtherTempPublicKeyY[32] = {
		0x0B, 0xEB, 0x90, 0x5B, 0x18, 0x52, 0x3E, 0xDB, 0xE2, 0x46, 0x45, 0xCD, 0x08, 0x8F, 0x7B, 0x02, 
		0x5C, 0xCE, 0xE4, 0xBF, 0xD8, 0x46, 0x8A, 0xA5, 0x0F, 0x02, 0x2C, 0x55, 0x32, 0xEF, 0x88, 0xB5
	};
	unsigned char pbOtherTempPrivateKey[32] = {
		0x6E, 0xDD, 0x1A, 0xD0, 0x2D, 0x5F, 0x4A, 0x8C, 0x10, 0x25, 0x42, 0xF2, 0x56, 0xCC, 0x30, 0xFC, 
		0x6C, 0x88, 0x67, 0xEB, 0x59, 0xC4, 0x67, 0xEC, 0x02, 0x36, 0x03, 0x5D, 0x2E, 0x17, 0x06, 0x9D
	};

	CK_KEY_TYPE  keyType=CKK_ECC;
	CK_OBJECT_CLASS pubclass=CKO_PUBLIC_KEY,priclass=CKO_PRIVATE_KEY;
	CK_UINT     modulusBits = 256;

	CK_BYTE      id[] = {0x01,0x01,0x02,0x03};

	CK_ATTRIBUTE publicKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, pub_x, sizeof(temp_pub_x)},
		{CKA_ECC_Y_COORDINATE, pub_y, sizeof(temp_pub_y)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_PRIVATE, prv, sizeof(temp_prv)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE temp_publicKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, temp_pub_x, sizeof(temp_pub_x)},
		{CKA_ECC_Y_COORDINATE, temp_pub_y, sizeof(temp_pub_y)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE temp_privateKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_PRIVATE, temp_prv, sizeof(temp_prv)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE publicOtherKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, pbOtherPublicKeyX, sizeof(pbOtherPublicKeyX)},
		{CKA_ECC_Y_COORDINATE, pbOtherPublicKeyY, sizeof(pbOtherPublicKeyY)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE privateOtherKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_PRIVATE, pbOtherPrivateKey, sizeof(pbOtherPrivateKey)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE temp_publicOtherKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, pbOtherTempPublicKeyX, sizeof(pbOtherTempPublicKeyX)},
		{CKA_ECC_Y_COORDINATE, pbOtherTempPublicKeyY, sizeof(pbOtherTempPublicKeyY)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE temp_privateOtherKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_PRIVATE, pbOtherTempPrivateKey, sizeof(pbOtherTempPrivateKey)},
		{CKA_ID, id, sizeof(id)}
	};

	//创建公钥对象
	rv = (*pC_CreateObject)(hSession, 
		publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hSponsorPublicKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectT1,rv);

	//创建私钥对象
	rv = (*pC_CreateObject)(hSession, 
		privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hSponsorPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectT2,rv);

	//创建临时公钥对象
	rv = (*pC_CreateObject)(hSession, 
		temp_publicKeyTemplate, sizeof(temp_publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hSponsorTempPublicKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectT3,rv);

	//创建临时私钥对象
	rv = (*pC_CreateObject)(hSession, 
		temp_privateKeyTemplate, sizeof(temp_privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hSponsorTempPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectT4,rv);

	//创建对方公钥对象
	rv = (*pC_CreateObject)(hSession, 
		publicOtherKeyTemplate, sizeof(publicOtherKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hReceiverPublicKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectR1,rv);

	//创建对方私钥对象
	rv = (*pC_CreateObject)(hSession, 
		privateOtherKeyTemplate, sizeof(privateOtherKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hReceiverPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectR2,rv);

	//创建对方临时公钥对象
	rv = (*pC_CreateObject)(hSession, 
		temp_publicOtherKeyTemplate, sizeof(temp_publicOtherKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hReceiverTempPublicKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectR3,rv);

	//创建对方临时私钥对象
	rv = (*pC_CreateObject)(hSession, 
		temp_privateOtherKeyTemplate, sizeof(temp_privateOtherKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hReceiverTempPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObjectR4,rv);
#endif //_SM2_KEYEX_GENKEY


	Utilsgettime(&ttc1);
	//sponsor
	sm2KeyExSponsorMech.ulParameterLen = sizeof(CK_OBJECT_HANDLE)*4 + sizeof(CK_UINT) + 64 + 64 + sizeof(CK_UINT);
	pSponsorMechanismParameter = (unsigned char *)malloc(sm2KeyExSponsorMech.ulParameterLen);
	sm2KeyExSponsorMech.pParameter = pSponsorMechanismParameter;

	//UtilsPrintData(VNAME(pbOtherPublicKeyX),32,0);
	//UtilsPrintData(VNAME(pbOtherPublicKeyY),32,0);
	//UtilsPrintData(VNAME(pbOtherTempPublicKeyX),32,0);
	//UtilsPrintData(VNAME(pbOtherTempPublicKeyY),32,0);

	(*(CK_OBJECT_HANDLE *)pSponsorMechanismParameter) = hSponsorPublicKey;
	(*(CK_OBJECT_HANDLE*)((unsigned char*)pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE))) = hSponsorPrivateKey;
	(*(CK_OBJECT_HANDLE*)((unsigned char*)pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 2)) = hSponsorTempPublicKey;
	(*(CK_OBJECT_HANDLE*)((unsigned char*)pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 3)) = hSponsorTempPrivateKey;
	(*(CK_UINT*)((unsigned char*)pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4)) = 48;
	memcpy(pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT), pbOtherPublicKeyX, 32);
	memcpy(pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT) + 32, pbOtherPublicKeyY, 32);
	memcpy(pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT) + 64, pbOtherTempPublicKeyX, 32);
	memcpy(pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT) + 96, pbOtherTempPublicKeyY, 32);
	(*(CK_UINT*)((unsigned char*)pSponsorMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4  + sizeof(CK_UINT) + 32 * 4)) = 1;

	rv = (*pC_DeriveKey)(hSession, &sm2KeyExSponsorMech, NULL_PTR, keySponsorAttrib, sizeof(keySponsorAttrib)/sizeof(CK_ATTRIBUTE), &hSponsorKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey1,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"Sponsor DeriveKey",1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb]+ nResultLength,"Sponsor DeriveKey",1);

	Utilsgettime(&ttc1);
	//receiver
	sm2KeyExReceiverMech.ulParameterLen = sizeof(CK_OBJECT_HANDLE)*4 + sizeof(CK_UINT) + 64 + 64 + sizeof(CK_UINT);
	pReceiverMechanismParameter = (unsigned char *)malloc(sm2KeyExReceiverMech.ulParameterLen);
	sm2KeyExReceiverMech.pParameter = pReceiverMechanismParameter;

	//UtilsPrintData(VNAME(pub_x),32,0);
	//UtilsPrintData(VNAME(pub_y),32,0);
	//UtilsPrintData(VNAME(temp_pub_x),32,0);
	//UtilsPrintData(VNAME(temp_pub_y),32,0);

	(*(CK_OBJECT_HANDLE *)pReceiverMechanismParameter) = hReceiverPublicKey;
	(*(CK_OBJECT_HANDLE*)((unsigned char*)pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE))) = hReceiverPrivateKey;
	(*(CK_OBJECT_HANDLE*)((unsigned char*)pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 2)) = hReceiverTempPublicKey;
	(*(CK_OBJECT_HANDLE*)((unsigned char*)pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 3)) = hReceiverTempPrivateKey;
	(*(CK_UINT*)((unsigned char*)pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4)) = 48;
	memcpy(pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT), pub_x, 32);
	memcpy(pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT) + 32, pub_y, 32);
	memcpy(pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT) + 64, temp_pub_x, 32);
	memcpy(pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4 + sizeof(CK_UINT) + 96, temp_pub_y, 32);
	(*(CK_UINT*)((unsigned char*)pReceiverMechanismParameter + sizeof(CK_OBJECT_HANDLE) * 4  + sizeof(CK_UINT) + 32 * 4)) = 0;

	rv = (*pC_DeriveKey)(hSession, &sm2KeyExReceiverMech, NULL_PTR, keyReceiverAttrib, sizeof(keyReceiverAttrib)/sizeof(CK_ATTRIBUTE), &hReceiverKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey2,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"Receiver DeriveKey",1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb]+ nResultLength,"Receiver DeriveKey",1);

	//zuc test
	//对称加密初始化
	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism, hSponsorKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);
	//对称解密初始化
	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism, hReceiverKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	for (i=0; i<loopTime; i++)
	{
		indatalen = sizeof(indata);
		memset(indata, 0, indatalen);
		outdatalen=sizeof(outdata);
		memset(outdata, 0, outdatalen);

		indatalen1 = sizeof(indata1);
		memset(indata1, 0, indatalen1);
		outdatalen1=sizeof(outdata1);
		memset(outdata1, 0, outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv, sizeof(ZUCiv));
		memcpy(indata+sizeof(ZUCiv), ZUCplain, sizeof(ZUCplain));
		indatalen = sizeof(ZUCiv) + sizeof(ZUCplain);

		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);

#ifndef _SM2_KEYEX_GENKEY
		if ((outdatalen != sizeof(ZUCCipher)) || (memcmp(outdata, ZUCCipher, outdatalen)))
		{
			printf("Error: test_SM2KeyCoordinate.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Error: test_SM2KeyCoordinate.<br>");
			bRtn=false;
			goto END;
		}
#endif
		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv, sizeof(ZUCiv));
		memcpy(indata1+sizeof(ZUCiv), outdata, outdatalen);
		indatalen1 = sizeof(ZUCiv) + outdatalen;

		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);

		if ((outdatalen1 != sizeof(ZUCplain)) || (memcmp(outdata1, ZUCplain, outdatalen1)))
		{
			printf("Error: test_SM2KeyCoordinate.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Error: test_SM2KeyCoordinate.<br>");
			bRtn=false;
			goto END;
		}	
		RandomGenerate(ZUCplain,136);
		RandomGenerate(ZUCiv,5);
	}
	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	bRtn = true;
END:
	if (hSponsorPublicKey)
	{
		rv = pC_DestroyObject(hSession, hSponsorPublicKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);
	}
	if (hSponsorPrivateKey)
	{
		rv = pC_DestroyObject(hSession, hSponsorPrivateKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);
	}
	if (hSponsorTempPublicKey)
	{
		rv = pC_DestroyObject(hSession, hSponsorTempPublicKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject3,rv);
	}
	if (hSponsorTempPrivateKey)
	{
		rv = pC_DestroyObject(hSession, hSponsorTempPrivateKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject4,rv);
	}
	if (hSponsorKey)
	{
		rv = pC_DestroyObject(hSession, hSponsorKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject5,rv);
	}
	if (hReceiverPublicKey)
	{
		rv = pC_DestroyObject(hSession, hReceiverPublicKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject6,rv);
	}
	if (hReceiverPrivateKey)
	{
		rv = pC_DestroyObject(hSession, hReceiverPrivateKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject7,rv);
	}
	if (hReceiverTempPublicKey)
	{
		rv = pC_DestroyObject(hSession, hReceiverTempPublicKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject8,rv);
	}
	if (hReceiverTempPrivateKey)
	{
		rv = pC_DestroyObject(hSession, hReceiverTempPrivateKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject9,rv);
	}
	if (hReceiverKey)
	{
		rv = pC_DestroyObject(hSession, hReceiverKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject10,rv);
	}

	if (sm2KeyExReceiverMech.pParameter)
	{
		free(sm2KeyExReceiverMech.pParameter);
	}
	if (sm2KeyExSponsorMech.pParameter)
	{
		free(sm2KeyExSponsorMech.pParameter);
	}
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2KeyCoordinatePerformace);
	return bRtn;
}


/*测试SM3密钥衍生*/
bool xtest_SM3KDF()
{
	bool	bRtn = false;
	CK_RV	rv = 0;
	//for set sm3 kdf basekey
	unsigned char	ucSM3KDFBaseKeyBuffer[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	CK_EXTEND_IN	ExtIn_SetSM3KDFBaseKey = {CK_EXTEND_SETSM3KDFBASEKEY, ucSM3KDFBaseKeyBuffer, sizeof(ucSM3KDFBaseKeyBuffer)};
	CK_EXTEND_OUT	ExtOut_SetSM3KDFBaseKey = {CK_EXTEND_SETSM3KDFBASEKEY, NULL, 0};
	//for calc sm3 kdf
	unsigned char	ucSM3KDFKeyBuffer1[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	CK_MECHANISM	sm3KDFPRESETMech = {CKM_DERIVE_SM3_KDF_WITH_PRESET, ucSM3KDFKeyBuffer1, sizeof(ucSM3KDFKeyBuffer1)};
	unsigned char	ucSM3KDFKeyBuffer2[17] = {0x11, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	CK_MECHANISM	sm3KDFBASEMech = {CKM_DERIVE_SM3_KDF, ucSM3KDFKeyBuffer2, sizeof(ucSM3KDFKeyBuffer2)};
	unsigned char	ucSM3KDFKeyBuffer3[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	CK_MECHANISM	sm3KDFNOBASEMech = {CKM_DERIVE_SM3_KDF, ucSM3KDFKeyBuffer3, sizeof(ucSM3KDFKeyBuffer3)};
	CK_BYTE	nBaseKeyID = CK_SESSKEY_ID0;
	CK_BYTE	nSessKeyID1 = CK_SESSKEY_ID0 + 14;
	CK_BYTE	nSessKeyID2 = CK_SESSKEY_ID0 + 15;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_ATTRIBUTE sm3BaseKeyAttrib[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nBaseKeyID, sizeof(CK_BYTE)}
	};
	CK_ATTRIBUTE sm3KDFAttrib1[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID1, sizeof(CK_BYTE)}
	};
	CK_ATTRIBUTE sm3KDFAttrib2[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID2, sizeof(CK_BYTE)}
	};
	CK_OBJECT_HANDLE	hBaseKey = NULL_PTR, hDerivedKey1 = NULL_PTR, hDerivedKey2 = NULL_PTR;
	//for zuc test
	unsigned char	ZUCplain[136] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, \
		0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03};
	unsigned char	ZUCiv[] = {0x00, 0x05, 0x68, 0x23, 0x38};
	CK_MECHANISM ZUCmechanism = {CKM_ZUC_CALC, ZUCiv, sizeof(ZUCiv)};
	unsigned char	ZUCCipher1[136] = {0x1b, 0xc8, 0xbe, 0x08, 0x9a, 0xe6, 0x93, 0x3e, 0x7a, 0xd1, 0x0a, 0x4b, 0xb2, 0x31, 0xeb, 0xf9, 0x12, 0x09, 0x76, 0xbe, 0x7c, 0x54, 0x82, 0x3d, 0x08, 0xa4, 0x96, 0xed, 0xbd, 0xb0, 0x80, 0xf3, 0x9c, 0x0e, 0xf7, 0xbf, 0x38, 0x4b, 0x4f, 0x83, 0x85, 0x75, 0xdc, 0x2c, 0x22, 0xef, 0xb4, 0x54, 0xa9, 0xee, 0x35, 0x8e, 0x11, 0xe6, 0x43, 0xd8, 0x79, 0x53, 0x3b, 0x3b, 0x4e, 0xb8, 0xd5, 0xed, 0x4c, 0x47, 0xf1, 0x0d, 0x45, 0x31, 0x24, 0xc3, 0xee, 0xbe, 0xe8, 0x87, 0xf6, 0x42, 0x88, 0x65, 0xcc, 0x79, 0x1a, 0xad, 0x80, 0x24, 0xfe, 0xc3, 0xc4, 0x8d, 0xd2, 0x49, 0xa6, 0x5a, 0xb3, 0x98, 0x2e, 0xfc, 0xab, 0xab, 0xdf, 0x7d, 0x45, 0xf3, 0x4a, 0x02, 0x08, 0x0c, 0xb5, 0xd6, 0x58, 0x4c, 0xd7, 0xce, 0x9f, 0x43, 0x69, 0x89, 0x4c, 0xda, 0x11, 0xf2, 0xe8, 0x40, 0x32, 0x15, 0x3e, 0x20, 0x5f, 0xf7, 0x5e, 0x4d, 0x3e, 0x69, 0x8a, 0x10};
	unsigned char	ZUCCipher2[136] = {0xB8, 0xE9, 0xA0, 0xEE, 0x6C, 0xB2, 0xE1, 0x54, 0xAB, 0x37, 0xBC, 0x80, 0x91, 0xF1, 0x6C, 0x46, 0xDC, 0x10, 0x5B, 0x81, 0xE1, 0x1E, 0x18, 0x1A, 0x4C, 0xB9, 0x5A, 0x66, 0x65, 0x33, 0x90, 0x1E, 0xFA, 0x95, 0x7C, 0x98, 0x5E, 0xF9, 0xEE, 0x23, 0x75, 0x6A, 0x38, 0x61, 0x17, 0x41, 0x71, 0x63, 0x22, 0x9B, 0x83, 0x8B, 0xB1, 0xAE, 0x99, 0xB2, 0x2C, 0x81, 0xC7, 0xB2, 0x5F, 0x2D, 0x0A, 0xD4, 0x18, 0x45, 0x96, 0xA5, 0x62, 0xFB, 0x17, 0x7D, 0x58, 0xF0, 0x50, 0x7B, 0x5A, 0xB2, 0x1E, 0x2A, 0xDC, 0xED, 0xD2, 0xFA, 0x6E, 0x37, 0x9A, 0xF5, 0x1E, 0xE5, 0xA1, 0x13, 0xB1, 0xB0, 0xE4, 0x07, 0x2A, 0x52, 0x5A, 0x8C, 0x0F, 0xB4, 0x48, 0x81, 0xCA, 0x79, 0xE8, 0x37, 0xC2, 0x9B, 0x64, 0xA6, 0x75, 0x59, 0x36, 0xD1, 0xB6, 0xAD, 0xB4, 0x8A, 0x8C, 0xE7, 0x30, 0xC3, 0x0C, 0x88, 0xE6, 0x4F, 0xBA, 0x7C, 0x5B, 0xA4, 0x36, 0xE9, 0xA2, 0x02};
	unsigned char	ZUCCipher3[136] = {0xA0, 0x3A, 0xCE, 0xA4, 0xA6, 0x27, 0x17, 0x4F, 0x87, 0x18, 0x71, 0x33, 0xA0, 0x45, 0x4F, 0x61, 0xEA, 0x1B, 0x8B, 0x0A, 0x1B, 0xE8, 0xA7, 0x35, 0xF5, 0x5F, 0xAE, 0xAD, 0xE2, 0x3D, 0xDB, 0x9A, 0xD3, 0x9F, 0x66, 0x64, 0x22, 0x22, 0x93, 0x8B, 0xD3, 0xE2, 0x67, 0x50, 0x2F, 0xF3, 0xBC, 0x50, 0x56, 0x7A, 0xA6, 0x49, 0xE3, 0xAE, 0x13, 0x78, 0xF1, 0x0B, 0xC9, 0x12, 0x7A, 0x68, 0xB5, 0x23, 0xF8, 0x2C, 0xC3, 0xD3, 0x5F, 0x1F, 0x9D, 0xF9, 0xE4, 0xFA, 0xA3, 0xCA, 0xC2, 0x05, 0x8E, 0x5E, 0x19, 0x5E, 0x9D, 0x4F, 0xCD, 0x97, 0x54, 0xBE, 0xEA, 0x83, 0x4D, 0x58, 0xC8, 0xDA, 0xAD, 0x72, 0x26, 0xF6, 0xC4, 0x87, 0xFD, 0x60, 0x74, 0xF2, 0x0E, 0xE9, 0x41, 0xFF, 0xAF, 0x8B, 0x71, 0x04, 0x18, 0x55, 0xD4, 0x59, 0xAC, 0xC4, 0x12, 0xD2, 0xC5, 0x0F, 0x85, 0x8E, 0x60, 0xAE, 0xBA, 0x62, 0x93, 0x19, 0x41, 0x66, 0x0C, 0x8B, 0x62, 0x51};
	unsigned int i = 0, j=0,loopTime = 10;
	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);
	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM3KDF);

	srand( (unsigned)time( NULL ) );//随机数初始化

	//set sm3 kdf basekey
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetSM3KDFBaseKey, &ExtOut_SetSM3KDFBaseKey, NULL);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CryptoExtend1,rv);
	//printf("hBaseKey=%08x\n",hBaseKey);
	//calculate sm3 kdf with preset
	rv = (*pC_DeriveKey)(hSession, &sm3KDFPRESETMech, NULL_PTR, sm3BaseKeyAttrib, sizeof(sm3BaseKeyAttrib)/sizeof(CK_ATTRIBUTE), &hBaseKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey1,rv);
	//printf("hBaseKey=%08x\n",hBaseKey);
	//zuc test
	//对称加密初始化
	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism, hBaseKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit1,rv);
	//对称解密初始化
	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism, hBaseKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit1,rv);

	for (i=0; i<loopTime; i++)
	{
		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv, sizeof(ZUCiv));
		memcpy(indata+sizeof(ZUCiv), ZUCplain, sizeof(ZUCplain));
		indatalen = sizeof(ZUCiv) + sizeof(ZUCplain);

		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate1,rv);
		if (i==0)
		{
			if ((outdatalen != sizeof(ZUCCipher1)) || (memcmp(outdata, ZUCCipher1, outdatalen)))
			{
				printf("Error: test_SM3KDF0.\n");
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: test_SM3KDF0.<br>");
				bRtn=false;
				goto END;
			}
		}		
		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv, sizeof(ZUCiv));
		memcpy(indata1+sizeof(ZUCiv), outdata, outdatalen);
		indatalen1 = sizeof(ZUCiv) + outdatalen;

		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate1,rv);
	
		if ((outdatalen1 != sizeof(ZUCplain)) || (memcmp(outdata1, ZUCplain, outdatalen1)))
		{
			printf("Error: test_SM3KDF1.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: test_SM3KDF1.<br>");
			bRtn=false;
			goto END;
		}
		RandomGenerate(ZUCplain,128);
		RandomGenerate(ZUCiv,5);
	}

	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal1,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal1,rv);

	//printf("hDerivedKey1=%08x\n",hDerivedKey1);
	//calculate sm3 kdf with basekey
	rv = (*pC_DeriveKey)(hSession, &sm3KDFBASEMech, hBaseKey, sm3KDFAttrib1, sizeof(sm3KDFAttrib1)/sizeof(CK_ATTRIBUTE), &hDerivedKey1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey2,rv);

	//printf("hDerivedKey1=%08x\n",hDerivedKey1);
	//zuc test
	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism, hDerivedKey1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit2,rv);

	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism, hDerivedKey1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit2,rv);
	for (i=0; i<loopTime; i++)
	{
		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv, sizeof(ZUCiv));
		memcpy(indata+sizeof(ZUCiv), ZUCplain, sizeof(ZUCplain));
		indatalen = sizeof(ZUCiv) + sizeof(ZUCplain);

		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate2,rv);

		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv, sizeof(ZUCiv));
		memcpy(indata1+sizeof(ZUCiv), outdata, outdatalen);
		indatalen1 = sizeof(ZUCiv) + outdatalen;

		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate2,rv);

		if ((outdatalen1 != sizeof(ZUCplain)) || (memcmp(outdata1, ZUCplain, outdatalen1)))
		{
			printf("Error: test_SM3KDF2.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: test_SM3KDF2.<br>");
			bRtn=false;
			goto END;
		}
		RandomGenerate(ZUCplain,128);
		RandomGenerate(ZUCiv,5);
	}
	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal2,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal2,rv);

	//calculate sm3 kdf without basekey
	rv = (*pC_DeriveKey)(hSession, &sm3KDFNOBASEMech, NULL_PTR, sm3KDFAttrib2, sizeof(sm3KDFAttrib2)/sizeof(CK_ATTRIBUTE), &hDerivedKey2);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey3,rv);

	//zuc test
	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism, hDerivedKey2);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit3,rv);
	//对称解密初始化
	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism, hDerivedKey2);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit3,rv);
	for (i=0; i<loopTime; i++)
	{
		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv, sizeof(ZUCiv));
		memcpy(indata+sizeof(ZUCiv), ZUCplain, sizeof(ZUCplain));
		indatalen = sizeof(ZUCiv) + sizeof(ZUCplain);

		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate3,rv);

		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv, sizeof(ZUCiv));
		memcpy(indata1+sizeof(ZUCiv), outdata, outdatalen);
		indatalen1 = sizeof(ZUCiv) + outdatalen;

		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate3,rv);

		if ((outdatalen1 != sizeof(ZUCplain)) || (memcmp(outdata1, ZUCplain, outdatalen1)))
		{
			printf("Error: test_SM3KDF3.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: test_SM3KDF3.<br>");
			bRtn=false;
			goto END;
		}
		RandomGenerate(ZUCplain,128);
		RandomGenerate(ZUCiv,5);
	}
	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal3,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal3,rv);
	bRtn = true;
END:
	if (hBaseKey)
	{
		rv = pC_DestroyObject(hSession, hBaseKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);
	}
	if (hDerivedKey1)
	{
		rv = pC_DestroyObject(hSession, hDerivedKey1);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);
	}
	if (hDerivedKey2)
	{
		rv = pC_DestroyObject(hSession, hDerivedKey2);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject3,rv);
	}
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM3KDF);
	
	return bRtn;
}


/*测试SM3密钥衍生性能*/
bool xtest_SM3KDF_Speed()
{
	bool	bRtn = false;
	CK_RV	rv = 0;
	//for set sm3 kdf basekey
	unsigned char	ucSM3KDFBaseKeyBuffer[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	CK_EXTEND_IN	ExtIn_SetSM3KDFBaseKey = {CK_EXTEND_SETSM3KDFBASEKEY, ucSM3KDFBaseKeyBuffer, sizeof(ucSM3KDFBaseKeyBuffer)};
	CK_EXTEND_OUT	ExtOut_SetSM3KDFBaseKey = {CK_EXTEND_SETSM3KDFBASEKEY, NULL, 0};
	//for calc sm3 kdf
	unsigned char	ucSM3KDFKeyBuffer1[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	CK_MECHANISM	sm3KDFPRESETMech = {CKM_DERIVE_SM3_KDF_WITH_PRESET, ucSM3KDFKeyBuffer1, sizeof(ucSM3KDFKeyBuffer1)};
	unsigned char	ucSM3KDFKeyBuffer2[17] = {0x11, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	CK_MECHANISM	sm3KDFBASEMech = {CKM_DERIVE_SM3_KDF, ucSM3KDFKeyBuffer2, sizeof(ucSM3KDFKeyBuffer2)};
	unsigned char	ucSM3KDFKeyBuffer3[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	CK_MECHANISM	sm3KDFNOBASEMech = {CKM_DERIVE_SM3_KDF, ucSM3KDFKeyBuffer3, sizeof(ucSM3KDFKeyBuffer3)};
	CK_BYTE	nBaseKeyID = CK_SESSKEY_ID0;
	CK_BYTE	nSessKeyID1 = CK_SESSKEY_ID0 + 14;
	CK_BYTE	nSessKeyID2 = CK_SESSKEY_ID0 + 15;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_ATTRIBUTE sm3BaseKeyAttrib[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nBaseKeyID, sizeof(CK_BYTE)}
	};
	CK_ATTRIBUTE sm3KDFAttrib1[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID1, sizeof(CK_BYTE)}
	};
	CK_ATTRIBUTE sm3KDFAttrib2[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID2, sizeof(CK_BYTE)}
	};
	CK_OBJECT_HANDLE	hBaseKey = NULL_PTR, hDerivedKey1 = NULL_PTR, hDerivedKey2 = NULL_PTR;
	//for zuc test
	unsigned char	ZUCplain[136] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, \
		0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03};
	unsigned char	ZUCiv[] = {0x00, 0x05, 0x68, 0x23, 0x38};
	CK_MECHANISM ZUCmechanism = {CKM_ZUC_CALC, ZUCiv, sizeof(ZUCiv)};
	unsigned char	ZUCCipher1[136] = {0x1b, 0xc8, 0xbe, 0x08, 0x9a, 0xe6, 0x93, 0x3e, 0x7a, 0xd1, 0x0a, 0x4b, 0xb2, 0x31, 0xeb, 0xf9, 0x12, 0x09, 0x76, 0xbe, 0x7c, 0x54, 0x82, 0x3d, 0x08, 0xa4, 0x96, 0xed, 0xbd, 0xb0, 0x80, 0xf3, 0x9c, 0x0e, 0xf7, 0xbf, 0x38, 0x4b, 0x4f, 0x83, 0x85, 0x75, 0xdc, 0x2c, 0x22, 0xef, 0xb4, 0x54, 0xa9, 0xee, 0x35, 0x8e, 0x11, 0xe6, 0x43, 0xd8, 0x79, 0x53, 0x3b, 0x3b, 0x4e, 0xb8, 0xd5, 0xed, 0x4c, 0x47, 0xf1, 0x0d, 0x45, 0x31, 0x24, 0xc3, 0xee, 0xbe, 0xe8, 0x87, 0xf6, 0x42, 0x88, 0x65, 0xcc, 0x79, 0x1a, 0xad, 0x80, 0x24, 0xfe, 0xc3, 0xc4, 0x8d, 0xd2, 0x49, 0xa6, 0x5a, 0xb3, 0x98, 0x2e, 0xfc, 0xab, 0xab, 0xdf, 0x7d, 0x45, 0xf3, 0x4a, 0x02, 0x08, 0x0c, 0xb5, 0xd6, 0x58, 0x4c, 0xd7, 0xce, 0x9f, 0x43, 0x69, 0x89, 0x4c, 0xda, 0x11, 0xf2, 0xe8, 0x40, 0x32, 0x15, 0x3e, 0x20, 0x5f, 0xf7, 0x5e, 0x4d, 0x3e, 0x69, 0x8a, 0x10};
	unsigned char	ZUCCipher2[136] = {0xB8, 0xE9, 0xA0, 0xEE, 0x6C, 0xB2, 0xE1, 0x54, 0xAB, 0x37, 0xBC, 0x80, 0x91, 0xF1, 0x6C, 0x46, 0xDC, 0x10, 0x5B, 0x81, 0xE1, 0x1E, 0x18, 0x1A, 0x4C, 0xB9, 0x5A, 0x66, 0x65, 0x33, 0x90, 0x1E, 0xFA, 0x95, 0x7C, 0x98, 0x5E, 0xF9, 0xEE, 0x23, 0x75, 0x6A, 0x38, 0x61, 0x17, 0x41, 0x71, 0x63, 0x22, 0x9B, 0x83, 0x8B, 0xB1, 0xAE, 0x99, 0xB2, 0x2C, 0x81, 0xC7, 0xB2, 0x5F, 0x2D, 0x0A, 0xD4, 0x18, 0x45, 0x96, 0xA5, 0x62, 0xFB, 0x17, 0x7D, 0x58, 0xF0, 0x50, 0x7B, 0x5A, 0xB2, 0x1E, 0x2A, 0xDC, 0xED, 0xD2, 0xFA, 0x6E, 0x37, 0x9A, 0xF5, 0x1E, 0xE5, 0xA1, 0x13, 0xB1, 0xB0, 0xE4, 0x07, 0x2A, 0x52, 0x5A, 0x8C, 0x0F, 0xB4, 0x48, 0x81, 0xCA, 0x79, 0xE8, 0x37, 0xC2, 0x9B, 0x64, 0xA6, 0x75, 0x59, 0x36, 0xD1, 0xB6, 0xAD, 0xB4, 0x8A, 0x8C, 0xE7, 0x30, 0xC3, 0x0C, 0x88, 0xE6, 0x4F, 0xBA, 0x7C, 0x5B, 0xA4, 0x36, 0xE9, 0xA2, 0x02};
	unsigned char	ZUCCipher3[136] = {0xA0, 0x3A, 0xCE, 0xA4, 0xA6, 0x27, 0x17, 0x4F, 0x87, 0x18, 0x71, 0x33, 0xA0, 0x45, 0x4F, 0x61, 0xEA, 0x1B, 0x8B, 0x0A, 0x1B, 0xE8, 0xA7, 0x35, 0xF5, 0x5F, 0xAE, 0xAD, 0xE2, 0x3D, 0xDB, 0x9A, 0xD3, 0x9F, 0x66, 0x64, 0x22, 0x22, 0x93, 0x8B, 0xD3, 0xE2, 0x67, 0x50, 0x2F, 0xF3, 0xBC, 0x50, 0x56, 0x7A, 0xA6, 0x49, 0xE3, 0xAE, 0x13, 0x78, 0xF1, 0x0B, 0xC9, 0x12, 0x7A, 0x68, 0xB5, 0x23, 0xF8, 0x2C, 0xC3, 0xD3, 0x5F, 0x1F, 0x9D, 0xF9, 0xE4, 0xFA, 0xA3, 0xCA, 0xC2, 0x05, 0x8E, 0x5E, 0x19, 0x5E, 0x9D, 0x4F, 0xCD, 0x97, 0x54, 0xBE, 0xEA, 0x83, 0x4D, 0x58, 0xC8, 0xDA, 0xAD, 0x72, 0x26, 0xF6, 0xC4, 0x87, 0xFD, 0x60, 0x74, 0xF2, 0x0E, 0xE9, 0x41, 0xFF, 0xAF, 0x8B, 0x71, 0x04, 0x18, 0x55, 0xD4, 0x59, 0xAC, 0xC4, 0x12, 0xD2, 0xC5, 0x0F, 0x85, 0x8E, 0x60, 0xAE, 0xBA, 0x62, 0x93, 0x19, 0x41, 0x66, 0x0C, 0x8B, 0x62, 0x51};
	unsigned int i = 0, j=0,loopTime = 10;
	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);
	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	//UtilsTime tt1, tt2;
	UtilscTime ttc1={0, 0},ttc2={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM3KDF_Speed);

	srand( (unsigned)time( NULL ) );//随机数初始化
	Utilsgettime(&ttc1);
	//set sm3 kdf basekey
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetSM3KDFBaseKey, &ExtOut_SetSM3KDFBaseKey, NULL);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CryptoExtend1,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"Set SM3KDF BaseKey",1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb]+ nResultLength,"Set SM3KDF BaseKey",1);

	Utilsgettime(&ttc1);
	//calculate sm3 kdf with preset
	rv = (*pC_DeriveKey)(hSession, &sm3KDFPRESETMech, NULL_PTR, sm3BaseKeyAttrib, sizeof(sm3BaseKeyAttrib)/sizeof(CK_ATTRIBUTE), &hBaseKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey1,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"Calculate SM3KDF with preset",1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb]+ nResultLength,"Calculate SM3KDF with preset",1);

	//zuc test
	//对称加密初始化
	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism, hBaseKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit1,rv);
	//对称解密初始化
	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism, hBaseKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit1,rv);

	for (i=0; i<loopTime; i++)
	{
		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv, sizeof(ZUCiv));
		memcpy(indata+sizeof(ZUCiv), ZUCplain, sizeof(ZUCplain));
		indatalen = sizeof(ZUCiv) + sizeof(ZUCplain);

		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate1,rv);
		if (i==0)
		{
			if ((outdatalen != sizeof(ZUCCipher1)) || (memcmp(outdata, ZUCCipher1, outdatalen)))
			{
				printf("Error: test_SM3KDF0.\n");
				nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: test_SM3KDF0.<br>");
				bRtn=false;
				goto END;
			}
		}		
		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv, sizeof(ZUCiv));
		memcpy(indata1+sizeof(ZUCiv), outdata, outdatalen);
		indatalen1 = sizeof(ZUCiv) + outdatalen;

		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate1,rv);
	
		if ((outdatalen1 != sizeof(ZUCplain)) || (memcmp(outdata1, ZUCplain, outdatalen1)))
		{
			printf("Error: test_SM3KDF1.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: test_SM3KDF1.<br>");
			bRtn=false;
			goto END;
		}
		RandomGenerate(ZUCplain,128);
		RandomGenerate(ZUCiv,5);
	}

	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal1,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal1,rv);

	Utilsgettime(&ttc1);
	//calculate sm3 kdf with basekey
	rv = (*pC_DeriveKey)(hSession, &sm3KDFBASEMech, hBaseKey, sm3KDFAttrib1, sizeof(sm3KDFAttrib1)/sizeof(CK_ATTRIBUTE), &hDerivedKey1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey2,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"Calculate SM3KDF with basekey",1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb]+ nResultLength,"Calculate SM3KDF with basekey",1);



	//zuc test
	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism, hDerivedKey1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit2,rv);

	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism, hDerivedKey1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit2,rv);

	for (i=0; i<loopTime; i++)
	{
		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv, sizeof(ZUCiv));
		memcpy(indata+sizeof(ZUCiv), ZUCplain, sizeof(ZUCplain));
		indatalen = sizeof(ZUCiv) + sizeof(ZUCplain);

		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate2,rv);

		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv, sizeof(ZUCiv));
		memcpy(indata1+sizeof(ZUCiv), outdata, outdatalen);
		indatalen1 = sizeof(ZUCiv) + outdatalen;

		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate2,rv);

		if ((outdatalen1 != sizeof(ZUCplain)) || (memcmp(outdata1, ZUCplain, outdatalen1)))
		{
			printf("Error: test_SM3KDF2.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: test_SM3KDF2.<br>");
			bRtn=false;
			goto END;
		}
		RandomGenerate(ZUCplain,128);
		RandomGenerate(ZUCiv,5);
	}
	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal2,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal2,rv);

	Utilsgettime(&ttc1);
	//calculate sm3 kdf without basekey
	rv = (*pC_DeriveKey)(hSession, &sm3KDFNOBASEMech, NULL_PTR, sm3KDFAttrib2, sizeof(sm3KDFAttrib2)/sizeof(CK_ATTRIBUTE), &hDerivedKey2);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey3,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"Calculate SM3KDF without basekey",1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb]+ nResultLength,"Calculate SM3KDF without basekey",1);

	//zuc test
	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism, hDerivedKey2);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit3,rv);
	//对称解密初始化
	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism, hDerivedKey2);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit3,rv);
	for (i=0; i<loopTime; i++)
	{
		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv, sizeof(ZUCiv));
		memcpy(indata+sizeof(ZUCiv), ZUCplain, sizeof(ZUCplain));
		indatalen = sizeof(ZUCiv) + sizeof(ZUCplain);

		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate3,rv);

		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv, sizeof(ZUCiv));
		memcpy(indata1+sizeof(ZUCiv), outdata, outdatalen);
		indatalen1 = sizeof(ZUCiv) + outdatalen;

		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate3,rv);

		if ((outdatalen1 != sizeof(ZUCplain)) || (memcmp(outdata1, ZUCplain, outdatalen1)))
		{
			printf("Error: test_SM3KDF3.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: test_SM3KDF3.<br>");
			bRtn=false;
			goto END;
		}
		RandomGenerate(ZUCplain,128);
		RandomGenerate(ZUCiv,5);
	}
	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal3,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal3,rv);
	bRtn = true;
END:
	if (hBaseKey)
	{
		rv = pC_DestroyObject(hSession, hBaseKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);
	}
	if (hDerivedKey1)
	{
		rv = pC_DestroyObject(hSession, hDerivedKey1);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);
	}
	if (hDerivedKey2)
	{
		rv = pC_DestroyObject(hSession, hDerivedKey2);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject3,rv);
	}
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM3KDF_Speed);
	return bRtn;
}


bool xtest_SM4ECB_Encrypt()
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	//加密参数
	unsigned char	SM4keyVal_Enc[]={0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
	unsigned char	SM4iv_Enc[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_BYTE SM4plain_Enc[32]={0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	CK_BYTE SM4cipher_Enc[32]={0x68, 0x6d, 0xb7, 0x66, 0xc9, 0x20, 0x48, 0x22, 0xbb, 0xfa, 0x6b, 0x84, 0x68, 0xc2, 0x86, 0x91, 0x68, 0x6d, 0xb7, 0x66, 0xc9, 0x20, 0x48, 0x22, 0xbb, 0xfa, 0x6b, 0x84, 0x68, 0xc2, 0x86, 0x91};
	CK_ATTRIBUTE SM4keyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},//FLASH ,FFLASE RAM
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal_Enc,sizeof(SM4keyVal_Enc)}
	};
	CK_MECHANISM SM4mechanism_Enc = {CKM_SM4_ECB, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	CK_BYTE indata[32] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[32] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4ECB_Encrypt);
	//对称加密初始化
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Enc, sizeof(SM4keyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	indatalen = sizeof(indata);
	memset(indata, 0, indatalen);
	outdatalen = sizeof(outdata);
	memset(outdata, 0, outdatalen);
	//加密的明文
	memcpy(indata, SM4plain_Enc, sizeof(SM4plain_Enc));
	indatalen = sizeof(SM4plain_Enc);

	//UtilsPrintData(VNAME(indata),indatalen,0);
	//加密运算
	rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
	//UtilsPrintData(VNAME(outdata),outdatalen,0);

	if ((outdatalen != sizeof(SM4cipher_Enc)) || (memcmp(outdata, SM4cipher_Enc, sizeof(SM4cipher_Enc))))
	{
		printf("SM4 ECB Encrypt Calc Error: test_SM4Encrypt_ECB.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"SM4 ECB Encrypt Calc Error: test_SM4Encrypt_ECB.<br>"); 
		bRtn = false;
		goto END;
	}
	rv = (*pC_EncryptFinal)(hSession, outdata, &outdatalen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	bRtn = true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4ECB_Encrypt);

	
	return bRtn;
}


bool xtest_SM4ECB_Decrypt()
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	//加密参数
	unsigned char	SM4keyVal_Enc[]={0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
	unsigned char	SM4iv_Enc[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_BYTE SM4plain_Enc[32]={0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	CK_BYTE SM4cipher_Enc[32]={0x68, 0x6d, 0xb7, 0x66, 0xc9, 0x20, 0x48, 0x22, 0xbb, 0xfa, 0x6b, 0x84, 0x68, 0xc2, 0x86, 0x91, 0x68, 0x6d, 0xb7, 0x66, 0xc9, 0x20, 0x48, 0x22, 0xbb, 0xfa, 0x6b, 0x84, 0x68, 0xc2, 0x86, 0x91};
	//解密参数
	CK_ATTRIBUTE SM4keyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal_Enc,sizeof(SM4keyVal_Enc)}
	};
	CK_MECHANISM SM4mechanism_Dec = {CKM_SM4_ECB, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;
	CK_BYTE indata1[32] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[32] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4ECB_Decrypt);	
	//对称解密初始化
	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Dec, sizeof(SM4keyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	rv = (*pC_DecryptInit)(hSession, &SM4mechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	indatalen1 = sizeof(indata1);
	memset(indata1, 0, indatalen1);
	outdatalen1=sizeof(outdata1);
	memset(outdata1, 0, outdatalen1);	

	//解密的密文
	memcpy(indata1, SM4cipher_Enc, sizeof(SM4cipher_Enc));
	indatalen1 = sizeof(SM4cipher_Enc);
	//解密运算
	rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);

	if ((outdatalen1 != sizeof(SM4plain_Enc)) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
	{
		printf("SM4 ECB Decrypt Calc Error: test_SM4Decrypt_ECB.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"SM4 ECB Decrypt Calc Error: test_SM4Decrypt_ECB.<br>"); 
		bRtn = false;
		goto END;
	}
	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);
	bRtn = true;
END: 
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4ECB_Decrypt);

	
	return bRtn;
}


bool xtest_SM4ECB_Speed(int looptime,int datalen)
{
	bool bRtn = false;
	unsigned int i = 0;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_BYTE nSessKeyID_Enc = CK_SESSKEY_PRESET_ID0;
	CK_BYTE nSessKeyID_Dec = CK_SESSKEY_PRESET_ID1;
	//加密参数
	unsigned char	SM4keyVal_Enc[]={0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
	unsigned char	SM4iv_Enc[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	unsigned char	SM4keyVal_Dec[]={0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
	unsigned char	SM4iv_Dec[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_BYTE SM4plain_Enc[1024]={0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	CK_ATTRIBUTE SM4keyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal_Enc,sizeof(SM4keyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Enc = {CKM_SM4_ECB, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;
	//解密参数
	CK_ATTRIBUTE SM4keyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal_Dec,sizeof(SM4keyVal_Dec)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Dec = {CKM_SM4_ECB, SM4iv_Dec, sizeof(SM4iv_Dec)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;

	CK_BYTE randomData[1024] = {0};

	CK_BYTE indata[1024] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[1024] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[1024] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[1024] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4ECB_Performance);

	//对称加密初始化
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Enc, sizeof(SM4keyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject1,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Dec, sizeof(SM4keyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject2,rv);

	rv = (*pC_DecryptInit)(hSession, &SM4mechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);
	for (i=0; i<looptime; i++)
	{
		rv = pC_GenerateRandom(hSession, randomData, datalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateRandom,rv);

		memcpy(SM4plain_Enc,randomData, datalen);

		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);

		//加密的明文
		memcpy(indata, SM4plain_Enc,datalen);
		indatalen = datalen;

		//UtilsPrintData(VNAME(SM4keyVal_Enc),sizeof(SM4keyVal_Enc),0);
		//UtilsPrintData(VNAME(SM4iv_Enc),sizeof(SM4iv_Enc),0);
		//UtilsPrintData(VNAME(indata),indatalen,0);
		Utilsgettime(&ttc1);
		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);

		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		//UtilsPrintData(VNAME(SM4keyVal_Enc),sizeof(SM4keyVal_Enc),0);
		//UtilsPrintData(VNAME(SM4iv_Enc),sizeof(SM4iv_Enc),0);
		//UtilsPrintData(VNAME(indata1),indatalen1,0);
		Utilsgettime(&ttc1);
		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);

		//UtilsPrintData(VNAME(outdata1),outdatalen1,0);
		if ((outdatalen1 != datalen) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4ECB_Speed.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Calc Error: test_SM4ECB_Speed.<br>"); 
			bRtn = false;
			goto END;
		}
	}
	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	Utilsprint(&ttc3,"ECB Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"ECB Encrypt", looptime);
	Utilsprint(&ttc4,"ECB Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"ECB Decrypt", looptime);

	bRtn = true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4ECB_Performance);
	return bRtn;
}


bool xtest_SM4OFB_Speed(int looptime,int datalen)
{
	bool bRtn = false;
	unsigned int i = 0;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_BYTE nSessKeyID_Enc = CK_SESSKEY_PRESET_ID1;
	CK_BYTE nSessKeyID_Dec = CK_SESSKEY_PRESET_ID0;
	//加密参数
	unsigned char	SM4keyVal_Enc[]={0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
	unsigned char	SM4iv_Enc[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	unsigned char	SM4plain_Enc[192]={0};
	CK_ATTRIBUTE SM4keyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal_Enc,sizeof(SM4keyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Enc = {CKM_SM4_OFB, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	//解密参数
	CK_ATTRIBUTE SM4keyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal_Enc,sizeof(SM4keyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Dec = {CKM_SM4_OFB, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;
	CK_BYTE randomData[192] = {0};

	CK_BYTE indata[192] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[192] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[192] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[192] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);	
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4OFB_Performance);
	//对称加密初始化
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Enc, sizeof(SM4keyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Dec, sizeof(SM4keyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	rv = (*pC_DecryptInit)(hSession, &SM4mechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);

	for (i=0; i<looptime; i++)
	{
		rv = pC_GenerateRandom(hSession, randomData, datalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateRandom,rv);

		memcpy(SM4plain_Enc, randomData, sizeof(randomData));

		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);
		//加密的明文
		memcpy(indata, SM4plain_Enc, sizeof(SM4plain_Enc));
		indatalen = sizeof(SM4plain_Enc);

		//printf("datalen1=%d,outdatalen1=%d\n",datalen,outdatalen);
		Utilsgettime(&ttc1);
		//加密运算
		//UtilsPrintData(VNAME(indata),datalen,0);
		rv = (*pC_EncryptUpdate)(hSession, indata, datalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
		//printf("datalen2=%d,outdatalen2=%d\n",datalen,outdatalen);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);

		//UtilsPrintData(VNAME(outdata),outdatalen,0);

		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		Utilsgettime(&ttc1);
		//解密运算
		//UtilsPrintData(VNAME(indata1),indatalen1,0);
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);

		//UtilsPrintData(VNAME(outdata1),outdatalen1,0);
		if ((outdatalen1 != datalen) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4OFB.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Calc Error: test_SM4OFB.<br>");
			bRtn = false;
			goto END;
		}
	}
	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	Utilsprint(&ttc3,"OFB Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"OFB Encrypt", looptime);
	Utilsprint(&ttc4,"OFB Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"OFB Decrypt", looptime);
	bRtn = true;	
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4OFB_Performance);
	return bRtn;
}


bool xtest_SM4CBC_Speed(int looptime,int datalen)
{
	bool bRtn = false;
	unsigned int i = 0;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_BYTE nSessKeyID_Enc = CK_SESSKEY_PRESET_ID1;
	CK_BYTE nSessKeyID_Dec = CK_SESSKEY_PRESET_ID1;

	//加密参数
	unsigned char	SM4keyVal_Enc[]={0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
	unsigned char	SM4iv_Enc[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_BYTE SM4plain_Enc[1024]={0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	CK_ATTRIBUTE SM4keyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal_Enc,sizeof(SM4keyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Enc = {CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;
	//解密参数
	CK_ATTRIBUTE SM4keyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal_Enc,sizeof(SM4keyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Dec = {CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;

	CK_BYTE randomData[1024] = {0};

	CK_BYTE indata[1024] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[1024] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[1024] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[1024] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4CBC_Performance);

	//对称加密初始化
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Enc, sizeof(SM4keyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject1,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Dec, sizeof(SM4keyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject2,rv);

	rv = (*pC_DecryptInit)(hSession, &SM4mechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);


	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);
	for (i=0; i<looptime; i++)
	{
		rv = pC_GenerateRandom(hSession, randomData, datalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateRandom,rv);

		//memcpy(SM4plain_Enc,randomData, datalen);

		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);
		//加密的明文
		memcpy(indata, SM4plain_Enc, datalen);
		indatalen = datalen;

		//UtilsPrintData(VNAME(SM4keyVal_Enc),sizeof(SM4keyVal_Enc),0);
		//UtilsPrintData(VNAME(SM4iv_Enc),sizeof(SM4iv_Enc),0);
		//UtilsPrintData(VNAME(indata),indatalen,0);

		Utilsgettime(&ttc1);
		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);

		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		//UtilsPrintData(VNAME(SM4keyVal_Enc),sizeof(SM4keyVal_Enc),0);
		//UtilsPrintData(VNAME(SM4iv_Enc),sizeof(SM4iv_Enc),0);
		//UtilsPrintData(VNAME(indata1),indatalen1,0);

		Utilsgettime(&ttc1);
		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);

		//UtilsPrintData(VNAME(outdata1),outdatalen1,0);
		if ((outdatalen1 != datalen) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4CBC_Speed.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Calc Error: test_SM4CBC_Speed.<br>"); 
			bRtn = false;
			goto END;
		}
	}
	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);

	Utilsprint(&ttc3,"CBC Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"CBC Encrypt", looptime);
	Utilsprint(&ttc4,"CBC Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"CBC Decrypt", looptime);

	bRtn = true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4CBC_Performance);
	return bRtn;
}


bool xtest_SetDestroyKeyIncorrect()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;
	int i =0;
	//for set destroy key
	unsigned char pbDestroyKey[64] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, \
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};

	unsigned char pbDestroyKeyincorrect[65] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, \
			0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x01,};

	char *pbSOPin = (char*)adminpin;
	unsigned char pbBuffer[128] = {0};
	CK_EXTEND_IN ExtIn_SetDestroyKey = {CK_EXTEND_SETDESTROYKEY, (void*)pbBuffer, 0};
	CK_EXTEND_OUT ExtOut_SetDestroyKey = {CK_EXTEND_SETDESTROYKEY, NULL, 0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestDestroyKeyIncorrect);


	//uese wrong Random to set destroy key

	pbSOPin = (char*)adminpin;
	memcpy(ExtIn_SetDestroyKey.pParameter, pbDestroyKeyincorrect, sizeof(pbDestroyKeyincorrect));
	memcpy(((unsigned char*)(ExtIn_SetDestroyKey.pParameter)) + sizeof(pbDestroyKeyincorrect), pbSOPin, strlen(pbSOPin));
	ExtIn_SetDestroyKey.ulParameterLen = sizeof(pbDestroyKeyincorrect) + strlen(pbSOPin);

	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetDestroyKey, &ExtOut_SetDestroyKey, NULL);
	
	if (rv == CKR_OK)
	{
		printf("Set Destroy Key OK: should fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Set Destroy Key OK: should fail.<br>"); 
		bRtn = FALSE;
		goto END;
	}
	else
	{
		printf("Set Destroy Key Fail:should be failed. rv = 0x%08x.\n",rv);
		printf("rv=0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Set Destroy Key Fail：should be failed (wrong Randomlen).<br>"); 
		//bRtn = FALSE;
		//goto END;
	}

	//uese wrong Sopin to set destroy key
	pbSOPin = "1234567";
	memcpy(ExtIn_SetDestroyKey.pParameter, pbDestroyKey, sizeof(pbDestroyKey));
	memcpy(((unsigned char*)(ExtIn_SetDestroyKey.pParameter)) + sizeof(pbDestroyKey), pbSOPin, strlen(pbSOPin));
	ExtIn_SetDestroyKey.ulParameterLen = sizeof(pbDestroyKey) + strlen(pbSOPin);
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetDestroyKey, &ExtOut_SetDestroyKey, NULL);
	
	if (rv == CKR_OK)
	{
		printf("Set Destroy Key OK: should fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Set Destroy Key OK: should fail.<br>"); 
		bRtn = FALSE;
		goto END;
	}
	else
	{
		printf("Set Destroy Key Fail:should be failed. rv = 0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Set Destroy Key Fail：should be failed (wrong SoPin).<br>"); 
		//bRtn = FALSE;
		//goto END;
	}


	bRtn = TRUE;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestDestroyKeyIncorrect);

	
	return bRtn;
}


bool xtest_SetDestroyKeyCorrect()
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

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestDestroyKeyCorrect);
	//set destroy key 1
	memcpy(ExtIn_SetDestroyKey.pParameter, pbDestroyKey, 64);
	memcpy(((unsigned char*)(ExtIn_SetDestroyKey.pParameter)) + 64, pbSOPin, strlen(pbSOPin));
	ExtIn_SetDestroyKey.ulParameterLen = 64 + strlen(pbSOPin);
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetDestroyKey, &ExtOut_SetDestroyKey, NULL);
	if (rv == CKR_OK)
	{
		printf("Set Destroy Key OK.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Set Destroy Key OK.<br>"); 		
	}
	else
	{
		printf("Set Destroy Key Fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Set Destroy Key Fail.<br>"); 
		bRtn = FALSE;
		goto END;
	}

	//set destroy key 2
	memcpy(ExtIn_SetDestroyKey.pParameter, pbDestroyKey, 64);
	memcpy(((unsigned char*)(ExtIn_SetDestroyKey.pParameter)) + 64, pbSOPin, strlen(pbSOPin));
	ExtIn_SetDestroyKey.ulParameterLen = 64 + strlen(pbSOPin);
	((unsigned char*)ExtIn_SetDestroyKey.pParameter)[0] += 1;
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetDestroyKey, &ExtOut_SetDestroyKey, NULL);
	if (rv == CKR_OK)
	{
		printf("Set Destroy Key OK.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Set Destroy Key OK.<br>"); 		
	}
	else
	{
		printf("Set Destroy Key Fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Set Destroy Key Fail.<br>"); 
		bRtn = FALSE;
		goto END;
	}

	
	//do destroy 1
	memcpy(ExtIn_DoDestroy.pParameter, pbDestroyKey, 32);
	ExtIn_DoDestroy.ulParameterLen = 32;
	memset(ExtOut_DoDestroy.pParameter, 0, 32);
	ExtOut_DoDestroy.ulParameterLen = 32;
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_DoDestroy, &ExtOut_DoDestroy, NULL);
	if (rv == CKR_OK)
	{
		printf("Do Destroy 1 Should Be Fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Do Destroy 1 Should Be Fail.<br>");
		bRtn = FALSE;
		goto END;
	}
	else
	{
		printf("Do Destroy 1 Failed OK.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength , "Do Destroy 1 Failed OK.<br>");
	}

	

	
	bRtn = TRUE;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestDestroyKeyCorrect);

	
	return bRtn;
}


bool xtest_DestroyKey()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;
	//for set destroy key
	unsigned char pbDestroyKey[64] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, \
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};

	//for do destroy
	unsigned char pbUpDestroyKey[32] = {0};
	unsigned char pbDownDestroyKey[32] = {0};
	CK_EXTEND_IN ExtIn_DoDestroy = {CK_EXTEND_DODESTROY, (void*)pbUpDestroyKey, 32};
	CK_EXTEND_OUT ExtOut_DoDestroy = {CK_EXTEND_DODESTROY, (void*)pbDownDestroyKey, 32};


	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestDestroyKey);


	//do destroy 
	memcpy(ExtIn_DoDestroy.pParameter, pbDestroyKey, 32);
	ExtIn_DoDestroy.ulParameterLen = 32;
	memset(ExtOut_DoDestroy.pParameter, 0, 32);
	ExtOut_DoDestroy.ulParameterLen = 32;
	((unsigned char*)ExtIn_DoDestroy.pParameter)[0] += 1;
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_DoDestroy, &ExtOut_DoDestroy, NULL);
	if (rv == CKR_OK)
	{
		printf("Do Destroy  OK.\n");
		nResultLength += sprintf(Testresult[nItemNumb]+nResultLength , "Do Destroy  OK.<br>");
	}
	else
	{
		printf("Do Destroy  Fail.\n");
		nResultLength += sprintf(Testresult[nItemNumb]+nResultLength , "Do Destroy  Fail.<br>");
		bRtn = FALSE;
		goto END;
	}
	if(!memcmp(ExtOut_DoDestroy.pParameter,pbDestroyKey + 32,ExtOut_DoDestroy.ulParameterLen))
	{
		printf("The DownLink Destory Key is Wrong!\n");

	}
	bRtn = true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestDestroyKey);

	
	return bRtn;
}

/*密钥生成测试*/
bool xtest_GenerateKey_Performace( int looptime)
{
	CK_RV rv=0;	
	bool bRtn=false;
	CK_OBJECT_HANDLE hKey=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID0;
	unsigned int i=0;
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0};

	CK_MECHANISM SM4mechanismkeygen = {
		CKM_SM4_KEY_GEN , NULL_PTR, 0
	};
	CK_ATTRIBUTE SM4keytemplate[] = {
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS,	&keyClass,	sizeof(keyClass)},
		{CKA_KEY_TYPE,	&SM4keyType,sizeof(SM4keyType)},
		{CKA_SESSKEY_ID,	&nSessKeyID,	sizeof(nSessKeyID)}
		//{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)},
		//{CKA_EXTRACTABLE, &ttrue, sizeof(ttrue)}
	};	

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xtest_GenerateKey_Performace);
	for(i=0;i<looptime;i++)
	{
		Utilsgettime(&ttc1);
		rv=(*pC_GenerateKey)(hSession,&SM4mechanismkeygen,SM4keytemplate,sizeof(SM4keytemplate)/sizeof(CK_ATTRIBUTE),&hKey);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CryptoExtend,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);
		if (hKey)
		{
			rv = pC_DestroyObject(hSession, hKey);
			RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);
		}
	}
	Utilsprint(&ttc3,"GenerateSM4Key", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"GenerateSM4Key", looptime);
	bRtn=true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestGenerateSM4Key_Performance);	
	return bRtn;
}


char* my_itoa(unsigned int value, char* result) {
	char* ptr = result, *ptr1 = result, tmp_char;
	unsigned int tmp_value;

	if (value < 10)
	{
		result[0] = 0x30;
		result[1] = 0x30 + value;
		return result;
	}

	do {
		tmp_value = value;
		value /= 10;
		*ptr++ = 0x30 + (tmp_value - value * 10);

	} while ( value );

	*ptr--;

	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}

	return result;
}


CK_RV Get_Date_For_Key_Gen(CK_DATE* start_date, CK_DATE* end_date)
{
	struct tm *localt;
	time_t t;
	t=time(NULL);
	localt=localtime(&t);

	my_itoa(localt->tm_year+1900,start_date->year);
	my_itoa(localt->tm_mon+1,start_date->month);
	my_itoa(localt->tm_mday,start_date->day);

	my_itoa(localt->tm_year+1900+10,end_date->year);
	my_itoa(localt->tm_mon+1,end_date->month);
	my_itoa(localt->tm_mday,end_date->day);

	return CKR_OK;
}


bool xtest_GenerateKeyPair_Performance(int looptime)
{
	bool bRtn = false;
	CK_RV rv = 0;
	unsigned int i = 0;
	CK_BYTE idid[] = {1,2,4,3};

	CK_BBOOL ttrue = CK_TRUE,ffalse = CK_FALSE;
	CK_MECHANISM      ECCMechanism = {CKM_ECC_KEY_PAIR_GEN, NULL_PTR, 0};
	CK_UINT     ECCModulesBits = 256;
	CK_KEY_TYPE  ECCKeyType = CKK_ECC;
	CK_ATTRIBUTE publicKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &ECCModulesBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(CK_BBOOL)},
		{CKA_ENCRYPT, &ffalse, sizeof(CK_BBOOL)},
		{CKA_VERIFY, &ffalse, sizeof(CK_BBOOL)},
		{CKA_ISEXCHANGEKEY, &ttrue, sizeof(CK_BBOOL)},
		{CKA_ID, idid, sizeof(idid)},
		//{CKA_START_DATE, &start_date,sizeof(start_date)},
		//{CKA_END_DATE, &end_date,sizeof(end_date)},
		{CKA_KEY_TYPE,&ECCKeyType,sizeof(CK_KEY_TYPE)}
	};
	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &ECCModulesBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(CK_BBOOL)},
		{CKA_PRIVATE, &ttrue, sizeof(CK_BBOOL)},
		{CKA_SENSITIVE, &ttrue, sizeof(CK_BBOOL)},
		{CKA_DECRYPT, &ffalse, sizeof(CK_BBOOL)},
		{CKA_SIGN, &ffalse, sizeof(CK_BBOOL)},
		{CKA_ISEXCHANGEKEY, &ttrue, sizeof(CK_BBOOL)},
		{CKA_ID, idid, sizeof(idid)},
		//{CKA_START_DATE, &start_date,sizeof(start_date)},
		//{CKA_END_DATE, &end_date,sizeof(end_date)},
		{CKA_KEY_TYPE,&ECCKeyType,sizeof(CK_KEY_TYPE)}
	};
	CK_OBJECT_HANDLE hPublicKey = NULL_PTR, hPrivateKey = NULL_PTR;	
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0};
	
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestGenerateSM2KeyPair_Performance);

	for (i=0; i<looptime; i++)
	{
		printf("%*c\r",79,0x20);
		printf("i=%d\r",i);
		fflush(stdout);
		Utilsgettime(&ttc1);
		rv = (*pC_GenerateKeyPair)(hSession, &ECCMechanism,
			publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
			privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
			&hPublicKey, &hPrivateKey);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPair,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);
		if (hPublicKey)
		{
			rv = pC_DestroyObject(hSession, hPublicKey);
			RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject0,rv);
		}
		if (hPrivateKey)
		{
			rv = pC_DestroyObject(hSession, hPrivateKey);
			RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);
		}
	}	
	Utilsprint(&ttc3,"GenerateKeyPair", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"GenerateKeyPair", looptime);
	bRtn = true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestGenerateSM2KeyPair_Performance);	
	return bRtn;
}

bool xtest_GenerateKeyPairAndOperateDate()
{
	CK_DATE start_date;
	CK_DATE end_date;
	bool bRtn = false;
	CK_RV rv = 0;
	unsigned int i = 0;
	CK_BYTE idid[] = {1,2,4,3};

	CK_BBOOL ttrue = CK_TRUE,ffalse = CK_FALSE;
	CK_MECHANISM      ECCMechanism = {CKM_ECC_KEY_PAIR_GEN, NULL_PTR, 0};
	CK_UINT     ECCModulesBits = 256;
	CK_KEY_TYPE  ECCKeyType = CKK_ECC;
	CK_ATTRIBUTE publicKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &ECCModulesBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(CK_BBOOL)},
		{CKA_ENCRYPT, &ffalse, sizeof(CK_BBOOL)},
		{CKA_VERIFY, &ffalse, sizeof(CK_BBOOL)},
		{CKA_WRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_ID, idid, sizeof(idid)},
		{CKA_START_DATE, &start_date,sizeof(start_date)},
		{CKA_END_DATE, &end_date,sizeof(end_date)},
		{CKA_KEY_TYPE,&ECCKeyType,sizeof(CK_KEY_TYPE)}
	};
	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &ECCModulesBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(CK_BBOOL)},
		{CKA_PRIVATE, &ttrue, sizeof(CK_BBOOL)},
		{CKA_SENSITIVE, &ttrue, sizeof(CK_BBOOL)},
		{CKA_DECRYPT, &ffalse, sizeof(CK_BBOOL)},
		{CKA_SIGN, &ffalse, sizeof(CK_BBOOL)},
		{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_ID, idid, sizeof(idid)},
		{CKA_START_DATE, &start_date,sizeof(start_date)},
		{CKA_END_DATE, &end_date,sizeof(end_date)},
		{CKA_KEY_TYPE,&ECCKeyType,sizeof(CK_KEY_TYPE)}
	};
	CK_OBJECT_HANDLE hPublicKey = NULL_PTR, hPrivateKey = NULL_PTR;	


	CK_DATE get_start_date;
	CK_DATE get_end_date;
	CK_ATTRIBUTE GetDateTemplate[] = {
		{CKA_START_DATE, &get_start_date,sizeof(get_start_date)},
		{CKA_END_DATE, &get_end_date,sizeof(get_end_date)},
	};

	CK_DATE set_date={"9999","04","28"};

	CK_ATTRIBUTE SetDateTemplate[] = {
		{CKA_START_DATE, &set_date,sizeof(set_date)}		
	};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestGenerateSM2KeyPair_Performance);


	rv = Get_Date_For_Key_Gen(&start_date, &end_date);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,Get_Date_For_Key_Gen,rv);

	rv = (*pC_GenerateKeyPair)(hSession, &ECCMechanism,
		publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
		privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
		&hPublicKey, &hPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPair,rv);



	rv = (*pC_GetAttributeValue)(hSession,hPrivateKey,GetDateTemplate,sizeof(GetDateTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValue1,rv);

	printf("pristart_date1:%.4s-%.2s-%.2s\n",start_date.year,start_date.month,start_date.day);
	printf("priend_date1:%.4s-%.2s-%.2s\n",end_date.year,end_date.month,end_date.day);

	rv = (*pC_SetAttributeValue)(hSession,hPrivateKey,SetDateTemplate,sizeof(SetDateTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_SetAttributeValue1,rv);

	rv = (*pC_GetAttributeValue)(hSession,hPrivateKey,GetDateTemplate,sizeof(GetDateTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValue1,rv);

	printf("pristart_date2:%.4s-%.2s-%.2s\n",start_date.year,start_date.month,start_date.day);
	printf("priend_date2:%.4s-%.2s-%.2s\n",end_date.year,end_date.month,end_date.day);


	rv = (*pC_GetAttributeValue)(hSession,hPublicKey,GetDateTemplate,sizeof(GetDateTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValue2,rv);

	printf("pubstart_date1:%.4s-%.2s-%.2s\n",start_date.year,start_date.month,start_date.day);
	printf("pubend_date1:%.4s-%.2s-%.2s\n",end_date.year,end_date.month,end_date.day);

	rv = (*pC_SetAttributeValue)(hSession,hPublicKey,SetDateTemplate,sizeof(SetDateTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_SetAttributeValue2,rv);

	rv = (*pC_GetAttributeValue)(hSession,hPublicKey,GetDateTemplate,sizeof(GetDateTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValue2,rv);
	printf("pubstart_date2:%.4s-%.2s-%.2s\n",start_date.year,start_date.month,start_date.day);
	printf("pubend_date2:%.4s-%.2s-%.2s\n",end_date.year,end_date.month,end_date.day);

	if (hPublicKey)
	{
		rv = pC_DestroyObject(hSession, hPublicKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject0,rv);
	}
	if (hPrivateKey)
	{
		rv = pC_DestroyObject(hSession, hPrivateKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);
	}

	bRtn = true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestGenerateSM2KeyPair_Performance);	

	
	return bRtn;
}


bool xtest_GenerateKeyPair_NotDestory()
{
	bool bRtn = false;
	CK_RV rv = 0;
	unsigned int i = 0;

	CK_BBOOL ttrue = CK_TRUE,ffalse = CK_FALSE;
	CK_MECHANISM      ECCMechanism = {CKM_ECC_KEY_PAIR_GEN, NULL_PTR, 0};
	CK_UINT     ECCModulesBits = 256;
	CK_KEY_TYPE  ECCKeyType = CKK_ECC;
	CK_ATTRIBUTE publicKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &ECCModulesBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(CK_BBOOL)},
		{CKA_ENCRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_VERIFY, &ttrue, sizeof(CK_BBOOL)},
		//{CKA_WRAP, &ffalse, sizeof(CK_BBOOL)},
		{CKA_KEY_TYPE,&ECCKeyType,sizeof(CK_KEY_TYPE)}
	};
	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &ECCModulesBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(CK_BBOOL)},
		{CKA_PRIVATE, &ttrue, sizeof(CK_BBOOL)},
		{CKA_SENSITIVE, &ttrue, sizeof(CK_BBOOL)},
		{CKA_DECRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_SIGN, &ttrue, sizeof(CK_BBOOL)},
		//{CKA_UNWRAP, &ffalse, sizeof(CK_BBOOL)},
		{CKA_KEY_TYPE,&ECCKeyType,sizeof(CK_KEY_TYPE)}
	};
	CK_OBJECT_HANDLE hPublicKey = NULL_PTR, hPrivateKey = NULL_PTR;	

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestGenerateSM2KeyPair_Performance);
	rv = (*pC_GenerateKeyPair)(hSession, &ECCMechanism,
			publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
			privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
			&hPublicKey, &hPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPair,rv);

	bRtn = true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestGenerateSM2KeyPair_Performance);	

	
	return bRtn;
}


#define DESTORYUSELESSKEY
bool xtest_FindKeyObjectAndDestory()
{
	CK_RV rv=0;
	bool bRtn=false;
	CK_BYTE IDIDid[]={};//0x00};//,0x01};//,0x02};//,0x03};
	CK_OBJECT_CLASS pubclass=CKO_PUBLIC_KEY,priclass=CKO_PRIVATE_KEY,secretclass = CKO_SECRET_KEY;	

	CK_ATTRIBUTE pubFindKeyTemplate[] = {
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		//{CKA_ID, IDIDid,sizeof(IDIDid)}
		//{CKA_LABEL, IDIDid,sizeof(IDIDid)}
		//{CKA_APPLICATION, IDIDid,sizeof(IDIDid)}
	};

	CK_ATTRIBUTE prvFindKeyTemplate[] = {
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		//{CKA_ID, IDIDid,sizeof(IDIDid)}
		//{CKA_LABEL, IDIDid,sizeof(IDIDid)}
		//{CKA_APPLICATION, IDIDid,sizeof(IDIDid)}
	};

	CK_ATTRIBUTE secFindKeyTemplate[] = {
		{CKA_CLASS, &secretclass, sizeof(CK_OBJECT_CLASS)}
	};

	CK_OBJECT_HANDLE_PTR hObject = NULL_PTR;
	CK_OBJECT_HANDLE hObjectDestory = NULL_PTR;
	CK_ULONG ulObjectCount = 0;
	int i;

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xtest_FindKeyObjectAndDestory);
	//查找私钥对象	
	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 16;

	//UtilsPrintData(VNAME(prvFindKeyTemplate),256,0);
	//printf("count=%d\n",sizeof(prvFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	rv = (*pC_FindObjectsInit)(hSession, prvFindKeyTemplate, sizeof(prvFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit1,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects1,rv);

	printf("Private Key count=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Private Key count=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal1,rv);

#ifdef DESTORYUSELESSKEY
	hObjectDestory = NULL_PTR;
	for(i=0;i<ulObjectCount;i++)
	{
		hObjectDestory=hObject[i];
		printf("Private i=%d,KeyObject=0x%08x.\n",i,hObjectDestory);
		rv = (*pC_DestroyObject)(hSession, hObjectDestory);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);
	}
#endif
	free(hObject);
	//查找公钥对象
	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16);
	ulObjectCount = 16;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate, sizeof(pubFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit2,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects2,rv);
	printf("Public Key count=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Public Key count=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal2,rv);


#ifdef DESTORYUSELESSKEY
	hObjectDestory = NULL_PTR;
	for(i=0;i<ulObjectCount;i++)
	{
		hObjectDestory=hObject[i];
		printf("Public i=%d,KeyObject=0x%08x.\n",i,hObjectDestory);
		rv = (*pC_DestroyObject)(hSession, hObjectDestory);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);
	}
#endif
	free(hObject);

	//查找加密密钥对象
	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16);
	ulObjectCount = 16;
	rv = (*pC_FindObjectsInit)(hSession, secFindKeyTemplate, sizeof(secFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit3,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects3,rv);
	printf("Secret Key count=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Secret Key count=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal3,rv);


#ifdef DESTORYUSELESSKEY
	hObjectDestory = NULL_PTR;
	for(i=0;i<ulObjectCount;i++)
	{
		hObjectDestory=hObject[i];
		printf("Secret i=%d,KeyObject=0x%08x.\n",i,hObjectDestory);
		rv = (*pC_DestroyObject)(hSession, hObjectDestory);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject3,rv);
	}
#endif
	free(hObject);

	bRtn=true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xtest_FindKeyObjectAndDestory);

	
	return bRtn;
}


//#define TEST_CKA_ID
//#define TEST_CKA_LABEL
#define TEST_CKA_APPLICAITON
bool xtest_FindPublicPrivateKey()
{
	CK_RV rv=0;
	bool bRtn=false;
	CK_BYTE IDIDid0[]={};//0x00};//,0x01};//,0x02};//,0x03};
	CK_BYTE IDIDid1[]={0x00};//,0x01};//,0x02};//,0x03};
	CK_BYTE IDIDid2[]={0x00,0x01,0x02,0x03};

	CK_OBJECT_CLASS pubclass=CKO_PUBLIC_KEY,priclass=CKO_PRIVATE_KEY,secretclass = CKO_SECRET_KEY;	

	CK_ATTRIBUTE pubFindKeyTemplate1[] = {
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
	};

	CK_ATTRIBUTE pubFindKeyTemplate2[] = {
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
#ifdef TEST_CKA_ID
		{CKA_ID, IDIDid0,sizeof(IDIDid0)}
#endif
#ifdef TEST_CKA_LABEL
		{CKA_LABEL, IDIDid0,sizeof(IDIDid0)}
#endif
#ifdef TEST_CKA_APPLICAITON
		{CKA_APPLICATION, IDIDid0,sizeof(IDIDid0)}
#endif
	};

	CK_ATTRIBUTE pubFindKeyTemplate3[] = {
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
#ifdef TEST_CKA_ID
		{CKA_ID, IDIDid1,sizeof(IDIDid1)}
#endif
#ifdef TEST_CKA_LABEL
		{CKA_LABEL, IDIDid1,sizeof(IDIDid1)}
#endif
#ifdef TEST_CKA_APPLICAITON
		{CKA_APPLICATION, IDIDid1,sizeof(IDIDid1)}
#endif
	};


	CK_ATTRIBUTE pubFindKeyTemplate4[] = {
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
#ifdef TEST_CKA_ID
		{CKA_ID, IDIDid2,sizeof(IDIDid2)}
#endif
#ifdef TEST_CKA_LABEL
		{CKA_LABEL, IDIDid2,sizeof(IDIDid2)}
#endif
#ifdef TEST_CKA_APPLICAITON
		{CKA_APPLICATION, IDIDid2,sizeof(IDIDid2)}
#endif
	};


	CK_ATTRIBUTE pubFindKeyTemplate5[] = {
	};


	CK_OBJECT_HANDLE_PTR hObject = NULL_PTR;
	CK_OBJECT_HANDLE hObjectDestory = NULL_PTR;
	CK_ULONG ulObjectCount = 0;
	int i;

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xtest_FindPublicPrivateKey);
	//查找私钥对象	
	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 16;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate1, sizeof(pubFindKeyTemplate1)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit1,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects1,rv);

	printf("Key count1=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Key count1=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal1,rv);

	free(hObject);

	//查找公钥对象
	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 16;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate2, sizeof(pubFindKeyTemplate2)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit2,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects2,rv);

	printf("Key count2=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Key count2=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal2,rv);
	
	free(hObject);

	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 16;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate3, sizeof(pubFindKeyTemplate3)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit3,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects3,rv);

	printf("Key count3=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Key count3=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal3,rv);

	free(hObject);

	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 16;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate4, sizeof(pubFindKeyTemplate4)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit4,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects4,rv);

	printf("Key count4=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Key count4=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal4,rv);

	free(hObject);

	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 16;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate5,sizeof(pubFindKeyTemplate5)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit5,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects5,rv);

	printf("Key count5=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Key count5=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal5,rv);

	free(hObject);

	bRtn=true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xtest_FindPublicPrivateKey);

	
	return bRtn;
}

bool xtest_object_data_create()
{
	bool	bRtn = false;

	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;

	CK_RV	rv = 0;
	CK_OBJECT_HANDLE hCertificate = NULL_PTR;

	printf("enter test_object_cert.\n");

	CK_BYTE datadata[1024]={0x30,0x82,0x05,0x6D,0x30,0x82,0x04,0x55,0xA0,0x03,0x02,0x01,0x02,0x02,0x0A,0x22,
		0x09,0x63,0xB1,0x00,0x00,0x00,0x00,0x64,0x45,0x30,0x0D,0x06,0x09,0x2A,0x86,0x48,
		0x86,0xF7,0x0D,0x01,0x01,0x05,0x05,0x00,0x30,0x3E,0x31,0x13,0x30,0x11,0x06,0x0A,
		0x09,0x92,0x26,0x89,0x93,0xF2,0x2C,0x64,0x01,0x19,0x16,0x03,0x63,0x6F,0x6D,0x31,
		0x18,0x30,0x16,0x06,0x0A,0x09,0x92,0x26,0x89,0x93,0xF2,0x2C,0x64,0x01,0x19,0x16,
		0x08,0x4D,0x69,0x63,0x72,0x6F,0x53,0x65,0x63,0x31,0x0D,0x30,0x0B,0x06,0x03,0x55,
		0x04,0x03,0x13,0x04,0x42,0x4A,0x49,0x53,0x30,0x1E,0x17,0x0D,0x31,0x33,0x30,0x36,
		0x30,0x34,0x30,0x35,0x34,0x38,0x32,0x33,0x5A,0x17,0x0D,0x31,0x34,0x30,0x36,0x30,
		0x34,0x30,0x35,0x34,0x38,0x32,0x33,0x5A,0x30,0x58,0x31,0x13,0x30,0x11,0x06,0x0A,
		0x09,0x92,0x26,0x89,0x93,0xF2,0x2C,0x64,0x01,0x19,0x16,0x03,0x63,0x6F,0x6D,0x31,
		0x18,0x30,0x16,0x06,0x0A,0x09,0x92,0x26,0x89,0x93,0xF2,0x2C,0x64,0x01,0x19,0x16,
		0x08,0x4D,0x69,0x63,0x72,0x6F,0x53,0x65,0x63,0x31,0x18,0x30,0x16,0x06,0x03,0x55,
		0x04,0x0B,0x0C,0x0F,0x44,0x65,0x70,0x61,0x72,0x74,0x5F,0x69,0x6E,0x66,0x6F,0x74,
		0x65,0x63,0x68,0x31,0x0D,0x30,0x0B,0x06,0x03,0x55,0x04,0x03,0x13,0x04,0x74,0x65,
		0x73,0x74,0x30,0x81,0x9F,0x30,0x0D,0x06,0x09,0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,
		0x01,0x01,0x05,0x00,0x03,0x81,0x8D,0x00,0x30,0x81,0x89,0x02,0x81,0x81,0x00,0xBC,
		0x13,0x01,0xAE,0xF2,0x5F,0x39,0x5A,0xF2,0xB2,0x95,0xC3,0xD1,0x04,0x64,0x60,0x09,
		0xAA,0xAB,0x8B,0xA2,0x8F,0x00,0x4A,0x94,0x71,0x93,0xD9,0xD9,0xCA,0x39,0xAA,0xE9,
		0x6E,0xED,0xAE,0xA9,0x87,0x29,0x22,0xB3,0x52,0x40,0x2B,0xDE,0xC3,0x6F,0x0D,0x8B,
		0xDD,0x74,0x89,0xE0,0x82,0xD1,0xDB,0x7F,0x8E,0xD0,0x8E,0x66,0x5A,0x2C,0x80,0x2C,
		0xCF,0xEB,0x31,0x77,0x54,0xC8,0x16,0xBA,0xE9,0x76,0x56,0x89,0x52,0xA1,0x09,0xA3,
		0xBA,0xC7,0x0E,0x3F,0xF9,0xBE,0x7C,0x1A,0xA7,0x3B,0xC0,0xFF,0x60,0x40,0x1A,0xCE,
		0xA0,0x1A,0xF1,0xD6,0x33,0xF2,0xBD,0xB3,0x0F,0x30,0x39,0x1D,0x25,0xC2,0xDF,0xA5,
		0x58,0xE7,0xD4,0x69,0x79,0xAC,0x3B,0x0D,0xDF,0x1C,0xAD,0xC1,0x9E,0xA3,0x6D,0x02,
		0x03,0x01,0x00,0x01,0xA3,0x82,0x02};

	CK_BYTE subject[] = {0x00,0x01,0x02,0x03};
	CK_BYTE label[] = {0x00,0x01,0x02,0x03};
	CK_BYTE idid[] = {0x00,0x01,0x02,0x03};

	CK_ATTRIBUTE_TYPE cko_class = CKO_DATA;

	CK_ATTRIBUTE dataTemplate[] = {
	{ CKA_APPLICATION, subject, sizeof(subject) },
	//{ CKA_LABEL, label, sizeof(label)},
	//{ CKA_ID, idid, sizeof(idid) },
	{ CKA_VALUE, datadata, sizeof(datadata) },
	{ CKA_CLASS, &cko_class, sizeof(CK_ATTRIBUTE_TYPE)},
	{ CKA_TOKEN, &ttrue, sizeof(CK_BBOOL) },
	{ CKA_PRIVATE, &ttrue, sizeof(CK_BBOOL) } 
	};

	//创建证书对象
	rv = (*pC_CreateObject)(hSession, 
		dataTemplate, sizeof(dataTemplate)/sizeof(CK_ATTRIBUTE),
		&hCertificate);

	if(rv!=CKR_OK)
	{
		printf("test_object_cert failed,pC_CreateObject1. rv=0x%08x.\n",rv);
		goto END;
	}
	else
	{
		printf("pC_CreateObject1 Sucecss.\n");
	}
	
	bRtn = true;
END:
	printf("leave test_object_cert.\n");

	
	return bRtn;
}
bool xtest_FindData()
{
	CK_RV rv=0;
	bool bRtn=false;
	CK_BYTE IDIDid0[]={};//0x00};//,0x01};//,0x02};//,0x03};
	CK_BYTE IDIDid1[]={0x00};//,0x01};//,0x02};//,0x03};
	CK_BYTE IDIDid2[]={0x00,0x01,0x02,0x03};

	CK_OBJECT_CLASS pubclass=CKO_DATA;

	CK_ATTRIBUTE pubFindKeyTemplate1[] = {
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
	};

	CK_ATTRIBUTE pubFindKeyTemplate2[] = {
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
#ifdef TEST_CKA_ID
		{CKA_ID, IDIDid0,sizeof(IDIDid0)}
#endif
#ifdef TEST_CKA_LABEL
		{CKA_LABEL, IDIDid0,sizeof(IDIDid0)}
#endif
#ifdef TEST_CKA_APPLICAITON
		{CKA_APPLICATION, IDIDid0,sizeof(IDIDid0)}
#endif
	};

	CK_ATTRIBUTE pubFindKeyTemplate3[] = {
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
#ifdef TEST_CKA_ID
		{CKA_ID, IDIDid1,sizeof(IDIDid1)}
#endif
#ifdef TEST_CKA_LABEL
		{CKA_LABEL, IDIDid1,sizeof(IDIDid1)}
#endif
#ifdef TEST_CKA_APPLICAITON
		{CKA_APPLICATION, IDIDid1,sizeof(IDIDid1)}
#endif
	};


	CK_ATTRIBUTE pubFindKeyTemplate4[] = {
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
#ifdef TEST_CKA_ID
		{CKA_ID, IDIDid2,sizeof(IDIDid2)}
#endif
#ifdef TEST_CKA_LABEL
		{CKA_LABEL, IDIDid2,sizeof(IDIDid2)}
#endif
#ifdef TEST_CKA_APPLICAITON
		{CKA_APPLICATION, IDIDid2,sizeof(IDIDid2)}
#endif
	};


	CK_ATTRIBUTE pubFindKeyTemplate5[] = {
	};


	CK_OBJECT_HANDLE_PTR hObject = NULL_PTR;
	CK_OBJECT_HANDLE hObjectDestory = NULL_PTR;
	CK_ULONG ulObjectCount = 0;
	int i;

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xtest_FindPublicPrivateKey);
	//查找私钥对象	
	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 16;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate1, sizeof(pubFindKeyTemplate1)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit1,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects1,rv);

	printf("Key count1=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Key count1=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal1,rv);

	free(hObject);

	//查找公钥对象
	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 16;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate2, sizeof(pubFindKeyTemplate2)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit2,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects2,rv);

	printf("Key count2=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Key count2=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal2,rv);
	
	free(hObject);

	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 16;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate3, sizeof(pubFindKeyTemplate3)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit3,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects3,rv);

	printf("Key count3=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Key count3=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal3,rv);

	free(hObject);

	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 16;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate4, sizeof(pubFindKeyTemplate4)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit4,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects4,rv);

	printf("Key count4=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Key count4=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal4,rv);

	free(hObject);

	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 16;
	printf("count=%d\n",sizeof(pubFindKeyTemplate5)/sizeof(CK_ATTRIBUTE));
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate5,sizeof(pubFindKeyTemplate5)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit5,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects5,rv);

	printf("Key count5=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Key count5=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal5,rv);

	free(hObject);

	bRtn=true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xtest_FindPublicPrivateKey);

	
	return bRtn;
}

bool xtest_xxxxSM2ImportPerformance(int looptime,int datalen)
{
	CK_RV rv=0;
	bool bRtn=false;
	CK_OBJECT_HANDLE hPublicKey, hPrivateKey;
	CK_OBJECT_HANDLE * phPublicKey=&hPublicKey,*phPrivateKey=&hPrivateKey;

	CK_BYTE nSessKeyID_Enc = CK_SESSKEY_ID0;
	int i=0;	
	CK_BYTE pData[256]={0};

	CK_BYTE     temp_pub_x[32]={
		0xec,0x91,0x81,0x8d,0xe0,0xb7,0x01,0x21,0x73,0xf5,0x1c,0x33,0x75,0x43,0x6e,0x43,
			0xb6,0xa9,0xa2,0x6a,0xbd,0x6d,0xbc,0xb7,0x9f,0x85,0x1c,0xde,0xaf,0x7a,0x0f,0x6c
	};
	CK_BYTE     temp_pub_y[32]={
		0xcb,0xf4,0xb5,0xa1,0x5f,0xb8,0x7e,0x60,0xfc,0x0b,0x3a,0x92,0x3d,0x12,0xe8,0x66,
			0x36,0x4a,0x93,0x5f,0xfb,0x30,0x84,0x2b,0xc9,0x13,0x9e,0xbd,0x2d,0xdc,0xe9,0x61
	};
	CK_BYTE     temp_prv[32]={
		0xc5,0x6a,0x2b,0x58,0xa0,0x94,0xef,0x24,0x41,0x03,0x79,0x45,0xba,0xb1,0x39,0x8c,
			0xc0,0xdf,0x9f,0xc4,0xf9,0x9e,0x9a,0x60,0x2c,0xd8,0x6f,0xc2,0xc3,0x88,0xad,0x0c
	};
	
	CK_BBOOL     ttrue = CK_TRUE;
	CK_KEY_TYPE  keyType=CKK_ECC;
	CK_OBJECT_CLASS pubclass=CKO_PUBLIC_KEY,priclass=CKO_PRIVATE_KEY;
	CK_UINT     modulusBits = 256;
	CK_BYTE     id[] = {0x01,0x01,0x02,0x03};
	CK_BYTE     idid[] = {0x01,0x01,0x02};
	CK_ATTRIBUTE publicKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, temp_pub_x, sizeof(temp_pub_x)},
		{CKA_ECC_Y_COORDINATE, temp_pub_y, sizeof(temp_pub_y)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_PRIVATE, temp_prv, sizeof(temp_prv)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)},
		{CKA_ID, id, sizeof(id)}
	};

	unsigned char	ECCPriv3[32] = {0x3D, 0x73, 0x7C, 0x01, 0x52, 0x36, 0x79, 0xB0, 0x39, 0x95, 0x60, 0xD1, 0x27, 0x4E, 0x77, 0x7C, 0x59, 0xD0, 0x40, 0xBF, 0x28, 0x38, 0x0E, 0x33, 0x1F, 0xFA, 0x66, 0x79, 0x66, 0xF2, 0x02, 0x89};
	unsigned char	ECCGX3[32] = {0x38, 0xCD, 0xAD, 0xDD, 0xDA, 0x1C, 0x6C, 0x51, 0x35, 0x23, 0xCC, 0xB3, 0x2E, 0x0E, 0x28, 0x01, 0x26, 0x2B, 0x7F, 0x95, 0xF2, 0x5E, 0x85, 0x3C, 0x86, 0xE1, 0x01, 0x28, 0x6C, 0x82, 0xD7, 0x18};
	unsigned char	ECCGY3[32] = {0x6A, 0x1C, 0x8A, 0x61, 0xC4, 0xD3, 0xED, 0x61, 0x9D, 0xFA, 0xA0, 0x33, 0x20, 0x5C, 0x5F, 0x76, 0x9B, 0x3B, 0xE1, 0x2C, 0x8F, 0x15, 0x7C, 0x2F, 0x55, 0x75, 0xFF, 0xEE, 0xFB, 0x42, 0xA0, 0x64};

	CK_ATTRIBUTE publicKeyTemplate1[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, ECCGX3, sizeof(ECCGX3)},
		{CKA_ECC_Y_COORDINATE, ECCGY3, sizeof(ECCGY3)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)},
		{CKA_ID, idid, sizeof(idid)}
	};

	CK_ATTRIBUTE pubFindKeyTemplate[] = {
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE pubFindKeyTemplate1[] = {
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ID, idid, sizeof(idid)}
	};

	CK_ATTRIBUTE prvFindKeyTemplate[] = {
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_BYTE get_pub_x[32] = {0};
	CK_BYTE get_pub_y[32] = {0};
	CK_ATTRIBUTE pubGetKeyTemplate[] = {
		{CKA_ECC_X_COORDINATE, get_pub_x, sizeof(get_pub_x)},
		{CKA_ECC_Y_COORDINATE, get_pub_y, sizeof(get_pub_y)}
	};

	CK_BYTE get_prv[32] = {0};
	CK_ATTRIBUTE prvGetKeyTemplate[] = {
		{CKA_ECC_PRIVATE, get_prv, sizeof(get_prv)}
	};

	CK_OBJECT_HANDLE hObject = NULL_PTR;
	CK_ULONG ulObjectCount = 0;

	CK_MECHANISM  mechanismcalc = {CKM_ECC_CALC, NULL_PTR, 0};
	CK_BYTE       pOutData[256];   
	CK_ULONG      ulOutDataLen=sizeof(pOutData);
	CK_BYTE       pOutData1[256];   
	CK_ULONG      ulOutDataLen1=sizeof(pOutData1);
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0},ttc5={0,0},ttc6={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2ImportPerformance);

	//创建公钥对象
	rv = (*pC_CreateObject)(hSession, 
						  publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  phPublicKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject1,rv);

	//创建私钥对象
	rv = (*pC_CreateObject)(hSession, 
						  privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  phPrivateKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject2,rv);

	//查找公钥对象，并读取公钥对象属性
	hObject = NULL_PTR;
	ulObjectCount = 0;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate, sizeof(pubFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit2,rv);

	rv = (*pC_FindObjects)(hSession, &hObject, 1, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects2,rv);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal2,rv);

	memset(get_pub_x,0,32);
	memset(get_pub_y,0,32);
	rv = (*pC_GetAttributeValue)(hSession, hObject, pubGetKeyTemplate, sizeof(pubGetKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValue2,rv);

	UtilsPrintData(VNAME(get_pub_x),32,0);
	UtilsPrintData(VNAME(get_pub_y),32,0);


	rv = (*pC_CreateObject)(hSession, 
						  publicKeyTemplate1, sizeof(publicKeyTemplate1)/sizeof(CK_ATTRIBUTE),
						  phPublicKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject1,rv);

	hObject = NULL_PTR;
	ulObjectCount = 0;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate, sizeof(pubFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit2,rv);

	rv = (*pC_FindObjects)(hSession, &hObject, 1, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects2,rv);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal2,rv);

	memset(get_pub_x,0,32);
	memset(get_pub_y,0,32);
	rv = (*pC_GetAttributeValue)(hSession, hObject, pubGetKeyTemplate, sizeof(pubGetKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValue2,rv);

	UtilsPrintData(VNAME(get_pub_x),32,0);
	UtilsPrintData(VNAME(get_pub_y),32,0);


	hObject = NULL_PTR;
	ulObjectCount = 0;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate1, sizeof(pubFindKeyTemplate1)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit2,rv);

	rv = (*pC_FindObjects)(hSession, &hObject, 1, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects2,rv);

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal2,rv);
	printf("ulObjectCount=%d\n",ulObjectCount);
	memset(get_pub_x,0,32);
	memset(get_pub_y,0,32);
	rv = (*pC_GetAttributeValue)(hSession, hObject, pubGetKeyTemplate, sizeof(pubGetKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GetAttributeValue2,rv);

	UtilsPrintData(VNAME(get_pub_x),32,0);
	UtilsPrintData(VNAME(get_pub_y),32,0);


	//rv = pC_DestroyObject(hSession,hPublicKey);
	//RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);

	//rv = pC_DestroyObject(hSession,hPrivateKey);
	//RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);

	bRtn = true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM2ImportPerformance);

	
	return bRtn;
}
/*SM3KDF密钥衍生结果测试*/
bool xtest_SM3KDF_Object()
{
	bool	bRtn = false;
	CK_RV	rv = 0;
	//for set sm3 kdf basekey
	unsigned char	ucSM3KDFBaseKeyBuffer[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	CK_EXTEND_IN	ExtIn_SetSM3KDFBaseKey = {CK_EXTEND_SETSM3KDFBASEKEY, ucSM3KDFBaseKeyBuffer, sizeof(ucSM3KDFBaseKeyBuffer)};
	CK_EXTEND_OUT	ExtOut_SetSM3KDFBaseKey = {CK_EXTEND_SETSM3KDFBASEKEY, NULL, 0};
	//for calc sm3 kdf
	unsigned char	ucSM3KDFKeyBuffer1[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	CK_MECHANISM	sm3KDFPRESETMech = {CKM_DERIVE_SM3_KDF_WITH_PRESET, ucSM3KDFKeyBuffer1, sizeof(ucSM3KDFKeyBuffer1)};
	unsigned char	ucSM3KDFKeyBuffer2[17] = {0x11, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	CK_MECHANISM	sm3KDFBASEMech = {CKM_DERIVE_SM3_KDF, ucSM3KDFKeyBuffer2, sizeof(ucSM3KDFKeyBuffer2)};
	unsigned char	ucSM3KDFKeyBuffer3[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	CK_MECHANISM	sm3KDFNOBASEMech = {CKM_DERIVE_SM3_KDF, ucSM3KDFKeyBuffer3, sizeof(ucSM3KDFKeyBuffer3)};
	CK_BYTE	nBaseKeyID = CK_SESSKEY_ID0;
	CK_BYTE	nSessKeyID1 = CK_SESSKEY_ID0 + 1;
	CK_BYTE	nSessKeyID2 = CK_SESSKEY_ID0 + 2;
	CK_BYTE	nSessKeyID3 = CK_SESSKEY_ID0 + 3;
	CK_BYTE	nSessKeyID4 = CK_SESSKEY_ID0 + 4;
	CK_BYTE	nSessKeyID5 = CK_SESSKEY_ID0 + 5;
	CK_BYTE	nSessKeyID6 = CK_SESSKEY_ID0 + 6;
	CK_BYTE	nSessKeyID7 = CK_SESSKEY_ID0 + 7;
	CK_BYTE	nSessKeyID8 = CK_SESSKEY_ID0 + 8;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_ATTRIBUTE sm3BaseKeyAttrib[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nBaseKeyID, sizeof(CK_BYTE)}
	};
	CK_ATTRIBUTE sm3KDFAttrib1[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID1, sizeof(CK_BYTE)}
	};
	CK_ATTRIBUTE sm3KDFAttrib2[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID2, sizeof(CK_BYTE)}
	};

	CK_ATTRIBUTE sm3KDFAttrib3[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID3, sizeof(CK_BYTE)}
	};

	CK_ATTRIBUTE sm3KDFAttrib4[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID4, sizeof(CK_BYTE)}
	};

	CK_ATTRIBUTE sm3KDFAttrib5[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID5, sizeof(CK_BYTE)}
	};

	CK_ATTRIBUTE sm3KDFAttrib6[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID6, sizeof(CK_BYTE)}
	};

	CK_ATTRIBUTE sm3KDFAttrib7[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID7, sizeof(CK_BYTE)}
	};

	CK_ATTRIBUTE sm3KDFAttrib8[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID8, sizeof(CK_BYTE)}
	};
	
	CK_ATTRIBUTE pubFindKeyTemplate[] = {
		{CKA_CLASS, &keyClass, sizeof(CK_OBJECT_CLASS)},
	};

	CK_OBJECT_HANDLE_PTR hObject = NULL_PTR;
	CK_OBJECT_HANDLE hObjectDestory = NULL_PTR;
	CK_ULONG ulObjectCount = 0;
	CK_OBJECT_HANDLE	hBaseKey = NULL_PTR, hDerivedKey1 = NULL_PTR, hDerivedKey2 = NULL_PTR, hDerivedKey3 = NULL_PTR, hDerivedKey4 = NULL_PTR, hDerivedKey5 = NULL_PTR, hDerivedKey6 = NULL_PTR, hDerivedKey7 = NULL_PTR, hDerivedKey8 = NULL_PTR;;
	unsigned int i = 0;

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM3KDF_Object);	

	//set sm3 kdf basekey
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetSM3KDFBaseKey, &ExtOut_SetSM3KDFBaseKey, NULL);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CryptoExtend,rv);
	printf("%*c\r",79,0x20);
	printf("test_SM3KDF times: %d .\r", i++);
	fflush(stdout);

	//calculate sm3 kdf with preset 0
	rv = (*pC_DeriveKey)(hSession, &sm3KDFPRESETMech, NULL_PTR, sm3BaseKeyAttrib, sizeof(sm3BaseKeyAttrib)/sizeof(CK_ATTRIBUTE), &hBaseKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey0,rv);
	printf("%*c\r",79,0x20);
	printf("test_SM3KDF times: %d .\r", i++);
	fflush(stdout);

	//calculate sm3 kdf with basekey 1
	rv = (*pC_DeriveKey)(hSession, &sm3KDFBASEMech, hBaseKey, sm3KDFAttrib1, sizeof(sm3KDFAttrib1)/sizeof(CK_ATTRIBUTE), &hDerivedKey1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey1,rv);
	printf("%*c\r",79,0x20);
	printf("test_SM3KDF times: %d .\r", i++);
	fflush(stdout);

	//calculate sm3 kdf without basekey 2
	rv = (*pC_DeriveKey)(hSession, &sm3KDFNOBASEMech, NULL_PTR, sm3KDFAttrib2, sizeof(sm3KDFAttrib2)/sizeof(CK_ATTRIBUTE), &hDerivedKey2);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey2,rv);
	printf("%*c\r",79,0x20);
	printf("test_SM3KDF times: %d .\r", i++);

	//calculate sm3 kdf without basekey 3
	rv = (*pC_DeriveKey)(hSession, &sm3KDFNOBASEMech, NULL_PTR, sm3KDFAttrib3, sizeof(sm3KDFAttrib3)/sizeof(CK_ATTRIBUTE), &hDerivedKey3);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey3,rv);
	printf("%*c\r",79,0x20);
	printf("test_SM3KDF times: %d .\r", i++);
	fflush(stdout);

	//calculate sm3 kdf without basekey 4
	rv = (*pC_DeriveKey)(hSession, &sm3KDFNOBASEMech, NULL_PTR, sm3KDFAttrib4, sizeof(sm3KDFAttrib4)/sizeof(CK_ATTRIBUTE), &hDerivedKey4);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey4,rv);
	printf("%*c\r",79,0x20);
	printf("test_SM3KDF times: %d .\r", i++);
	fflush(stdout);

	//calculate sm3 kdf without basekey 5
	rv = (*pC_DeriveKey)(hSession, &sm3KDFNOBASEMech, NULL_PTR, sm3KDFAttrib5, sizeof(sm3KDFAttrib5)/sizeof(CK_ATTRIBUTE), &hDerivedKey5);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey5,rv);
	printf("%*c\r",79,0x20);
	printf("test_SM3KDF times: %d .\r", i++);
	fflush(stdout);
	//calculate sm3 kdf without basekey 6
	rv = (*pC_DeriveKey)(hSession, &sm3KDFNOBASEMech, NULL_PTR, sm3KDFAttrib6, sizeof(sm3KDFAttrib6)/sizeof(CK_ATTRIBUTE), &hDerivedKey6);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey6,rv);
	printf("%*c\r",79,0x20);
	printf("test_SM3KDF times: %d .\r", i++);
	fflush(stdout);

	//calculate sm3 kdf without basekey 7
	rv = (*pC_DeriveKey)(hSession, &sm3KDFNOBASEMech, NULL_PTR, sm3KDFAttrib7, sizeof(sm3KDFAttrib7)/sizeof(CK_ATTRIBUTE), &hDerivedKey7);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey7,rv);
	printf("%*c\r",79,0x20);
	printf("test_SM3KDF times: %d .\r", i++);
	fflush(stdout);
	//calculate sm3 kdf without basekey 8
	rv = (*pC_DeriveKey)(hSession, &sm3KDFNOBASEMech, NULL_PTR, sm3KDFAttrib8, sizeof(sm3KDFAttrib8)/sizeof(CK_ATTRIBUTE), &hDerivedKey8);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey8,rv);
	printf("%*c\r",79,0x20);
	printf("test_SM3KDF times: %d .\r", i++);
	fflush(stdout);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_SM3KDF The Total DeriveKey Times: %d .<br>", i);

	bRtn = true;
END:
	if (hBaseKey)
	{
		rv = pC_DestroyObject(hSession, hBaseKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject0,rv);
	}
	if (hDerivedKey1)
	{
		rv = pC_DestroyObject(hSession, hDerivedKey1);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);
	}
	if (hDerivedKey2)
	{
		rv = pC_DestroyObject(hSession, hDerivedKey2);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject2,rv);
	}
	if (hDerivedKey3)
	{
		rv = pC_DestroyObject(hSession, hDerivedKey3);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject3,rv);
	}
	if (hDerivedKey4)
	{
		rv = pC_DestroyObject(hSession, hDerivedKey4);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject4,rv);
	}
	if (hDerivedKey5)
	{
		rv = pC_DestroyObject(hSession, hDerivedKey5);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject5,rv);
	}
	if (hDerivedKey6)
	{
		rv = pC_DestroyObject(hSession, hDerivedKey6);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject6,rv);
	}
	if (hDerivedKey7)
	{
		rv = pC_DestroyObject(hSession, hDerivedKey7);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject7,rv);
	}
	if (hDerivedKey8)
	{
		rv = pC_DestroyObject(hSession, hDerivedKey8);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject8,rv);
	}
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM3KDF_Object);

	
	return bRtn;	
}	


/*测试SM3 KDF密钥衍生*/
bool xtest_SM3KDF_OFF()
{
	bool	bRtn = false;
	CK_RV	rv = 0;
	//for set sm3 kdf basekey
	unsigned char	ucSM3KDFBaseKeyBuffer[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	CK_EXTEND_IN	ExtIn_SetSM3KDFBaseKey = {CK_EXTEND_SETSM3KDFBASEKEY, ucSM3KDFBaseKeyBuffer, sizeof(ucSM3KDFBaseKeyBuffer)};
	CK_EXTEND_OUT	ExtOut_SetSM3KDFBaseKey = {CK_EXTEND_SETSM3KDFBASEKEY, NULL, 0};
	//for calc sm3 kdf
	unsigned char	ucSM3KDFKeyBuffer1[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	CK_MECHANISM	sm3KDFPRESETMech = {CKM_DERIVE_SM3_KDF_WITH_PRESET, ucSM3KDFKeyBuffer1, sizeof(ucSM3KDFKeyBuffer1)};
	CK_BYTE	nBaseKeyID = CK_SESSKEY_ID0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_ATTRIBUTE sm3BaseKeyAttrib[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nBaseKeyID, sizeof(CK_BYTE)}
	};

	CK_ATTRIBUTE pubFindKeyTemplate[] = {
		{CKA_CLASS, &keyClass, sizeof(CK_OBJECT_CLASS)},
	};

	CK_OBJECT_HANDLE hObject[16] ={0};
	CK_OBJECT_HANDLE hObjectDestory = NULL_PTR;
	CK_ULONG ulObjectCount = 0;
	CK_OBJECT_HANDLE	hBaseKey = NULL_PTR;

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM3KDF_OFF);
	//查找生成密钥对象	
	memset(hObject, 0, 16);
	ulObjectCount = 0;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate, sizeof(pubFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects,rv);

	printf("Before SM3KDF count=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength ,"Before SM3KDF count=%d.<br>", ulObjectCount);		

	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal,rv);	

	//set sm3 kdf basekey
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetSM3KDFBaseKey, &ExtOut_SetSM3KDFBaseKey, NULL);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CryptoExtend,rv);	

	//calculate sm3 kdf with preset
	rv = (*pC_DeriveKey)(hSession, &sm3KDFPRESETMech, NULL_PTR, sm3BaseKeyAttrib, sizeof(sm3BaseKeyAttrib)/sizeof(CK_ATTRIBUTE), &hBaseKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey,rv);

	//查找生成密钥对象	
	memset(hObject, 0, 16);
	ulObjectCount = 0;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate, sizeof(pubFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects,rv);

	printf("After SM3KDF count=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"After SM3KDF count=%d.<br>", ulObjectCount);

	bRtn = true;
END:	
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM3KDF_OFF Please turn off your device and test again.);

	
	return bRtn;
}


/*密钥交换测试*/
bool xtest_KeyExchange()
{
	bool bRtn = false;

	CK_RV rv = 0;
	CK_OBJECT_HANDLE hPartKey1 = NULL_PTR, hPartKey2 = NULL_PTR, hSessKey = NULL_PTR;
	CK_BYTE keyID1 = CK_SESSKEY_ID0, keyID2 = CK_SESSKEY_ID0, keyID3 = CK_SESSKEY_ID0;
	//for C_GenerateKey
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;

	CK_KEY_TYPE SessKeyExchangeKeyType = CKK_SESSKEY_EXCHANGE;
	CK_MECHANISM SessKeyExchangeMechanism = {CKM_SESSKEY_EXCHANGE_GEN, NULL, 0};
	CK_ATTRIBUTE SessKeyExchangeTemplate1[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SessKeyExchangeKeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &keyID1, sizeof(CK_BYTE)}
	};

	//for C_GenerateKeyPair
	CK_MECHANISM      ECCMechanism = {CKM_ECC_KEY_PAIR_GEN, NULL_PTR, 0};
	CK_UINT     ECCModulesBits = 256;
	CK_KEY_TYPE  ECCKeyType = CKK_ECC;
	CK_ATTRIBUTE publicKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &ECCModulesBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(CK_BBOOL)},
		{CKA_ENCRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_VERIFY, &ttrue, sizeof(CK_BBOOL)},
		{CKA_WRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_KEY_TYPE,&ECCKeyType,sizeof(CK_KEY_TYPE)}
	};
	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &ECCModulesBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(CK_BBOOL)},
		{CKA_PRIVATE, &ttrue, sizeof(CK_BBOOL)},
		{CKA_SENSITIVE, &ttrue, sizeof(CK_BBOOL)},
		{CKA_DECRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_SIGN, &ttrue, sizeof(CK_BBOOL)},
		{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_KEY_TYPE,&ECCKeyType,sizeof(CK_KEY_TYPE)}
	};
	CK_OBJECT_HANDLE hPublicKey = NULL_PTR, hPrivateKey = NULL_PTR;

	//for get exchange public key
	CK_BYTE			pSM2PublicKey[64] = {0};
	CK_UINT			nSM2PublicKeyLen = sizeof(pSM2PublicKey);
	CK_EXTEND_IN	ExtIn_GetExchangePubKey = {CK_EXTEND_GETEXCHANGEPUBKEY, NULL, 0};
	CK_EXTEND_OUT	ExtOut_GetExchangePubKey = {CK_EXTEND_GETEXCHANGEPUBKEY, pSM2PublicKey, nSM2PublicKeyLen};

	//for C_WrapKey
	CK_MECHANISM	SM2WrapMechanism = {CKM_SM2_WRAP, pSM2PublicKey, nSM2PublicKeyLen};
	CK_BYTE			pbWrappedKey[128] = {0};
	CK_ULONG		ulWrappedKeyLen = sizeof(pbWrappedKey);

	//for C_UnwrapKey
	CK_MECHANISM	SM2UnwrapMechanism = {CKM_SM2_UNWRAP, NULL, 0};
	CK_ATTRIBUTE SessKeyExchangeTemplate2[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SessKeyExchangeKeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &keyID2, sizeof(CK_BYTE)}
	};

	//for C_DeriveKey
	CK_OBJECT_HANDLE	keyHandles[2] = {0};
	CK_MECHANISM	DeriveKeyMechanism = {CKM_30RAYCOM_DERIVE, keyHandles, 2*sizeof(CK_OBJECT_HANDLE)};
	CK_ATTRIBUTE SessKeyDeriveTemplate[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SessKeyExchangeKeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &keyID3, sizeof(CK_BYTE)}
	};

	//for get exchange part key 1
	CK_BYTE			pMonitorSM2PublicKey[64] = {0x1D, 0x18, 0x8B, 0x6F, 0x97, 0x9D, 0xB6, 0x3D, 0xF1, 0x5D, 0x93, 0x33, 0x54, 0x3D, 0xA9, 0x39, 0x69, 0x25, 0x4B, 0xD4, 0x30, 0x70, 0x7B, 0x62, 0x9D, 0xE2, 0x1D, 0x3F, 0x29, 0xAD, 0x49, 0x23, 0x5C, 0xF7, 0x4F, 0x72, 0x80, 0x44, 0x46, 0xC1, 0x48, 0x40, 0x80, 0xB7, 0x78, 0xF8, 0xD7, 0xDD, 0x6B, 0x1D, 0x32, 0x11, 0x28, 0x35, 0x20, 0x1C, 0xC8, 0xBA, 0xE0, 0x6A, 0xA8, 0xDB, 0x0C, 0xD7};
	CK_UINT			nMonitorSM2PublicKeyLen = sizeof(pMonitorSM2PublicKey);
	CK_BYTE			pSOPin[8] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	CK_UINT			nSOPinLen = sizeof(pSOPin);
	CK_BYTE			pMonitorParameter[64+8] = {0};
	CK_UINT			nMonitorParameterLen = sizeof(pMonitorParameter);
	CK_EXTEND_IN	ExtIn_SetMonitorSM2PublicKey = {CK_EXTEND_SETMONITORSM2PUBKEY, pMonitorParameter, nMonitorParameterLen};
	CK_EXTEND_OUT	ExtOut_SetMonitorSM2PublicKey = {CK_EXTEND_SETMONITORSM2PUBKEY, NULL, 0};

	//for get exchange part key 1
	CK_BYTE			pExchangeSessKey1[128] = {0};
	CK_UINT			nExchangeSessKey1Len = sizeof(pExchangeSessKey1);
	CK_EXTEND_IN	ExtIn_GetExchangeSessKey1 = {CK_EXTEND_GETEXCHANGESESSKEY, &keyID1, sizeof(CK_BYTE)};
	CK_EXTEND_OUT	ExtOut_GetExchangeSessKey1 = {CK_EXTEND_GETEXCHANGESESSKEY, pExchangeSessKey1, nExchangeSessKey1Len};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestKeyExchange);

	//for (i=0; i<loopTime; i++)
	//{
		hPartKey1 = NULL_PTR;
		hPartKey2 = NULL_PTR;
		hSessKey = NULL_PTR;
		//aaa
		//t1 = PlatformAPI::GetTickCount();
		//导入用于监听的公钥（需要管理员权限）
		memcpy(pMonitorParameter, pMonitorSM2PublicKey, nSM2PublicKeyLen);
		memcpy(pMonitorParameter + nSM2PublicKeyLen, pSOPin, nSOPinLen);
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetMonitorSM2PublicKey, &ExtOut_SetMonitorSM2PublicKey, NULL);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CryptoExtend0,rv);

		//aaa pra，pba
		//生成用于密钥交换的公私钥对（一个SD内只有一对）
		rv = (*pC_GenerateKeyPair)(hSession, &ECCMechanism,
			publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
			privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
			&hPublicKey, &hPrivateKey);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKeyPair,rv);

		
		//aaa   pba
		//获取之前生成的用于密钥交换的公钥值
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetExchangePubKey, &ExtOut_GetExchangePubKey, NULL);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CryptoExtend1,rv);

		
		//aaa  ra=hPartKey1
		//获取用于密钥交换中的部分密钥1的句柄
		rv = (*pC_GenerateKey)(hSession, &SessKeyExchangeMechanism, SessKeyExchangeTemplate1, sizeof(SessKeyExchangeTemplate1)/sizeof(CK_ATTRIBUTE), &hPartKey1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKey,rv);


		//应该把自己的公钥导出去，把对方的公钥导进来，此处没有传出
		//aaa
		//生成部分密钥1，并使用传入的公钥值加密后导出
		rv = (*pC_WrapKey)(hSession, &SM2WrapMechanism, 0, hPartKey1, pbWrappedKey, &ulWrappedKeyLen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_WrapKey,rv);

		//导入部分密钥2（使用之前生成的用于密钥交换的私钥解密）
		rv = (*pC_UnwrapKey)(hSession, &SM2UnwrapMechanism, 0, pbWrappedKey, ulWrappedKeyLen, SessKeyExchangeTemplate2, sizeof(SessKeyExchangeTemplate2)/sizeof(CK_ATTRIBUTE), &hPartKey2);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_UnwrapKey,rv);

		//使用部分密钥1和部分密钥2，协商生成密钥
		keyHandles[0] = hPartKey1;
		keyHandles[1] = hPartKey2;
		rv = (*pC_DeriveKey)(hSession, &DeriveKeyMechanism, NULL_PTR, SessKeyDeriveTemplate, sizeof(SessKeyDeriveTemplate)/sizeof(CK_ATTRIBUTE), &hSessKey);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DeriveKey,rv);

		//导出之前生成的部分密钥1（监听）
		rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetExchangeSessKey1, &ExtOut_GetExchangeSessKey1, NULL);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CryptoExtend2,rv);

		if (hPublicKey)
		{
			rv = pC_DestroyObject(hSession, hPublicKey);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hPublicKey,rv);
			hPublicKey = NULL_PTR;
		}
		if (hPrivateKey)
		{
			rv = pC_DestroyObject(hSession, hPrivateKey);
			RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hPrivateKey,rv);
			hPrivateKey = NULL_PTR;
		}
	//}
	bRtn = true;
END:
	if (hPublicKey)
	{
		rv = pC_DestroyObject(hSession, hPublicKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject0,rv);
	}
	if (hPrivateKey)
	{
		rv = pC_DestroyObject(hSession, hPrivateKey);
		RV_NOT_OK(Testresult[nItemNumb],&nResultLength,pC_DestroyObject1,rv);
	}	
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestKeyExchange);

	
	return bRtn;
}


#define DESTORYUSELESSKEY
bool xtest_FindCertObjectAndDestory()
{
	CK_RV rv=0;
	bool bRtn = false;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_OBJECT_HANDLE_PTR hObject = NULL_PTR;
	CK_OBJECT_HANDLE hObjectDestory = NULL_PTR;
	CK_ULONG ulObjectCount = 0;
	int i;

	//有效证书的value
	CK_OBJECT_CLASS certclass=CKO_CERTIFICATE;
	CK_ATTRIBUTE certTemplate[] = {
		{CKA_CLASS, &certclass, sizeof(CK_OBJECT_CLASS)}
	};

	printf("enter test_FindCertObjectNumber.\n");
	nResultLength += sprintf(Testresult[nItemNumb]+nResultLength , "enter test_FindCertObjectNumber.<br>");
	//查找私钥对象	
	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 0;
	rv = (*pC_FindObjectsInit)(hSession, certTemplate, sizeof(certTemplate)/sizeof(CK_ATTRIBUTE));
	if(rv!=CKR_OK)
	{
		printf("test_FindCertObjectNumber failed,pC_FindObjectsInit,1. rv=0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"test_FindCertObjectNumber failed,pC_FindObjectsInit,1. rv=0x%08x.<br>",rv);
		bRtn =false;
		goto END;
	}

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	if(rv!=CKR_OK)
	{
		printf("test_FindCertObjectNumber failed,pC_FindprvObjects,1. rv=0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"test_FindCertObjectNumber failed,pC_FindprvObjects,1. rv=0x%08x.<br>",rv);
		bRtn =false;
		goto END;
	}
	printf("Cert count=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Cert count=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	if(rv!=CKR_OK)
	{
		printf("test_FindCertObjectNumber failed,pC_FindpObjectsFinal,1. rv=0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"test_FindCertObjectNumber failed,pC_FindpObjectsFinal,1. rv=0x%08x.<br>",rv);
		bRtn =false;
		goto END;
	}

#ifdef DESTORYUSELESSKEY
	hObjectDestory = NULL_PTR;
	for(i=0;i<ulObjectCount;i++)
	{
		hObjectDestory=hObject[i];
		printf("%*c\r",79,0x20);
		printf("Cert i=%d,CertObject=0x%08x.\r",i,hObjectDestory);
		fflush(stdout);
		//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Cert i=%d,CertObject=0x%08x.\r",i,hObjectDestory);
		
		rv = (*pC_DestroyObject)(hSession, hObjectDestory);
		if(rv!=CKR_OK)
		{
			printf("test_FindCertObjectNumberfailed, pC_DestroyCertObject. rv=0x%08x.\n",rv);
			nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"test_FindCertObjectNumberfailed, pC_DestroyCertObject. rv=0x%08x.<br>",rv);	
			bRtn =false;
			goto END;
		}
	}
#endif

	free(hObject);
	printf("leave xtest_FindObjectNumber.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "leave xtest_FindObjectNumber.<br>");

	bRtn =true;
END:
	
	return bRtn;
}

bool xtest_FindDataObjectAndDestory()
{
	CK_RV rv=0;
	bool bRtn = false;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_OBJECT_HANDLE_PTR hObject = NULL_PTR;
	CK_OBJECT_HANDLE hObjectDestory = NULL_PTR;
	CK_ULONG ulObjectCount = 0;
	int i;

	//有效证书的value
	CK_OBJECT_CLASS certclass=CKO_DATA;
	CK_ATTRIBUTE certTemplate[] = {
		{CKA_CLASS, &certclass, sizeof(CK_OBJECT_CLASS)}
	};

	printf("enter xtest_FindDataObjectAndDestory.\n");
	nResultLength += sprintf(Testresult[nItemNumb]+nResultLength , "enter xtest_FindDataObjectAndDestory.<br>");
	//查找私钥对象	
	hObject = NULL_PTR;
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16); 
	ulObjectCount = 0;
	rv = (*pC_FindObjectsInit)(hSession, certTemplate, sizeof(certTemplate)/sizeof(CK_ATTRIBUTE));
	if(rv!=CKR_OK)
	{
		printf("xtest_FindDataObjectAndDestory failed,pC_FindObjectsInit,1. rv=0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"xtest_FindDataObjectAndDestory failed,pC_FindObjectsInit,1. rv=0x%08x.<br>",rv);
		bRtn =false;
		goto END;
	}

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	if(rv!=CKR_OK)
	{
		printf("xtest_FindDataObjectAndDestory failed,pC_FindprvObjects,1. rv=0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"xtest_FindDataObjectAndDestory failed,pC_FindprvObjects,1. rv=0x%08x.<br>",rv);
		bRtn =false;
		goto END;
	}
	printf("Cert count=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Cert count=%d.<br>", ulObjectCount);

	rv = (*pC_FindObjectsFinal)(hSession);
	if(rv!=CKR_OK)
	{
		printf("xtest_FindDataObjectAndDestory failed,pC_FindpObjectsFinal,1. rv=0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"xtest_FindDataObjectAndDestory failed,pC_FindpObjectsFinal,1. rv=0x%08x.<br>",rv);
		bRtn =false;
		goto END;
	}

#ifdef DESTORYUSELESSKEY
	hObjectDestory = NULL_PTR;
	for(i=0;i<ulObjectCount;i++)
	{
		hObjectDestory=hObject[i];
		printf("%*c\r",79,0x20);
		printf("Cert i=%d,CertObject=0x%08x.\r",i,hObjectDestory);
		fflush(stdout);
		//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Cert i=%d,CertObject=0x%08x.\r",i,hObjectDestory);
		
		rv = (*pC_DestroyObject)(hSession, hObjectDestory);
		if(rv!=CKR_OK)
		{
			printf("xtest_FindDataObjectAndDestory, pC_DestroyCertObject. rv=0x%08x.\n",rv);
			nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"xtest_FindDataObjectAndDestory, pC_DestroyCertObject. rv=0x%08x.<br>",rv);	
			bRtn =false;
			goto END;
		}
	}
#endif

	free(hObject);
	printf("leave xtest_FindDataObjectAndDestory.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "leave xtest_FindDataObjectAndDestory.<br>");

	bRtn =true;
END:
	
	return bRtn;
}
bool xtest_Flash_Normal()
{
	bool	bRtn = false;

	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_RV	rv = 0;
	CK_OBJECT_HANDLE hBaseKey= NULL_PTR, hPublicKey = NULL_PTR, hPrivateKey = NULL_PTR, hCertificate = NULL_PTR, hSYSKey0 = NULL_PTR,hSYSKey1 = NULL_PTR,hSYSKey2 = NULL_PTR,hSYSKey3 = NULL_PTR,hSYSKey4 = NULL_PTR,hSYSKey5 = NULL_PTR,hSYSKey6 = NULL_PTR,hSYSKey7 = NULL_PTR,hSYSKey8 = NULL_PTR,hSYSKey9 = NULL_PTR,hSYSKey10 = NULL_PTR,hSYSKey11 = NULL_PTR,hSYSKey12 = NULL_PTR,hSYSKey13 = NULL_PTR,hSYSKey14 = NULL_PTR,hSYSKey15 = NULL_PTR,hSYSKey16 = NULL_PTR;
	CK_OBJECT_HANDLE hSYSKey[17] = { hSYSKey0, hSYSKey1, hSYSKey2,hSYSKey3,hSYSKey4,hSYSKey5,hSYSKey6,hSYSKey7,hSYSKey8,hSYSKey9,hSYSKey10,hSYSKey11,hSYSKey12,hSYSKey13,hSYSKey14,hSYSKey15,hSYSKey16};
	int i = 0, iMaxCount = 16;//iMaxCount = 1;

	CK_BYTE     prv[32]={
		0x65, 0x83, 0x2D, 0x7C, 0x7B, 0x27, 0x2E, 0x98, 0x57, 0xD1, 0x08, 0x4F, 0x3A, 0x75, 0x54, 0x0D, 
		0x40, 0xC2, 0x04, 0x23, 0x3A, 0x6A, 0x65, 0x6A, 0x15, 0x9D, 0x59, 0x52, 0x2D, 0x68, 0x4E, 0x3E
	};
	CK_BYTE     pub_x[32]={
		0xD6, 0xA7, 0x78, 0x8C, 0x41, 0xC3, 0xA3, 0x66, 0x18, 0xE8, 0x02, 0x1E, 0xE1, 0xEB, 0x27, 0x2F, 
		0xA7, 0x24, 0x7F, 0x77, 0xF4, 0x7F, 0x40, 0xBE, 0x6A, 0x31, 0x02, 0x48, 0x41, 0xA0, 0x14, 0xC6
	};
	CK_BYTE     pub_y[32]={
		0x39, 0xE2, 0xF0, 0xA4, 0x69, 0x22, 0xD3, 0x56, 0xBD, 0xB0, 0x39, 0x12, 0x26, 0xA9, 0xD1, 0x47, 
		0x40, 0x84, 0x3D, 0xFE, 0x57, 0x68, 0x0D, 0x28, 0x87, 0x44, 0xB7, 0xD8, 0x20, 0xE0, 0x32, 0xBB
	};

	CK_BYTE     temp_prv[32] = {
		0x3D, 0x73, 0x7C, 0x01, 0x52, 0x36, 0x79, 0xB0, 0x39, 0x95, 0x60, 0xD1, 0x27, 0x4E, 0x77, 0x7C, 
		0x59, 0xD0, 0x40, 0xBF, 0x28, 0x38, 0x0E, 0x33, 0x1F, 0xFA, 0x66, 0x79, 0x66, 0xF2, 0x02, 0x89};
	CK_BYTE     temp_pub_x[32] = {
		0x38, 0xCD, 0xAD, 0xDD, 0xDA, 0x1C, 0x6C, 0x51, 0x35, 0x23, 0xCC, 0xB3, 0x2E, 0x0E, 0x28, 0x01, 
		0x26, 0x2B, 0x7F, 0x95, 0xF2, 0x5E, 0x85, 0x3C, 0x86, 0xE1, 0x01, 0x28, 0x6C, 0x82, 0xD7, 0x18};
	CK_BYTE     temp_pub_y[32] = {
		0x6A, 0x1C, 0x8A, 0x61, 0xC4, 0xD3, 0xED, 0x61, 0x9D, 0xFA, 0xA0, 0x33, 0x20, 0x5C, 0x5F, 0x76, 
		0x9B, 0x3B, 0xE1, 0x2C, 0x8F, 0x15, 0x7C, 0x2F, 0x55, 0x75, 0xFF, 0xEE, 0xFB, 0x42, 0xA0, 0x64};

	CK_KEY_TYPE  keyType=CKK_ECC;
	CK_OBJECT_CLASS pubclass=CKO_PUBLIC_KEY,priclass=CKO_PRIVATE_KEY;

	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;

	CK_UINT     modulusBits = 256;

	CK_BYTE      id[] = {0x01,0x01,0x02,0x03};

	CK_MECHANISM SM4mechanismkeygen = {
		CKM_SM4_KEY_GEN , NULL_PTR, 0
	};

	CK_ATTRIBUTE publicKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, pub_x, sizeof(temp_pub_x)},
		{CKA_ECC_Y_COORDINATE, pub_y, sizeof(temp_pub_y)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_PRIVATE, prv, sizeof(temp_prv)},
		{CKA_ID, id, sizeof(id)}
	};

	//有效证书的value
	CK_BYTE cert[]={0x30,0x82,0x05,0x6D,0x30,0x82,0x04,0x55,0xA0,0x03,0x02,0x01,0x02,0x02,0x0A,0x22,
		0x09,0x63,0xB1,0x00,0x00,0x00,0x00,0x64,0x45,0x30,0x0D,0x06,0x09,0x2A,0x86,0x48,
		0x86,0xF7,0x0D,0x01,0x01,0x05,0x05,0x00,0x30,0x3E,0x31,0x13,0x30,0x11,0x06,0x0A,
		0x09,0x92,0x26,0x89,0x93,0xF2,0x2C,0x64,0x01,0x19,0x16,0x03,0x63,0x6F,0x6D,0x31,
		0x18,0x30,0x16,0x06,0x0A,0x09,0x92,0x26,0x89,0x93,0xF2,0x2C,0x64,0x01,0x19,0x16,
		0x08,0x4D,0x69,0x63,0x72,0x6F,0x53,0x65,0x63,0x31,0x0D,0x30,0x0B,0x06,0x03,0x55,
		0x04,0x03,0x13,0x04,0x42,0x4A,0x49,0x53,0x30,0x1E,0x17,0x0D,0x31,0x33,0x30,0x36,
		0x30,0x34,0x30,0x35,0x34,0x38,0x32,0x33,0x5A,0x17,0x0D,0x31,0x34,0x30,0x36,0x30,
		0x34,0x30,0x35,0x34,0x38,0x32,0x33,0x5A,0x30,0x58,0x31,0x13,0x30,0x11,0x06,0x0A,
		0x09,0x92,0x26,0x89,0x93,0xF2,0x2C,0x64,0x01,0x19,0x16,0x03,0x63,0x6F,0x6D,0x31,
		0x18,0x30,0x16,0x06,0x0A,0x09,0x92,0x26,0x89,0x93,0xF2,0x2C,0x64,0x01,0x19,0x16,
		0x08,0x4D,0x69,0x63,0x72,0x6F,0x53,0x65,0x63,0x31,0x18,0x30,0x16,0x06,0x03,0x55,
		0x04,0x0B,0x0C,0x0F,0x44,0x65,0x70,0x61,0x72,0x74,0x5F,0x69,0x6E,0x66,0x6F,0x74,
		0x65,0x63,0x68,0x31,0x0D,0x30,0x0B,0x06,0x03,0x55,0x04,0x03,0x13,0x04,0x74,0x65,
		0x73,0x74,0x30,0x81,0x9F,0x30,0x0D,0x06,0x09,0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,
		0x01,0x01,0x05,0x00,0x03,0x81,0x8D,0x00,0x30,0x81,0x89,0x02,0x81,0x81,0x00,0xBC,
		0x13,0x01,0xAE,0xF2,0x5F,0x39,0x5A,0xF2,0xB2,0x95,0xC3,0xD1,0x04,0x64,0x60,0x09,
		0xAA,0xAB,0x8B,0xA2,0x8F,0x00,0x4A,0x94,0x71,0x93,0xD9,0xD9,0xCA,0x39,0xAA,0xE9,
		0x6E,0xED,0xAE,0xA9,0x87,0x29,0x22,0xB3,0x52,0x40,0x2B,0xDE,0xC3,0x6F,0x0D,0x8B,
		0xDD,0x74,0x89,0xE0,0x82,0xD1,0xDB,0x7F,0x8E,0xD0,0x8E,0x66,0x5A,0x2C,0x80,0x2C,
		0xCF,0xEB,0x31,0x77,0x54,0xC8,0x16,0xBA,0xE9,0x76,0x56,0x89,0x52,0xA1,0x09,0xA3,
		0xBA,0xC7,0x0E,0x3F,0xF9,0xBE,0x7C,0x1A,0xA7,0x3B,0xC0,0xFF,0x60,0x40,0x1A,0xCE,
		0xA0,0x1A,0xF1,0xD6,0x33,0xF2,0xBD,0xB3,0x0F,0x30,0x39,0x1D,0x25,0xC2,0xDF,0xA5,
		0x58,0xE7,0xD4,0x69,0x79,0xAC,0x3B,0x0D,0xDF,0x1C,0xAD,0xC1,0x9E,0xA3,0x6D,0x02,
		0x03,0x01,0x00,0x01,0xA3,0x82,0x02,0xD5,0x30,0x82,0x02,0xD1,0x30,0x0B,0x06,0x03,
		0x55,0x1D,0x0F,0x04,0x04,0x03,0x02,0x05,0xA0,0x30,0x1D,0x06,0x03,0x55,0x1D,0x0E,
		0x04,0x16,0x04,0x14,0x39,0x6B,0x69,0x14,0x6F,0x73,0x4B,0x5B,0xC9,0xDE,0x86,0x9D,
		0xC1,0x25,0xF8,0x12,0x5B,0x4A,0x34,0x8F,0x30,0x2B,0x06,0x09,0x2B,0x06,0x01,0x04,
		0x01,0x82,0x37,0x14,0x02,0x04,0x1E,0x1E,0x1C,0x00,0x53,0x00,0x6D,0x00,0x61,0x00,
		0x72,0x00,0x74,0x00,0x63,0x00,0x61,0x00,0x72,0x00,0x64,0x00,0x4C,0x00,0x6F,0x00,
		0x67,0x00,0x6F,0x00,0x6E,0x30,0x1F,0x06,0x03,0x55,0x1D,0x23,0x04,0x18,0x30,0x16,
		0x80,0x14,0xC8,0x7A,0xC5,0x17,0x08,0x7F,0x56,0x4D,0x5A,0x45,0xBE,0x6B,0x71,0x09,
		0xD0,0x05,0xDE,0x6F,0x5B,0x04,0x30,0x81,0xF6,0x06,0x03,0x55,0x1D,0x1F,0x04,0x81,
		0xEE,0x30,0x81,0xEB,0x30,0x81,0xE8,0xA0,0x81,0xE5,0xA0,0x81,0xE2,0x86,0x81,0xAD,
		0x6C,0x64,0x61,0x70,0x3A,0x2F,0x2F,0x2F,0x43,0x4E,0x3D,0x42,0x4A,0x49,0x53,0x2C,
		0x43,0x4E,0x3D,0x53,0x65,0x72,0x76,0x65,0x72,0x30,0x32,0x2C,0x43,0x4E,0x3D,0x43,
		0x44,0x50,0x2C,0x43,0x4E,0x3D,0x50,0x75,0x62,0x6C,0x69,0x63,0x25,0x32,0x30,0x4B,
		0x65,0x79,0x25,0x32,0x30,0x53,0x65,0x72,0x76,0x69,0x63,0x65,0x73,0x2C,0x43,0x4E,
		0x3D,0x53,0x65,0x72,0x76,0x69,0x63,0x65,0x73,0x2C,0x43,0x4E,0x3D,0x43,0x6F,0x6E,
		0x66,0x69,0x67,0x75,0x72,0x61,0x74,0x69,0x6F,0x6E,0x2C,0x44,0x43,0x3D,0x4D,0x69,
		0x63,0x72,0x6F,0x53,0x65,0x63,0x2C,0x44,0x43,0x3D,0x63,0x6F,0x6D,0x3F,0x63,0x65,
		0x72,0x74,0x69,0x66,0x69,0x63,0x61,0x74,0x65,0x52,0x65,0x76,0x6F,0x63,0x61,0x74,
		0x69,0x6F,0x6E,0x4C,0x69,0x73,0x74,0x3F,0x62,0x61,0x73,0x65,0x3F,0x6F,0x62,0x6A,
		0x65,0x63,0x74,0x43,0x6C,0x61,0x73,0x73,0x3D,0x63,0x52,0x4C,0x44,0x69,0x73,0x74,
		0x72,0x69,0x62,0x75,0x74,0x69,0x6F,0x6E,0x50,0x6F,0x69,0x6E,0x74,0x86,0x30,0x68,
		0x74,0x74,0x70,0x3A,0x2F,0x2F,0x73,0x65,0x72,0x76,0x65,0x72,0x30,0x32,0x2E,0x6D,
		0x69,0x63,0x72,0x6F,0x73,0x65,0x63,0x2E,0x63,0x6F,0x6D,0x2F,0x43,0x65,0x72,0x74,
		0x45,0x6E,0x72,0x6F,0x6C,0x6C,0x2F,0x42,0x4A,0x49,0x53,0x2E,0x63,0x72,0x6C,0x30,
		0x82,0x01,0x0B,0x06,0x08,0x2B,0x06,0x01,0x05,0x05,0x07,0x01,0x01,0x04,0x81,0xFE,
		0x30,0x81,0xFB,0x30,0x81,0xA4,0x06,0x08,0x2B,0x06,0x01,0x05,0x05,0x07,0x30,0x02,
		0x86,0x81,0x97,0x6C,0x64,0x61,0x70,0x3A,0x2F,0x2F,0x2F,0x43,0x4E,0x3D,0x42,0x4A,
		0x49,0x53,0x2C,0x43,0x4E,0x3D,0x41,0x49,0x41,0x2C,0x43,0x4E,0x3D,0x50,0x75,0x62,
		0x6C,0x69,0x63,0x25,0x32,0x30,0x4B,0x65,0x79,0x25,0x32,0x30,0x53,0x65,0x72,0x76,
		0x69,0x63,0x65,0x73,0x2C,0x43,0x4E,0x3D,0x53,0x65,0x72,0x76,0x69,0x63,0x65,0x73,
		0x2C,0x43,0x4E,0x3D,0x43,0x6F,0x6E,0x66,0x69,0x67,0x75,0x72,0x61,0x74,0x69,0x6F,
		0x6E,0x2C,0x44,0x43,0x3D,0x4D,0x69,0x63,0x72,0x6F,0x53,0x65,0x63,0x2C,0x44,0x43,
		0x3D,0x63,0x6F,0x6D,0x3F,0x63,0x41,0x43,0x65,0x72,0x74,0x69,0x66,0x69,0x63,0x61,
		0x74,0x65,0x3F,0x62,0x61,0x73,0x65,0x3F,0x6F,0x62,0x6A,0x65,0x63,0x74,0x43,0x6C,
		0x61,0x73,0x73,0x3D,0x63,0x65,0x72,0x74,0x69,0x66,0x69,0x63,0x61,0x74,0x69,0x6F,
		0x6E,0x41,0x75,0x74,0x68,0x6F,0x72,0x69,0x74,0x79,0x30,0x52,0x06,0x08,0x2B,0x06,
		0x01,0x05,0x05,0x07,0x30,0x02,0x86,0x46,0x68,0x74,0x74,0x70,0x3A,0x2F,0x2F,0x73,
		0x65,0x72,0x76,0x65,0x72,0x30,0x32,0x2E,0x6D,0x69,0x63,0x72,0x6F,0x73,0x65,0x63,
		0x2E,0x63,0x6F,0x6D,0x2F,0x43,0x65,0x72,0x74,0x45,0x6E,0x72,0x6F,0x6C,0x6C,0x2F,
		0x53,0x65,0x72,0x76,0x65,0x72,0x30,0x32,0x2E,0x4D,0x69,0x63,0x72,0x6F,0x53,0x65,
		0x63,0x2E,0x63,0x6F,0x6D,0x5F,0x42,0x4A,0x49,0x53,0x2E,0x63,0x72,0x74,0x30,0x1F,
		0x06,0x03,0x55,0x1D,0x25,0x04,0x18,0x30,0x16,0x06,0x08,0x2B,0x06,0x01,0x05,0x05,
		0x07,0x03,0x02,0x06,0x0A,0x2B,0x06,0x01,0x04,0x01,0x82,0x37,0x14,0x02,0x02,0x30,
		0x2C,0x06,0x03,0x55,0x1D,0x11,0x04,0x25,0x30,0x23,0xA0,0x21,0x06,0x0A,0x2B,0x06,
		0x01,0x04,0x01,0x82,0x37,0x14,0x02,0x03,0xA0,0x13,0x0C,0x11,0x74,0x65,0x73,0x74,
		0x40,0x4D,0x69,0x63,0x72,0x6F,0x53,0x65,0x63,0x2E,0x63,0x6F,0x6D,0x30,0x0D,0x06,
		0x09,0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x01,0x05,0x05,0x00,0x03,0x82,0x01,0x01,
		0x00,0x79,0x84,0x2F,0x6E,0x39,0x9C,0x0A,0xC8,0xDE,0x9E,0x2B,0xEA,0x5D,0xD4,0x88,
		0x04,0x98,0xFA,0x60,0x90,0x9E,0xEA,0xBC,0xBE,0x95,0xAB,0x3E,0xA8,0x52,0xF6,0x96,
		0x65,0x7B,0x1A,0x5A,0x46,0x0D,0x3A,0x12,0xE1,0x4F,0x8F,0xF8,0xDE,0xB6,0x5F,0x88,
		0xBF,0xDB,0xEF,0x08,0x11,0x16,0x9E,0xCA,0x35,0x4A,0xE6,0x76,0xA9,0x87,0xD5,0x5B,
		0x3D,0x7D,0x8F,0x9B,0x14,0xA4,0x69,0x6F,0x1F,0xEC,0xB7,0x36,0x8D,0xD2,0x04,0xF4,
		0x0D,0x70,0x4E,0xE8,0xF0,0xF2,0xDA,0x4E,0x8C,0x8F,0x12,0xF5,0x29,0x4A,0xC1,0x25,
		0x26,0x3E,0x7A,0x26,0x5E,0x78,0x6B,0x5D,0xED,0x14,0x57,0xDA,0x01,0xC7,0x36,0xCD,
		0xD1,0xA9,0xC9,0x53,0x4F,0xB2,0xDD,0xB1,0xB0,0x11,0x05,0x57,0xFA,0x75,0x7C,0x24,
		0x81,0xD1,0xEF,0xE2,0x1D,0x5B,0x59,0x9B,0x4C,0x14,0x6C,0x4E,0x6D,0xEF,0xDB,0x20,
		0x75,0x7A,0x96,0x81,0x46,0xA6,0x13,0xC5,0xD5,0x69,0x45,0x32,0xE7,0x3F,0x24,0x48,
		0x6A,0xCF,0xA5,0x5A,0x20,0x8C,0x43,0xF3,0xDB,0x66,0x6E,0x45,0x2B,0x5D,0x3A,0x5C,
		0x4E,0xA5,0x5F,0x09,0x70,0xBF,0xF5,0x6A,0x3A,0xA9,0xCA,0x4C,0x2D,0x93,0x3C,0x6C,
		0xB3,0xFA,0xA4,0x2F,0x8F,0x75,0x97,0xD2,0x85,0xF4,0x51,0x18,0x38,0x48,0xE7,0xEA,
		0x89,0x66,0x55,0x26,0x28,0x30,0x7F,0xA4,0x3F,0xE9,0xC7,0x99,0xAA,0x41,0x2B,0xEC,
		0x2B,0x17,0x3B,0x33,0x1E,0xCA,0xFD,0xF5,0x19,0xD4,0xF3,0xBD,0x7F,0xEE,0x65,0x03,
		0xB7,0xDD,0xBC,0x95,0x2D,0xDA,0x58,0x95,0x99,0x25,0x84,0xEC,0xC5,0xE0,0xE6,0x29,
		0x7E};
	CK_OBJECT_CLASS certclass=CKO_CERTIFICATE;
	CK_CERTIFICATE_TYPE certtype=CKC_X_509;
	CK_BYTE subject[] = {'s','u','b','j','e','c','t'};
	CK_BYTE issuer[] = {'i','s','s','u','e','r'};
	CK_BYTE SN[] = {'1','2','3','4','5','6'};
	CK_ATTRIBUTE certTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &certclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_CERTIFICATE_TYPE, &certtype, sizeof(CK_CERTIFICATE_TYPE)},
		{CKA_SUBJECT, subject, sizeof(subject)},
		{CKA_ISSUER,  issuer, sizeof(issuer)},
		{CKA_SERIAL_NUMBER, SN, sizeof(SN)},
		{CKA_VALUE, cert, sizeof(cert)}
	};

		//for set sm3 kdf basekey
	unsigned char	ucSM3KDFBaseKeyBuffer[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	CK_EXTEND_IN	ExtIn_SetSM3KDFBaseKey = {CK_EXTEND_SETSM3KDFBASEKEY, ucSM3KDFBaseKeyBuffer, sizeof(ucSM3KDFBaseKeyBuffer)};
	CK_EXTEND_OUT	ExtOut_SetSM3KDFBaseKey = {CK_EXTEND_SETSM3KDFBASEKEY, NULL, 0};

	//for calc sm3 kdf
	unsigned char	ucSM3KDFKeyBuffer1[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	CK_MECHANISM	sm3KDFPRESETMech = {CKM_DERIVE_SM3_KDF_WITH_PRESET, ucSM3KDFKeyBuffer1, sizeof(ucSM3KDFKeyBuffer1)};
	//CK_BYTE	nBaseKeyID = CK_SESSKEY_ID0;
	//CK_BYTE	nSessKeyID1 = CK_SESSKEY_ID0 + 1;
	CK_BYTE	nBaseKeyID[17] = {CK_SESSKEY_ID0, CK_SESSKEY_ID1, CK_SESSKEY_ID2,CK_SESSKEY_ID3,CK_SESSKEY_ID4,CK_SESSKEY_ID4,CK_SESSKEY_ID6,CK_SESSKEY_ID7,CK_SESSKEY_ID8, CK_SESSKEY_ID9,CK_SESSKEY_ID10,CK_SESSKEY_ID11,CK_SESSKEY_ID12,CK_SESSKEY_ID13,CK_SESSKEY_ID14,CK_SESSKEY_ID15,CK_SESSKEY_ID0};
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_RV rtx = FALSE;

	printf("enter xtest_Flash_Normal.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "enter xtest_Flash_Normal.<br>");

	rtx = xtest_FindKeyObjectAndDestory();
	if(rtx==FALSE)
	{
		return false;
	}

	rtx = xtest_FindCertObjectAndDestory();
	if(rtx==FALSE)
	{
		return false;
	}

	for (i=0; i<iMaxCount+1; i++)
	{
		//创建公钥对象
		rv = (*pC_CreateObject)(hSession, publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE), &hPublicKey);
		if(rv!=CKR_OK)
		{
			printf("pC_CreateObject%d, public key, rv=0x%08x.\n",i, rv);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_flash_object failed,pC_CreateObject%d, public key, rv=0x%08x.<br>",i, rv);
			//goto END;
		}
		else
		{
			printf("%*c\r",79,0x20);
			printf("pC_CreateObject, public key %d ok.\r", i);
			fflush(stdout);
			//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "pC_CreateObject, public key %d ok.<br>", i);
		}

		//创建私钥对象
		rv = (*pC_CreateObject)(hSession, privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE), &hPrivateKey);
		if(rv!=CKR_OK)
		{
			printf("pC_CreateObject%d, private key, rv=0x%08x.\n",i, rv);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_flash_object failed,pC_CreateObject%d, private key, rv=0x%08x.<br>",i, rv);
			//goto END;
		}else
		{
			printf("%*c\r",79,0x20);
			printf("C_CreateObject, private key object %d ok.\r", i);
			fflush(stdout);
			//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"C_CreateObject, private key object %d ok.<br>", i);
		}

		//创建证书对象
		rv = (*pC_CreateObject)(hSession, certTemplate, sizeof(certTemplate)/sizeof(CK_ATTRIBUTE), &hCertificate);
		if(rv!=CKR_OK)
		{
			printf("pC_CreateObject%d, cert, rv=0x%08x.\n",i, rv);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_flash_object failed,pC_CreateObject%d, cert, rv=0x%08x.<br>",i, rv);
			//goto END;
		}
		else
		{
			printf("%*c\r",79,0x20);
			printf("C_CreateObject, cert %d ok.\r", i);
			fflush(stdout);
			//nResultLength +=sprintf(Testresult[nItemNumb] + nResultLength,"C_CreateObject, certificate object %d ok.<br>", i);
		}
	}
	bRtn = true;
END:
	rtx == xtest_FindKeyObjectAndDestory();
	if(rtx==FALSE)
	{
		return false;
	}
	rtx == xtest_FindCertObjectAndDestory();
	if(rtx==FALSE)
	{
		return false;
	}

	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "leaving xtest_Flash_Normal.<br>");
	printf("leaving xtest_Flash_Normal.\n");
	
	return bRtn;
}


bool xtest_FindSecretKEY()
{
	CK_RV rv=0;
	bool bRtn=false;
	CK_OBJECT_CLASS secretclass = CKO_SECRET_KEY;	
	CK_ATTRIBUTE secFindKeyTemplate[] = {
		{CKA_CLASS, &secretclass, sizeof(CK_OBJECT_CLASS)}
	};

	CK_OBJECT_HANDLE_PTR hObject = NULL_PTR;
	CK_OBJECT_HANDLE hObjectDestory = NULL_PTR;
	CK_ULONG ulObjectCount = 0;
	int i;
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xtest_FindSecretKEY);

	//查找加密密钥对象
	hObject= (CK_OBJECT_HANDLE_PTR)malloc(sizeof(int)*16);
	ulObjectCount = 16;
	rv = (*pC_FindObjectsInit)(hSession, secFindKeyTemplate, sizeof(secFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsInit3,rv);

	rv = (*pC_FindObjects)(hSession, hObject, 16, &ulObjectCount);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjects3,rv);
	printf("Secret Key count=%d.\n", ulObjectCount);
	nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Secret Key count=%d.<br>", ulObjectCount);


	rv = (*pC_FindObjectsFinal)(hSession);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_FindObjectsFinal3,rv);

	free(hObject);

	bRtn=true;
END:
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xtest_FindSecretKEY);

	
	return bRtn;
}

bool test_SM4RAMValue(CK_MECHANISM_TYPE mAlgType, CK_BYTE ucEncID, CK_BYTE ucDecID)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;

	//加密参数
	unsigned char	SM4keyVal_Enc[]={0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
	unsigned char	SM4iv_Enc[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	unsigned char	SM4plain_Enc[192]={0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	unsigned char	SM4cipher_Enc_OFB[192]={0x73, 0xa3, 0x37, 0x80, 0x40, 0xad, 0x2f, 0x7c, 0x91, 0x81, 0x8e, 0xcd, 0x49, 0x6a, 0xe2, 0x62, 0xb8, 0x83, 0xc1, 0x38, 0x12, 0xfa, 0x3d, 0xb4, 0xfc, 0x2a, 0xf4, 0x97, 0x2b, 0xa9, 0xaf, 0xae, 0xcc, 0x8d, 0x58, 0x49, 0x07, 0x67, 0xd3, 0x76, 0xab, 0xb8, 0x1e, 0xe6, 0x8d, 0x19, 0xfa, 0xfb, 0x18, 0x3d, 0x10, 0xa9, 0x2f, 0xbb, 0xf1, 0x21, 0xa4, 0xd7, 0x2d, 0xb4, 0x1b, 0xf2, 0x42, 0x9e, 0x4b, 0x44, 0xfd, 0x08, 0x89, 0x20, 0x78, 0xf8, 0xd5, 0x7d, 0x48, 0xd1, 0x4e, 0x0a, 0x39, 0xa3, 0x88, 0xec, 0xfa, 0x04, 0x84, 0xa6, 0x24, 0x88, 0xd5, 0x91, 0xea, 0x27, 0xaa, 0x99, 0x9f, 0x29, 0xe4, 0xf0, 0x12, 0xde, 0x35, 0x07, 0x5f, 0xe2, 0x34, 0x96, 0xfb, 0x61, 0xc1, 0xff, 0xa2, 0xc7, 0x00, 0x4a, 0xd1, 0xca, 0x3b, 0xc2, 0xdb, 0x49, 0xc7, 0xd5, 0x7a, 0x04, 0x82, 0x9d, 0xfa, 0xff, 0xd2, 0xd8, 0x6c, 0x77, 0x4f, 0xa8, 0x44, 0x47, 0xdd, 0x84, 0xd4, 0xf1, 0x8e, 0xc6, 0x36, 0xfc, 0xa4, 0xd8, 0x1a, 0xa5, 0x38, 0x30, 0xc3, 0xf6, 0xde, 0xe8, 0x69, 0xb5, 0x37, 0x1b, 0x47, 0x26, 0x41, 0xf7, 0x9f, 0xac, 0x29, 0x69, 0x2e, 0xba, 0xbd, 0x55, 0x8d, 0x28, 0xa6, 0x03, 0x0e, 0xaf, 0xeb, 0x6b, 0xe9, 0xb3, 0x75, 0xe0, 0x81, 0x76, 0xc9, 0x60, 0xaa, 0x8c, 0xab, 0x70, 0x2f, 0x42};
	unsigned char	SM4cipher_Enc_ECB[192]={0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91};
	unsigned char	SM4cipher_Enc_CBC[192]={0x94, 0x0f, 0x58, 0xdf, 0xb5, 0x3e, 0x53, 0x48, 0x70, 0x14, 0xf6, 0x4d, 0x95, 0x9e, 0x12, 0x2e, 0x24, 0xd8, 0x02, 0xa7, 0x69, 0x09, 0x2f, 0xcb, 0xcd, 0xa7, 0xc0, 0x8b, 0xe3, 0x2c, 0xe8, 0x99, 0x94, 0xb3, 0x56, 0xe2, 0x90, 0x75, 0xc9, 0x82, 0x13, 0x53, 0x02, 0xc8, 0xf3, 0xe6, 0xc5, 0x7d, 0xed, 0x17, 0x16, 0x50, 0xed, 0x45, 0x2e, 0xa3, 0xaf, 0x2d, 0xce, 0xf2, 0x85, 0x42, 0x45, 0x17, 0x6c, 0xe7, 0x2e, 0x78, 0x3b, 0xfd, 0x9a, 0x8a, 0x9e, 0x6a, 0x7c, 0xa6, 0xad, 0xfa, 0x7d, 0xec, 0xde, 0xd6, 0x87, 0x3e, 0x45, 0xcd, 0x9a, 0xe9, 0x7f, 0xf5, 0x4a, 0x71, 0xe4, 0x04, 0x2b, 0x14, 0xca, 0xca, 0x43, 0x3a, 0x5a, 0x9d, 0xf3, 0x22, 0xee, 0x78, 0x7d, 0x27, 0xd1, 0x26, 0x15, 0x35, 0x5b, 0xf6, 0x78, 0x08, 0x67, 0xe8, 0xe2, 0xfd, 0xd8, 0x85, 0xbb, 0x2b, 0x41, 0xe7, 0xd3, 0xf7, 0xcb, 0x7c, 0xb3, 0x6c, 0x92, 0xdf, 0x9a, 0x07, 0x09, 0x81, 0x97, 0xec, 0x36, 0x93, 0xab, 0x96, 0xad, 0xb7, 0x61, 0x89, 0xa9, 0xa5, 0x20, 0x82, 0x5f, 0xba, 0x5f, 0xbc, 0x73, 0xdb, 0xba, 0x43, 0xc4, 0x46, 0x6f, 0xbd, 0x1e, 0x71, 0xfd, 0xb1, 0xd3, 0x80, 0xbe, 0x2d, 0xb1, 0x76, 0xbd, 0xb9, 0x3f, 0x5c, 0x58, 0x2a, 0x6d, 0x9a, 0x94, 0xca, 0x7f, 0x92, 0x8f, 0x56, 0x30, 0x9f, 0x06, 0x43};
	unsigned char	*SM4cipher_Enc = NULL;
	CK_BYTE nSessKeyID_Enc = ucEncID;
	CK_ATTRIBUTE SM4keyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal_Enc,sizeof(SM4keyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Enc = {mAlgType, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	//解密参数
	CK_BYTE nSessKeyID_Dec = ucDecID;
	CK_ATTRIBUTE SM4keyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal_Enc,sizeof(SM4keyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Dec = {mAlgType, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	int loopTime = 1;
	unsigned int i = 0;
	char  fname[4] = {0x00};
	char* fNameECB = "ECB";
	char* fNameCBC = "CBC";
	char* fNameOFB = "OFB";

	if (mAlgType == CKM_SM4_ECB)
	{
		SM4cipher_Enc = SM4cipher_Enc_ECB;
		SM4mechanism_Enc.pParameter = NULL;
		SM4mechanism_Enc.ulParameterLen = 0;
		SM4mechanism_Dec.pParameter = NULL;
		SM4mechanism_Dec.ulParameterLen = 0;
		strcpy(fname, fNameECB);

	}
	else if (mAlgType == CKM_SM4_CBC)
	{
		SM4cipher_Enc = SM4cipher_Enc_CBC;
		strcpy(fname, fNameCBC);
	}
	else if (mAlgType == CKM_SM4_OFB)
	{
		SM4cipher_Enc = SM4cipher_Enc_OFB;
		strcpy(fname, fNameOFB);
	}

	printf("enter %s:%s.\n",__FUNCTION__,fname);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "enter %s:%s.<br>",__FUNCTION__,fname);

	//对称加密初始化
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Enc, sizeof(SM4keyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Dec, sizeof(SM4keyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	rv = (*pC_DecryptInit)(hSession, &SM4mechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	for (i=0; i<loopTime; i++)
	{
		indatalen = sizeof(indata);
		memset(indata, 0, indatalen);
		outdatalen=sizeof(outdata);
		memset(outdata, 0, outdatalen);

		indatalen1 = sizeof(indata1);
		memset(indata1, 0, indatalen1);
		outdatalen1=sizeof(outdata1);
		memset(outdata1, 0, outdatalen1);

		//加密的明文
		memcpy(indata, SM4plain_Enc, sizeof(SM4plain_Enc));
		indatalen = sizeof(SM4plain_Enc);
		
		/*******************加密过程**********************/
		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);


		if ((/*(outdatalen != sizeof(SM4cipher_Enc)) || */memcmp(outdata, SM4cipher_Enc, outdatalen)) && (i == 0))
		{
			printf("test_SM4RAMnoValue failed, memcmp(outdata, SM4cipher_Enc, outdatalen).\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"SM4 Encrypt Calc Error: test_SM4Encrypt.<br>"); 
			bRtn = false;
			goto END;
		}
		/******************解密过程***********************/
		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);

		if ((outdatalen1 != sizeof(SM4plain_Enc)) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4RAMnoValue.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Calc Error: test_SM4RAMnoValue.<br>"); 
		}
	}

	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Enc,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Dec,rv);

	printf("leave %s:%s.\n",__FUNCTION__,fname);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "leave %s:%s.<br>",__FUNCTION__,fname);
	bRtn = true;
END:	
	return bRtn;
}

/*
使用P11接口测试交替加解密的速度，预建好的RAM密钥
*/
bool test_SM4RAMnoValue(CK_MECHANISM_TYPE mAlgType,CK_BYTE ucEncID,CK_BYTE ucDecID)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;

	//加密参数
	unsigned char	SM4keyVal_Enc[]={0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
	unsigned char	SM4iv_Enc[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	unsigned char	SM4plain_Enc[192]={0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	unsigned char	SM4cipher_Enc_OFB[192]={0x73, 0xa3, 0x37, 0x80, 0x40, 0xad, 0x2f, 0x7c, 0x91, 0x81, 0x8e, 0xcd, 0x49, 0x6a, 0xe2, 0x62, 0xb8, 0x83, 0xc1, 0x38, 0x12, 0xfa, 0x3d, 0xb4, 0xfc, 0x2a, 0xf4, 0x97, 0x2b, 0xa9, 0xaf, 0xae, 0xcc, 0x8d, 0x58, 0x49, 0x07, 0x67, 0xd3, 0x76, 0xab, 0xb8, 0x1e, 0xe6, 0x8d, 0x19, 0xfa, 0xfb, 0x18, 0x3d, 0x10, 0xa9, 0x2f, 0xbb, 0xf1, 0x21, 0xa4, 0xd7, 0x2d, 0xb4, 0x1b, 0xf2, 0x42, 0x9e, 0x4b, 0x44, 0xfd, 0x08, 0x89, 0x20, 0x78, 0xf8, 0xd5, 0x7d, 0x48, 0xd1, 0x4e, 0x0a, 0x39, 0xa3, 0x88, 0xec, 0xfa, 0x04, 0x84, 0xa6, 0x24, 0x88, 0xd5, 0x91, 0xea, 0x27, 0xaa, 0x99, 0x9f, 0x29, 0xe4, 0xf0, 0x12, 0xde, 0x35, 0x07, 0x5f, 0xe2, 0x34, 0x96, 0xfb, 0x61, 0xc1, 0xff, 0xa2, 0xc7, 0x00, 0x4a, 0xd1, 0xca, 0x3b, 0xc2, 0xdb, 0x49, 0xc7, 0xd5, 0x7a, 0x04, 0x82, 0x9d, 0xfa, 0xff, 0xd2, 0xd8, 0x6c, 0x77, 0x4f, 0xa8, 0x44, 0x47, 0xdd, 0x84, 0xd4, 0xf1, 0x8e, 0xc6, 0x36, 0xfc, 0xa4, 0xd8, 0x1a, 0xa5, 0x38, 0x30, 0xc3, 0xf6, 0xde, 0xe8, 0x69, 0xb5, 0x37, 0x1b, 0x47, 0x26, 0x41, 0xf7, 0x9f, 0xac, 0x29, 0x69, 0x2e, 0xba, 0xbd, 0x55, 0x8d, 0x28, 0xa6, 0x03, 0x0e, 0xaf, 0xeb, 0x6b, 0xe9, 0xb3, 0x75, 0xe0, 0x81, 0x76, 0xc9, 0x60, 0xaa, 0x8c, 0xab, 0x70, 0x2f, 0x42};
	unsigned char	SM4cipher_Enc_ECB[192]={0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91};
	unsigned char	SM4cipher_Enc_CBC[192]={0x94, 0x0f, 0x58, 0xdf, 0xb5, 0x3e, 0x53, 0x48, 0x70, 0x14, 0xf6, 0x4d, 0x95, 0x9e, 0x12, 0x2e, 0x24, 0xd8, 0x02, 0xa7, 0x69, 0x09, 0x2f, 0xcb, 0xcd, 0xa7, 0xc0, 0x8b, 0xe3, 0x2c, 0xe8, 0x99, 0x94, 0xb3, 0x56, 0xe2, 0x90, 0x75, 0xc9, 0x82, 0x13, 0x53, 0x02, 0xc8, 0xf3, 0xe6, 0xc5, 0x7d, 0xed, 0x17, 0x16, 0x50, 0xed, 0x45, 0x2e, 0xa3, 0xaf, 0x2d, 0xce, 0xf2, 0x85, 0x42, 0x45, 0x17, 0x6c, 0xe7, 0x2e, 0x78, 0x3b, 0xfd, 0x9a, 0x8a, 0x9e, 0x6a, 0x7c, 0xa6, 0xad, 0xfa, 0x7d, 0xec, 0xde, 0xd6, 0x87, 0x3e, 0x45, 0xcd, 0x9a, 0xe9, 0x7f, 0xf5, 0x4a, 0x71, 0xe4, 0x04, 0x2b, 0x14, 0xca, 0xca, 0x43, 0x3a, 0x5a, 0x9d, 0xf3, 0x22, 0xee, 0x78, 0x7d, 0x27, 0xd1, 0x26, 0x15, 0x35, 0x5b, 0xf6, 0x78, 0x08, 0x67, 0xe8, 0xe2, 0xfd, 0xd8, 0x85, 0xbb, 0x2b, 0x41, 0xe7, 0xd3, 0xf7, 0xcb, 0x7c, 0xb3, 0x6c, 0x92, 0xdf, 0x9a, 0x07, 0x09, 0x81, 0x97, 0xec, 0x36, 0x93, 0xab, 0x96, 0xad, 0xb7, 0x61, 0x89, 0xa9, 0xa5, 0x20, 0x82, 0x5f, 0xba, 0x5f, 0xbc, 0x73, 0xdb, 0xba, 0x43, 0xc4, 0x46, 0x6f, 0xbd, 0x1e, 0x71, 0xfd, 0xb1, 0xd3, 0x80, 0xbe, 0x2d, 0xb1, 0x76, 0xbd, 0xb9, 0x3f, 0x5c, 0x58, 0x2a, 0x6d, 0x9a, 0x94, 0xca, 0x7f, 0x92, 0x8f, 0x56, 0x30, 0x9f, 0x06, 0x43};
	unsigned char	*SM4cipher_Enc = NULL;
	CK_BYTE nSessKeyID_Enc = ucEncID;
	CK_ATTRIBUTE SM4keyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Enc = {mAlgType, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	//解密参数
	CK_BYTE nSessKeyID_Dec = ucDecID;
	CK_ATTRIBUTE SM4keyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Dec = {mAlgType, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	int loopTime = 1;
	unsigned int i = 0;
	char  fname[4] = {0x00};
	char* fNameECB = "ECB";
	char* fNameCBC = "CBC";
	char* fNameOFB = "OFB";

	if (mAlgType == CKM_SM4_ECB)
	{
		SM4cipher_Enc = SM4cipher_Enc_ECB;
		SM4mechanism_Enc.pParameter = NULL;
		SM4mechanism_Enc.ulParameterLen = 0;
		SM4mechanism_Dec.pParameter = NULL;
		SM4mechanism_Dec.ulParameterLen = 0;
		strcpy(fname, fNameECB);
	}
	else if (mAlgType == CKM_SM4_CBC)
	{
		SM4cipher_Enc = SM4cipher_Enc_CBC;
		strcpy(fname, fNameCBC);
	}
	else if (mAlgType == CKM_SM4_OFB)
	{
		SM4cipher_Enc = SM4cipher_Enc_OFB;
		strcpy(fname, fNameOFB);
	}
	printf("enter %s:%s.\n",__FUNCTION__,fname);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "enter %s:%s.<br>",__FUNCTION__,fname);

	//对称加密初始化
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Enc, sizeof(SM4keyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Dec, sizeof(SM4keyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	rv = (*pC_DecryptInit)(hSession, &SM4mechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	for (i=0; i<loopTime; i++)
	{
		indatalen = sizeof(indata);
		memset(indata, 0, indatalen);
		outdatalen=sizeof(outdata);
		memset(outdata, 0, outdatalen);

		indatalen1 = sizeof(indata1);
		memset(indata1, 0, indatalen1);
		outdatalen1=sizeof(outdata1);
		memset(outdata1, 0, outdatalen1);

		//加密的明文
		memcpy(indata, SM4plain_Enc, sizeof(SM4plain_Enc));
		indatalen = sizeof(SM4plain_Enc);
		
		

		/*******************加密过程**********************/
		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);


		if ((/*(outdatalen != sizeof(SM4cipher_Enc)) || */memcmp(outdata, SM4cipher_Enc, outdatalen)) && (i == 0))
		{
			printf("test_SM4RAMnoValue failed, memcmp(outdata, SM4cipher_Enc, outdatalen).\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"SM4 Encrypt Calc Error: test_SM4Encrypt.<br>"); 
			bRtn = false;
			goto END;
		}

		/******************解密过程***********************/
		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);


		if ((outdatalen1 != sizeof(SM4plain_Enc)) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4Encrypt.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Calc Error: test_SM4Encrypt.<br>"); 
		}
	}

	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Enc,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Dec,rv);

	printf("leave %s:%s.\n",__FUNCTION__,fname);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "leave %s:%s.<br>",__FUNCTION__,fname);
	bRtn = true;
END:

	
	return bRtn;
}

/*
使用P11接口测试交替加解密的速度，FLASH密钥
*/
bool test_SM4FLASHValue(CK_MECHANISM_TYPE mAlgType, CK_BYTE ucEncID, CK_BYTE ucDecID )
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;

	//加密参数
	unsigned char	SM4keyVal_Enc[]={0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
	unsigned char	SM4iv_Enc[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	unsigned char	SM4plain_Enc[192]={0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	unsigned char	SM4cipher_Enc_OFB[192]={0x73, 0xa3, 0x37, 0x80, 0x40, 0xad, 0x2f, 0x7c, 0x91, 0x81, 0x8e, 0xcd, 0x49, 0x6a, 0xe2, 0x62, 0xb8, 0x83, 0xc1, 0x38, 0x12, 0xfa, 0x3d, 0xb4, 0xfc, 0x2a, 0xf4, 0x97, 0x2b, 0xa9, 0xaf, 0xae, 0xcc, 0x8d, 0x58, 0x49, 0x07, 0x67, 0xd3, 0x76, 0xab, 0xb8, 0x1e, 0xe6, 0x8d, 0x19, 0xfa, 0xfb, 0x18, 0x3d, 0x10, 0xa9, 0x2f, 0xbb, 0xf1, 0x21, 0xa4, 0xd7, 0x2d, 0xb4, 0x1b, 0xf2, 0x42, 0x9e, 0x4b, 0x44, 0xfd, 0x08, 0x89, 0x20, 0x78, 0xf8, 0xd5, 0x7d, 0x48, 0xd1, 0x4e, 0x0a, 0x39, 0xa3, 0x88, 0xec, 0xfa, 0x04, 0x84, 0xa6, 0x24, 0x88, 0xd5, 0x91, 0xea, 0x27, 0xaa, 0x99, 0x9f, 0x29, 0xe4, 0xf0, 0x12, 0xde, 0x35, 0x07, 0x5f, 0xe2, 0x34, 0x96, 0xfb, 0x61, 0xc1, 0xff, 0xa2, 0xc7, 0x00, 0x4a, 0xd1, 0xca, 0x3b, 0xc2, 0xdb, 0x49, 0xc7, 0xd5, 0x7a, 0x04, 0x82, 0x9d, 0xfa, 0xff, 0xd2, 0xd8, 0x6c, 0x77, 0x4f, 0xa8, 0x44, 0x47, 0xdd, 0x84, 0xd4, 0xf1, 0x8e, 0xc6, 0x36, 0xfc, 0xa4, 0xd8, 0x1a, 0xa5, 0x38, 0x30, 0xc3, 0xf6, 0xde, 0xe8, 0x69, 0xb5, 0x37, 0x1b, 0x47, 0x26, 0x41, 0xf7, 0x9f, 0xac, 0x29, 0x69, 0x2e, 0xba, 0xbd, 0x55, 0x8d, 0x28, 0xa6, 0x03, 0x0e, 0xaf, 0xeb, 0x6b, 0xe9, 0xb3, 0x75, 0xe0, 0x81, 0x76, 0xc9, 0x60, 0xaa, 0x8c, 0xab, 0x70, 0x2f, 0x42};
	unsigned char	SM4cipher_Enc_ECB[192]={0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91};
	unsigned char	SM4cipher_Enc_CBC[192]={0x94, 0x0f, 0x58, 0xdf, 0xb5, 0x3e, 0x53, 0x48, 0x70, 0x14, 0xf6, 0x4d, 0x95, 0x9e, 0x12, 0x2e, 0x24, 0xd8, 0x02, 0xa7, 0x69, 0x09, 0x2f, 0xcb, 0xcd, 0xa7, 0xc0, 0x8b, 0xe3, 0x2c, 0xe8, 0x99, 0x94, 0xb3, 0x56, 0xe2, 0x90, 0x75, 0xc9, 0x82, 0x13, 0x53, 0x02, 0xc8, 0xf3, 0xe6, 0xc5, 0x7d, 0xed, 0x17, 0x16, 0x50, 0xed, 0x45, 0x2e, 0xa3, 0xaf, 0x2d, 0xce, 0xf2, 0x85, 0x42, 0x45, 0x17, 0x6c, 0xe7, 0x2e, 0x78, 0x3b, 0xfd, 0x9a, 0x8a, 0x9e, 0x6a, 0x7c, 0xa6, 0xad, 0xfa, 0x7d, 0xec, 0xde, 0xd6, 0x87, 0x3e, 0x45, 0xcd, 0x9a, 0xe9, 0x7f, 0xf5, 0x4a, 0x71, 0xe4, 0x04, 0x2b, 0x14, 0xca, 0xca, 0x43, 0x3a, 0x5a, 0x9d, 0xf3, 0x22, 0xee, 0x78, 0x7d, 0x27, 0xd1, 0x26, 0x15, 0x35, 0x5b, 0xf6, 0x78, 0x08, 0x67, 0xe8, 0xe2, 0xfd, 0xd8, 0x85, 0xbb, 0x2b, 0x41, 0xe7, 0xd3, 0xf7, 0xcb, 0x7c, 0xb3, 0x6c, 0x92, 0xdf, 0x9a, 0x07, 0x09, 0x81, 0x97, 0xec, 0x36, 0x93, 0xab, 0x96, 0xad, 0xb7, 0x61, 0x89, 0xa9, 0xa5, 0x20, 0x82, 0x5f, 0xba, 0x5f, 0xbc, 0x73, 0xdb, 0xba, 0x43, 0xc4, 0x46, 0x6f, 0xbd, 0x1e, 0x71, 0xfd, 0xb1, 0xd3, 0x80, 0xbe, 0x2d, 0xb1, 0x76, 0xbd, 0xb9, 0x3f, 0x5c, 0x58, 0x2a, 0x6d, 0x9a, 0x94, 0xca, 0x7f, 0x92, 0x8f, 0x56, 0x30, 0x9f, 0x06, 0x43};
	unsigned char	*SM4cipher_Enc = NULL;
	CK_BYTE nSessKeyID_Enc = ucEncID;
	CK_ATTRIBUTE SM4keyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal_Enc,sizeof(SM4keyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Enc = {mAlgType, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	//解密参数
	CK_BYTE nSessKeyID_Dec = ucDecID;
	CK_ATTRIBUTE SM4keyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal_Enc,sizeof(SM4keyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Dec = {mAlgType, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	int loopTime = 1;
	unsigned int i = 0;
	char  fname[4] = {0x00};
	char* fNameECB = "ECB";
	char* fNameCBC = "CBC";
	char* fNameOFB = "OFB";
	
	if (mAlgType == CKM_SM4_ECB)
	{
		SM4cipher_Enc = SM4cipher_Enc_ECB;
		SM4mechanism_Enc.pParameter = NULL;
		SM4mechanism_Enc.ulParameterLen = 0;
		SM4mechanism_Dec.pParameter = NULL;
		SM4mechanism_Dec.ulParameterLen = 0;
		strcpy(fname, fNameECB);
	}
	else if (mAlgType == CKM_SM4_CBC)
	{
		SM4cipher_Enc = SM4cipher_Enc_CBC;
		strcpy(fname, fNameCBC);
	}
	else if (mAlgType == CKM_SM4_OFB)
	{
		SM4cipher_Enc = SM4cipher_Enc_OFB;
		strcpy(fname, fNameOFB);
	}

	printf("enter %s:%s.\n",__FUNCTION__,fname);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "enter %s:%s.<br>",__FUNCTION__,fname);

#ifdef OPEN_LOGOUT_CLOSESSION	
	if(nSessKeyID_Enc == CK_SESSKEY_PRESET_ID7)
	{	
		rv = user_to_so();
		if(rv == FALSE)		
		{
			return false;
		}		
	}
#endif

	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Enc, sizeof(SM4keyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Dec, sizeof(SM4keyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

#ifdef OPEN_LOGOUT_CLOSESSION
	if(nSessKeyID_Enc == CK_SESSKEY_PRESET_ID7)
	{
		rv = so_to_user();
		if(rv == FALSE)
		{
			return false;
		}
	}
#endif

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	rv = (*pC_DecryptInit)(hSession, &SM4mechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	for (i=0; i<loopTime; i++)
	{
		indatalen = sizeof(indata);
		memset(indata, 0, indatalen);
		outdatalen=sizeof(outdata);
		memset(outdata, 0, outdatalen);

		indatalen1 = sizeof(indata1);
		memset(indata1, 0, indatalen1);
		outdatalen1=sizeof(outdata1);
		memset(outdata1, 0, outdatalen1);

		//加密的明文
		memcpy(indata, SM4plain_Enc, sizeof(SM4plain_Enc));
		indatalen = sizeof(SM4plain_Enc);
		
		/*******************加密过程**********************/
		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);


		if ((/*(outdatalen != sizeof(SM4cipher_Enc)) || */memcmp(outdata, SM4cipher_Enc, outdatalen)) && (i == 0))
		{
			printf("test_SM4RAMnoValue failed, memcmp(outdata, SM4cipher_Enc, outdatalen).\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"SM4 Encrypt Calc Error: test_SM4Encrypt.<br>"); 
			bRtn = false;
			goto END;
		}

		/******************解密过程***********************/
		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);


		if ((outdatalen1 != sizeof(SM4plain_Enc)) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4Encrypt.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Calc Error: test_SM4Encrypt.<br>"); 
		}
	}

	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Enc,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Dec,rv);

	printf("leave %s:%s.\n",__FUNCTION__,fname);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "leave %s:%s.<br>",__FUNCTION__,fname);
	bRtn = true;
END:

	
	return bRtn;
}

/*
使用P11接口测试交替加解密的速度，预建好的FLASH密钥
*/
bool test_SM4FLASHnoValue(CK_MECHANISM_TYPE mAlgType, CK_BYTE ucEncID, CK_BYTE ucDecID)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;

	//加密参数
	unsigned char	SM4keyVal_Enc[]={0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
	unsigned char	SM4iv_Enc[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	unsigned char	SM4plain_Enc[192]={0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	unsigned char	SM4cipher_Enc_OFB[192]={0x73, 0xa3, 0x37, 0x80, 0x40, 0xad, 0x2f, 0x7c, 0x91, 0x81, 0x8e, 0xcd, 0x49, 0x6a, 0xe2, 0x62, 0xb8, 0x83, 0xc1, 0x38, 0x12, 0xfa, 0x3d, 0xb4, 0xfc, 0x2a, 0xf4, 0x97, 0x2b, 0xa9, 0xaf, 0xae, 0xcc, 0x8d, 0x58, 0x49, 0x07, 0x67, 0xd3, 0x76, 0xab, 0xb8, 0x1e, 0xe6, 0x8d, 0x19, 0xfa, 0xfb, 0x18, 0x3d, 0x10, 0xa9, 0x2f, 0xbb, 0xf1, 0x21, 0xa4, 0xd7, 0x2d, 0xb4, 0x1b, 0xf2, 0x42, 0x9e, 0x4b, 0x44, 0xfd, 0x08, 0x89, 0x20, 0x78, 0xf8, 0xd5, 0x7d, 0x48, 0xd1, 0x4e, 0x0a, 0x39, 0xa3, 0x88, 0xec, 0xfa, 0x04, 0x84, 0xa6, 0x24, 0x88, 0xd5, 0x91, 0xea, 0x27, 0xaa, 0x99, 0x9f, 0x29, 0xe4, 0xf0, 0x12, 0xde, 0x35, 0x07, 0x5f, 0xe2, 0x34, 0x96, 0xfb, 0x61, 0xc1, 0xff, 0xa2, 0xc7, 0x00, 0x4a, 0xd1, 0xca, 0x3b, 0xc2, 0xdb, 0x49, 0xc7, 0xd5, 0x7a, 0x04, 0x82, 0x9d, 0xfa, 0xff, 0xd2, 0xd8, 0x6c, 0x77, 0x4f, 0xa8, 0x44, 0x47, 0xdd, 0x84, 0xd4, 0xf1, 0x8e, 0xc6, 0x36, 0xfc, 0xa4, 0xd8, 0x1a, 0xa5, 0x38, 0x30, 0xc3, 0xf6, 0xde, 0xe8, 0x69, 0xb5, 0x37, 0x1b, 0x47, 0x26, 0x41, 0xf7, 0x9f, 0xac, 0x29, 0x69, 0x2e, 0xba, 0xbd, 0x55, 0x8d, 0x28, 0xa6, 0x03, 0x0e, 0xaf, 0xeb, 0x6b, 0xe9, 0xb3, 0x75, 0xe0, 0x81, 0x76, 0xc9, 0x60, 0xaa, 0x8c, 0xab, 0x70, 0x2f, 0x42};
	unsigned char	SM4cipher_Enc_ECB[192]={0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91, 0x68, 0x6D, 0xB7, 0x66, 0xC9, 0x20, 0x48, 0x22, 0xBB, 0xFA, 0x6B, 0x84, 0x68, 0xC2, 0x86, 0x91};
	unsigned char	SM4cipher_Enc_CBC[192]={0x94, 0x0f, 0x58, 0xdf, 0xb5, 0x3e, 0x53, 0x48, 0x70, 0x14, 0xf6, 0x4d, 0x95, 0x9e, 0x12, 0x2e, 0x24, 0xd8, 0x02, 0xa7, 0x69, 0x09, 0x2f, 0xcb, 0xcd, 0xa7, 0xc0, 0x8b, 0xe3, 0x2c, 0xe8, 0x99, 0x94, 0xb3, 0x56, 0xe2, 0x90, 0x75, 0xc9, 0x82, 0x13, 0x53, 0x02, 0xc8, 0xf3, 0xe6, 0xc5, 0x7d, 0xed, 0x17, 0x16, 0x50, 0xed, 0x45, 0x2e, 0xa3, 0xaf, 0x2d, 0xce, 0xf2, 0x85, 0x42, 0x45, 0x17, 0x6c, 0xe7, 0x2e, 0x78, 0x3b, 0xfd, 0x9a, 0x8a, 0x9e, 0x6a, 0x7c, 0xa6, 0xad, 0xfa, 0x7d, 0xec, 0xde, 0xd6, 0x87, 0x3e, 0x45, 0xcd, 0x9a, 0xe9, 0x7f, 0xf5, 0x4a, 0x71, 0xe4, 0x04, 0x2b, 0x14, 0xca, 0xca, 0x43, 0x3a, 0x5a, 0x9d, 0xf3, 0x22, 0xee, 0x78, 0x7d, 0x27, 0xd1, 0x26, 0x15, 0x35, 0x5b, 0xf6, 0x78, 0x08, 0x67, 0xe8, 0xe2, 0xfd, 0xd8, 0x85, 0xbb, 0x2b, 0x41, 0xe7, 0xd3, 0xf7, 0xcb, 0x7c, 0xb3, 0x6c, 0x92, 0xdf, 0x9a, 0x07, 0x09, 0x81, 0x97, 0xec, 0x36, 0x93, 0xab, 0x96, 0xad, 0xb7, 0x61, 0x89, 0xa9, 0xa5, 0x20, 0x82, 0x5f, 0xba, 0x5f, 0xbc, 0x73, 0xdb, 0xba, 0x43, 0xc4, 0x46, 0x6f, 0xbd, 0x1e, 0x71, 0xfd, 0xb1, 0xd3, 0x80, 0xbe, 0x2d, 0xb1, 0x76, 0xbd, 0xb9, 0x3f, 0x5c, 0x58, 0x2a, 0x6d, 0x9a, 0x94, 0xca, 0x7f, 0x92, 0x8f, 0x56, 0x30, 0x9f, 0x06, 0x43};
	unsigned char	*SM4cipher_Enc = NULL;
	CK_BYTE nSessKeyID_Enc = ucEncID;
	CK_ATTRIBUTE SM4keyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Enc = {mAlgType, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	//解密参数
	CK_BYTE nSessKeyID_Dec = ucDecID;
	CK_ATTRIBUTE SM4keyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Dec = {mAlgType, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	int loopTime = 1;
	unsigned int i = 0;
	char  fname[4] = {0x00};
	char* fNameECB = "ECB";
	char* fNameCBC = "CBC";
	char* fNameOFB = "OFB";

	if (mAlgType == CKM_SM4_ECB)
	{
		SM4cipher_Enc = SM4cipher_Enc_ECB;
		SM4mechanism_Enc.pParameter = NULL;
		SM4mechanism_Enc.ulParameterLen = 0;
		SM4mechanism_Dec.pParameter = NULL;
		SM4mechanism_Dec.ulParameterLen = 0;
		strcpy(fname, fNameECB);
	}
	else if (mAlgType == CKM_SM4_CBC)
	{
		SM4cipher_Enc = SM4cipher_Enc_CBC;
		strcpy(fname, fNameCBC);
	}
	else if (mAlgType == CKM_SM4_OFB)
	{
		SM4cipher_Enc = SM4cipher_Enc_OFB;
		strcpy(fname, fNameOFB);
	}

	printf("enter %s:%s.\n",__FUNCTION__,fname);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "enter %s:%s.<br>",__FUNCTION__,fname);

	//对称加密初始化
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Enc, sizeof(SM4keyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_Dec, sizeof(SM4keyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

	rv = (*pC_DecryptInit)(hSession, &SM4mechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	for (i=0; i<loopTime; i++)
	{
		indatalen = sizeof(indata);
		memset(indata, 0, indatalen);
		outdatalen=sizeof(outdata);
		memset(outdata, 0, outdatalen);

		indatalen1 = sizeof(indata1);
		memset(indata1, 0, indatalen1);
		outdatalen1=sizeof(outdata1);
		memset(outdata1, 0, outdatalen1);

		//加密的明文
		memcpy(indata, SM4plain_Enc, sizeof(SM4plain_Enc));
		indatalen = sizeof(SM4plain_Enc);
		
		

		/*******************加密过程**********************/
		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);


		if ((/*(outdatalen != sizeof(SM4cipher_Enc)) || */memcmp(outdata, SM4cipher_Enc, outdatalen)) && (i == 0))
		{
			printf("test_SM4RAMnoValue failed, memcmp(outdata, SM4cipher_Enc, outdatalen).\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"SM4 Encrypt Calc Error: test_SM4Encrypt.<br>"); 
			bRtn = false;
			goto END;
		}

		/******************解密过程***********************/
		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);


		if ((outdatalen1 != sizeof(SM4plain_Enc)) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4Encrypt.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Calc Error: test_SM4Encrypt.<br>"); 
		}
	}

	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Enc,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Dec,rv);

	printf("leave %s:%s.\n",__FUNCTION__,fname);
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "leave %s:%s.<br>",__FUNCTION__,fname);

	bRtn = true;
END:
	
	return bRtn;
}

bool xtest_SM4_KEY(CK_MECHANISM_TYPE mAlgType)
{
	bool xnRtn = false;
	CK_RV rv = -1;
	
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xtest_SM4_KEY);

	xnRtn = test_SM4RAMValue(mAlgType, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1);//RAM密钥
	if(xnRtn == false)
	{
		return false;
	}

	xnRtn = test_SM4FLASHValue(mAlgType, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1);//FLASH密钥
	if(xnRtn == false)
	{
		return false;
	}

	xnRtn = test_SM4FLASHnoValue(mAlgType, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1);//FLASH预设密钥
	if(xnRtn == false)
	{
		return false;
	}


#ifdef OPEN_LOGOUT_CLOSESSION
	//RAM ID7 equals to ID0-6
	//xnRtn = test_SM4RAMnoValue(mAlgType, CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//RAM密钥
	//if(xnRtn = false)
	//{
	//	return false;
	//}
	//FLASH ID7 can not be used as en/decrypt key
	//xnRtn = test_SM4FLASHValue(mAlgType, CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH密钥
	//if(xnRtn = false)
	//{
	//	return false;
	//}

	//xnRtn = test_SM4FLASHnoValue(mAlgType, CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH预设密钥
	//if(xnRtn = false)
	//{
	//	return false;
	//}

#endif

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xtest_SM4_KEY);
	
	return true;

}

//p11接口测试ZUC算法性能
bool test_ZUCRAM(CK_BYTE ucEncID, CK_BYTE ucDecID)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;

	//加密参数
	///////////////////////////
	unsigned char	ZUCkeyVal_Enc[]={0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};

	unsigned char	ZUCplain_Enc[136] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, \
		0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03};
	unsigned char	ZUCcipher_Enc[136] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, 0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01, \
		0x9b, 0x48, 0xac, 0xd1, 0xfe, 0xf3, 0x01, 0x1f, 0x5e, 0x22, 0xd2, 0x97, 0x75, 0xcc, 0x94, 0xc2, 0xca, 0x6f, 0xb0, 0x9c, 0x2d, 0x86, 0xd6, 0xd0, 0x18, 0x3c, 0x24, 0x34, 0x09, 0x73, 0x5b, 0xad, 0x57, 0x5e, 0x9f, 0x91};

	unsigned char	ZUCiv_Enc[] = {0x00, 0x05, 0x68, 0x23, 0x38};

	CK_BYTE nSessKeyID_Enc = ucEncID;

	CK_ATTRIBUTE ZUCkeyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,ZUCkeyVal_Enc,sizeof(ZUCkeyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)}
	};
	CK_MECHANISM ZUCmechanism_Enc = {CKM_ZUC_CALC, ZUCiv_Enc, sizeof(ZUCiv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	//解密参数
	//////////////////////////////////////
	unsigned char	ZUCkeyVal_Dec[]={0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};

	unsigned char	ZUCplain_Dec[136] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, \
		0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03};
	unsigned char	ZUCcipher_Dec[136] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, 0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01, \
		0x9b, 0x48, 0xac, 0xd1, 0xfe, 0xf3, 0x01, 0x1f, 0x5e, 0x22, 0xd2, 0x97, 0x75, 0xcc, 0x94, 0xc2, 0xca, 0x6f, 0xb0, 0x9c, 0x2d, 0x86, 0xd6, 0xd0, 0x18, 0x3c, 0x24, 0x34, 0x09, 0x73, 0x5b, 0xad, 0x57, 0x5e, 0x9f, 0x91};

	unsigned char	ZUCiv_Dec[]={0x00, 0x05, 0x68, 0x23, 0x38};

	CK_BYTE nSessKeyID_Dec = ucDecID;

	CK_ATTRIBUTE ZUCkeyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,ZUCkeyVal_Dec,sizeof(ZUCkeyVal_Dec)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(CK_BYTE)}
	};
	CK_MECHANISM ZUCmechanism_Dec = {CKM_ZUC_CALC, ZUCiv_Dec, sizeof(ZUCiv_Dec)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	int loopTime = 1;
	unsigned int i = 0;
	
	printf("enter test_ZUCRAM.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "enter test_ZUCRAM.<br>");

	//对称加密初始化
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Enc, sizeof(ZUCkeyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject_hKey_Enc,rv);

	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Dec, sizeof(ZUCkeyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject_hKey_Dec,rv);

	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	for (i=0; i<loopTime; i++)
	{

		indatalen = sizeof(indata);
		memset(indata, 0, indatalen);
		outdatalen=sizeof(outdata);
		memset(outdata, 0, outdatalen);

		indatalen1 = sizeof(indata1);
		memset(indata1, 0, indatalen1);
		outdatalen1=sizeof(outdata1);
		memset(outdata1, 0, outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv_Enc, sizeof(ZUCiv_Enc));
		memcpy(indata+sizeof(ZUCiv_Enc), ZUCplain_Enc, sizeof(ZUCplain_Enc));
		indatalen = sizeof(ZUCiv_Enc) + sizeof(ZUCplain_Enc);

		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv_Dec, sizeof(ZUCiv_Dec));
		memcpy(indata1+sizeof(ZUCiv_Dec), ZUCcipher_Dec, sizeof(ZUCcipher_Dec));
		indatalen1 = sizeof(ZUCiv_Dec) + sizeof(ZUCcipher_Dec);

		/*******************加密过程**********************/
		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);

		/******************解密过程***********************/
		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);

		if ((outdatalen != sizeof(ZUCcipher_Enc)) || (memcmp(outdata, ZUCcipher_Enc, outdatalen)))
		{
			printf("Error: ZUC Encrypt.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"CAL Error: ZUC Encrypt.<br>"); 
		}

		if ((outdatalen1 != sizeof(ZUCplain_Dec)) || (memcmp(outdata1, ZUCplain_Dec, outdatalen1)))
		{
			printf("Error: ZUC Decrypt.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"CAL Error: ZUC Decrypt.<br>"); 
		}
	}

	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Enc,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Dec,rv);

	printf("leave test_ZUCRAM.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "leave test_ZUCRAM.<br>");

	bRtn = true;
END:

	
	return bRtn;
}

//p11接口测试ZUC算法性能，FLASH密钥
bool test_ZUCFLASHValue(CK_BYTE ucEncID, CK_BYTE ucDecID)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	//加密参数
	///////////////////////////
	unsigned char	ZUCkeyVal_Enc[]={0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};
	unsigned char	ZUCplain_Enc[136] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, \
		0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03};
	unsigned char	ZUCcipher_Enc[136] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, 0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01, \
		0x9b, 0x48, 0xac, 0xd1, 0xfe, 0xf3, 0x01, 0x1f, 0x5e, 0x22, 0xd2, 0x97, 0x75, 0xcc, 0x94, 0xc2, 0xca, 0x6f, 0xb0, 0x9c, 0x2d, 0x86, 0xd6, 0xd0, 0x18, 0x3c, 0x24, 0x34, 0x09, 0x73, 0x5b, 0xad, 0x57, 0x5e, 0x9f, 0x91};

	unsigned char	ZUCiv_Enc[] = {0x00, 0x05, 0x68, 0x23, 0x38};

	CK_BYTE nSessKeyID_Enc = ucEncID;

	CK_ATTRIBUTE ZUCkeyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,ZUCkeyVal_Enc,sizeof(ZUCkeyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)}
	};
	CK_MECHANISM ZUCmechanism_Enc = {CKM_ZUC_CALC, ZUCiv_Enc, sizeof(ZUCiv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	//解密参数
	//////////////////////////////////////
	unsigned char	ZUCkeyVal_Dec[]={0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};
	unsigned char	ZUCplain_Dec[136] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, \
		0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03};
	unsigned char	ZUCcipher_Dec[136] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, 0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01, \
		0x9b, 0x48, 0xac, 0xd1, 0xfe, 0xf3, 0x01, 0x1f, 0x5e, 0x22, 0xd2, 0x97, 0x75, 0xcc, 0x94, 0xc2, 0xca, 0x6f, 0xb0, 0x9c, 0x2d, 0x86, 0xd6, 0xd0, 0x18, 0x3c, 0x24, 0x34, 0x09, 0x73, 0x5b, 0xad, 0x57, 0x5e, 0x9f, 0x91};

	unsigned char	ZUCiv_Dec[]={0x00, 0x05, 0x68, 0x23, 0x38};

	CK_BYTE nSessKeyID_Dec = ucDecID;

	CK_ATTRIBUTE ZUCkeyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,ZUCkeyVal_Dec,sizeof(ZUCkeyVal_Dec)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(CK_BYTE)}
	};
	CK_MECHANISM ZUCmechanism_Dec = {CKM_ZUC_CALC, ZUCiv_Dec, sizeof(ZUCiv_Dec)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	int loopTime = 1;
	unsigned int i = 0;

	printf("enter test_ZUCFLASHValue.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "enter test_ZUCFLASHValue.<br>");

#ifdef OPEN_LOGOUT_CLOSESSION	
	if(nSessKeyID_Enc == CK_SESSKEY_PRESET_ID7)
	{	
		rv = user_to_so();
		if(rv == FALSE)		
		{
			return false;
		}		
	}
#endif

	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Enc, sizeof(ZUCkeyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject_hKey_Enc,rv);

	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Dec, sizeof(ZUCkeyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject_hKey_Dec,rv);


#ifdef OPEN_LOGOUT_CLOSESSION	
		if(nSessKeyID_Enc == CK_SESSKEY_PRESET_ID7)
		{	
			rv = so_to_user();
			if(rv == FALSE) 	
			{
				return false;
			}		
		}
#endif


	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	for (i=0; i<loopTime; i++)
	{

		indatalen = sizeof(indata);
		memset(indata, 0, indatalen);
		outdatalen=sizeof(outdata);
		memset(outdata, 0, outdatalen);

		indatalen1 = sizeof(indata1);
		memset(indata1, 0, indatalen1);
		outdatalen1=sizeof(outdata1);
		memset(outdata1, 0, outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv_Enc, sizeof(ZUCiv_Enc));
		memcpy(indata+sizeof(ZUCiv_Enc), ZUCplain_Enc, sizeof(ZUCplain_Enc));
		indatalen = sizeof(ZUCiv_Enc) + sizeof(ZUCplain_Enc);

		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv_Dec, sizeof(ZUCiv_Dec));
		memcpy(indata1+sizeof(ZUCiv_Dec), ZUCcipher_Dec, sizeof(ZUCcipher_Dec));
		indatalen1 = sizeof(ZUCiv_Dec) + sizeof(ZUCcipher_Dec);

		/*******************加密过程**********************/

		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);



		/******************解密过程***********************/
		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);

		if ((outdatalen != sizeof(ZUCcipher_Enc)) || (memcmp(outdata, ZUCcipher_Enc, outdatalen)))
		{
			printf("Error: ZUC Encrypt.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"CAL Error: ZUC Encrypt.<br>"); 
		}

		if ((outdatalen1 != sizeof(ZUCplain_Dec)) || (memcmp(outdata1, ZUCplain_Dec, outdatalen1)))
		{
			printf("Error: ZUC Decrypt.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"CAL Error: ZUC Decrypt.<br>"); 
		}

	}

	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Enc,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Dec,rv);

	printf("leave test_ZUCFLASHValue.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "leave test_ZUCFLASHValue.<br>");

	bRtn = true;
END:

	
	return bRtn;
}


//p11接口测试ZUC算法性能，FLASH密钥，使用预设密钥
bool test_ZUCFLASHnoValue(CK_BYTE ucEncID, CK_BYTE ucDecID)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;

	//加密参数
	///////////////////////////
	unsigned char	ZUCkeyVal_Enc[]={0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};

	unsigned char	ZUCplain_Enc[136] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, \
		0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03};
	unsigned char	ZUCcipher_Enc[136] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, 0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01, \
		0x9b, 0x48, 0xac, 0xd1, 0xfe, 0xf3, 0x01, 0x1f, 0x5e, 0x22, 0xd2, 0x97, 0x75, 0xcc, 0x94, 0xc2, 0xca, 0x6f, 0xb0, 0x9c, 0x2d, 0x86, 0xd6, 0xd0, 0x18, 0x3c, 0x24, 0x34, 0x09, 0x73, 0x5b, 0xad, 0x57, 0x5e, 0x9f, 0x91};

	unsigned char	ZUCiv_Enc[] = {0x00, 0x05, 0x68, 0x23, 0x38};

	CK_BYTE nSessKeyID_Enc = ucEncID;

	CK_ATTRIBUTE ZUCkeyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(CK_BYTE)}
	};
	CK_MECHANISM ZUCmechanism_Enc = {CKM_ZUC_CALC, ZUCiv_Enc, sizeof(ZUCiv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	//解密参数
	//////////////////////////////////////
	unsigned char	ZUCkeyVal_Dec[]={0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};

	unsigned char	ZUCplain_Dec[136] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, \
		0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03};
	unsigned char	ZUCcipher_Dec[136] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, 0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01, \
		0x9b, 0x48, 0xac, 0xd1, 0xfe, 0xf3, 0x01, 0x1f, 0x5e, 0x22, 0xd2, 0x97, 0x75, 0xcc, 0x94, 0xc2, 0xca, 0x6f, 0xb0, 0x9c, 0x2d, 0x86, 0xd6, 0xd0, 0x18, 0x3c, 0x24, 0x34, 0x09, 0x73, 0x5b, 0xad, 0x57, 0x5e, 0x9f, 0x91};

	unsigned char	ZUCiv_Dec[]={0x00, 0x05, 0x68, 0x23, 0x38};

	CK_BYTE nSessKeyID_Dec = ucDecID;

	CK_ATTRIBUTE ZUCkeyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(CK_BYTE)}
	};
	CK_MECHANISM ZUCmechanism_Dec = {CKM_ZUC_CALC, ZUCiv_Dec, sizeof(ZUCiv_Dec)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	int loopTime = 1;
	unsigned int i = 0;

	printf("enter test_ZUCFLASHnoValue.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "enter test_ZUCFLASHnoValue.<br>");

	//对称加密初始化
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Enc, sizeof(ZUCkeyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject_hKey_Enc,rv);

	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism_Enc, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	hKey_Dec = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Dec, sizeof(ZUCkeyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject_hKey_Dec,rv);

	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism_Dec, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);



	for (i=0; i<loopTime; i++)
	{
		indatalen = sizeof(indata);
		memset(indata, 0, indatalen);
		outdatalen=sizeof(outdata);
		memset(outdata, 0, outdatalen);

		indatalen1 = sizeof(indata1);
		memset(indata1, 0, indatalen1);
		outdatalen1=sizeof(outdata1);
		memset(outdata1, 0, outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv_Enc, sizeof(ZUCiv_Enc));
		memcpy(indata+sizeof(ZUCiv_Enc), ZUCplain_Enc, sizeof(ZUCplain_Enc));
		indatalen = sizeof(ZUCiv_Enc) + sizeof(ZUCplain_Enc);

		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv_Dec, sizeof(ZUCiv_Dec));
		memcpy(indata1+sizeof(ZUCiv_Dec), ZUCcipher_Dec, sizeof(ZUCcipher_Dec));
		indatalen1 = sizeof(ZUCiv_Dec) + sizeof(ZUCcipher_Dec);

		/*******************加密过程**********************/

		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);



		/******************解密过程***********************/
		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);

		if ((outdatalen != sizeof(ZUCcipher_Enc)) || (memcmp(outdata, ZUCcipher_Enc, outdatalen)))
		{
			printf("Error: ZUC Encrypt.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"CAL Error: ZUC Encrypt.<br>"); 
		}

		if ((outdatalen1 != sizeof(ZUCplain_Dec)) || (memcmp(outdata1, ZUCplain_Dec, outdatalen1)))
		{
			printf("Error: ZUC Decrypt.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"CAL Error: ZUC Decrypt.<br>"); 
		}
	}

	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Enc);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Enc,rv);

	rv = (*pC_DestroyObject)(hSession, hKey_Dec);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject_hKey_Dec,rv);

	printf("leave test_ZUCFLASHnoValue.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "leave test_ZUCFLASHnoValue.<br>");

	bRtn = true;
END:

	
	return bRtn;
}

bool xtest_ZUC_KEY()
{
	bool xnRtn = false;
	CK_RV rv = -1;

	xnRtn = test_ZUCRAM(CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1);//RAM密钥
	if(xnRtn = false)
	{
		return false;
	}

	xnRtn = test_ZUCFLASHValue(CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1);//FLASH密钥
	if(xnRtn = false)
	{
		return false;
	}

	xnRtn = test_ZUCFLASHnoValue( CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1);//FLASH预设密钥
	if(xnRtn = false)
	{
		return false;
	}

#ifdef OPEN_LOGOUT_CLOSESSION
	//xnRtn = test_ZUCRAM(CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//RAM密钥
	//if(xnRtn = false)
	//{
		//return false;
	//}
	
	//FLASH ID7 can not be used as en/decrypt key
	//xnRtn = test_ZUCFLASHValue(CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH密钥
	//if(xnRtn = false)
	//{
	//	return false;
	//}

	//xnRtn = test_ZUCFLASHnoValue(CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH预设密钥
	//if(xnRtn = false)
	//{
	//	return false;
	//}

#endif

	
	return true;

}


/*测试SM3HMAC 软算法*/
bool test_HMACWithPreset()
{
	CK_RV rv = 0;
	bool	bRtn = false;
	int i;
	CK_BYTE_PTR inputDatas[1]; 
	
	CK_OBJECT_CLASS key_class = CKO_SECRET_KEY;
	CK_KEY_TYPE key_type = CKK_SM3;
	CK_ATTRIBUTE_TYPE keySize = 64;
	CK_BBOOL _true = TRUE;
    CK_BBOOL _false = FALSE;
	CK_BYTE KeyVal[64]={0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a,0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a,0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a,0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};
	CK_BYTE opadData[]={0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,
	                     0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,};
	CK_BYTE ipadData[]={0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,
						0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,};
	CK_BYTE okeypad[64]={0};	
    CK_BYTE ikeypad[64]={0};
	CK_BYTE inputdata1[96]={0};
	CK_BYTE inputdata2[96]={0};
	int attrcount = 0;
	CK_OBJECT_HANDLE hKey=0;
	CK_OBJECT_HANDLE_PTR pSM3Key =&hKey;

	CK_ATTRIBUTE secretKeyTemplate[] = {
			{ CKA_CLASS, &key_class, sizeof(key_class)},
			{ CKA_TOKEN, &_true, sizeof(_false) },			
			{ CKA_KEY_TYPE, &key_type, sizeof(key_type) },
			{ CKA_SIGN, &_true, sizeof(_true) },
			{ CKA_VALUE, &keySize, sizeof(keySize)}
	};

	CK_BYTE_PTR secret_key_data = (CK_BYTE_PTR)malloc(sizeof(CK_BYTE) * 64);
	memset(secret_key_data,0,sizeof(CK_BYTE) * 64);

	CK_MECHANISM_PTR pPmechanism = (CK_MECHANISM_PTR)malloc(sizeof(CK_MECHANISM));
	memset(pPmechanism, 0 , sizeof(CK_MECHANISM));
	pPmechanism->mechanism = CKM_HMAC_SM3_KEY_GEN;

	rv = pC_GenerateKey(hSession,pPmechanism,secretKeyTemplate,5,pSM3Key);
	free(secret_key_data);
	free(pPmechanism);

	
	for(i = 0;i < 1;++i)
	{
		//inputDatas[i] = (CK_BYTE_PTR)malloc(sizeof(CK_BYTE) * (i+1) * 4);
		//rv = C_GenerateRandom(session,inputDatas[i],(i+1) * 4);
		inputDatas[i] = (CK_BYTE_PTR)malloc(sizeof(CK_BYTE) * 32);
		rv = pC_GenerateRandom(hSession,inputDatas[i],32);
		//memset(inputDatas[i], 0 , sizeof(CK_BYTE) * 32);
		//rv = C_GenerateRandom(session, inputDatas[i], 32);
	}
	
	pPmechanism = (CK_MECHANISM_PTR)malloc(sizeof(CK_MECHANISM));
	memset(pPmechanism, 0 , sizeof(CK_MECHANISM));
	pPmechanism->mechanism = CKM_HASH_SM3;

 

	for(i=0;i<sizeof(opadData);i++)
	{
		okeypad[i]=opadData[i]^KeyVal[i];
	}
	for(i=0;i<sizeof(ipadData);i++)
	{
		ikeypad[i]=ipadData[i]^KeyVal[i];
	}
	
	for(i=0;i<sizeof(opadData);i++)
	{
		printf(" %x",okeypad[i]);
	}
	printf("\n");
	
	for(i=0;i<sizeof(opadData);i++)
	{
		printf(" %x",ikeypad[i]);
	}
	printf("\n");
	memcpy(inputdata1,ikeypad,sizeof(ikeypad));
	memcpy(inputdata1+sizeof(ikeypad),inputDatas[0],32);
	for(i=0;i<sizeof(inputdata1);i++)
	{
		printf(" %x",inputdata1[i]);
	}
	printf("\n");
	
	
	//第一次Hash
	    CK_BYTE_PTR SM3DigestFinalData1;
		CK_ULONG SM3DigestLen1;
		rv = pC_DigestInit(hSession, pPmechanism);

		if(rv != CKR_OK)
		{
			printf("C_DigestInit failed: %0x\n", rv);
			return rv;
		}

		
		rv = pC_DigestUpdate(hSession,inputdata1,sizeof(inputdata1));

		if(rv != CKR_OK)
		{
			printf("C_DigestUpdate failed: %0x\n", rv);
			return rv;
		}
		
		rv = pC_DigestFinal(hSession,NULL, &SM3DigestLen1);

		if(rv != CKR_OK)
		{
			printf("C_DigestFinal1 failed: %0x\n", rv);
			return rv;
		}

		SM3DigestFinalData1 = (CK_BYTE_PTR)malloc(sizeof(CK_BYTE) * SM3DigestLen1);
		memset(SM3DigestFinalData1 , 0, sizeof(CK_BYTE) * SM3DigestLen1);

		rv = pC_DigestFinal(hSession,SM3DigestFinalData1, &SM3DigestLen1);

		if(rv != CKR_OK)
		{
			printf("C_DigestFinal2 failed: %0x\n", rv);
			return rv;
		}

		memcpy(inputdata2,okeypad,sizeof(okeypad));
	    memcpy(inputdata2+sizeof(okeypad),SM3DigestFinalData1,SM3DigestLen1);
		
		printf("digest out put: \n");
		//print_data(SM3DigestFinalData1,SM3DigestLen1);
		printf("\n\n");
		
		for(i=0;i<sizeof(inputdata2);i++)
		{
			printf(" %x",inputdata2[i]);
		}
		printf("\n");
		free(SM3DigestFinalData1);
		
	//第二次hash
		CK_BYTE_PTR SM3DigestFinalData2;
		CK_ULONG SM3DigestLen2;
		rv = pC_DigestInit(hSession, pPmechanism);

		if(rv != CKR_OK)
		{
			printf("C_DigestInit failed: %0x\n", rv);
			return rv;
		}

		
		rv = pC_DigestUpdate(hSession,inputdata2,sizeof(inputdata2));
		if(rv != CKR_OK)
		{
			printf("C_DigestUpdate failed: %0x\n", rv);
			return rv;
		}
		
		rv = pC_DigestFinal(hSession,NULL, &SM3DigestLen2);

		if(rv != CKR_OK)
		{
			printf("C_DigestFinal1 failed: %0x\n", rv);
			return rv;
		}

		SM3DigestFinalData2 = (CK_BYTE_PTR)malloc(sizeof(CK_BYTE) * SM3DigestLen2);
		memset(SM3DigestFinalData2 , 0, sizeof(CK_BYTE) * SM3DigestLen2);

		rv = pC_DigestFinal(hSession,SM3DigestFinalData2, &SM3DigestLen2);

		if(rv != CKR_OK)
		{
			printf("C_DigestFinal2 failed: %0x\n", rv);
			return rv;
		}

		
		printf("digest out2 put: \n");
		//print_data(SM3DigestFinalData2,SM3DigestLen2);
		printf("\n\n");
		
	
		printf("\n");
		free(pPmechanism);
	
	
	
	for(i = 0;i < 1;++i)
	{	
		
		pPmechanism = (CK_MECHANISM_PTR)malloc(sizeof(CK_MECHANISM));
		memset(pPmechanism, 0 , sizeof(CK_MECHANISM));
		pPmechanism->mechanism = CKM_HMAC_SM3_WITH_PRESET;

		
		pPmechanism->ulParameterLen = 64;
		pPmechanism->pParameter = (CK_VOID_PTR)malloc(sizeof(CK_BYTE) * 64);
		memset(pPmechanism->pParameter, 0, sizeof(CK_BYTE) * 64);
		memcpy(pPmechanism->pParameter, KeyVal, 64);
	
	
		CK_BYTE_PTR SM3DigestFinalData;
		CK_ULONG SM3DigestLen;

		//printf("raw data: %d\n", (i+1) * 4);
		//print_data(inputDatas[i],(i+1) * 4);

		printf("raw data: 32\n");
		//print_data(inputDatas[i],32);

		rv = pC_DigestInit(hSession, pPmechanism);
		

		if(rv != CKR_OK)
		{
			printf("C_DigestInit failed: %0x\n", rv);
			return rv;
		}
		
		rv = pC_DigestUpdate(hSession,inputDatas[i],32);
		

		if(rv != CKR_OK)
		{
			printf("C_DigestUpdate failed: %0x\n", rv);
			return rv;
		}
		else
		{
			printf("C_DigestUpdate success!\n");
		}
		
		rv = pC_DigestFinal(hSession,NULL, &SM3DigestLen);

		if(rv != CKR_OK)
		{
			printf("C_DigestFinal1 failed: %0x\n", rv);
			return rv;
		}

		SM3DigestFinalData = (CK_BYTE_PTR)malloc(sizeof(CK_BYTE) * SM3DigestLen);
		memset(SM3DigestFinalData , 0, sizeof(CK_BYTE) * SM3DigestLen);

		rv = pC_DigestFinal(hSession,SM3DigestFinalData, &SM3DigestLen);

		if(rv != CKR_OK)
		{
			printf("C_DigestFinal2 failed: %0x\n", rv);
			return rv;
		}

		printf("HMAC-SM3 digest out put: \n");
		//print_data(SM3DigestFinalData,32);
		printf("\n\n");
		
		if ((SM3DigestLen != SM3DigestLen2) || (memcmp(SM3DigestFinalData, SM3DigestFinalData2, SM3DigestLen)))
		{
			printf("Error: HMAC-SM3 .\n");
		}
		else {
			printf("Success: HMAC-SM3.\n");
		}
		
		free(SM3DigestFinalData2);
		free(SM3DigestFinalData);
		free(inputDatas[i]);
	}
	//free(inputDatas[i]);
	free(pPmechanism);
	return rv;
}


bool xtest_SM4OFB_Speed_GenV5(int looptime, int datalen)
{
	bool bRtn = false;
	unsigned int i = 0;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID1;
	//加密参数
	unsigned char	SM4iv[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	unsigned char	SM4plain_Enc[192]={0};
	CK_ATTRIBUTE SM4keyTemplate_Gen[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Gen = {CKM_SM4_KEY_GEN, NULL_PTR, 0};
	CK_MECHANISM SM4mechanism = {CKM_SM4_OFB, SM4iv, sizeof(SM4iv)};
	CK_OBJECT_HANDLE hKey = NULL_PTR;


	CK_BYTE randomData[192] = {0};

	CK_BYTE indata[192] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[192] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[192] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[192] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);	
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4OFB_Performance_Gen);

	//对称加密初始化
	hKey = NULL_PTR;
	Utilsgettime(&ttc1);
	rv = pC_GenerateKey(hSession, &SM4mechanism_Gen, SM4keyTemplate_Gen, sizeof(SM4keyTemplate_Gen)/sizeof(CK_ATTRIBUTE), &hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKey,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"SM4OFB GenerateKey", 1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb]+ nResultLength,"SM4OFB GenerateKey", 1);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism, hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	rv = (*pC_DecryptInit)(hSession, &SM4mechanism, hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);


	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);

	for (i=0; i<looptime; i++)
	{
		rv = pC_GenerateRandom(hSession, randomData, datalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateRandom,rv);

		memcpy(SM4plain_Enc, randomData, sizeof(randomData));

		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);
		//加密的明文
		memcpy(indata, SM4plain_Enc, sizeof(SM4plain_Enc));
		indatalen = sizeof(SM4plain_Enc);

		//printf("datalen1=%d,outdatalen1=%d\n",datalen,outdatalen);
		Utilsgettime(&ttc1);
		//加密运算
		//UtilsPrintData(VNAME(indata),datalen,0);
		rv = (*pC_EncryptUpdate)(hSession, indata, datalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
		//printf("datalen2=%d,outdatalen2=%d\n",datalen,outdatalen);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);

		//UtilsPrintData(VNAME(outdata),outdatalen,0);

		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		Utilsgettime(&ttc1);
		//解密运算
		//UtilsPrintData(VNAME(indata1),indatalen1,0);
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);

		//UtilsPrintData(VNAME(outdata1),outdatalen1,0);
		if ((outdatalen1 != datalen) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4OFB.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Calc Error: test_SM4OFB.<br>");
			bRtn = false;
			goto END;
		}
	}
	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	Utilsprint(&ttc3,"OFB Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"OFB Encrypt", looptime);
	Utilsprint(&ttc4,"OFB Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"OFB Decrypt", looptime);

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4OFB_Performance_Gen);
	bRtn = true;	
END:	
	return bRtn;
}

bool xtest_SM4CBC_Speed_GenV5(int looptime, int datalen)
{
	bool bRtn = false;
	unsigned int i = 0;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID0;

	//加密参数
	unsigned char	SM4iv[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_BYTE SM4plain_Enc[1024]={0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	CK_ATTRIBUTE SM4keyTemplate_Gen[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(CK_BYTE)}
	};

	CK_MECHANISM SM4mechanism_Gen = {CKM_SM4_KEY_GEN, NULL_PTR, 0};
	CK_MECHANISM SM4mechanism = {CKM_SM4_CBC, SM4iv, sizeof(SM4iv)};
	CK_OBJECT_HANDLE hKey = NULL_PTR;

	CK_BYTE randomData[1024] = {0};

	CK_BYTE indata[1024] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[1024] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[1024] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[1024] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4CBC_Performance_Gen);

	//对称加密初始化
	hKey = NULL_PTR;
	Utilsgettime(&ttc1);
	rv = pC_GenerateKey(hSession, &SM4mechanism_Gen, SM4keyTemplate_Gen, sizeof(SM4keyTemplate_Gen)/sizeof(CK_ATTRIBUTE), &hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKey,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"SM4CBC GenerateKey", 1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb]+ nResultLength,"SM4CBC GenerateKey", 1);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism, hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	rv = (*pC_DecryptInit)(hSession, &SM4mechanism, hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);


	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);
	for (i=0; i<looptime; i++)
	{
		rv = pC_GenerateRandom(hSession, randomData, datalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateRandom,rv);

		//memcpy(SM4plain_Enc,randomData, datalen);

		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);
		//加密的明文
		memcpy(indata, SM4plain_Enc, datalen);
		indatalen = datalen;

		//UtilsPrintData(VNAME(SM4keyVal_Enc),sizeof(SM4keyVal_Enc),0);
		//UtilsPrintData(VNAME(SM4iv_Enc),sizeof(SM4iv_Enc),0);
		//UtilsPrintData(VNAME(indata),indatalen,0);

		Utilsgettime(&ttc1);
		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);

		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		//UtilsPrintData(VNAME(SM4keyVal_Enc),sizeof(SM4keyVal_Enc),0);
		//UtilsPrintData(VNAME(SM4iv_Enc),sizeof(SM4iv_Enc),0);
		//UtilsPrintData(VNAME(indata1),indatalen1,0);

		Utilsgettime(&ttc1);
		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);

		//UtilsPrintData(VNAME(outdata1),outdatalen1,0);
		if ((outdatalen1 != datalen) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4CBC_Speed.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Calc Error: test_SM4CBC_Speed.<br>"); 
			bRtn = false;
			goto END;
		}
	}
	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	Utilsprint(&ttc3,"CBC Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"CBC Encrypt", looptime);
	Utilsprint(&ttc4,"CBC Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"CBC Decrypt", looptime);
	
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4CBC_Performance_Gen);
	bRtn = true;
END:	
	return bRtn;
}

bool xtest_SM4ECB_Speed_GenV5(int looptime, int datalen)
{
	bool bRtn = false;
	unsigned int i = 0;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID0;

	//加密参数
	unsigned char	SM4iv[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_BYTE SM4plain_Enc[1024]={0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	CK_ATTRIBUTE SM4keyTemplate_Gen[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Gen = {CKM_SM4_KEY_GEN, NULL_PTR, 0};
	CK_MECHANISM SM4mechanism = {CKM_SM4_ECB, SM4iv, sizeof(SM4iv)};
	CK_OBJECT_HANDLE hKey = NULL_PTR;

	CK_BYTE randomData[1024] = {0};

	CK_BYTE indata[1024] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[1024] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[1024] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[1024] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4ECB_Performance_Gen);

	//对称加密初始化
	hKey = NULL_PTR;
	Utilsgettime(&ttc1);
	rv = pC_GenerateKey(hSession, &SM4mechanism_Gen, SM4keyTemplate_Gen, sizeof(SM4keyTemplate_Gen)/sizeof(CK_ATTRIBUTE), &hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKey,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"SM4ECB GenerateKey", 1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb]+ nResultLength,"SM4ECB GenerateKey", 1);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism, hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);


	rv = (*pC_DecryptInit)(hSession, &SM4mechanism, hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);
	for (i=0; i<looptime; i++)
	{
		rv = pC_GenerateRandom(hSession, randomData, datalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateRandom,rv);

		memcpy(SM4plain_Enc,randomData, datalen);

		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);

		//加密的明文
		memcpy(indata, SM4plain_Enc,datalen);
		indatalen = datalen;

		Utilsgettime(&ttc1);
		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);

		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		Utilsgettime(&ttc1);
		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);

		if ((outdatalen1 != datalen) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4ECB_Speed.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Calc Error: test_SM4ECB_Speed.<br>"); 
			bRtn = false;
			goto END;
		}
	}
	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	Utilsprint(&ttc3,"ECB Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"ECB Encrypt", looptime);
	Utilsprint(&ttc4,"ECB Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"ECB Decrypt", looptime);

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4ECB_Performance_Gen);
	bRtn = true;
END:	
	return bRtn;
}

bool xtest_ZUCPerformance_GenV5(int looptime, int datalen)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID0;
	//加密参数

	unsigned char	ZUCplain_Enc[128] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, \
		0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, 0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00};
	unsigned char	ZUCcipher_Enc[128] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, \
		0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01, 0x9b, 0x48, 0xac, 0xd1, 0xfe, 0xf3, 0x01, 0x1f, 0x5e, 0x22, 0xd2, 0x97, 0x75, 0xcc, 0x94, 0xc2, 0xca, 0x6f, 0xb0, 0x9c, 0x2d, 0x86, 0xd6, 0xd0, 0x18, 0x3c, 0x24, 0x34};	
	
	unsigned char	ZUCiv[] = {0x00, 0x05, 0x68, 0x23, 0x38};

	CK_MECHANISM ZUCmechanismGen = {CKM_ZUC_KEY_GEN, NULL_PTR, 0};
	CK_MECHANISM ZUCmechanism = {CKM_ZUC_CALC, ZUCiv, sizeof(ZUCiv)};
	CK_OBJECT_HANDLE hKey = NULL_PTR;

	CK_ATTRIBUTE ZUCkeyTemplate_Gen[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(CK_BYTE)}
	};

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);

	unsigned int i = 0,j=0;
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCPerformance_Gen);

	srand( (unsigned)time( NULL ) );//随机数初始化

	hKey = NULL_PTR;
	Utilsgettime(&ttc1);
	rv = pC_GenerateKey(hSession, &ZUCmechanismGen,ZUCkeyTemplate_Gen, sizeof(ZUCkeyTemplate_Gen)/sizeof(CK_ATTRIBUTE), &hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateKey,rv);
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	Utilsprint(&ttc2,"ZUC GenerateKey", 1);
	nResultLength += Utilssprint(&ttc2,Testresult[nItemNumb]+ nResultLength,"ZUC GenerateKey", 1);

	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism, hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism, hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);
	for (i=0; i<looptime; i++)
	{

		//生成随机数明文	
		RandomGenerate(ZUCplain_Enc,datalen);
		//生成随机数初始向量
		RandomGenerate(ZUCiv,5);

		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv, sizeof(ZUCiv));
		memcpy(indata+sizeof(ZUCiv), ZUCplain_Enc, datalen);
		indatalen = sizeof(ZUCiv) + datalen;

		/*******************加密过程**********************/
		Utilsgettime(&ttc1);
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);
		//tt2.printn("ZUC Encrypt", i);

		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv, sizeof(ZUCiv));
		memcpy(indata1+sizeof(ZUCiv), outdata, outdatalen);
		indatalen1 = sizeof(ZUCiv) + outdatalen;

		/******************解密过程***********************/
		Utilsgettime(&ttc1);
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);
		//tt2.printn("ZUC Decrypt", i);

		if ((outdatalen1 != datalen) || (memcmp(outdata1, ZUCplain_Enc, outdatalen1)))
		{
			printf("Error: ZUCPerformance.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: ZUCPerformance.<br>");
			bRtn = false;
			goto END;
		}

	}
	rv = (*pC_EncryptFinal)(hSession, outdata, &outdatalen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	Utilsprint(&ttc3,"ZUC Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"ZUC Encrypt", looptime);
	Utilsprint(&ttc4,"ZUC Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"ZUC Decrypt", looptime);
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCPerformance_Gen);
	bRtn = true;	
END:	
	return bRtn;
}

bool xtest_SM4OFB_Speed_GenV4(int looptime, int datalen)
{
	bool bRtn = false;
	unsigned int i = 0;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID1;
	//加密参数
	unsigned char	SM4iv[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	unsigned char	SM4plain_Enc[192]={0};
	CK_ATTRIBUTE SM4keyTemplate_Gen[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Gen = {CKM_SM4_KEY_GEN, NULL_PTR, 0};
	CK_MECHANISM SM4mechanism = {CKM_SM4_OFB, SM4iv, sizeof(SM4iv)};
	CK_OBJECT_HANDLE hKey = NULL_PTR;


	CK_BYTE randomData[192] = {0};

	CK_BYTE indata[192] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[192] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[192] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[192] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);	
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4OFB_Performance_Gen);

	//对称加密初始化
	hKey = NULL_PTR;
	rv = pC_GenerateKey(hSession, &SM4mechanism_Gen, SM4keyTemplate_Gen, sizeof(SM4keyTemplate_Gen)/sizeof(CK_ATTRIBUTE), &hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_GenerateKey,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism, hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	rv = (*pC_DecryptInit)(hSession, &SM4mechanism, hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);

	for (i=0; i<looptime; i++)
	{
		rv = pC_GenerateRandom(hSession, randomData, datalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateRandom,rv);

		memcpy(SM4plain_Enc, randomData, sizeof(randomData));

		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);
		//加密的明文
		memcpy(indata, SM4plain_Enc, sizeof(SM4plain_Enc));
		indatalen = sizeof(SM4plain_Enc);

		//printf("datalen1=%d,outdatalen1=%d\n",datalen,outdatalen);
		Utilsgettime(&ttc1);
		//加密运算
		//UtilsPrintData(VNAME(indata),datalen,0);
		rv = (*pC_EncryptUpdate)(hSession, indata, datalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
		//printf("datalen2=%d,outdatalen2=%d\n",datalen,outdatalen);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);

		//UtilsPrintData(VNAME(outdata),outdatalen,0);

		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		Utilsgettime(&ttc1);
		//解密运算
		//UtilsPrintData(VNAME(indata1),indatalen1,0);
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);

		//UtilsPrintData(VNAME(outdata1),outdatalen1,0);
		if ((outdatalen1 != datalen) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4OFB.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Calc Error: test_SM4OFB.<br>");
		}
	}
	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	Utilsprint(&ttc3,"OFB Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"OFB Encrypt", looptime);
	Utilsprint(&ttc4,"OFB Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"OFB Decrypt", looptime);
	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4OFB_Performance_Gen);
	bRtn = true;	
END:

	
	return bRtn;
}

bool xtest_SM4CBC_Speed_GenV4(int looptime, int datalen)
{
	bool bRtn = false;
	unsigned int i = 0;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID0;

	//加密参数
	unsigned char	SM4iv[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_BYTE SM4plain_Enc[1024]={0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	CK_ATTRIBUTE SM4keyTemplate_Gen[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(CK_BYTE)}
	};

	CK_MECHANISM SM4mechanism_Gen = {CKM_SM4_KEY_GEN, NULL_PTR, 0};
	CK_MECHANISM SM4mechanism = {CKM_SM4_CBC, SM4iv, sizeof(SM4iv)};
	CK_OBJECT_HANDLE hKey = NULL_PTR;


	CK_BYTE randomData[1024] = {0};

	CK_BYTE indata[1024] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[1024] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[1024] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[1024] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4CBC_Performance_Gen);

	//对称加密初始化
	hKey = NULL_PTR;
	rv = pC_GenerateKey(hSession, &SM4mechanism_Gen, SM4keyTemplate_Gen, sizeof(SM4keyTemplate_Gen)/sizeof(CK_ATTRIBUTE), &hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_GenerateKey,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism, hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	//对称解密初始化
	rv = (*pC_DecryptInit)(hSession, &SM4mechanism, hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);
	for (i=0; i<looptime; i++)
	{
		rv = pC_GenerateRandom(hSession, randomData, datalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateRandom,rv);

		//memcpy(SM4plain_Enc,randomData, datalen);

		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);
		//加密的明文
		memcpy(indata, SM4plain_Enc, datalen);
		indatalen = datalen;

		//UtilsPrintData(VNAME(SM4keyVal_Enc),sizeof(SM4keyVal_Enc),0);
		//UtilsPrintData(VNAME(SM4iv_Enc),sizeof(SM4iv_Enc),0);
		//UtilsPrintData(VNAME(indata),indatalen,0);

		Utilsgettime(&ttc1);
		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);

		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		//UtilsPrintData(VNAME(SM4keyVal_Enc),sizeof(SM4keyVal_Enc),0);
		//UtilsPrintData(VNAME(SM4iv_Enc),sizeof(SM4iv_Enc),0);
		//UtilsPrintData(VNAME(indata1),indatalen1,0);

		Utilsgettime(&ttc1);
		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);

		//UtilsPrintData(VNAME(outdata1),outdatalen1,0);
		if ((outdatalen1 != datalen) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4CBC_Speed.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Calc Error: test_SM4CBC_Speed.<br>"); 
		}
	}
	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	Utilsprint(&ttc3,"CBC Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"CBC Encrypt", looptime);
	Utilsprint(&ttc4,"CBC Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"CBC Decrypt", looptime);

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4CBC_Performance_Gen);
	bRtn = true;
END:

	
	return bRtn;
}

bool xtest_SM4ECB_Speed_GenV4(int looptime, int datalen)
{
	bool bRtn = false;
	unsigned int i = 0;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID0;

	//加密参数
	unsigned char	SM4iv[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_BYTE SM4plain_Enc[1024]={0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	CK_ATTRIBUTE SM4keyTemplate_Gen[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(CK_BYTE)}
	};
	CK_MECHANISM SM4mechanism_Gen = {CKM_SM4_KEY_GEN, NULL_PTR, 0};
	CK_MECHANISM SM4mechanism = {CKM_SM4_ECB, SM4iv, sizeof(SM4iv)};
	CK_OBJECT_HANDLE hKey = NULL_PTR;

	CK_BYTE randomData[1024] = {0};

	CK_BYTE indata[1024] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[1024] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[1024] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[1024] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4ECB_Performance_Gen);

	//对称加密初始化
	hKey = NULL_PTR;
	rv = pC_GenerateKey(hSession, &SM4mechanism_Gen, SM4keyTemplate_Gen, sizeof(SM4keyTemplate_Gen)/sizeof(CK_ATTRIBUTE), &hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_GenerateKey,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism, hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);


	rv = (*pC_DecryptInit)(hSession, &SM4mechanism, hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);
	for (i=0; i<looptime; i++)
	{
		rv = pC_GenerateRandom(hSession, randomData, datalen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_GenerateRandom,rv);

		memcpy(SM4plain_Enc,randomData, datalen);

		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);

		//加密的明文
		memcpy(indata, SM4plain_Enc,datalen);
		indatalen = datalen;

		Utilsgettime(&ttc1);
		//加密运算
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);

		//解密的密文
		memcpy(indata1, outdata, outdatalen);
		indatalen1 = outdatalen;

		Utilsgettime(&ttc1);
		//解密运算
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);

		if ((outdatalen1 != datalen) || (memcmp(outdata1, SM4plain_Enc, outdatalen1)))
		{
			printf("Calc Error: test_SM4ECB_Speed.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Calc Error: test_SM4ECB_Speed.<br>"); 
		}
	}
	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_EncryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	Utilsprint(&ttc3,"ECB Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"ECB Encrypt", looptime);
	Utilsprint(&ttc4,"ECB Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"ECB Decrypt", looptime);

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestSM4ECB_Performance_Gen);
	bRtn = true;
END:

	
	return bRtn;
}

bool xtest_ZUCPerformance_GenV4(int looptime, int datalen)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID0;
	//加密参数

	unsigned char	ZUCplain_Enc[128] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, \
		0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d, 0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00};
	unsigned char	ZUCcipher_Enc[128] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, \
		0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01, 0x9b, 0x48, 0xac, 0xd1, 0xfe, 0xf3, 0x01, 0x1f, 0x5e, 0x22, 0xd2, 0x97, 0x75, 0xcc, 0x94, 0xc2, 0xca, 0x6f, 0xb0, 0x9c, 0x2d, 0x86, 0xd6, 0xd0, 0x18, 0x3c, 0x24, 0x34};	
	
	unsigned char	ZUCiv[] = {0x00, 0x05, 0x68, 0x23, 0x38};

	CK_MECHANISM ZUCmechanismGen = {CKM_ZUC_KEY_GEN, NULL_PTR, 0};
	CK_MECHANISM ZUCmechanism = {CKM_ZUC_CALC, ZUCiv, sizeof(ZUCiv)};
	CK_OBJECT_HANDLE hKey = NULL_PTR;

	CK_ATTRIBUTE ZUCkeyTemplate_Gen[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(CK_BYTE)}
	};

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);

	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);

	unsigned int i = 0,j=0;
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0},ttc4={0,0};

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCPerformance_Gen);

	srand( (unsigned)time( NULL ) );//随机数初始化

	
	hKey = NULL_PTR;
	rv = pC_GenerateKey(hSession, &ZUCmechanismGen,ZUCkeyTemplate_Gen, sizeof(ZUCkeyTemplate_Gen)/sizeof(CK_ATTRIBUTE), &hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_GenerateKey,rv);

	rv = (*pC_EncryptInit)(hSession, &ZUCmechanism, hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	rv = (*pC_DecryptInit)(hSession, &ZUCmechanism, hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DecryptInit,rv);

	//printf("Datalen=%d.\n",datalen);
	//nResultLength += sprintf(Testresult[nItemNumb]+nResultLength ,"Datalen=%d.<br>",datalen);
	for (i=0; i<looptime; i++)
	{

		//生成随机数明文	
		RandomGenerate(ZUCplain_Enc,datalen);
		//生成随机数初始向量
		RandomGenerate(ZUCiv,5);

		BUFFER_REFRESH_ZERO(indata,indatalen,outdata,outdatalen);
		BUFFER_REFRESH_ZERO(indata1,indatalen1,outdata1,outdatalen1);

		//将IV拼接到明文之前
		memcpy(indata, ZUCiv, sizeof(ZUCiv));
		memcpy(indata+sizeof(ZUCiv), ZUCplain_Enc, datalen);
		indatalen = sizeof(ZUCiv) + datalen;

		/*******************加密过程**********************/
		Utilsgettime(&ttc1);
		rv = (*pC_EncryptUpdate)(hSession, indata, indatalen, outdata, &outdatalen);
		RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_EncryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);
		//tt2.printn("ZUC Encrypt", i);

		//将IV拼接到密文之前
		memcpy(indata1, ZUCiv, sizeof(ZUCiv));
		memcpy(indata1+sizeof(ZUCiv), outdata, outdatalen);
		indatalen1 = sizeof(ZUCiv) + outdatalen;

		/******************解密过程***********************/
		Utilsgettime(&ttc1);
		rv = (*pC_DecryptUpdate)(hSession, indata1, indatalen1, outdata1, &outdatalen1);
		RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DecryptUpdate,rv);
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc4,&ttc2);
		//tt2.printn("ZUC Decrypt", i);

		if ((outdatalen1 != datalen) || (memcmp(outdata1, ZUCplain_Enc, outdatalen1)))
		{
			printf("Error: ZUCPerformance.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error: ZUCPerformance.<br>");
		}

	}
	rv = (*pC_EncryptFinal)(hSession, outdata, &outdatalen);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_EncryptFinal,rv);

	rv = (*pC_DecryptFinal)(hSession, outdata1, &outdatalen1);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DecryptFinal,rv);

	rv = (*pC_DestroyObject)(hSession, hKey);
	RV_NOT_OK_RETURN_TRUE(Testresult[nItemNumb],&nResultLength,pC_DestroyObject,rv);

	Utilsprint(&ttc3,"ZUC Encrypt", looptime);
	nResultLength += Utilssprint(&ttc3,Testresult[nItemNumb]+ nResultLength,"ZUC Encrypt", looptime);
	Utilsprint(&ttc4,"ZUC Decrypt", looptime);
	nResultLength += Utilssprint(&ttc4,Testresult[nItemNumb]+ nResultLength,"ZUC Decrypt", looptime);

	ENTER_LEAVE_FUNCTION(Testresult[nItemNumb],&nResultLength,xTestZUCPerformance_Gen);
	bRtn = true;	
END:

	
	return bRtn;
}

bool xtest_SM4OFB_Speed_Gen(int looptime, int datalen)
{
		if(CardVersionData <= CARD_VERSION_413)
		{
			printf("Card Version 4.x not support this function.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Card Version 4.x not support this function.<br>");
			return FALSE;
			//return xtest_SM4OFB_Speed_GenV4(looptime, datalen);
		}
		else if( CardVersionData <= CARD_VERSION_501)
		{
			return xtest_SM4OFB_Speed_GenV5(looptime, datalen);
		}
		else
		{
			printf("CardVersionData <%x> Error.\n",CardVersionData);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "CardVersionData <%x> Error.<br>",CardVersionData);
			return FALSE;
		}
}

bool xtest_SM4CBC_Speed_Gen(int looptime, int datalen)
{
		if(CardVersionData <= CARD_VERSION_413)
		{
			printf("Card Version 4.x not support this function.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Card Version 4.x not support this function.<br>");
			return FALSE;
			//return xtest_SM4CBC_Speed_GenV4(looptime, datalen);
		}
		else if( CardVersionData <= CARD_VERSION_501)
		{
			return xtest_SM4CBC_Speed_GenV5(looptime, datalen);
		}
		else
		{
			printf("CardVersionData <%x> Error.\n",CardVersionData);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "CardVersionData <%x> Error.<br>",CardVersionData);
			return FALSE;
		}
}

bool xtest_SM4ECB_Speed_Gen(int looptime, int datalen)
{
		if(CardVersionData <= CARD_VERSION_413)
		{
			printf("Card Version 4.x not support this function.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Card Version 4.x not support this function.<br>");
			return FALSE;
			//return xtest_SM4ECB_Speed_GenV4(looptime, datalen);
		}
		else if( CardVersionData <= CARD_VERSION_501)
		{
			return xtest_SM4ECB_Speed_GenV5(looptime, datalen);
		}
		else
		{
			printf("CardVersionData <%x> Error.\n",CardVersionData);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "CardVersionData <%x> Error.<br>",CardVersionData);
			return FALSE;
		}
}

bool xtest_ZUCPerformance_Gen(int looptime, int datalen)
{
		if(CardVersionData <= CARD_VERSION_413)
		{			
			printf("Card Version 4.x not support this function.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Card Version 4.x not support this function.<br>");
			return FALSE;
			//return xtest_ZUCPerformance_GenV4(looptime, datalen);
		}
		else if( CardVersionData <= CARD_VERSION_501)
		{
			return xtest_ZUCPerformance_GenV5(looptime, datalen);
		}
		else
		{
			printf("CardVersionData <%x> Error.\n",CardVersionData);
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "CardVersionData <%x> Error.<br>",CardVersionData);
			return FALSE;
		}
}



/*密文导入密钥*/
CK_RV test_ImportKeyWithCipher(CK_MECHANISM_TYPE mechUnwrapType, unsigned char *pbIV, unsigned int uiIVLen, CK_OBJECT_HANDLE hUnwrapKey, CK_OBJECT_CLASS keyClass, CK_KEY_TYPE keyType, CK_BBOOL bToken, CK_BYTE keyID, unsigned char *pbCipher, unsigned int uiCipherLen)
{
	bool bRtn = false;
	int i = 0;
	CK_RV rv = 0;

	CK_BYTE     temp_pub_x[32]={
		0xec,0x91,0x81,0x8d,0xe0,0xb7,0x01,0x21,0x73,0xf5,0x1c,0x33,0x75,0x43,0x6e,0x43,
		0xb6,0xa9,0xa2,0x6a,0xbd,0x6d,0xbc,0xb7,0x9f,0x85,0x1c,0xde,0xaf,0x7a,0x0f,0x6c
	};
	CK_BYTE     temp_pub_y[32]={
		0xcb,0xf4,0xb5,0xa1,0x5f,0xb8,0x7e,0x60,0xfc,0x0b,0x3a,0x92,0x3d,0x12,0xe8,0x66,
		0x36,0x4a,0x93,0x5f,0xfb,0x30,0x84,0x2b,0xc9,0x13,0x9e,0xbd,0x2d,0xdc,0xe9,0x61
	};

	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_ATTRIBUTE ImportKeyTemplate[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &bToken, sizeof(bToken)},
		{CKA_KEY_TYPE, &keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &keyID, sizeof(CK_BYTE)}
	};
	CK_OBJECT_CLASS pubclass=CKO_PUBLIC_KEY;
	CK_OBJECT_CLASS priclass=CKO_PRIVATE_KEY;
	CK_UINT     modulusBits = 256;
	CK_BYTE      id[] = {0x01,0x01,0x02,0x03};
	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_UNWRAP, &ttrue, sizeof(ttrue)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_ATTRIBUTE publicKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_ECC_X_COORDINATE, temp_pub_x, sizeof(temp_pub_x)},
		{CKA_ECC_Y_COORDINATE, temp_pub_y, sizeof(temp_pub_y)},
		{CKA_ID, id, sizeof(id)}
	};

	CK_MECHANISM  mechanismcalc = {CKM_ECC_CALC, NULL_PTR, 0};

	CK_MECHANISM UnwrapMechanism = {mechUnwrapType, pbIV, uiIVLen};
	CK_BYTE_PTR   pData=(CK_BYTE_PTR)"12345678901234567890123456789012";
	CK_ULONG      ulDataLen=strlen((char*)pData);
	CK_BYTE       pOutData[256];   
	CK_ULONG      ulOutDataLen=sizeof(pOutData);

	CK_OBJECT_HANDLE hKey = NULL_PTR;
	CK_OBJECT_HANDLE hPubKey = NULL_PTR;
	//char* Char_UnwrapKey[]={"Run Correct", "hSession=NULL", "&UnwrapMechanism=NULL", "hUnwrapKey=NULL", "pbCipher=NULL", "uiCipherLen=NULL","privateKeyTemplate=NULL","Count=NULL","&hKey=NULL"};
	//int Count_Char_UnwrapKey = sizeof(Char_UnwrapKey)/sizeof(Char_UnwrapKey[0]);
	//Count_Char_UnwrapKey =1;

	//printf("enter test_ImportKeyWithCipher.\n");
	//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","enter test_ImportKeyWithCipher.<br>");

	if (keyType == CKK_ECC)
	{
		//printf("enter ECC\n");
		//导入非对称密钥
		//for (i = Count_Char_UnwrapKey-1; i >= 0; --i)
		//{
		//	hKey = NULL_PTR;
		//	rv = generalcall(pC_UnwrapKey, i, hSession, &UnwrapMechanism, hUnwrapKey, pbCipher, uiCipherLen, privateKeyTemplate, (int)(sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE)), &hKey);
		//	bRtn = Show_Result("pC_UnwrapKey", i, Char_UnwrapKey[i], rv, Testresult[nItemNumb], &nResultLength);
		//	RV_FALSE_RETURN(bRtn);
		//}

		rv = (*pC_UnwrapKey)(hSession, &UnwrapMechanism, hUnwrapKey, pbCipher, uiCipherLen, privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE), &hKey);
	  	  if (rv != CKR_OK)
		{
			printf("test_ImportKeyWithCipher failed,pC_UnwrapKey. rv=0x%08x.\n", rv);
			nResultLength += sprintf(Testresult[nItemNumb]+ nResultLength, "%s:<%s> Error(rv=0x%08x:%s)<br>", "test_ImportKeyWithCipher", "pC_UnwrapKey", rv, pC_StrErr(rv));
			bRtn = FALSE;
			goto END;
		}

		rv = (*pC_CreateObject)(hSession,publicKeyTemplate,sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE), &hPubKey);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject,rv);

		rv=(*pC_SignInit)(hSession,&mechanismcalc, hKey);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_SignInit,rv);

		rv=(*pC_Sign)(hSession, pData, ulDataLen, pOutData, &ulOutDataLen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Sign,rv);


		rv=(*pC_VerifyInit)(hSession,&mechanismcalc, hPubKey);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_VerifyInit,rv);

		rv=(*pC_Verify)(hSession, pData, ulDataLen, pOutData, ulOutDataLen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Verify,rv);

	}
	else
	{
		//for (i = Count_Char_UnwrapKey-1; i >= 0; --i)
		//{
		//	hKey = NULL_PTR;
		//	rv = generalcall(pC_UnwrapKey, i, hSession, &UnwrapMechanism, hUnwrapKey, pbCipher, uiCipherLen, ImportKeyTemplate, (int)(sizeof(ImportKeyTemplate)/sizeof(CK_ATTRIBUTE)), &hKey);
		//	bRtn = Show_Result("pC_UnwrapKey", i, Char_UnwrapKey[i], rv, Testresult[nItemNumb], &nResultLength);
		//	RV_FALSE_RETURN(bRtn);
		//}
		////导入密钥
		rv = (*pC_UnwrapKey)(hSession, &UnwrapMechanism, hUnwrapKey, pbCipher, uiCipherLen, ImportKeyTemplate, sizeof(ImportKeyTemplate)/sizeof(CK_ATTRIBUTE), &hKey);
		
	   	 if (rv != CKR_OK)
		{
			printf("test_ImportKeyWithCipher failed,pC_UnwrapKey. rv=0x%08x.\n", rv);
			nResultLength += sprintf(Testresult[nItemNumb]+ nResultLength, "%s:<%s> Error(rv=0x%08x:%s)<br>", "test_ImportKeyWithCipher", "pC_UnwrapKey", rv, pC_StrErr(rv));
			bRtn = FALSE;
			goto END;
		}
	}

	bRtn = true;
END:
	if (hKey)
	{
		rv = (*pC_DestroyObject)(hSession, hKey);
		if (rv != CKR_OK)
		{
			printf("test_ImportKeyWithCipher failed,pC_DestroyObject hKey. rv=0x%08x.\n", rv);
			bRtn = false;
		}
		hKey = NULL_PTR;
	}

	if (hPubKey)
	{
		rv = (*pC_DestroyObject)(hSession, hPubKey);
		if (rv != CKR_OK)
		{
			printf("test_ImportKeyWithCipher failed,pC_DestroyObject hPubKey. rv=0x%08x.\n", rv);
			bRtn = false;
		}
		hKey = NULL_PTR;
	}

	//printf("leave test_ImportKeyWithCipher.\n");
	//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","leave test_ImportKeyWithCipher.<br>");
	printf("test_ImportKeyWithCipher, return %s.\n", bRtn?"true":"false");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,  "test_ImportKeyWithCipher, return %s.<br>", bRtn?"true":"false");
	
	
	return bRtn;
}


CK_RV TestUnwrapAll()
{
	int i = 0;
	CK_RV bRtn = FALSE;
	CK_USER_TYPE userType = CKU_USER, sotype = CKU_SO;
	CK_OBJECT_HANDLE hPublicKey = NULL_PTR, hPrivateKey = NULL_PTR;

	CK_MECHANISM  mechanismcalc = {CKM_ECC_CALC, NULL_PTR, 0};
	CK_BYTE   pbData[16] = {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
	CK_ULONG      ulDataLen = sizeof(pbData);
	CK_BYTE   pbPrvData[32] = {0xc5,0x6a,0x2b,0x58,0xa0,0x94,0xef,0x24,0x41,0x03,0x79,0x45,0xba,0xb1,0x39,0x8c,0xc0,0xdf,0x9f,0xc4,0xf9,0x9e,0x9a,0x60,0x2c,0xd8,0x6f,0xc2,0xc3,0x88,0xad,0x0c};
	CK_ULONG  ulPrvDataLen = sizeof(pbPrvData);
	CK_BYTE       pbCipher[256] = {0};
	CK_ULONG      uiCipherLen = sizeof(pbCipher);
	CK_BYTE   key_value[16] = {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};

	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID0;
	CK_ATTRIBUTE SM4keyTemplate_RAM_en[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_VALUE, key_value, sizeof(key_value)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(CK_BYTE)}
	};

	CK_ATTRIBUTE SM4keyTemplate_RAM_unwrap[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_UNWRAP, &ttrue, sizeof(ttrue)},
		{CKA_VALUE, key_value, sizeof(key_value)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(CK_BYTE)}
	};
	CK_ATTRIBUTE SM4keyTemplate_FLASH[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		//{CKA_UNWRAP, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE, key_value, sizeof(key_value)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(CK_BYTE)}
	};
	unsigned char	SM4iv_Enc[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_MECHANISM SM4mechanism_ECB = {CKM_SM4_ECB, NULL, 0};
	CK_MECHANISM SM4mechanism_CBC = {CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_MECHANISM SM4mechanism_OFB = {CKM_SM4_OFB, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey = NULL_PTR;
	CK_OBJECT_HANDLE hKey_en = NULL_PTR;
	CK_OBJECT_HANDLE hKey_unwrap = NULL_PTR;
	CK_RV rv = -1;
	int prtcnt=0;

	//非对称导入对称
	bRtn=test_ECC_importKeyPair(&hPublicKey,&hPrivateKey);
	if(bRtn)
	{
		rv=(*pC_EncryptInit)(hSession,&mechanismcalc,hPublicKey);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

		rv=(*pC_Encrypt)(hSession, pbData, ulDataLen, pbCipher, &uiCipherLen);
		RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Encrypt,rv);

		//导入RAM密钥
		bRtn = test_ImportKeyWithCipher(CKM_ECC_CALC, NULL, 0, hPrivateKey, CKO_SECRET_KEY, CKK_SM4, FALSE, CK_SESSKEY_PRESET_ID0, pbCipher, uiCipherLen);
		RV_FALSE_RETURN(bRtn);

		bRtn = test_ImportKeyWithCipher(CKM_ECC_CALC, NULL, 0, hPrivateKey, CKO_SECRET_KEY, CKK_SM4, FALSE, CK_SESSKEY_PRESET_ID1, pbCipher, uiCipherLen);
		RV_FALSE_RETURN(bRtn);
		
		//bRtn = test_ImportKeyWithCipher(CKM_ECC_CALC, NULL, 0, hPrivateKey, CKO_SECRET_KEY, CKK_ZUC, FALSE, CK_SESSKEY_PRESET_ID2, pbCipher, uiCipherLen);
		//RV_FALSE_RETURN(bRtn);
		
		////导入FLASH密钥
		bRtn = test_ImportKeyWithCipher(CKM_ECC_CALC, NULL, 0, hPrivateKey, CKO_SECRET_KEY, CKK_SM4, TRUE, CK_SESSKEY_PRESET_ID0, pbCipher, uiCipherLen);
		RV_FALSE_RETURN(bRtn);

		bRtn = test_ImportKeyWithCipher(CKM_ECC_CALC, NULL, 0, hPrivateKey, CKO_SECRET_KEY, CKK_SM4, TRUE, CK_SESSKEY_PRESET_ID1, pbCipher, uiCipherLen);
		RV_FALSE_RETURN(bRtn);
		
		//bRtn = test_ImportKeyWithCipher(CKM_ECC_CALC, NULL, 0, hPrivateKey, CKO_SECRET_KEY, CKK_ZUC, TRUE, CK_SESSKEY_PRESET_ID2, pbCipher, uiCipherLen);
		//RV_FALSE_RETURN(bRtn);
		
		
		////SM4加解密
		bRtn = test_SM4RAMnoValue(CKM_SM4_ECB, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1);//RAM预设密钥
		RV_FALSE_RETURN(bRtn);
		bRtn = test_SM4FLASHnoValue(CKM_SM4_ECB, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1);//FLASH预设密钥
		RV_FALSE_RETURN(bRtn);

		bRtn = test_SM4RAMnoValue(CKM_SM4_CBC, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1);//RAM预设密钥
		RV_FALSE_RETURN(bRtn);
		bRtn = test_SM4FLASHnoValue(CKM_SM4_CBC, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1);//FLASH预设密钥
		RV_FALSE_RETURN(bRtn);

		bRtn = test_SM4RAMnoValue(CKM_SM4_OFB, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1);//RAM预设密钥
		RV_FALSE_RETURN(bRtn);	
		bRtn = test_SM4FLASHnoValue(CKM_SM4_OFB, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1);//FLASH预设密钥
		RV_FALSE_RETURN(bRtn);

#ifdef OPEN_LOGOUT_CLOSESSION
		bRtn=test_logout();
		RV_FALSE_RETURN(bRtn);

		//登录管理员
		bRtn=test_login(CKU_SO);
		RV_FALSE_RETURN(bRtn);

		bRtn = test_ImportKeyWithCipher(CKM_ECC_CALC, NULL, 0, hPrivateKey, CKO_SECRET_KEY, CKK_SM4, TRUE, CK_SESSKEY_PRESET_ID7, pbCipher, uiCipherLen);
		RV_FALSE_RETURN(bRtn);

		//bRtn = test_SM4FLASHnoValue(CKM_SM4_ECB, CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH预设密钥
		//RV_FALSE_RETURN(bRtn);
		
		bRtn = test_ImportKeyWithCipher(CKM_ECC_CALC, NULL, 0, hPrivateKey, CKO_SECRET_KEY, CKK_SM4, TRUE, CK_SESSKEY_PRESET_ID7, pbCipher, uiCipherLen);
		RV_FALSE_RETURN(bRtn);

		//bRtn = test_SM4FLASHnoValue(CKM_SM4_CBC, CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH预设密钥
		//RV_FALSE_RETURN(bRtn);
		
		bRtn = test_ImportKeyWithCipher(CKM_ECC_CALC, NULL, 0, hPrivateKey, CKO_SECRET_KEY, CKK_ZUC, TRUE, CK_SESSKEY_PRESET_ID7, pbCipher, uiCipherLen);
		RV_FALSE_RETURN(bRtn);

		//bRtn = test_SM4FLASHnoValue(CKM_SM4_OFB, CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH预设密钥
		//RV_FALSE_RETURN(bRtn);

		bRtn=test_logout();
		RV_FALSE_RETURN(bRtn);

		bRtn=test_login(CKU_USER);
		RV_FALSE_RETURN(bRtn);
#endif
		bRtn=test_deleteobject(hPublicKey);
		RV_FALSE_RETURN(bRtn);

		bRtn=test_deleteobject(hPrivateKey);
		RV_FALSE_RETURN(bRtn);
	}

	//对称ECB导入对称
	//--------------使用刚才导入的RAM密钥导入另一个RAM密钥和FLASH密钥-------------------
	nSessKeyID = CK_SESSKEY_PRESET_ID0;
	rv = pC_CreateObject(hSession, SM4keyTemplate_RAM_en, sizeof(SM4keyTemplate_RAM_en)/sizeof(CK_ATTRIBUTE), &hKey_en);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject1,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_ECB, hKey_en);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	uiCipherLen = sizeof(pbCipher);
	rv=(*pC_Encrypt)(hSession, pbData, ulDataLen, pbCipher, &uiCipherLen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Encrypt,rv);

	rv = pC_CreateObject(hSession, SM4keyTemplate_RAM_unwrap, sizeof(SM4keyTemplate_RAM_unwrap)/sizeof(CK_ATTRIBUTE), &hKey_unwrap);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject2,rv);

	//导入RAM密钥
	bRtn = test_ImportKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKO_SECRET_KEY, CKK_SM4, FALSE, CK_SESSKEY_PRESET_ID2, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);
	
	bRtn = test_ImportKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKO_SECRET_KEY, CKK_SM4, FALSE, CK_SESSKEY_PRESET_ID3, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);
	
	//bRtn = test_ImportKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKO_SECRET_KEY, CKK_ZUC, FALSE, CK_SESSKEY_PRESET_ID4, pbCipher, uiCipherLen);
	//RV_FALSE_RETURN(bRtn);
	
	//导入FLASH密钥
	bRtn = test_ImportKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKO_SECRET_KEY, CKK_SM4, TRUE, CK_SESSKEY_PRESET_ID2, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);
	
	bRtn = test_ImportKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKO_SECRET_KEY, CKK_SM4, TRUE, CK_SESSKEY_PRESET_ID3, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);
	
	//bRtn = test_ImportKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKO_SECRET_KEY, CKK_ZUC, TRUE, CK_SESSKEY_PRESET_ID4, pbCipher, uiCipherLen);
	//RV_FALSE_RETURN(bRtn);
	////SM4加解密

	bRtn = test_SM4RAMnoValue(CKM_SM4_ECB, CK_SESSKEY_PRESET_ID2, CK_SESSKEY_PRESET_ID3);//RAM预设密钥
	RV_FALSE_RETURN(bRtn);

	bRtn = test_SM4FLASHnoValue(CKM_SM4_ECB, CK_SESSKEY_PRESET_ID2, CK_SESSKEY_PRESET_ID3);//FLASH预设密钥
	RV_FALSE_RETURN(bRtn);


	bRtn = test_SM4RAMnoValue(CKM_SM4_CBC, CK_SESSKEY_PRESET_ID2, CK_SESSKEY_PRESET_ID3);//RAM预设密钥
	RV_FALSE_RETURN(bRtn);
	
	bRtn = test_SM4FLASHnoValue(CKM_SM4_CBC, CK_SESSKEY_PRESET_ID2, CK_SESSKEY_PRESET_ID3);//FLASH预设密钥
	RV_FALSE_RETURN(bRtn); 


	bRtn = test_SM4RAMnoValue(CKM_SM4_OFB, CK_SESSKEY_PRESET_ID2, CK_SESSKEY_PRESET_ID3);//RAM预设密钥
	RV_FALSE_RETURN(bRtn);

	bRtn = test_SM4FLASHnoValue(CKM_SM4_OFB, CK_SESSKEY_PRESET_ID2, CK_SESSKEY_PRESET_ID3);//FLASH预设密钥
	RV_FALSE_RETURN(bRtn);
#ifdef OPEN_LOGOUT_CLOSESSION
	bRtn=test_logout();
	RV_FALSE_RETURN(bRtn);

	bRtn=test_login(CKU_SO);
	RV_FALSE_RETURN(bRtn);

	bRtn = test_ImportKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKO_SECRET_KEY, CKK_SM4, TRUE, CK_SESSKEY_PRESET_ID7, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);

	//bRtn = test_SM4FLASHnoValue(CKM_SM4_ECB, CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH预设密钥
	//RV_FALSE_RETURN(bRtn);


	bRtn = test_ImportKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKO_SECRET_KEY, CKK_SM4, TRUE, CK_SESSKEY_PRESET_ID7, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);

	//bRtn = test_SM4FLASHnoValue(CKM_SM4_CBC, CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH预设密钥
	//RV_FALSE_RETURN(bRtn);

	
	bRtn = test_ImportKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKO_SECRET_KEY, CKK_ZUC, TRUE, CK_SESSKEY_PRESET_ID7, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);

	//bRtn = test_SM4FLASHnoValue(CKM_SM4_OFB, CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH预设密钥
	//RV_FALSE_RETURN(bRtn);

	bRtn=test_logout();
	RV_FALSE_RETURN(bRtn);

	bRtn=test_login(CKU_USER);
	RV_FALSE_RETURN(bRtn);
#endif
	hKey_en = NULL_PTR;
	//对称CBC导入对称
	//-------------------使用刚才导入的FLASH密钥导入另一个RAM密钥和FLASH密钥-------------------
	nSessKeyID = CK_SESSKEY_PRESET_ID2;
	rv = pC_CreateObject(hSession, SM4keyTemplate_FLASH, sizeof(SM4keyTemplate_FLASH)/sizeof(CK_ATTRIBUTE), &hKey_en);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject1,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_CBC, hKey_en);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	uiCipherLen = sizeof(pbCipher);
	rv=(*pC_Encrypt)(hSession, pbData, ulDataLen, pbCipher, &uiCipherLen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Encrypt,rv);

	rv = pC_CreateObject(hSession, SM4keyTemplate_RAM_unwrap, sizeof(SM4keyTemplate_RAM_unwrap)/sizeof(CK_ATTRIBUTE), &hKey_unwrap);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject2,rv);

	//导入RAM密钥
	bRtn = test_ImportKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKO_SECRET_KEY, CKK_SM4, FALSE, CK_SESSKEY_PRESET_ID4, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);
	
	bRtn = test_ImportKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKO_SECRET_KEY, CKK_SM4, FALSE, CK_SESSKEY_PRESET_ID5, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);
	
	//bRtn = test_ImportKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKO_SECRET_KEY, CKK_ZUC, FALSE, CK_SESSKEY_PRESET_ID6, pbCipher, uiCipherLen);
	//RV_FALSE_RETURN(bRtn);
	
	//导入FLASH密钥
	bRtn = test_ImportKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKO_SECRET_KEY, CKK_SM4, TRUE, CK_SESSKEY_PRESET_ID4, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);
	
	bRtn = test_ImportKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKO_SECRET_KEY, CKK_SM4, TRUE, CK_SESSKEY_PRESET_ID5, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);
	
	//bRtn = test_ImportKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKO_SECRET_KEY, CKK_ZUC, TRUE, CK_SESSKEY_PRESET_ID6, pbCipher, uiCipherLen);
	//RV_FALSE_RETURN(bRtn);
	////SM4加解密
	bRtn = test_SM4RAMnoValue(CKM_SM4_ECB, CK_SESSKEY_PRESET_ID4, CK_SESSKEY_PRESET_ID5);//RAM预设密钥
	RV_FALSE_RETURN(bRtn);
	
	bRtn = test_SM4FLASHnoValue(CKM_SM4_ECB, CK_SESSKEY_PRESET_ID4, CK_SESSKEY_PRESET_ID5);//FLASH预设密钥
	RV_FALSE_RETURN(bRtn);


	bRtn = test_SM4RAMnoValue(CKM_SM4_CBC, CK_SESSKEY_PRESET_ID4, CK_SESSKEY_PRESET_ID5);//RAM预设密钥
	RV_FALSE_RETURN(bRtn);
	
	bRtn = test_SM4FLASHnoValue(CKM_SM4_CBC, CK_SESSKEY_PRESET_ID4, CK_SESSKEY_PRESET_ID5);//FLASH预设密钥
	RV_FALSE_RETURN(bRtn);


	bRtn = test_SM4RAMnoValue(CKM_SM4_OFB, CK_SESSKEY_PRESET_ID4, CK_SESSKEY_PRESET_ID5);//RAM预设密钥
	RV_FALSE_RETURN(bRtn);

	bRtn = test_SM4FLASHnoValue(CKM_SM4_OFB, CK_SESSKEY_PRESET_ID4, CK_SESSKEY_PRESET_ID5);//FLASH预设密钥
	RV_FALSE_RETURN(bRtn);
#ifdef OPEN_LOGOUT_CLOSESSION
	bRtn=test_logout();
	RV_FALSE_RETURN(bRtn);

	//登录管理员
	bRtn=test_login(CKU_SO);
	RV_FALSE_RETURN(bRtn);

	bRtn = test_ImportKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKO_SECRET_KEY, CKK_SM4, TRUE, CK_SESSKEY_PRESET_ID7, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);
	
	//bRtn = test_SM4FLASHnoValue(CKM_SM4_ECB, CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH预设密钥
	//RV_FALSE_RETURN(bRtn);


	bRtn = test_ImportKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKO_SECRET_KEY, CKK_SM4, TRUE, CK_SESSKEY_PRESET_ID7, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);

	//bRtn = test_SM4FLASHnoValue(CKM_SM4_CBC, CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH预设密钥
	//RV_FALSE_RETURN(bRtn);
	

	bRtn = test_ImportKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKO_SECRET_KEY, CKK_ZUC, TRUE, CK_SESSKEY_PRESET_ID7, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);
	
	//bRtn = test_SM4FLASHnoValue(CKM_SM4_OFB, CK_SESSKEY_PRESET_ID7, CK_SESSKEY_PRESET_ID7);//FLASH预设密钥
	//RV_FALSE_RETURN(bRtn);

	bRtn=test_logout();
	RV_FALSE_RETURN(bRtn);

	bRtn=test_login(CKU_USER);
	RV_FALSE_RETURN(bRtn);
#endif

	//对称CBC导入非对称
	//-------------------使用刚才导入的FLASH密钥导入非对称密钥-------------------
	nSessKeyID = CK_SESSKEY_PRESET_ID2;
	rv = pC_CreateObject(hSession, SM4keyTemplate_FLASH, sizeof(SM4keyTemplate_FLASH)/sizeof(CK_ATTRIBUTE), &hKey_en);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject1,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_CBC, hKey_en);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	uiCipherLen = sizeof(pbCipher);
	rv=(*pC_Encrypt)(hSession, pbPrvData, ulPrvDataLen, pbCipher, &uiCipherLen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Encrypt,rv);

	rv = pC_CreateObject(hSession, SM4keyTemplate_RAM_unwrap, sizeof(SM4keyTemplate_RAM_unwrap)/sizeof(CK_ATTRIBUTE), &hKey_unwrap);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject2,rv);

	bRtn = test_ImportKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKO_PRIVATE_KEY, CKK_ECC, TRUE, 0, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);

	//对称ECB导入非对称
	//-------------------使用刚才导入的FLASH密钥导入非对称密钥-------------------
	nSessKeyID = CK_SESSKEY_PRESET_ID2;
	//rv = pC_CreateObject(hSession, SM4keyTemplate_RAM_en, sizeof(SM4keyTemplate_RAM_en)/sizeof(CK_ATTRIBUTE), &hKey_en);
	rv = pC_CreateObject(hSession, SM4keyTemplate_FLASH, sizeof(SM4keyTemplate_FLASH)/sizeof(CK_ATTRIBUTE), &hKey_en);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject1,rv);

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_ECB, hKey_en);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_EncryptInit,rv);

	uiCipherLen = sizeof(pbCipher);
	rv=(*pC_Encrypt)(hSession, pbPrvData, ulPrvDataLen, pbCipher, &uiCipherLen);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_Encrypt,rv);

	rv = pC_CreateObject(hSession, SM4keyTemplate_RAM_unwrap, sizeof(SM4keyTemplate_RAM_unwrap)/sizeof(CK_ATTRIBUTE), &hKey_unwrap);
	RV_NOT_OK_RETURN_FALSE(Testresult[nItemNumb],&nResultLength,pC_CreateObject2,rv);

	bRtn = test_ImportKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKO_PRIVATE_KEY, CKK_ECC, TRUE, 0, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(bRtn);
	
	return TRUE;
END:

	
	return FALSE;
}
