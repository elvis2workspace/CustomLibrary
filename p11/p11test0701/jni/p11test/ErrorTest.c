
#include <assert.h>
#include "general.h"
#include <stdarg.h>
#include "ErrorTest.h"
#include "sm2.h"
#include "sm4ofb.h"
//#include "Utils.h"


#include "Utils_c.h"
#include "GeneralCall.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define SM2_EN_DATALEN 128		 //SM2加密最大长度
#define SM2_DE_DATALEN 128+96 //SM2加解最大长度+96
#define SM2_SI_IN_DATALEN 32
#define SM2_SI_OUT_DATALEN 32+32//SM2签名验签固定长度 
#define RUN_CORRECT 0			//generalcall模块正常运行调用具体模块参数
#define RUN_INCORRECT 1			//generalcall模块正常运行调用具体模块参数
#define ENTER_FUNCTION 0		//控制显示进入函数
#define LEAVE_FUNCTION 0		//控制显示离开函数
#define SECRET_KEY_LEN 16    //对称密钥的长度

//调用一次函数free多少个内存指针
#define FREE_0	0
#define FREE_1	1
#define FREE_2	2
#define FREE_3	3
#define FREE_4	4
#define FREE_5	5
#define FREE_6	6
#define FREE_7	7
#define FREE_8	8
#define FREE_9	9




#define VNAME(name) (#name), name
#define PRINT_LINE printf("--------------------------------------------------------------------------\n")
void PrintDataNew(const char *title, const void *data, unsigned int size, unsigned int offset)
{
	if (!data || !size)
	{
		return;
	}

	printf("%s[%d]{%d-%d}:\n", title, size, offset, offset + size - 1);
	PRINT_LINE;
	printf("   ADDR                          HEX                            DATA      \n");
	PRINT_LINE;

	int i, j, iterator;
	int row = (size + 15) >> 4, col = (size) & 15;
	for (i = 0; i < row; i++)
	{
		printf("0x%08X:      ", (unsigned int)data + offset + (i << 4));
		for (j = 0; j < ((i == row - 1) ? (((col == 0) ? 16 : col)) : (16)); j++)
		{
			if (j && !(j & 3))
			{
				printf(" ");
			}
			iterator = ((unsigned char*)data)[(i << 4) + j + offset];
			printf("%02X", iterator);
		}
		printf("%*c", 6, 0x20);

		if (i == row - 1 && col)
		{
			printf("%*c", 36 - (col << 1) - ((col + 3) >> 2), 0x20);
		}

		for (j = 0; j < ((i == row - 1) ? (((col == 0) ? 16 : col)) : (16)); j++)
		{
			iterator = ((unsigned char*)data)[(i << 4) + j + offset];

			printf("%c", ((iterator >= 0x20 && iterator <= 0x80)? iterator : 0x2E));
		}
		printf("\n");
	}

	PRINT_LINE;
}


CK_RV Show_Result(char* func_name, int i, const char* run_type, CK_RV rtn , char* storage_start, int* storage_address)
{
	if((func_name == NULL)||(run_type == NULL)||(storage_start == NULL)||(storage_address == NULL_PTR))
	{
		printf("Error:Some Input == NULL\n");
		return FALSE;
	}

	if(*storage_address >9000)
	{
		printf("Error:Storage may overflow!\n");
		return FALSE;
	}

	if(i == 0)
	{
		if((rtn != CKR_OK) && (rtn != 0x90000000))
		{
#ifdef USE_PROXY_OPEN
			printf("%s:<%s> Error(rv=0x%08x:%s)\n", func_name, run_type, rtn, pC_StrErr(rtn));
			*storage_address += sprintf(storage_start + *storage_address, "%s:<%s> Error(rv=0x%08x:%s)<br>", func_name, run_type, rtn, pC_StrErr(rtn));
#else
			printf("%s:<%s> Error(rv=0x%08x)\n", func_name, run_type, rtn);
			*storage_address += sprintf(storage_start + *storage_address, "%s:<%s> Error(rv=0x%08x)<br>", func_name, run_type, rtn);
#endif
			return FALSE;
		}
		return TRUE;
	}

	if(rtn != 0x00000091)
	{
		if((rtn != CKR_OK) && (rtn != 0x90000000))
		{
#ifdef SHOW_ERROR_TEST
#ifdef USE_PROXY_OPEN
			printf("%s:", func_name);
			printf("<%s>should failed",  run_type);
			printf("(rv=0x%08x",  rtn);
			printf(":%s)\n",  pC_StrErr(rtn));
			*storage_address += sprintf(storage_start + *storage_address, "%s:<%s> should failed(rv=0x%08x:%s)<br>", func_name, run_type, rtn, pC_StrErr(rtn));
#else
			printf("%s:<%s> should failed(rv=0x%08x)\n", func_name, run_type, rtn);
			*storage_address += sprintf(storage_start + *storage_address, "%s:<%s> should failed(rv=0x%08x)<br>", func_name, run_type, rtn);
#endif
#endif
			return TRUE;
		}
		else
		{
			printf("%s may succeed <%s>? rv=0x%08x\n", func_name, run_type, rtn);
			*storage_address += sprintf(storage_start + *storage_address, "%s may succeed <%s>? rv=0x%08x<br>", func_name, run_type, rtn);
			return FALSE;
		}
	}
}

CK_RV Enter_Function(char* func_name, char* storage_start, int* storage_address)
{	
	if((func_name == NULL)||(storage_start == NULL)||(storage_address == NULL_PTR))
	{
		printf("Error:Some Input == NULL\n");
		return FALSE;
	}
	
	if(*storage_address >9000)
	{
		printf("Error:Storage may overflow!\n");
		return FALSE;
	}
	printf("Enter function [%s].\n", func_name);
	*storage_address += sprintf(storage_start + *storage_address, "Enter function [%s].<br>", func_name);
	return TRUE;

}

CK_RV Leave_Function(char* func_name, char* storage_start, int* storage_address)
{	
	if((func_name == NULL)||(storage_start == NULL)||(storage_address == NULL_PTR))
	{
		printf("Error:Some Input == NULL\n");
		return FALSE;
	}
	
	if(*storage_address >9000)
	{
		printf("Error:Storage may overflow!\n");
		return FALSE;
	}
	printf("Leave function [%s].\n", func_name);
	*storage_address += sprintf(storage_start + *storage_address, "Leave function [%s].<br>", func_name);
	return TRUE;
}


CK_RV Result_Compare(BYTE* outdata, int outdatalen, BYTE* srcdata, int srcdatalen, char* storage_start, int* storage_address)
{
	if((outdata == NULL_PTR)||(outdatalen == 0)||(srcdata == NULL_PTR)||(srcdatalen == 0)||(storage_start == NULL_PTR)||(storage_address == NULL_PTR))
	{
		printf("Error:Some Input == NULL\n");
		return FALSE;
	}

	if(*storage_address >9000)
	{
		printf("Error:Storage may overflow!\n");
		return FALSE;
	}

	if(outdatalen != srcdatalen)
	{
		printf("Error: Datalen not Match.\n");
		*storage_address += sprintf(storage_start + *storage_address, "Error: Datalen not Match.<br>");
		return FALSE;
	}

	if(memcmp(outdata, srcdata, outdatalen))
	{
		printf("Error: Data not Match.\n");
		*storage_address += sprintf(storage_start + *storage_address, "Error: Data not Match.<br>");
		return FALSE;
	}

	return TRUE;
}

CK_RV Rv_False_Free_Memory(unsigned int _rv, unsigned int _free_nums, ...) //最多支持9个参数free,传入内存指针的指针
{
	va_list arg_ptr;
	void** p[9] = { 0 };
	int i = 0;

	
	if (_rv == TRUE)
	{
		return TRUE;
	}

	va_start(arg_ptr, _free_nums);

	if (_free_nums >= 10)
	{
		printf("Free numbers overflow!\n");
		return FALSE;
	}

	for (i = 0; i < _free_nums; ++i)
	{
		p[i] = va_arg(arg_ptr, void**);
		free(*p[i]);
		*p[i] = NULL;
	}
	va_end(arg_ptr);
	return FALSE;
}

CK_RV Free_Memory(unsigned int _free_nums, ...)
{
	va_list arg_ptr;
	void** p[9] = { 0 };
	int i = 0;
	
	if (_free_nums >= 10)
	{
		printf("Free numbers overflow!\n");
		return FALSE;
	}

	va_start(arg_ptr, _free_nums);

	for (i = 0; i < _free_nums; ++i)
	{

		p[i] = va_arg(arg_ptr, void**);
		free(*p[i]);
		*p[i] = NULL_PTR;
	}

	va_end(arg_ptr);
	return TRUE;
}

CK_RV etest_ZUCEncrypt()
{ 
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;	
	int i = 0;

	unsigned char	ZUCkeyVal_Enc[] = {0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};
	unsigned char	ZUCplain_Enc[100] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d};
	unsigned char	ZUCcipher_Enc[100] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, 0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01};
	unsigned char	ZUCiv_Enc[] = {0x00, 0x05, 0x68, 0x23, 0x38};

	CK_BYTE nSessKeyID_Dec = CK_SESSKEY_PRESET_ID0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_ATTRIBUTE ZUCkeyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,ZUCkeyVal_Enc,sizeof(ZUCkeyVal_Enc)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(nSessKeyID_Dec)}
	};

	CK_MECHANISM ZUCmechanism_Enc = {CKM_ZUC_CALC, ZUCiv_Enc, sizeof(ZUCiv_Enc)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen = sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen = sizeof(outdata);

	indatalen = sizeof(indata);
	memset(indata, 0, indatalen);
	memcpy(indata, ZUCiv_Enc, sizeof(ZUCiv_Enc));
	memcpy(indata+sizeof(ZUCiv_Enc), ZUCplain_Enc, sizeof(ZUCplain_Enc));
	indatalen = sizeof(ZUCiv_Enc) + sizeof(ZUCplain_Enc);
	
	bRtn = Enter_Function("etest_ZUCEncrypt", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(bRtn);

	char* Char_CreateObject[5]={"Run Correct", "hSession=NULL", "ZUCkeyTemplate=NULL", "Count=NULL", "&hKey_Enc=NULL"};
	for (i = 4; i >= 0; --i)
	{
		hKey_Enc = NULL_PTR;
		rv = generalcall(pC_CreateObject, i, hSession, ZUCkeyTemplate_Enc, 5, &hKey_Enc);
		bRtn = Show_Result("CreateObject", i, Char_CreateObject[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(bRtn);
	}

	char* Char_EncryptInit[4]={"Run Correct", "hSession=NULL", "&ZUCmechanism=NULL", "hKey_Enc=NULL"};
	for (i = 3; i >= 0; --i)
	{
		rv = generalcall(pC_EncryptInit, i, hSession, &ZUCmechanism_Enc, hKey_Enc);
		bRtn = Show_Result("EncryptInit",i, Char_EncryptInit[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(bRtn);
	}	

	char* Char_EncryptUpdate[6]={"Run Correct", "hSession=NULL", "indata=NULL", "indatalen=NULL", "outdata=NULL" ,"&outdatalen=NULL"};
	for (i = 5; i >= 0; --i)
	{
		if(i != 4)
		{
			//传空取长是正确用法,屏蔽4
			rv = generalcall(pC_EncryptUpdate, i, hSession, indata, indatalen, outdata, &outdatalen);
			bRtn = Show_Result("EncryptUpdate",i, Char_EncryptUpdate[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(bRtn);
			
			//if((rv!=CKR_OK)&&(rv != CKR_BUFFER_TOO_SMALL))
			//{//前置状态需要重新初始化
			//	rv = generalcall(pC_EncryptInit, RUN_CORRECT, hSession, &ZUCmechanism_Enc, hKey_Enc);
			//	bRtn = Show_Result("EncryptInit",RUN_CORRECT, "re_init", rv, Testresult[nItemNumb], &nResultLength);
			//	RV_FALSE_RETURN(bRtn);
			//}			
		}
	}
	bRtn = Result_Compare(outdata, outdatalen, ZUCcipher_Enc, sizeof(ZUCcipher_Enc), Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(bRtn);
	
	char* Char_EncryptFinal[4]={"Run Correct", "hSession=NULL", "outdatalen=NULL" ,"&outdatalen=NULL"};
	for (i = 3; i >= 0; --i)
	{
		if(i != 2)
		{

			//传空取长是正确用法,屏蔽2
			rv = generalcall(pC_EncryptFinal, i, hSession, outdata, &outdatalen);
			bRtn = Show_Result("EncryptFinal",i, Char_EncryptFinal[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(bRtn);

			if((rv == CKR_OK)&&(i!=0))
			{	
				printf("1111\n");
				//前置状态需要重新初始化
				rv = generalcall(pC_EncryptInit, RUN_CORRECT, hSession, &ZUCmechanism_Enc, hKey_Enc);
				bRtn = Show_Result("EncryptInit",RUN_CORRECT, "re_init", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(bRtn);

				//前置状态需要重新运行
				rv = generalcall(pC_EncryptUpdate, RUN_CORRECT, hSession, indata, indatalen, outdata, &outdatalen);
				bRtn = Show_Result("pC_EncryptUpdate", RUN_CORRECT, "re_update", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(bRtn);
			}
		}
	}

	char* Char_DestroyObject[3]={"Run Correct", "hSession=NULL", "hKey_Enc=NULL"};
	for (i = 2; i >= 0; --i)
	{
		rv = generalcall(pC_DestroyObject, i, hSession, hKey_Enc);
		bRtn = Show_Result("DestroyObject",i, Char_DestroyObject[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(bRtn);
	}
	
	bRtn = Leave_Function("etest_ZUCEncrypt", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(bRtn);

	bRtn = TRUE;
	
	return bRtn;
}

CK_RV Destory_Useless_KeyPair(CK_OBJECT_HANDLE hPublicKey, CK_OBJECT_HANDLE PrivateKey, char* storage_start, int* storage_address)
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;

	if(hPublicKey != NULL_PTR)
	{
		rv = pC_DestroyObject(hSession, hPublicKey);
		bRtn = Show_Result("DestroyObject",RUN_CORRECT, "hPublicKey", rv, storage_start, storage_address);
		RV_FALSE_RETURN(bRtn);
	}

	if(PrivateKey != NULL_PTR)
	{
		rv = pC_DestroyObject(hSession, PrivateKey);
		bRtn = Show_Result("DestroyObject",RUN_CORRECT, "PrivateKey", rv, storage_start, storage_address);
		RV_FALSE_RETURN(bRtn);
	}

	bRtn = TRUE;
	
	return bRtn;
}


CK_RV user_to_so()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;
	int i = 0;

	rv = (*pC_Logout)(hSession);
	if(rv ==CKR_USER_NOT_LOGGED_IN)
	{
		rv = CKR_OK;
	}
	bRtn = Show_Result("pC_Logout", RUN_CORRECT, "user_to_so", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));


	rv=(*pC_Login)(hSession,CKU_SO,adminpin,strlen(adminpin));
	bRtn = Show_Result("pC_Login", RUN_CORRECT, "user_to_so", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//printf("user_to_so OK!\n");
	bRtn = TRUE;
	
	return bRtn;
}

CK_RV so_to_user()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;
	int i = 0;

	rv=(*pC_Logout)(hSession);
	if(rv ==CKR_USER_NOT_LOGGED_IN)
	{
		rv = CKR_OK;
	}
	bRtn = Show_Result("pC_Logout", RUN_CORRECT, "so_to_user", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));


	rv=(*pC_Login)(hSession,CKU_USER,userpin,strlen(userpin));
	bRtn = Show_Result("pC_Login", RUN_CORRECT, "so_to_user", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//printf("so_to_user OK!\n");
	bRtn = TRUE;
	
	return bRtn;
}

CK_RV UnwrapKeyOnce(CK_MECHANISM* pUnwrapMechanism, CK_OBJECT_HANDLE hUnwrapKey, CK_BYTE* pbCipher, unsigned int uiCipherLen,CK_BYTE nSessKeyID ,CK_BBOOL TokenValue, CK_BBOOL EncryptValue, CK_BBOOL DecryptValue, CK_BBOOL WrapValue, CK_BBOOL UnwrapValue)
{
	bool bRtn = false;
	CK_RV rv = 0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_OBJECT_HANDLE hKey = NULL_PTR;
	CK_MECHANISM mechanismGen = {CKM_SM4_KEY_GEN, NULL_PTR, 0};

	CK_ATTRIBUTE ImportKeyTemplate[] = 
	{
		{CKA_CLASS,			&keyClass,		sizeof(keyClass)},
		{CKA_TOKEN,			&TokenValue,	sizeof(TokenValue)},
		{CKA_KEY_TYPE,		&SM4keyType,	sizeof(SM4keyType)},
		{CKA_ENCRYPT,		&EncryptValue,	sizeof(EncryptValue)},
		{CKA_DECRYPT,		&DecryptValue,	sizeof(DecryptValue)},
		{CKA_WRAP,			&WrapValue,		sizeof(WrapValue)},
		{CKA_UNWRAP,		&UnwrapValue,	sizeof(UnwrapValue)},
		{CKA_SESSKEY_ID,	&nSessKeyID,	sizeof(nSessKeyID)}
	};

	int Run_Flag = 0;

	Run_Flag = RUN_CORRECT;

	if((TokenValue == FALSE)&&(nSessKeyID == CK_SESSKEY_PRESET_ID7))
	{
		Run_Flag = RUN_INCORRECT;
	}

	if((EncryptValue == TRUE)&&(UnwrapValue == TRUE))
	{
		Run_Flag = RUN_INCORRECT;
	}

	if((DecryptValue == TRUE)&&(WrapValue == TRUE))
	{
		Run_Flag = RUN_INCORRECT;
	}

	rv = pC_UnwrapKey(hSession, pUnwrapMechanism, hUnwrapKey, pbCipher, uiCipherLen, ImportKeyTemplate, sizeof(ImportKeyTemplate)/sizeof(CK_ATTRIBUTE), &hKey);
	bRtn = Show_Result("pC_UnwrapKey",Run_Flag, "UnwrapKeyOnce", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	
	return Free_Memory(FREE_0);
}



CK_RV test_UnwrapKeyWithCipher(CK_MECHANISM_TYPE mechUnwrapType, unsigned char *pbIV, unsigned int uiIVLen, CK_OBJECT_HANDLE hUnwrapKey, CK_KEY_TYPE keyType, unsigned char *pbCipher, unsigned int uiCipherLen)
{
	CK_RV bRtn = FALSE;
	CK_RV rv = -1;
	int ii = 0;
	int i = 0, j = 0, n = 0, m = 0, p = 0, q = 0;

	CK_BBOOL TokenValue[8]	=	{ CK_TRUE, CK_FALSE};
	CK_BBOOL WrapValue[8]	=	{ CK_TRUE, CK_FALSE};
	CK_BBOOL EncryptValue[8] =	{ CK_TRUE, CK_FALSE};
	CK_BBOOL DecryptValue[8] =	{ CK_TRUE, CK_FALSE};
	CK_BBOOL UnwrapValue[8] =	{ CK_TRUE, CK_FALSE};

	CK_BYTE     temp_pub_x[32] = {
		0xec,0x91,0x81,0x8d,0xe0,0xb7,0x01,0x21,0x73,0xf5,0x1c,0x33,0x75,0x43,0x6e,0x43,
		0xb6,0xa9,0xa2,0x6a,0xbd,0x6d,0xbc,0xb7,0x9f,0x85,0x1c,0xde,0xaf,0x7a,0x0f,0x6c
	};
	CK_BYTE     temp_pub_y[32] = {
		0xcb,0xf4,0xb5,0xa1,0x5f,0xb8,0x7e,0x60,0xfc,0x0b,0x3a,0x92,0x3d,0x12,0xe8,0x66,
		0x36,0x4a,0x93,0x5f,0xfb,0x30,0x84,0x2b,0xc9,0x13,0x9e,0xbd,0x2d,0xdc,0xe9,0x61
	};

	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;

	CK_OBJECT_CLASS pubclass = CKO_PUBLIC_KEY;
	CK_OBJECT_CLASS priclass = CKO_PRIVATE_KEY;
	CK_UINT     modulusBits = 256;
	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_UNWRAP, &ttrue, sizeof(ttrue)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
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
	};

	CK_MECHANISM  mechanismcalc = {CKM_ECC_CALC, NULL_PTR, 0};

	CK_MECHANISM UnwrapMechanism = {mechUnwrapType, pbIV, uiIVLen};
	CK_BYTE_PTR   pData = (CK_BYTE_PTR)"12345678901234567890123456789012";
	CK_ULONG      ulDataLen = strlen((char*)pData);
	CK_BYTE       pOutData[256];   
	CK_ULONG      ulOutDataLen = sizeof(pOutData);

	CK_OBJECT_HANDLE hKey = NULL_PTR;
	CK_OBJECT_HANDLE hPubKey = NULL_PTR;

	if (keyType == CKK_ECC)
	{
		rv = (*pC_UnwrapKey)(hSession, &UnwrapMechanism, hUnwrapKey, pbCipher, uiCipherLen, privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE), &hKey);
		bRtn = Show_Result("pC_UnwrapKey",RUN_CORRECT, "test_UnwrapKeyWithCipher", rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		rv = (*pC_CreateObject)(hSession,publicKeyTemplate,sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE), &hPubKey);
		bRtn = Show_Result("pC_CreateObject",RUN_CORRECT, "test_UnwrapKeyWithCipher", rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		rv=(*pC_SignInit)(hSession,&mechanismcalc, hKey);
		bRtn = Show_Result("pC_SignInit",RUN_CORRECT, "test_UnwrapKeyWithCipher", rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		rv=(*pC_Sign)(hSession, pData, ulDataLen, pOutData, &ulOutDataLen);
		bRtn = Show_Result("pC_Sign",RUN_CORRECT, "test_UnwrapKeyWithCipher", rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		rv=(*pC_VerifyInit)(hSession,&mechanismcalc, hPubKey);
		bRtn = Show_Result("pC_VerifyInit",RUN_CORRECT, "test_UnwrapKeyWithCipher", rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		rv=(*pC_Verify)(hSession, pData, ulDataLen, pOutData, ulOutDataLen);
		bRtn = Show_Result("pC_Verify",RUN_CORRECT, "test_UnwrapKeyWithCipher", rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		bRtn =Destory_Useless_KeyPair(hKey, hPubKey,Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}
	else
	{
		q = 0;
		for(i=0;i<2;++i)
		{
			for(j=0;j<2;++j)
			{
				for(m=0;m<2;++m)
				{
					for(n=0;n<2;++n)
					{
						for(p=0;p<2;++p)
						{
							//printf("TRUE=%x,FALSE=%x\n",TRUE,FALSE);
							//printf("TokenValue = %x,EncryptValue = %x,DecryptValue = %x,WrapValue = %x,UnwrapValue = %x,count =%d\n",TokenValue[p], EncryptValue[n], DecryptValue[m], WrapValue[j], UnwrapValue[i],q);
							bRtn = UnwrapKeyOnce(&UnwrapMechanism,hUnwrapKey, pbCipher, uiCipherLen,CK_SESSKEY_PRESET_ID0, TokenValue[p], EncryptValue[n], DecryptValue[m], WrapValue[j], UnwrapValue[i]);
							RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));					
							++q;
						}
					}
				}
			}
		}
#ifdef OPEN_LOGOUT_CLOSESSION
		bRtn = user_to_so();
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
		q = 0;
		for(i=0;i<2;++i)
		{
			for(j=0;j<2;++j)
			{
				for(m=0;m<2;++m)
				{
					for(n=0;n<2;++n)
					{
						for(p=0;p<2;++p)
						{
							//printf("TRUE=%x,FALSE=%x\n",TRUE,FALSE);
							//printf("TokenValue = %x,EncryptValue = %x,DecryptValue = %x,WrapValue = %x,UnwrapValue = %x,count =%d\n",TokenValue[p], EncryptValue[n], DecryptValue[m], WrapValue[j], UnwrapValue[i],q);
							bRtn = UnwrapKeyOnce(&UnwrapMechanism,hUnwrapKey, pbCipher, uiCipherLen,CK_SESSKEY_PRESET_ID7, TokenValue[p], EncryptValue[n], DecryptValue[m], WrapValue[j], UnwrapValue[i]);
							RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));					
							++q;
						}
					}
				}
			}
		}
		bRtn = so_to_user();
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
#endif
	}

	
	return Free_Memory(FREE_0);
}


CK_RV test_UnwrapKey()
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

	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_BYTE nSessKeyID1 = CK_SESSKEY_PRESET_ID1;
	CK_BYTE nSessKeyID2 = CK_SESSKEY_PRESET_ID2;
	CK_BYTE Key_Value[16] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	CK_ATTRIBUTE SM4keyTemplate_en[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE, Key_Value, sizeof(Key_Value)},
		{CKA_SESSKEY_ID, &nSessKeyID1, sizeof(CK_BYTE)}
	};

	CK_ATTRIBUTE SM4keyTemplate_unwrap[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_UNWRAP, &ttrue, sizeof(ttrue)},
		{CKA_VALUE, Key_Value, sizeof(Key_Value)},
		{CKA_SESSKEY_ID, &nSessKeyID2, sizeof(CK_BYTE)}
	};

	unsigned char	SM4iv_Enc[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_MECHANISM SM4mechanism_ECB = {CKM_SM4_ECB, NULL, 0};
	CK_MECHANISM SM4mechanism_CBC = {CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_MECHANISM SM4mechanism_OFB = {CKM_SM4_OFB, SM4iv_Enc, sizeof(SM4iv_Enc)};
	CK_OBJECT_HANDLE hKey = NULL_PTR;
	CK_OBJECT_HANDLE hKey_en = NULL_PTR;
	CK_OBJECT_HANDLE hKey_unwrap = NULL_PTR;
	CK_RV rv = -1;

	bRtn = Enter_Function("test_UnwrapKey", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(bRtn);
	//非对称导入对称
	hPublicKey = NULL_PTR;
	hPrivateKey = NULL_PTR;
	bRtn=test_ECC_importKeyPair(&hPublicKey,&hPrivateKey);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv=(*pC_EncryptInit)(hSession,&mechanismcalc,hPublicKey);
	bRtn = Show_Result("pC_EncryptInit",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv=(*pC_Encrypt)(hSession, pbData, ulDataLen, pbCipher, &uiCipherLen);
	bRtn = Show_Result("pC_Encrypt",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = test_UnwrapKeyWithCipher(CKM_ECC_CALC, NULL, 0, hPrivateKey, CKK_SM4, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = test_UnwrapKeyWithCipher(CKM_ECC_CALC, NULL, 0, hPrivateKey, CKK_ZUC, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn =Destory_Useless_KeyPair(hPublicKey, hPrivateKey,Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	
	//对称ECB导入对称
	hKey_en = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_en, sizeof(SM4keyTemplate_en)/sizeof(CK_ATTRIBUTE), &hKey_en);
	bRtn = Show_Result("pC_CreateObjectECB",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_ECB, hKey_en);
	bRtn = Show_Result("pC_EncryptInitECB",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	uiCipherLen = sizeof(pbCipher);
	rv=(*pC_Encrypt)(hSession, pbData, ulDataLen, pbCipher, &uiCipherLen);
	bRtn = Show_Result("pC_EncryptECB",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = pC_CreateObject(hSession, SM4keyTemplate_unwrap, sizeof(SM4keyTemplate_unwrap)/sizeof(CK_ATTRIBUTE), &hKey_unwrap);
	bRtn = Show_Result("pC_CreateObjectunwrapECB",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = test_UnwrapKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKK_SM4, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = test_UnwrapKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKK_ZUC, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//对称CBC导入对称
	hKey_en = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_en, sizeof(SM4keyTemplate_en)/sizeof(CK_ATTRIBUTE), &hKey_en);
	bRtn = Show_Result("pC_CreateObjectCBC1",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_CBC, hKey_en);
	bRtn = Show_Result("pC_EncryptInitCBC1",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	uiCipherLen = sizeof(pbCipher);
	rv=(*pC_Encrypt)(hSession, pbData, ulDataLen, pbCipher, &uiCipherLen);
	bRtn = Show_Result("pC_EncryptCBC1",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = pC_CreateObject(hSession, SM4keyTemplate_unwrap, sizeof(SM4keyTemplate_unwrap)/sizeof(CK_ATTRIBUTE), &hKey_unwrap);
	bRtn = Show_Result("pC_CreateObjectunwrapCBC1",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = test_UnwrapKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKK_SM4, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = test_UnwrapKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKK_ZUC, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//对称CBC导入非对称
	hKey_en = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_en, sizeof(SM4keyTemplate_en)/sizeof(CK_ATTRIBUTE), &hKey_en);
	bRtn = Show_Result("pC_CreateObjectCBC2",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_CBC, hKey_en);
	bRtn = Show_Result("pC_EncryptInitCBC2",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	uiCipherLen = sizeof(pbCipher);
	rv=(*pC_Encrypt)(hSession, pbPrvData, ulPrvDataLen, pbCipher, &uiCipherLen);
	bRtn = Show_Result("pC_EncryptCBC2",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = pC_CreateObject(hSession, SM4keyTemplate_unwrap, sizeof(SM4keyTemplate_unwrap)/sizeof(CK_ATTRIBUTE), &hKey_unwrap);
	bRtn = Show_Result("pC_CreateObjectunwrapCBC2",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = test_UnwrapKeyWithCipher(CKM_SM4_CBC, SM4iv_Enc, sizeof(SM4iv_Enc), hKey_unwrap, CKK_ECC, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//对称ECB导入非对称
	hKey_en = NULL_PTR;
	rv = pC_CreateObject(hSession, SM4keyTemplate_en, sizeof(SM4keyTemplate_en)/sizeof(CK_ATTRIBUTE), &hKey_en);
	bRtn = Show_Result("pC_CreateObjectECB2",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism_ECB, hKey_en);
	bRtn = Show_Result("pC_EncryptInitECB2",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	uiCipherLen = sizeof(pbCipher);
	rv=(*pC_Encrypt)(hSession, pbPrvData, ulPrvDataLen, pbCipher, &uiCipherLen);
	bRtn = Show_Result("pC_EncryptECB2",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = pC_CreateObject(hSession, SM4keyTemplate_unwrap, sizeof(SM4keyTemplate_unwrap)/sizeof(CK_ATTRIBUTE), &hKey_unwrap);
	bRtn = Show_Result("pC_CreateObjectunwrapECB2",RUN_CORRECT, "test_UnwrapKey", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = test_UnwrapKeyWithCipher(CKM_SM4_ECB, NULL, 0, hKey_unwrap, CKK_ECC, pbCipher, uiCipherLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = Leave_Function("test_UnwrapKey", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(bRtn);


	
	return Free_Memory(FREE_0);
}

CK_RV etest_ZUCDecrypt()
{
	CK_RV bRtn = FALSE;
	int i = 0;
	CK_RV rv = 0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_ZUC;
	CK_BYTE nSessKeyID_Dec = CK_SESSKEY_PRESET_ID0;
	unsigned char	ZUCkeyVal_Dec[] = {0xe5, 0xbd, 0x3e, 0xa0, 0xeb, 0x55, 0xad, 0xe8, 0x66, 0xc6, 0xac, 0x58, 0xbd, 0x54, 0x30, 0x2a};
	unsigned char	ZUCplain_Dec[100] = {0x14, 0xa8, 0xef, 0x69, 0x3d, 0x67, 0x85, 0x07, 0xbb, 0xe7, 0x27, 0x0a, 0x7f, 0x67, 0xff, 0x50, 0x06, 0xc3, 0x52, 0x5b, 0x98, 0x07, 0xe4, 0x67, 0xc4, 0xe5, 0x60, 0x00, 0xba, 0x33, 0x8f, 0x5d, 0x42, 0x95, 0x59, 0x03, 0x67, 0x51, 0x82, 0x22, 0x46, 0xc8, 0x0d, 0x3b, 0x38, 0xf0, 0x7f, 0x4b, 0xe2, 0xd8, 0xff, 0x58, 0x05, 0xf5, 0x13, 0x22, 0x29, 0xbd, 0xe9, 0x3b, 0xbb, 0xdc, 0xaf, 0x38, 0x2b, 0xf1, 0xee, 0x97, 0x2f, 0xbf, 0x99, 0x77, 0xba, 0xda, 0x89, 0x45, 0x84, 0x7a, 0x2a, 0x6c, 0x9a, 0xd3, 0x4a, 0x66, 0x75, 0x54, 0xe0, 0x4d, 0x1f, 0x7f, 0xa2, 0xc3, 0x32, 0x41, 0xbd, 0x8f, 0x01, 0xba, 0x22, 0x0d};
	unsigned char	ZUCcipher_Dec[100] = {0x13, 0x1d, 0x43, 0xe0, 0xde, 0xa1, 0xbe, 0x5c, 0x5a, 0x1b, 0xfd, 0x97, 0x1d, 0x85, 0x2c, 0xbf, 0x71, 0x2d, 0x7b, 0x4f, 0x57, 0x96, 0x1f, 0xea, 0x32, 0x08, 0xaf, 0xa8, 0xbc, 0xa4, 0x33, 0xf4, 0x56, 0xad, 0x09, 0xc7, 0x41, 0x7e, 0x58, 0xbc, 0x69, 0xcf, 0x88, 0x66, 0xd1, 0x35, 0x3f, 0x74, 0x86, 0x5e, 0x80, 0x78, 0x1d, 0x20, 0x2d, 0xfb, 0x3e, 0xcf, 0xf7, 0xfc, 0xbc, 0x3b, 0x19, 0x0f, 0xe8, 0x2a, 0x20, 0x4e, 0xd0, 0xe3, 0x50, 0xfc, 0x0f, 0x6f, 0x26, 0x13, 0xb2, 0xf2, 0xbc, 0xa6, 0xdf, 0x5a, 0x47, 0x3a, 0x57, 0xa4, 0xa0, 0x0d, 0x98, 0x5e, 0xba, 0xd8, 0x80, 0xd6, 0xf2, 0x38, 0x64, 0xa0, 0x7b, 0x01};
	unsigned char	ZUCiv_Dec[] = {0x00, 0x05, 0x68, 0x23, 0x38};

	CK_ATTRIBUTE ZUCkeyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,ZUCkeyVal_Dec,sizeof(ZUCkeyVal_Dec)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(nSessKeyID_Dec)}
	};

	CK_MECHANISM ZUCmechanism_Dec = {CKM_ZUC_CALC, ZUCiv_Dec, sizeof(ZUCiv_Dec)};
	CK_OBJECT_HANDLE hKey_Dec = NULL_PTR;

	CK_BYTE indata1[256] = {0};
	CK_ULONG indatalen1=sizeof(indata1);
	CK_BYTE outdata1[256] = {0};
	CK_ULONG outdatalen1=sizeof(outdata1);

	indatalen1 = sizeof(indata1);
	memset(indata1, 0, indatalen1);
	memcpy(indata1, ZUCiv_Dec, sizeof(ZUCiv_Dec));
	memcpy(indata1+sizeof(ZUCiv_Dec), ZUCcipher_Dec, sizeof(ZUCcipher_Dec));
	indatalen1 = sizeof(ZUCiv_Dec) + sizeof(ZUCcipher_Dec);

	bRtn = Enter_Function("etest_ZUCDecrypt", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(bRtn);

	char* Char_CreateObject[5]={"Run Correct", "hSession=NULL", "ZUCkeyTemplate=NULL", "Count=NULL", "&hKey_Dec=NULL"};
	for (i = 4; i >= 0; --i)
	{
		hKey_Dec = NULL_PTR;
		rv = generalcall(pC_CreateObject, i, hSession, ZUCkeyTemplate_Dec, 5, &hKey_Dec);
		bRtn = Show_Result("CreateObject", i, Char_CreateObject[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(bRtn);
	}

	char* Char_DecryptInit[4]={"Run Correct", "hSession=NULL", "&ZUCmechanism=NULL", "hKey_Dec=NULL"};
	for (i = 3; i >= 0; --i)
	{
		rv = generalcall(pC_DecryptInit, i, hSession, &ZUCmechanism_Dec, hKey_Dec);
		bRtn = Show_Result("DecryptInit",i, Char_DecryptInit[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(bRtn);
	}

	char* Char_DecryptUpdate[6]={"Run Correct", "hSession=NULL", "indata1=NULL", "indatalen1=NULL", "outdata1=NULL" ,"&outdatalen1=NULL"};
	for (i = 5; i >= 0; --i)
	{
		if(i != 4)
		{
			//传空取长是正确用法,屏蔽4
			rv = generalcall(pC_DecryptUpdate, i, hSession, indata1, indatalen1, outdata1, &outdatalen1);
			bRtn = Show_Result("DecryptUpdate",i, Char_DecryptUpdate[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(bRtn);
			
			//if((rv!=CKR_OK)&&(rv != CKR_BUFFER_TOO_SMALL))
			//{//前置状态需要重新初始化
			//	rv = generalcall(pC_DecryptInit, RUN_CORRECT,  hSession, &ZUCmechanism_Dec, hKey_Dec);
			//	bRtn = Show_Result("DecryptInit",RUN_CORRECT, "re_init", rv, Testresult[nItemNumb], &nResultLength);
			//	RV_FALSE_RETURN(bRtn);
			//}			
		}
	}
	bRtn = Result_Compare(outdata1, outdatalen1, ZUCplain_Dec, sizeof(ZUCplain_Dec), Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(bRtn);

	char* Char_DecryptFinal[6]={"Run Correct", "hSession=NULL", "outdata1=NULL" ,"&outdatalen1=NULL"};
	for (i = 3; i >= 0; --i)
	{
		if(i != 2)
		{
			//传空取长是正确用法,屏蔽2
			rv = generalcall(pC_DecryptFinal, i, hSession, outdata1, &outdatalen1);
			bRtn = Show_Result("DecryptFinal",i, Char_DecryptFinal[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(bRtn);

			if((rv == CKR_OK)&&(i!=0))
			{	
				//前置状态需要重新初始化
				rv = generalcall(pC_DecryptInit, RUN_CORRECT, hSession, &ZUCmechanism_Dec, hKey_Dec);
				bRtn = Show_Result("DecryptInit",RUN_CORRECT, "re_init", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(bRtn);

				//前置状态需要重新运行
				rv = generalcall(pC_DecryptUpdate, RUN_CORRECT, hSession, indata1, indatalen1, outdata1, &outdatalen1);
				bRtn = Show_Result("DecryptUpdate",RUN_CORRECT, "re_update", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(bRtn);
			}
		}
	}

	char* Char_DestroyObject[3]={"Run Correct", "hSession=NULL", "hKey_Dec=NULL"};
	for (i = 2; i >= 0; --i)
	{
		rv = generalcall(pC_DestroyObject, i, hSession, hKey_Dec);
		bRtn = Show_Result("DestroyObject",i, Char_DestroyObject[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(bRtn);
	}
	bRtn = Leave_Function("etest_ZUCDecrypt", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(bRtn);

	
	return TRUE;
}


CK_RV etest_SM2GenEnDe()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;
	int i=0;	

	CK_OBJECT_HANDLE hPublicKey = NULL_PTR, hPrivateKey = NULL_PTR;
	CK_OBJECT_HANDLE * phPublicKey = &hPublicKey, *phPrivateKey = &hPrivateKey;
	CK_MECHANISM      mechanism = {CKM_ECC_KEY_PAIR_GEN, NULL_PTR, 0};
	CK_UINT     modulusBits = 256;
	CK_BYTE      subject[] = {0x00,0x01,0x02};
	CK_BYTE      id[] = {0x00,0x01,0x02,0x03};
	CK_BBOOL     ttrue = CK_TRUE, ffalse = CK_FALSE;
	CK_KEY_TYPE  keyType=CKK_ECC;
	CK_ATTRIBUTE publicKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_SUBJECT, subject, sizeof(subject)},
		{CKA_ID, id, sizeof(id)},
		{CKA_WRAP, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)}
	};
	int pubKeyTempNumb = sizeof(publicKeyTemplate) / sizeof(CK_ATTRIBUTE);
	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_SUBJECT, subject, sizeof(subject)},
		{CKA_ID, id, sizeof(id)},
		{CKA_SENSITIVE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_UNWRAP, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)}
	};
	int priKeyTempNumb = sizeof(privateKeyTemplate) / sizeof(CK_ATTRIBUTE);

	CK_MECHANISM  mechanismcalc = {CKM_ECC_CALC, NULL_PTR, 0};
	
	int SM2DataCnt = SM2_EN_DATALEN;
	CK_BYTE_PTR pSM2Data = NULL_PTR;
	CK_ULONG      ulSM2OutDataLen = SM2_DE_DATALEN;
	CK_BYTE_PTR pSM2OutData = NULL_PTR; 
	CK_ULONG      ulSM2OutDataLen1 = SM2_DE_DATALEN;
	CK_BYTE_PTR pSM2OutData1 = NULL_PTR; 

	pSM2Data = (CK_BYTE_PTR)malloc(sizeof(CK_BYTE)*SM2DataCnt);
	pSM2OutData = (CK_BYTE_PTR)malloc(sizeof(CK_BYTE)*ulSM2OutDataLen);	
	pSM2OutData1 = (CK_BYTE_PTR)malloc(sizeof(CK_BYTE)*ulSM2OutDataLen1); 

	srand( (unsigned)time( NULL ) );//随机数初始化
	RandomGenerate(pSM2Data,SM2_EN_DATALEN);
	memset(pSM2OutData,0,ulSM2OutDataLen);

	bRtn = Enter_Function("etest_SM2GenEnDe", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1));

	char* Char_GenerateKeyPair[9] = {"Run Correct", "hSession=NULL",  "&mechanism=NULL",
		"publicKeyTemplate=NULL", "CountPublic=NULL", 
		"privateKeyTemplate=NULL", "CountPrivate=NULL", 
		"phPublicKey=NULL", "phPrivateKey=NULL"};
	for (i = 8; i >= 0; --i)
	{
		//接口运行异常情况
		//i=1，正常返错
		//i=2，不加代理和加载代理会发生段错误，暂时屏蔽
		//i=3，不加代理会发生段错误
		//i=4，不加代理返82，加载代理返
		//i=5，不加代理会发生段错误
		//i=6，不加代理时，输入正常公钥模板和不正常私钥模板会返错，创建公私钥对象失败\
		但是公钥句柄会返回值，而该句柄不代表一个成功的对象，无法删除
		//i=7，不加代理会发生段错误，加载代理会创建成功
		//i=8，不加代理会发生段错误，加载代理会创建成功，且对象可以删除
		if(i != 2)
		{
			rv = generalcall(pC_GenerateKeyPair, i, hSession, &mechanism, publicKeyTemplate, pubKeyTempNumb, privateKeyTemplate, priKeyTempNumb, phPublicKey, phPrivateKey);
			bRtn = Show_Result("GenerateKeyPair", i, Char_GenerateKeyPair[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1));
			
			if(i != 0)
			{//删除错误创建有可能创建出的对象
				bRtn = Destory_Useless_KeyPair(hPublicKey, hPrivateKey, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1));
			}
		}
	}

	char* Char_EncryptInit[4]={"Run Correct", "hSession=NULL", "&mechanismcalc=NULL", "hPublicKey=NULL"};
	for (i = 3; i >= 0; --i)
	{
		rv = generalcall(pC_EncryptInit, i, hSession, &mechanismcalc, hPublicKey);
		bRtn = Show_Result("EncryptInit",i, Char_EncryptInit[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1));
	}

	char* Char_DecryptInit[4]={"Run Correct", "hSession=NULL", "&mechanismcalc=NULL", "hPrivateKey=NULL"};
	for (i = 3; i >= 0; --i)
	{
		rv = generalcall(pC_DecryptInit, i, hSession, &mechanismcalc, hPrivateKey);
		bRtn = Show_Result("DecryptInit",i, Char_DecryptInit[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1));
	}

	char* Char_Encrypt[6]={"Run Correct", "hSession=NULL", "pData=NULL", "DataCnt=NULL", "pOutData=NULL" ,"&ulOutDataLen=NULL"};
	for (i = 5; i >= 0; --i)
	{
		if(i != 4)
		{
			//传空取长是正确用法,屏蔽4
			rv = generalcall(pC_Encrypt, i, hSession, pSM2Data, SM2DataCnt, pSM2OutData, &ulSM2OutDataLen);
			bRtn = Show_Result("Encrypt",i, Char_Encrypt[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1));
			
			if((rv == CKR_OK)&&(i!=0))
			{//前置状态需要重新初始化
				rv = generalcall(pC_EncryptInit, RUN_CORRECT, hSession, &mechanismcalc, hPublicKey);
				bRtn = Show_Result("EncryptInit",RUN_CORRECT, "re_init", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1));
			}			
		}
	}

	char* Char_Decrypt[6]={"Run Correct", "hSession=NULL", "pOutData=NULL", "ulOutDataLen=NULL", "pOutData1=NULL" ,"&ulOutDataLen1=NULL"};
	for (i = 5; i >= 0; --i)
	{
		if(i != 4)
		{
			//传空取长是正确用法,屏蔽4
			rv = generalcall(pC_Decrypt, i, hSession,pSM2OutData,ulSM2OutDataLen,pSM2OutData1,&ulSM2OutDataLen1);
			bRtn = Show_Result("Decrypt",i, Char_Decrypt[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1));
			
			if((rv == CKR_OK)&&(i!=0))
			{//前置状态需要重新初始化
				rv = generalcall(pC_DecryptInit, RUN_CORRECT, hSession, &mechanismcalc, hPrivateKey);
				bRtn = Show_Result("DecryptInit",RUN_CORRECT, "re_init", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1));
			}			
		}
	}
	bRtn = Result_Compare(pSM2OutData1, ulSM2OutDataLen1, pSM2Data, SM2DataCnt, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1));

	bRtn = Destory_Useless_KeyPair(hPublicKey, hPrivateKey, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1));

	bRtn = Leave_Function("etest_SM2GenEnDe", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1));

	
	return Free_Memory(FREE_3, &pSM2Data, &pSM2OutData, &pSM2OutData1);
}


CK_RV etest_SM2ImpSiVr()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;
	int i = 0;

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
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE  keyType = CKK_ECC;
	CK_OBJECT_CLASS pubclass = CKO_PUBLIC_KEY;
	CK_OBJECT_CLASS priclass = CKO_PRIVATE_KEY;
	CK_UINT     modulusBits = 256;
	CK_BYTE     id[] = {0x01,0x01,0x02,0x03};
	CK_OBJECT_HANDLE hPublicKey = NULL_PTR;
	CK_OBJECT_HANDLE hPrivateKey = NULL_PTR;
	CK_OBJECT_HANDLE *phPublicKey = &hPublicKey;
	CK_OBJECT_HANDLE *phPrivateKey = &hPrivateKey;
	CK_MECHANISM  mechanismcalc = {CKM_ECC_CALC, NULL_PTR, 0};

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
	int pubKeyTempNumb = sizeof(publicKeyTemplate) / sizeof(CK_ATTRIBUTE);

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
	int priKeyTempNumb = sizeof(privateKeyTemplate) / sizeof(CK_ATTRIBUTE);

	CK_ULONG  DataCnt = SM2_SI_IN_DATALEN;
	CK_BYTE_PTR pData=(CK_BYTE_PTR)malloc(sizeof(CK_BYTE)*DataCnt);
	CK_ULONG      ulOutDataLen = SM2_SI_OUT_DATALEN;
	CK_BYTE_PTR pOutData=(CK_BYTE_PTR)malloc(sizeof(CK_BYTE)*ulOutDataLen); 

	srand((unsigned)time( NULL ));//随机数初始化
	RandomGenerate(pData,SM2_SI_IN_DATALEN);
	memset(pOutData,0,ulOutDataLen);

	bRtn = Enter_Function("etest_SM2ImpSiVr", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &pData, &pOutData));

	rv = generalcall(pC_CreateObject, RUN_CORRECT, hSession, publicKeyTemplate, pubKeyTempNumb, phPublicKey);
	bRtn = Show_Result("CreateObject_pu",RUN_CORRECT, "Run Correct", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &pData, &pOutData));

	rv = generalcall(pC_CreateObject, RUN_CORRECT, hSession, privateKeyTemplate, priKeyTempNumb, phPrivateKey);
	bRtn = Show_Result("CreateObject_pr",RUN_CORRECT, "Run Correct", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &pData, &pOutData));

	char* Char_SignInit[4]={"Run Correct", "hSession=NULL", "&mechanismcalc=NULL", "hPrivateKey=NULL"};
	for (i = 3; i >= 0; --i)
	{
		rv = generalcall(pC_SignInit, i, hSession, &mechanismcalc, hPrivateKey);
		bRtn = Show_Result("SignInit",i, Char_SignInit[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &pData, &pOutData));
	}

	char* Char_Sign[6]={"Run Correct", "hSession=NULL", "pData=NULL", "DataCnt=NULL", "pOutData=NULL" ,"&ulOutDataLen=NULL"};
	for (i = 5; i >= 0; --i)
	{
		if(i != 4)
		{
			//传空取长是正确用法,屏蔽4
			rv = generalcall(pC_Sign, i, hSession, pData, DataCnt, pOutData, &ulOutDataLen);
			bRtn = Show_Result("Sign",i, Char_Sign[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &pData, &pOutData));
			
			if((rv == CKR_DATA_LEN_RANGE)||(rv==CKR_SIGNATURE_INVALID))
			{//前置状态需要重新初始化
				rv = generalcall(pC_SignInit, RUN_CORRECT, hSession, &mechanismcalc, hPrivateKey);
				bRtn = Show_Result("pC_SignInit",RUN_CORRECT, "re_init", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &pData, &pOutData));
			}			
		}
	}

	char* Char_VerifyInit[4]={"Run Correct", "hSession=NULL", "&mechanismcalc=NULL", "hPublicKey=NULL"};
	for (i = 3; i >= 0; --i)
	{
		rv = generalcall(pC_VerifyInit, i, hSession, &mechanismcalc, hPublicKey);
		bRtn = Show_Result("VerifyInit",i, Char_VerifyInit[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &pData, &pOutData));
	}

	char* Char_Verify[6]={"Run Correct", "hSession=NULL", "pData=NULL", "DataCnt=NULL", "pOutData=NULL" ,"ulOutDataLen=NULL"};
	for (i = 5; i >= 0; --i)
	{
		rv = generalcall(pC_Verify, i, hSession,pData,DataCnt,pOutData,ulOutDataLen);
		bRtn = Show_Result("Verify",i, Char_Verify[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &pData, &pOutData));
			
		if((rv == CKR_DATA_LEN_RANGE)||(rv==CKR_SIGNATURE_INVALID))
		{//前置状态需要重新初始化
			rv = generalcall(pC_VerifyInit, RUN_CORRECT, hSession, &mechanismcalc, hPublicKey);
			bRtn = Show_Result("pC_VerifyInit",RUN_CORRECT, "re_init", rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &pData, &pOutData));
		}	
	}

	bRtn = Destory_Useless_KeyPair(hPublicKey, hPrivateKey, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &pData, &pOutData));

	bRtn = Leave_Function("etest_SM2ImpSiVr", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &pData, &pOutData));

	
	return Free_Memory( FREE_2, &pData, &pOutData);
}
#define DIG_LEN 32
#define DATA_FOR_DIG_LEN 64
CK_RV etest_SM3Digest()
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;
	int i = 0;
	unsigned char pszCorrectResult_SM3[] = {0xde, 0xbe, 0x9f, 0xf9, 0x22, 0x75, 0xb8, 0xa1, 0x38, 0x60, 0x48, 0x89, 0xc1, 0x8e, 0x5a, 0x4d, 0x6f, 0xdb, 0x70, 0xe5, 0x38, 0x7e, 0x57, 0x65, 0x29, 0x3d, 0xcb, 0xa3, 0x9c, 0x0c, 0x57, 0x32};
	
	CK_BYTE_PTR srcData = NULL_PTR, digData = NULL_PTR;	
	CK_ULONG srcLen = DATA_FOR_DIG_LEN, ulDigLen = DIG_LEN;
	CK_MECHANISM mechanism = {CKM_HASH_SM3, NULL_PTR, 0};

	digData = (CK_BYTE_PTR)malloc(sizeof(CK_BYTE) * ulDigLen);
	memset(digData, 0, ulDigLen);
	
	srcData = (CK_BYTE_PTR)malloc(sizeof(CK_BYTE) * srcLen);
	memset(srcData, 0, srcLen);

	for(i = 0; i < srcLen/4; ++i)
	{
		memcpy(&srcData[i*4],"abcd",4);
	}

	bRtn = Enter_Function("etest_SM3Digest", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &digData, &srcData));

	char* Char_DigestInit[3]={"Run Correct", "hSession=NULL", "&mechanism=NULL"};
	for (i = 2; i >= 0; --i)
	{
		rv = generalcall(pC_DigestInit, i, hSession, &mechanism);
		bRtn = Show_Result("DigestInit",i, Char_DigestInit[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &digData, &srcData));
	}

	char* Char_Digest[6]={"Run Correct", "hSession=NULL", "srcData=NULL", "srcLen=NULL","digData=NULL", "&ulDigLen=NULL"};
	for (i = 5; i >= 0; --i)
	{
		if(i != 4)
		{
			//传空取长是正确用法,屏蔽4
			rv = generalcall(pC_Digest, i, hSession, srcData, srcLen, digData, &ulDigLen);
			bRtn = Show_Result("Digest",i, Char_Digest[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &digData, &srcData));
		}
	}

	bRtn = Result_Compare(digData, ulDigLen, pszCorrectResult_SM3, sizeof(pszCorrectResult_SM3), Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &digData, &srcData));

	rv = generalcall(pC_DigestInit, RUN_CORRECT, hSession, &mechanism);
	bRtn = Show_Result("DigestInit", RUN_CORRECT, "init_again", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &digData, &srcData));

	char* Char_DigestUpdate[4]={"Run Correct", "hSession=NULL", "srcData=NULL", "srcLen/2=NULL"};
	for (i = 3; i >= 0; --i)
	{
		rv = generalcall(pC_DigestUpdate, i, hSession, srcData, srcLen/2);
		bRtn = Show_Result("DigestUpdate",i, Char_DigestUpdate[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &digData, &srcData));
	}

	rv = generalcall(pC_DigestUpdate, RUN_CORRECT, hSession, srcData + srcLen/2, srcLen - srcLen/2);
	bRtn = Show_Result("DigestUpdate", RUN_CORRECT, "Update_again", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &digData, &srcData));

	char* Char_DigestFinal[4]={"Run Correct", "hSession=NULL", "digData=NULL", "&ulDigLen=NULL"};
	for (i = 3; i >= 0; --i)
	{
		if(i != 2)
		{
			//传空取长是正确用法,屏蔽2
			rv = generalcall(pC_DigestFinal, i, hSession, digData, &ulDigLen);
			bRtn = Show_Result("DigestFinal",i, Char_DigestFinal[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &digData, &srcData));
		}
	}

	bRtn = Result_Compare(digData, ulDigLen, pszCorrectResult_SM3, sizeof(pszCorrectResult_SM3), Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &digData, &srcData));

	bRtn = Leave_Function("etest_SM3Digest", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2, &digData, &srcData));

	
	return Free_Memory( FREE_2, &digData, &srcData);
}

#define SM4_LEN 1024
#define SM4_OFB_LEN 192
#define SM4_KEY_LEN 16
#define SM4_IV_LEN 16

#define SM4_MODE_CBC 1
#define SM4_MODE_ECB 2
#define SM4_MODE_OFB 3

CK_RV etest_SM4_Mode(unsigned int sm4_mode)
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;
	int i = 0;

	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	unsigned char	SM4keyVal[SM4_KEY_LEN] = {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
	unsigned char	SM4iv[SM4_IV_LEN] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_BYTE SM4plain_Enc[SM4_LEN] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	CK_BYTE SM4cipher_Enc[SM4_LEN] = {0x00};
	CK_BYTE SM4plain_Dec[SM4_LEN] = {0x00};
	int SM4plain_Enc_Len = 0;
	int SM4cipher_Enc_Len = SM4_LEN;
	int SM4plain_Dec_Len = SM4_LEN;
	int SM4_Last_Len = 0;
	CK_MECHANISM *pSM4mechanism = NULL;
	CK_MECHANISM SM4mechanismCBC = {CKM_SM4_CBC, SM4iv, sizeof(SM4iv)};
	CK_MECHANISM SM4mechanismECB = {CKM_SM4_ECB, SM4iv, sizeof(SM4iv)};
	CK_MECHANISM SM4mechanismOFB = {CKM_SM4_OFB, SM4iv, sizeof(SM4iv)};

	CK_ATTRIBUTE SM4keyTemplate[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &SM4keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,SM4keyVal,sizeof(SM4keyVal)}
	};
	int SM4KeyTempNumb = sizeof(SM4keyTemplate) / sizeof(CK_ATTRIBUTE);
	CK_OBJECT_HANDLE hKey_SM4 = NULL_PTR;

	if(sm4_mode == SM4_MODE_CBC)
	{
		SM4plain_Enc_Len = SM4_LEN;
		pSM4mechanism = &SM4mechanismCBC;
	}
	else if(sm4_mode == SM4_MODE_ECB)
	{
		SM4plain_Enc_Len = SM4_LEN;
		pSM4mechanism = &SM4mechanismECB;
	}
	else if(sm4_mode == SM4_MODE_OFB)
	{
		SM4plain_Enc_Len = SM4_OFB_LEN;
		pSM4mechanism = &SM4mechanismOFB;		
	}
	else
	{
		printf("Error:Mode %d not exsist!\n",sm4_mode);
		return FALSE;
	}

	char* Char_CreateObject[5]={"Run Correct", "hSession=NULL", "SM4keyTemplate=NULL", "SM4KeyTempNumb=NULL", "&hKey_SM4=NULL"};
	for (i = 4; i >= 0; --i)
	{
		hKey_SM4 = NULL_PTR;
		rv = generalcall(pC_CreateObject, i, hSession, SM4keyTemplate, SM4KeyTempNumb, &hKey_SM4);
		bRtn = Show_Result("CreateObject", i, Char_CreateObject[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}


	char* Char_EncryptInit[4]={"Run Correct", "hSession=NULL", "pSM4mechanism=NULL", "hKey_SM4=NULL"};
	for (i = 3; i >= 0; --i)
	{
		rv = generalcall(pC_EncryptInit, i, hSession, pSM4mechanism, hKey_SM4);
		bRtn = Show_Result("EncryptInit",i, Char_EncryptInit[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}	

	char* Char_EncryptUpdate[6]={"Run Correct", "hSession=NULL", "SM4plain_Enc=NULL", "SM4plain_Enc_Len=NULL", "SM4cipher_Enc=NULL" ,"&SM4cipher_Enc_Len=NULL"};
	for (i = 5; i >= 0; --i)
	{
		if(i != 4)
		{
			//传空取长是正确用法,屏蔽4
			rv = generalcall(pC_EncryptUpdate, i, hSession, SM4plain_Enc, SM4plain_Enc_Len, SM4cipher_Enc, &SM4cipher_Enc_Len);
			bRtn = Show_Result("EncryptUpdate",i, Char_EncryptUpdate[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
			
			if(rv == CKR_DATA_LEN_RANGE)
			{//前置状态需要重新初始化
				rv = generalcall(pC_EncryptInit, RUN_CORRECT, hSession, pSM4mechanism, hKey_SM4);
				bRtn = Show_Result("EncryptInit",RUN_CORRECT, "re_init", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
			}			
		}
	}

	char* Char_EncryptFinal[6]={"Run Correct", "hSession=NULL", "SM4cipher_Enc=NULL" ,"&SM4_Last_Len=NULL"};
	for (i = 3; i >= 0; --i)
	{
		if(i != 2)
		{
			//传空取长是正确用法,屏蔽2
			rv = generalcall(pC_EncryptFinal, i, hSession, SM4cipher_Enc, &SM4_Last_Len);
			bRtn = Show_Result("EncryptFinal",i, Char_EncryptFinal[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

			if(rv == CKR_DATA_LEN_RANGE)
			{	
				//前置状态需要重新初始化
				rv = generalcall(pC_EncryptInit, RUN_CORRECT, hSession, pSM4mechanism, hKey_SM4);
				bRtn = Show_Result("EncryptInit",RUN_CORRECT, "re_init", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

				//前置状态需要重新运行
				rv = generalcall(pC_EncryptUpdate, RUN_CORRECT, hSession, SM4plain_Enc, SM4plain_Enc_Len, SM4cipher_Enc, &SM4cipher_Enc_Len);
				bRtn = Show_Result("EncryptInit",RUN_CORRECT, "re_update", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
			}
		}
	}

	char* Char_DecryptInit[4]={"Run Correct", "hSession=NULL", "pSM4mechanism=NULL", "hKey_SM4=NULL"};
	for (i = 3; i >= 0; --i)
	{
		rv = generalcall(pC_DecryptInit, i, hSession, pSM4mechanism, hKey_SM4);
		bRtn = Show_Result("DecryptInit",i, Char_DecryptInit[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}

	char* Char_DecryptUpdate[6]={"Run Correct", "hSession=NULL", "SM4cipher_Enc=NULL", "SM4cipher_Enc_Len=NULL", "SM4plain_Dec=NULL" ,"&SM4plain_Dec_Len=NULL"};
	for (i = 5; i >= 0; --i)
	{
		if(i != 4)
		{
			//传空取长是正确用法,屏蔽4
			rv = generalcall(pC_DecryptUpdate, i, hSession, SM4cipher_Enc, SM4cipher_Enc_Len, SM4plain_Dec, &SM4plain_Dec_Len);
			bRtn = Show_Result("DecryptUpdate",i, Char_DecryptUpdate[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
			
			if(rv == CKR_DATA_LEN_RANGE)
			{//前置状态需要重新初始化
				rv = generalcall(pC_DecryptInit, RUN_CORRECT, hSession, pSM4mechanism, hKey_SM4);
				bRtn = Show_Result("DecryptInit",RUN_CORRECT, "re_init", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
			}			
		}
	}

	char* Char_DecryptFinal[6]={"Run Correct", "hSession=NULL", "SM4plain_Dec=NULL" ,"&SM4_Last_Len=NULL"};
	for (i = 3; i >= 0; --i)
	{
		if(i != 2)
		{
			//传空取长是正确用法,屏蔽2
			rv = generalcall(pC_DecryptFinal, i, hSession, SM4plain_Dec, &SM4_Last_Len);
			bRtn = Show_Result("DecryptFinal",i, Char_DecryptFinal[i], rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

			if(rv == CKR_DATA_LEN_RANGE)
			{	
				//前置状态需要重新初始化
				rv = generalcall(pC_DecryptInit, RUN_CORRECT, hSession, hSession, pSM4mechanism, hKey_SM4);
				bRtn = Show_Result("DecryptInit",RUN_CORRECT, "re_init", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

				//前置状态需要重新运行
				rv = generalcall(pC_DecryptUpdate, RUN_CORRECT, hSession, SM4cipher_Enc, SM4cipher_Enc_Len, SM4plain_Dec, &SM4plain_Dec_Len);
				bRtn = Show_Result("DecryptUpdate",RUN_CORRECT, "re_update", rv, Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
			}
		}
	}

	char* Char_DestroyObject[3]={"Run Correct", "hSession=NULL", "hKey_SM4=NULL"};
	for (i = 2; i >= 0; --i)
	{
		rv = generalcall(pC_DestroyObject, i, hSession, hKey_SM4);
		bRtn = Show_Result("DestroyObject",i, Char_DestroyObject[i], rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}
	
	bRtn = Result_Compare(SM4plain_Dec, SM4plain_Dec_Len, SM4plain_Enc, SM4plain_Enc_Len, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	
	return Free_Memory(FREE_0);
}

CK_RV etest_SM4()
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;
	int i = 0;
	unsigned int SM4_Mode_Buf[3] = {SM4_MODE_CBC, SM4_MODE_ECB, SM4_MODE_OFB};
	char* Char_SM4_Mode[3]={"CBC", "ECB", "OFB"};


	bRtn = Enter_Function("etest_SM4", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	for (i = 2; i >= 0; --i)
	{
		bRtn = Enter_Function(Char_SM4_Mode[i], Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		bRtn = etest_SM4_Mode(SM4_Mode_Buf[i]);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		bRtn = Leave_Function(Char_SM4_Mode[i], Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}

	bRtn = Leave_Function("etest_SM4", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	
	return Free_Memory(FREE_0);
}




#define T_LEN  80
#define IV_LEN  16
#define REMOTE_DATA_VERSION_LEN 4
#define TT_R_LEN 32
CK_RV Generate_R_for_TT(unsigned char *RA_TT,unsigned char *RB_TT,unsigned char *R_TT,int length_TT)
{
	int i = 0;

	if((RA_TT == NULL)||(RB_TT == NULL)||(R_TT == NULL))
	{
		printf("Error:Some Input == NULL\n");
		return FALSE;
	}
	
	if(length_TT!= TT_R_LEN)
	{
		printf("Error:length_TT(%d) != TT_R_LEN\n",length_TT);
		return FALSE;
	}

	for(i = 0;i < TT_R_LEN; ++i)
	{
		R_TT[i] = RA_TT[i] ^ RB_TT[i];
	}

	return TRUE;
}


#define SM3_DATA_LEN 32
#define KEY_LEN 16
#define PUB_KEY_LEN 64
#define PART_OF_PUB_KEY_LEN 12
#define PRI_KEY_LEN 32
#define PIN_LEN_MAX 24
#define PIN_LEN_MIN 6

CK_RV Generate_MPK_T(unsigned char *pub_key_value, int pub_key_value_len,unsigned char *IV_T,int iv_len, unsigned char *so_pin,int so_pin_len,unsigned char *version_mpk,int Version_MPK_Len,unsigned char *T_mpk)
{
	BYTE SM3_In[IV_LEN + PIN_LEN_MAX] = { 0 };
	BYTE SM3_Out[SM3_DATA_LEN] = { 0 };
	BYTE SM4_Key[KEY_LEN] = { 0 };
	BYTE SM4_In[T_LEN] = { 0 };
	BYTE SM4_Out[T_LEN] = { 0 };
	BYTE Version_MPK_T[REMOTE_DATA_VERSION_LEN] = { 0 };
	unsigned long SM3_In_Len = 0;
	unsigned long SM3_Out_Len = SM3_DATA_LEN;
	unsigned long SM4_In_Len = 0;
	unsigned long SM4_Out_Len = T_LEN;
	int rv = 0;


	if((pub_key_value == NULL)||(IV_T == NULL)||(so_pin == NULL)||(version_mpk == NULL)||(T_mpk == NULL))
	{
		printf("Generate_MPK_T Error:Some Input == NULL\n");
		return FALSE;
	}

	if((pub_key_value_len != PUB_KEY_LEN) || (iv_len != IV_LEN) || (Version_MPK_Len != Version_MPK_Len))
	{
		printf("Generate_MPK_T Error:Some data length not macth.\n");
		return FALSE;
	}

	if((so_pin_len > PIN_LEN_MAX) || (so_pin_len < PIN_LEN_MIN))
	{
		printf("Generate_MPK_T Error:Pin length %d not macth.\n",so_pin_len);
		return FALSE;
	}

	memcpy(Version_MPK_T, version_mpk, Version_MPK_Len);

	memcpy(SM3_In + SM3_In_Len, so_pin, so_pin_len);
	SM3_In_Len += so_pin_len;
	memcpy(SM3_In + SM3_In_Len, IV_T, IV_LEN);
	SM3_In_Len += iv_len;

	//sm3摘要运算
	SM3_Data(SM3_In, SM3_In_Len, SM3_Out, SM3_DATA_LEN);

	memcpy(SM4_Key, SM3_Out, KEY_LEN);
	memcpy(SM4_In + SM4_In_Len, pub_key_value, PUB_KEY_LEN);
	SM4_In_Len += PUB_KEY_LEN;
	memcpy(SM4_In + SM4_In_Len, Version_MPK_T, Version_MPK_Len);
	SM4_In_Len += Version_MPK_Len;
	memcpy(SM4_In + SM4_In_Len, pub_key_value, PART_OF_PUB_KEY_LEN);
	SM4_In_Len += PART_OF_PUB_KEY_LEN;

	if(SM4_In_Len != T_LEN)
	{
		printf("Error:SM4_In_Len=%ld\n", SM4_In_Len);
		return FALSE;
	}

	//sm4加密运算
	rv = SEA_Encrypt(SM4_Key, KEY_LEN, IV_T, IV_LEN, SM4_In, T_LEN, SM4_Out, &SM4_Out_Len);
	if(rv != 0)
	{
		printf("Error:SEA_Encrypt,rv=%d\n",rv);
		return FALSE;
	}

	if(SM4_Out_Len != T_LEN)
	{
		printf("Error:SM4_Out_Len=%ld\n", SM4_Out_Len);
		return FALSE;
	}

	memcpy(T_mpk, SM4_Out,SM4_Out_Len);

	return TRUE;

}


#define CRC_INIT 0xffff   //CCITT初始CRC为全1
//#define GOOD_CRC 0xf0b8   //校验时计算出的固定结果值
/****下表是常用ccitt 16,生成式1021反转成8408后的查询表格****/
unsigned short crc16_ccitt_table[256] =
{
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};
/*****CRC计算函数,可将第一个参数reg_init简化掉********/
unsigned short do_crc(unsigned short reg_init, unsigned char *message, unsigned int len)
{
    unsigned short crc_reg = reg_init;
         
    while (len--)
        crc_reg = (crc_reg >> 8) ^ crc16_ccitt_table[(crc_reg ^ *message++) & 0xff];
       
    return crc_reg^0xFFFF;
} 



#define REMOTE_VERSION_LEN 4
#define REMOTE_IV_LEN 16
#define SWAP8(x)	((x)&0xFF)
#define SWAP16(x)	((SWAP8(x)<<8)|SWAP8((x)>>8))

CK_RV Generate_T(unsigned char *remoteSetData, unsigned short remoteSetDataLen, unsigned char *remoteSetIv, unsigned char *remoteSoPin, unsigned short remoteSoPinLen, unsigned char *remoteSetDataVersion, unsigned char *remoteGeneratedT,unsigned short* remoteGeneratedTLen)
{
	int rv = 0;
	unsigned short remoteSetDataLenBigEndian = 0;
	unsigned int modTemp = 0;

	unsigned char* SM3InData = 0;
	unsigned char* SM3OutData = 0 ;
	unsigned int SM3InDatalen = 0;
	unsigned int SM3OutDatalen = 0;

	unsigned char* SM4InData = 0;
	unsigned char* SM4OutData = 0;
	unsigned int SM4InDatalen = 0;
	unsigned long SM4OutDatalen = 0;
	unsigned int SM4AddDataLen = 0;

	unsigned char* CRCInData = 0;
	unsigned int CRCInDatalen = 0;
	unsigned short CRCResult = 0;
	unsigned short CRCResultBigEndian = 0;

	if((remoteSetData == NULL)||(remoteSetIv == NULL)||(remoteSoPin == NULL)||(remoteSetDataVersion == NULL)||(remoteGeneratedT == NULL))
	{
		printf("Generate_T Error:Some Input == NULL\n");
		return FALSE;
	}

	if(remoteSetDataLen == 0)
	{
		printf("Generate_T Error:remoteSetDataLen length is 0.\n");
		return FALSE;
	}

	if((remoteSoPinLen > PIN_LEN_MAX) || (remoteSoPinLen < PIN_LEN_MIN))
	{
		printf("Generate_T Error:So pin length %d range.\n",remoteSoPinLen);
		return FALSE;
	}

	SM3InDatalen = REMOTE_IV_LEN + remoteSoPinLen;
	SM3OutDatalen = SM3_DATA_LEN;
	SM3InData  = (unsigned char*)malloc(sizeof(unsigned char) * SM3InDatalen);
	SM3OutData = (unsigned char*)malloc(sizeof(unsigned char) * SM3OutDatalen);	
	memcpy(SM3InData, remoteSoPin, remoteSoPinLen);
	memcpy(SM3InData + remoteSoPinLen, remoteSetIv, REMOTE_IV_LEN);
	SM3_Data(SM3InData, SM3InDatalen, SM3OutData, SM3OutDatalen);

	remoteSetDataLenBigEndian = SWAP16(remoteSetDataLen);
	CRCInDatalen = REMOTE_VERSION_LEN + sizeof(remoteSetDataLenBigEndian) + remoteSetDataLen;
	CRCInData = (unsigned char*)malloc(sizeof(unsigned char) * CRCInDatalen);
	memcpy(CRCInData, remoteSetDataVersion, REMOTE_VERSION_LEN);
	memcpy(CRCInData + REMOTE_VERSION_LEN, &remoteSetDataLenBigEndian, sizeof(remoteSetDataLenBigEndian));
	memcpy(CRCInData + REMOTE_VERSION_LEN + sizeof(remoteSetDataLenBigEndian), remoteSetData, remoteSetDataLen);
	CRCResult = do_crc(CRC_INIT,CRCInData,CRCInDatalen);
	CRCResultBigEndian = SWAP16(CRCResult);

	modTemp = REMOTE_VERSION_LEN + sizeof(remoteSetDataLenBigEndian) + sizeof(CRCResultBigEndian) - remoteSetDataLen;
	SM4AddDataLen = modTemp%16;
	SM4InDatalen = REMOTE_VERSION_LEN + sizeof(remoteSetDataLenBigEndian) + remoteSetDataLen + sizeof(CRCResultBigEndian) + SM4AddDataLen;
	SM4OutDatalen = SM4InDatalen;
	SM4InData  = (unsigned char*)malloc(sizeof(unsigned char) * SM4InDatalen);
	SM4OutData = (unsigned char*)malloc(sizeof(unsigned char) * SM4OutDatalen);
	memcpy(SM4InData, remoteSetDataVersion, REMOTE_VERSION_LEN);
	memcpy(SM4InData + REMOTE_VERSION_LEN, &remoteSetDataLenBigEndian, sizeof(remoteSetDataLenBigEndian));
	memcpy(SM4InData + REMOTE_VERSION_LEN + sizeof(remoteSetDataLenBigEndian), remoteSetData, remoteSetDataLen);
	memcpy(SM4InData + REMOTE_VERSION_LEN + sizeof(remoteSetDataLenBigEndian) + remoteSetDataLen, &CRCResultBigEndian, sizeof(CRCResultBigEndian));
	memset(SM4InData + REMOTE_VERSION_LEN + sizeof(remoteSetDataLenBigEndian) + remoteSetDataLen + sizeof(CRCResultBigEndian), 0xFF, SM4AddDataLen);
	
	rv = SEA_Encrypt(SM3OutData, KEY_LEN, remoteSetIv, REMOTE_IV_LEN, SM4InData, SM4InDatalen, SM4OutData, &SM4OutDatalen);
	if(rv != 0)
	{
		printf("Generate_T Error:SEA_Encrypt,rv=%d\n",rv);
		return Rv_False_Free_Memory(FALSE,FREE_5, &SM3InData, &SM3OutData, &CRCInData, &SM4InData, &SM4OutData);
	}

	if(*remoteGeneratedTLen != SM4OutDatalen)
	{
		printf("Generate_T Error:remoteGeneratedTLen(%d) length not match SM4OutDatalen(%d).\n" ,*remoteGeneratedTLen,(int)SM4OutDatalen);
		return Rv_False_Free_Memory(FALSE,FREE_5, &SM3InData, &SM3OutData, &CRCInData, &SM4InData, &SM4OutData);
	}

	memcpy(remoteGeneratedT, SM4OutData,SM4OutDatalen);
	return Free_Memory(FREE_5, &SM3InData, &SM3OutData, &CRCInData, &SM4InData, &SM4OutData);
}


CK_RV Remote_Set_Data(unsigned char *remoteGeneratedT, unsigned short remoteGeneratedTLen, unsigned char *remoteSetIv, unsigned int remoteDataType)
{
	CK_RV bRtn = FALSE;
	unsigned int remoteSetType = remoteDataType;
	CK_RV rv = 0;	
	unsigned char* pbBuffer_T = NULL_PTR;

	CK_EXTEND_IN ExtIn_SetData_Remote = {CK_EXTEND_REMOTE_SET_DATA, NULL, 0};
	CK_EXTEND_OUT ExtOut_SetData_Remote = {CK_EXTEND_REMOTE_SET_DATA, NULL, 0};
	
	if((remoteGeneratedT == NULL)||(remoteSetIv == NULL))
	{
		printf("Remote_Set_Data Error:Some Input == NULL\n");
		return FALSE;
	}

	if(remoteGeneratedTLen == 0)
	{
		printf("Remote_Set_Data Error:remoteSetDataLen length is 0.\n");
		return FALSE;
	}

	ExtIn_SetData_Remote.ulParameterLen = sizeof(remoteSetType) + IV_LEN + remoteGeneratedTLen;
	pbBuffer_T=(unsigned char*)malloc(sizeof(unsigned char)*(ExtIn_SetData_Remote.ulParameterLen));
	ExtIn_SetData_Remote.pParameter = pbBuffer_T;

	memcpy(pbBuffer_T, &remoteSetType, sizeof(remoteSetType));
	memcpy(pbBuffer_T + sizeof(remoteSetType), remoteSetIv, IV_LEN);
	memcpy(pbBuffer_T + sizeof(remoteSetType) + IV_LEN, remoteGeneratedT, remoteGeneratedTLen);

	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetData_Remote, &ExtOut_SetData_Remote, NULL);
	if(remoteSetType != CKO_REMOTE_TT)
	{
		if(CardVersionData <= CARD_VERSION_413)
		{
			printf("Version4.x do not support this function.\n");
			return Free_Memory(FREE_1,&pbBuffer_T);
		}
		else if(CardVersionData <= CARD_VERSION_501)
		{
			bRtn = Show_Result("pC_CryptoExtend", RUN_CORRECT, "Remote_Set_Data", rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&pbBuffer_T));
		}
		else
		{
			printf("CardVersionData <%x> Error.\n",CardVersionData);
			RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_1,&pbBuffer_T));
		}
	}
	else
	{
		bRtn = Show_Result("pC_CryptoExtend", RUN_CORRECT, "Remote_Set_Data", rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&pbBuffer_T));
	}


	//if((Version412Flag == 1)&&(remoteSetType !=CKO_REMOTE_TT))
	//{
	//	bRtn = Show_Result("pC_CryptoExtend", RUN_INCORRECT, "Remote_Set_Data", rv, Testresult[nItemNumb], &nResultLength);
	//	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&pbBuffer_T));
	//	return Free_Memory( FREE_1, &pbBuffer_T);
	//}
	bRtn = Show_Result("pC_CryptoExtend", RUN_CORRECT, "Remote_Set_Data", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&pbBuffer_T));

	printf("Set Data Remote OK.\n");	

	
	return Free_Memory( FREE_1, &pbBuffer_T);
}

CK_RV Get_Update_Data_Version(unsigned int DataType, unsigned char * pVersion_Update)
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;	
	//for Get Monitor PubKey Version
	unsigned char pbBuffer[4] = {0};
	CK_EXTEND_IN ExtIn_GetUpdateData_Version = {CK_EXTEND_REMOTE_GET_DATAVER, NULL, 0};
	CK_EXTEND_OUT ExtOut_GetUpdateData_Version = {CK_EXTEND_REMOTE_GET_DATAVER, NULL, 0};


	ExtIn_GetUpdateData_Version.pParameter = &DataType;
	ExtIn_GetUpdateData_Version.ulParameterLen = sizeof(DataType);	
	//Get Monitor PubKey Version
	ExtOut_GetUpdateData_Version.pParameter = pbBuffer;
	ExtOut_GetUpdateData_Version.ulParameterLen = sizeof(pbBuffer);	

	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetUpdateData_Version, &ExtOut_GetUpdateData_Version, NULL);
	if(DataType != CKO_REMOTE_TT)
	{
		if(CardVersionData <= CARD_VERSION_413)
		{
			printf("Version4.x do not support this function.\n");
			bRtn = Show_Result("pC_CryptoExtend",RUN_CORRECT, "Get_Update_Data_Version", rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
		}
		else if(CardVersionData <= CARD_VERSION_501)
		{
			bRtn = Show_Result("pC_CryptoExtend", RUN_CORRECT, "Get_Update_Data_Version", rv, Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
		}
		else
		{
			printf("CardVersionData <%x> Error.\n",CardVersionData);
			RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_0));
		}
	}
	else
	{
		bRtn = Show_Result("pC_CryptoExtend", RUN_CORRECT, "Get_Update_Data_Version", rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}
	
	memcpy(pVersion_Update, pbBuffer, 3);
	pVersion_Update[3] = pbBuffer[3] + 1;

	
	printf("Get Update Data Version OK, Version=%02x.%02x.%02x.%02x.\n", pbBuffer[0],pbBuffer[1],pbBuffer[2],pbBuffer[3]);
	return TRUE;
}
//Get Monitor PubKey Version
CK_RV Get_Monitor_Pubkey_Version()
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;	
	//for Get Monitor PubKey Version
	unsigned char pbBuffer[4] = {0};
	CK_EXTEND_IN ExtIn_GetMonitorSM2PubKey_Version = {CK_EXTEND_REMOTE_GET_DATAVER, NULL, 0};
	CK_EXTEND_OUT ExtOut_GetMonitorSM2PubKey_Version = {CK_EXTEND_REMOTE_GET_DATAVER, NULL, 0};

	//Get Monitor PubKey Version
	ExtOut_GetMonitorSM2PubKey_Version.pParameter = pbBuffer;
	ExtOut_GetMonitorSM2PubKey_Version.ulParameterLen = sizeof(pbBuffer);	

	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetMonitorSM2PubKey_Version, &ExtOut_GetMonitorSM2PubKey_Version, NULL);
	bRtn = Show_Result("pC_CryptoExtend", RUN_CORRECT, "Version", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	
	printf("Get Monitor PubKey Version OK, Version=%02x.%02x.%02x.%02x.\n", pbBuffer[0],pbBuffer[1],pbBuffer[2],pbBuffer[3]);
	return TRUE;
}

CK_RV Set_Monitor_Pubkey_Local(unsigned char *pub_key_valueA, int pub_key_value_lenA,unsigned char *pbVersion,int nVersionLen,unsigned char *pSOPin,int nSOPinLen)
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;	
	//for Import Monitor PubKey
	CK_UINT			nMonitorParameterLen = PUB_KEY_LEN + REMOTE_DATA_VERSION_LEN + nSOPinLen;

	CK_BYTE			pMonitorParameter[PUB_KEY_LEN + REMOTE_DATA_VERSION_LEN + PIN_LEN_MAX] = {0};
	CK_EXTEND_IN	ExtIn_SetMonitorSM2PublicKey = {CK_EXTEND_SETMONITORSM2PUBKEY, pMonitorParameter, nMonitorParameterLen};
	CK_EXTEND_OUT	ExtOut_SetMonitorSM2PublicKey = {CK_EXTEND_SETMONITORSM2PUBKEY, NULL, 0};
	
	if((pub_key_valueA == NULL)||(pbVersion == NULL)||(pSOPin == NULL))
	{
		printf("Set_Monitor_Pubkey_Local Error:Some Input == NULL\n");
		return FALSE;
	}

	if((pub_key_value_lenA != PUB_KEY_LEN)||(nVersionLen != REMOTE_DATA_VERSION_LEN))
	{
		printf("Set_Monitor_Pubkey_Local Error:Some data length not macth.\n");
		return FALSE;
	}

	if((nSOPinLen > PIN_LEN_MAX) || (nSOPinLen < PIN_LEN_MIN))
	{
		printf("Set_Monitor_Pubkey_Local Error:Pin length not macth.\n");
		return FALSE;
	}

	//Set Monitor PubKey
	memcpy(pMonitorParameter, pub_key_valueA, PUB_KEY_LEN);
	memcpy(pMonitorParameter + PUB_KEY_LEN, pbVersion, REMOTE_DATA_VERSION_LEN);
	memcpy(pMonitorParameter + PUB_KEY_LEN + REMOTE_DATA_VERSION_LEN, pSOPin, nSOPinLen);

	if((pMonitorParameter == NULL)||(nMonitorParameterLen == 0))
	{
		printf("Set_Monitor_Pubkey_Local Error:Arguments bad\n");
		return FALSE;
	}

	//memcpy(pMonitorParameter + nMonitorSM2PublicKeyLen + nVersionLen, "0x00", 1);错误SO_PIN 返30
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_SetMonitorSM2PublicKey, &ExtOut_SetMonitorSM2PublicKey, NULL);
	bRtn = Show_Result("pC_CryptoExtend", RUN_CORRECT, "Local", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	
	printf("Set Monitor PubKey OK, Version=%02x.%02x.%02x.%02x.\n", pbVersion[0],pbVersion[1],pbVersion[2],pbVersion[3]);
	return Free_Memory(FREE_0);
}

CK_RV Init_Token(CK_BYTE* pbSOPIN, int ulSOPINLen)
{
	CK_RV rv=0;
	CK_UTF8CHAR labelToken[32]={' '};
	UtilscTime ttc1={0, 0},ttc2={0,0},ttc3={0,0};
	printf("enter InitToken.\n");
	memcpy(labelToken,"label",5);

	if(pbSOPIN == NULL)
	{
		printf("Init_Token Error:Some data length not macth.\n");
		return FALSE;
	}

	if((ulSOPINLen > PIN_LEN_MAX) || (ulSOPINLen < PIN_LEN_MIN))
	{
		printf("Init_Token Error:Pin length not macth.\n");
		return FALSE;
	}

	//先关闭所有的会话
	rv=(*pC_CloseAllSessions)(slotID);
	if(rv!=CKR_OK)
	{
		printf("test_InitToken failed,pC_CloseAllSessions. rv=0x%08x.\n",rv);
		return FALSE;
	}
	
	Utilsgettime(&ttc1);	
	rv=(*pC_InitToken)(slotID, adminpin, 8, labelToken);
	if(rv!=CKR_OK)
	{
		Utilsgettime(&ttc2);
		UtilsTimeSubstracted(&ttc2,&ttc1);
		UtilsTimeAdded(&ttc3,&ttc2);
		Utilsprint(&ttc3,"pC_InitToken", 1);
		printf("test_InitToken failed,pC_InitToken. rv=0x%08x.\n",rv);
		return FALSE;
	}
	Utilsgettime(&ttc2);
	UtilsTimeSubstracted(&ttc2,&ttc1);
	UtilsTimeAdded(&ttc3,&ttc2);
	Utilsprint(&ttc3,"pC_InitToken", 1);

	rv=(*pC_OpenSession)(slotID,CKF_SERIAL_SESSION|CKF_RW_SESSION,NULL_PTR,NULL_PTR,&hSession);
	if(rv!=CKR_OK)
	{
		printf("test_InitToken failed,C_OpenSession. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv=(*pC_Login)(hSession,CKU_USER,userpin,strlen((char*)userpin));
	if(rv!=CKR_OK)
	{
		printf("test_InitToken failed,pC_Login. rv=0x%08x.\n",rv);
		return FALSE;
	}
	
	
	printf("Init_Token OK.\n");
	return TRUE;
}



CK_RV Generate_TT_Wrap_Key_Pair(CK_OBJECT_HANDLE_PTR phPublicKey, CK_OBJECT_HANDLE_PTR phPrivateKey)
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;	
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;

	//for C_GenerateKeyPair
	CK_MECHANISM      ECCMechanism = {CKM_ECC_KEY_PAIR_GEN, NULL_PTR, 0};
	CK_UINT     ECCModulesBits = 256;
	CK_KEY_TYPE  ECCKeyType = CKK_ECC;
	CK_ATTRIBUTE publicKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &ECCModulesBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(CK_BBOOL)},
		//{CKA_ENCRYPT, &ffalse, sizeof(CK_BBOOL)},
		//{CKA_VERIFY, &ffalse, sizeof(CK_BBOOL)},
		{CKA_ISEXCHANGEKEY, &ttrue, sizeof(CK_BBOOL)},
		{CKA_KEY_TYPE,&ECCKeyType,sizeof(CK_KEY_TYPE)}
	};
	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_ECC_BITS_LEN, &ECCModulesBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(CK_BBOOL)},
		{CKA_PRIVATE, &ttrue, sizeof(CK_BBOOL)},
		{CKA_SENSITIVE, &ttrue, sizeof(CK_BBOOL)},
		//{CKA_DECRYPT, &ffalse, sizeof(CK_BBOOL)},
		//{CKA_SIGN, &ffalse, sizeof(CK_BBOOL)},
		{CKA_ISEXCHANGEKEY, &ttrue, sizeof(CK_BBOOL)},
		{CKA_KEY_TYPE,&ECCKeyType,sizeof(CK_KEY_TYPE)}
	};

	//真实卡A生成用于密钥交换的公私钥对（一个SD内只有一对）
	rv = (*pC_GenerateKeyPair)(hSession, &ECCMechanism,
			publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
			privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
			phPublicKey, phPrivateKey);
	bRtn = Show_Result("pC_GenerateKeyPair",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(bRtn);

	
	return TRUE;
}
CK_RV Check_TT_Wrap_Key_Pair(CK_OBJECT_HANDLE phPublicKey)
{
	CK_RV rv = 0;
	CK_BYTE temp_pub_x[32] = { 0 };
	CK_BYTE temp_pub_y[32] = { 0 };
	CK_ATTRIBUTE GetPubKeyTemplate[] = {
		{CKA_ECC_X_COORDINATE, temp_pub_x, sizeof(temp_pub_x)},
		{CKA_ECC_Y_COORDINATE, temp_pub_y, sizeof(temp_pub_y)},
	};
	rv = (*pC_GetAttributeValue)(hSession,phPublicKey,GetPubKeyTemplate,sizeof(GetPubKeyTemplate)/sizeof(CK_ATTRIBUTE));
	if((GetPubKeyTemplate[0].ulValueLen != 0)&&(rv == CKR_OK))
	{
		printf("Check_TT_Wrap_Key_Pair Failed!\n");
		return FALSE ;		
	}
	else
	{
		printf("Check_TT_Wrap_Key_Pair Success 0x%08x!\n",rv);
		return TRUE ;
	}
}



#define TT_WRAPPED_LEN TT_R_LEN+96

CK_RV logout_user()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;
	int i = 0;

	rv=(*pC_Logout)(hSession);
	bRtn = Show_Result("pC_Logout", RUN_CORRECT, "logout", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	
	return Free_Memory(FREE_0);
}

CK_RV test_TTV4V5()
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;	
	unsigned int modTemp = 0;

	CK_BYTE pbSOPIN_T[8] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	int ulSOPINLen_T = sizeof(pbSOPIN_T);

	BYTE Version_MPK[REMOTE_DATA_VERSION_LEN] = { 0 };
	BYTE IV_mpk[IV_LEN] = { 0 };
	//BYTE T_mpk[T_LEN] = { 0 };
	BYTE* T_mpk= NULL;
	unsigned short T_mpkLen = 0;
	unsigned char pri_key_valueT[PRI_KEY_LEN] = {0};		
	unsigned char pub_key_valueT[PUB_KEY_LEN] = {0};		
	int pri_key_value_lenT = PRI_KEY_LEN;
	int pub_key_value_lenT = PUB_KEY_LEN;
	unsigned char pri_key_valueB[PRI_KEY_LEN] = {0};		
	unsigned char pub_key_valueB[PUB_KEY_LEN] = {0};		
	int pri_key_value_lenB = PRI_KEY_LEN;
	int pub_key_value_lenB = PUB_KEY_LEN;
	unsigned char RA[TT_R_LEN] = {0};		
	unsigned char RB[TT_R_LEN] = {0};		
	unsigned char R[TT_R_LEN] = {0};		
	unsigned char RR[TT_R_LEN] = {0};		
	int RALen = TT_R_LEN;
	int RRLen = TT_R_LEN;
	CK_BYTE pbWrappedKey[TT_WRAPPED_LEN] = {0};
	CK_ULONG ulWrappedKeyLen = TT_WRAPPED_LEN;
	unsigned char  WrappedKeyB[TT_WRAPPED_LEN] = { 0 };
	int WrappedKeyBLen = TT_WRAPPED_LEN ;

	CK_OBJECT_HANDLE hPartKey1 = NULL_PTR, hPartKey2 = NULL_PTR, hSessKey = NULL_PTR;
	CK_BYTE keyID1 = CK_SESSKEY_ID0, keyID2 = CK_SESSKEY_ID0, keyID3 = CK_SESSKEY_ID0;
	CK_OBJECT_HANDLE hPublicKey = NULL_PTR, hPrivateKey = NULL_PTR;
	//for get exchange public key
	CK_BYTE			pSM2PublicKey[PUB_KEY_LEN] = {0};
	CK_UINT			nSM2PublicKeyLen = PUB_KEY_LEN;
	CK_EXTEND_IN	ExtIn_GetExchangePubKey = {CK_EXTEND_GETEXCHANGEPUBKEY, NULL, 0};
	CK_EXTEND_OUT	ExtOut_GetExchangePubKey = {CK_EXTEND_GETEXCHANGEPUBKEY, pSM2PublicKey, nSM2PublicKeyLen};
	//for C_GenerateKey
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
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
	//for C_WrapKey
	CK_MECHANISM	SM2WrapMechanism = {CKM_SM2_WRAP, pub_key_valueB, pub_key_value_lenB};
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
	unsigned char* pMechanismParameter = NULL ;
	CK_MECHANISM	DeriveKeyMechanism = {CKM_30RAYCOM_DERIVE, NULL, 0};
	CK_ATTRIBUTE SessKeyDeriveTemplate[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &SessKeyExchangeKeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &keyID3, sizeof(CK_BYTE)}
	};

	//for get exchange sess key
	CK_BYTE			pExchangeSessKey[128] = {0};
	CK_UINT			nExchangeSessKeyLen = sizeof(pExchangeSessKey);
	CK_EXTEND_IN	ExtIn_GetExchangeSessKey = {CK_EXTEND_GETEXCHANGESESSKEY, &keyID1, sizeof(CK_BYTE)};
	CK_EXTEND_OUT	ExtOut_GetExchangeSessKey = {CK_EXTEND_GETEXCHANGESESSKEY, pExchangeSessKey, nExchangeSessKeyLen};

	srand( (unsigned)time( NULL ) );//随机数初始化
	bRtn = Enter_Function("test_TTV4V5", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//真实卡A生成用于密钥交换的公私钥对（一个SD内只有一对）,并获取公钥
	bRtn = Generate_TT_Wrap_Key_Pair(&hPublicKey, &hPrivateKey);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = Check_TT_Wrap_Key_Pair(hPublicKey);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));


	if (SM2Init() != SM2_OK)
	{
		printf("SM2InitA fail\n");
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_0));
	}

	if (SM2GenKey(pri_key_valueT, &pri_key_value_lenT, pub_key_valueT, &pub_key_value_lenT) != SM2_OK)
	{
		printf("SM2GenKeyA fail\n");
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_0));
	}
	//Get Monitor PubKey Version
	//bRtn = Get_Monitor_Pubkey_Version();
	bRtn =Get_Update_Data_Version(CKO_REMOTE_TT, Version_MPK);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//Initial Monitor PubKey
	memset(Version_MPK, 0x00, REMOTE_DATA_VERSION_LEN);
	bRtn = Set_Monitor_Pubkey_Local(pub_key_valueT, pub_key_value_lenT, Version_MPK, REMOTE_DATA_VERSION_LEN, pbSOPIN_T, ulSOPINLen_T);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//Get Monitor PubKey Version
	//bRtn = Get_Monitor_Pubkey_Version();
	bRtn =Get_Update_Data_Version(CKO_REMOTE_TT, Version_MPK);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//生成在线更新特通的T
	RandomGenerate(IV_mpk, IV_LEN);	
	//memset(Version_MPK, 0x01, REMOTE_DATA_VERSION_LEN);
	modTemp = 8 - PUB_KEY_LEN;
	T_mpkLen = 8 + PUB_KEY_LEN + modTemp%16;
	T_mpk = (BYTE*)malloc(sizeof(BYTE) * T_mpkLen);

	if(CardVersionData <= CARD_VERSION_413)
	{
		bRtn = Generate_MPK_T(pub_key_valueT, PUB_KEY_LEN, IV_mpk, IV_LEN, pbSOPIN_T, ulSOPINLen_T, Version_MPK, REMOTE_DATA_VERSION_LEN, T_mpk);	
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_mpk));
	}
	else if(CardVersionData <= CARD_VERSION_501)
	{
		bRtn = Generate_T(pub_key_valueT, PUB_KEY_LEN, IV_mpk, pbSOPIN_T, ulSOPINLen_T, Version_MPK, T_mpk, &T_mpkLen);	
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_mpk));
	}
	else
	{
		printf("CardVersionData <%x> Error.\n",CardVersionData);
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_1,&T_mpk));
	}

	// Set Monitor PubKey Remote
	bRtn = Remote_Set_Data(T_mpk, T_mpkLen, IV_mpk,CKO_REMOTE_TT);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_mpk));

	//Get Monitor PubKey Version
	//bRtn = Get_Monitor_Pubkey_Version();
	bRtn =Get_Update_Data_Version(CKO_REMOTE_TT, Version_MPK);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_mpk));

	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetExchangePubKey, &ExtOut_GetExchangePubKey, NULL);
	bRtn = Show_Result("pC_CryptoExtend1",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_mpk));

	//虚拟卡B生成用于密钥交换的公私钥对
	if (SM2Init() != SM2_OK)
	{
		printf("SM2InitB fail\n");
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_1,&T_mpk));
	}

	if (SM2GenKey(pri_key_valueB, &pri_key_value_lenB, pub_key_valueB, &pub_key_value_lenB) != SM2_OK)
	{
		printf("SM2GenKeyB fail\n");
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_1,&T_mpk));
	}


	//卡A生成随机数RA，用公钥B打包生成EA，卡B用私钥B解密(解包)EA得到RA
	rv = (*pC_GenerateKey)(hSession, &SessKeyExchangeMechanism, SessKeyExchangeTemplate1, sizeof(SessKeyExchangeTemplate1)/sizeof(CK_ATTRIBUTE), &hPartKey1);
	bRtn = Show_Result("pC_GenerateKey",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_mpk));

	rv = (*pC_WrapKey)(hSession, &SM2WrapMechanism, 0, hPartKey1, pbWrappedKey, &ulWrappedKeyLen);
	bRtn = Show_Result("pC_WrapKey",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_mpk));

	if (SM2Decrypt(pbWrappedKey, ulWrappedKeyLen, pri_key_valueB, pri_key_value_lenB, RA, &RALen) != SM2_OK)
	{
		printf("SM2Decrypt fail\n");
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_1,&T_mpk)); 
	}

	//卡B生成随机数RB，用公钥A打包（加密）成EB，卡A用私钥A解密(解包)EB得到RB	
	RandomGenerate(RB, TT_R_LEN);

	if (SM2Encrypt(RB, TT_R_LEN, pSM2PublicKey, nSM2PublicKeyLen, WrappedKeyB, &WrappedKeyBLen) != SM2_OK)
	{
		printf("SM2Encrypt fail\n");
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_1,&T_mpk));
	}

	rv = (*pC_UnwrapKey)(hSession, &SM2UnwrapMechanism, 0, WrappedKeyB, WrappedKeyBLen, SessKeyExchangeTemplate2, sizeof(SessKeyExchangeTemplate2)/sizeof(CK_ATTRIBUTE), &hPartKey2);
	bRtn = Show_Result("pC_UnwrapKey",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_mpk));

	//使用部分密钥RA（KEY）和部分密钥RB（KEY），异或（协商）生成密钥R（KEY）
	bRtn = Generate_R_for_TT(RA, RB, R, TT_R_LEN);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_mpk));

	//keyHandles[0] = hPartKey1;
	//keyHandles[1] = hPartKey2;

	DeriveKeyMechanism.ulParameterLen = sizeof(CK_OBJECT_HANDLE)*2 + 16;
	pMechanismParameter = (unsigned char*)malloc(DeriveKeyMechanism.ulParameterLen);
	memset(pMechanismParameter, 0, DeriveKeyMechanism.ulParameterLen);
	memcpy(pMechanismParameter, &hPartKey1, sizeof(CK_OBJECT_HANDLE));
	memcpy(pMechanismParameter+sizeof(CK_OBJECT_HANDLE),&hPartKey2, sizeof(CK_OBJECT_HANDLE));
	DeriveKeyMechanism.pParameter = pMechanismParameter;

	//bRtn = logout_user();
	//RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	rv = (*pC_DeriveKey)(hSession, &DeriveKeyMechanism, NULL_PTR, SessKeyDeriveTemplate, sizeof(SessKeyDeriveTemplate)/sizeof(CK_ATTRIBUTE), &hSessKey);
	bRtn = Show_Result("pC_DeriveKey",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));


	if(CardVersionData <= CARD_VERSION_413)
	{
		printf("Test Version 4.x not support get IV\n");
	}
	else if(CardVersionData <= CARD_VERSION_501)
	{
		bRtn = Result_Compare(DeriveKeyMechanism.pParameter+8, TT_R_LEN/2, R+TT_R_LEN/2, TT_R_LEN/2, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));
	}
	else
	{
		printf("CardVersionData <%x> Error.\n",CardVersionData);
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_2,&T_mpk,&pMechanismParameter));
	}

	//特通公钥A0加密R（KEY）得到M，私钥A0解密M得到RR
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetExchangeSessKey, &ExtOut_GetExchangeSessKey, NULL);
	bRtn = Show_Result("pC_CryptoExtend2",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));

	if (SM2Decrypt(pExchangeSessKey, nExchangeSessKeyLen, pri_key_valueT, pri_key_value_lenT, RR, &RRLen) != SM2_OK)
	{
		printf("SM2Decrypt fail\n");
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_2,&T_mpk,&pMechanismParameter));
	}
	bRtn = Result_Compare(RR, RRLen, R, TT_R_LEN, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));

#ifdef TT_INIT_TOKEN
	bRtn = Init_Token(pbSOPIN_T, ulSOPINLen_T);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));


	//bRtn = Get_Monitor_Pubkey_Version();
	bRtn =Get_Update_Data_Version(CKO_REMOTE_TT, Version_MPK);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));

	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetExchangePubKey, &ExtOut_GetExchangePubKey, NULL);
	bRtn = Show_Result("pC_CryptoExtend1",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));

	//虚拟卡B生成用于密钥交换的公私钥对
	if (SM2Init() != SM2_OK)
	{
		printf("SM2InitB fail\n");
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_2,&T_mpk,&pMechanismParameter));
	}

	if (SM2GenKey(pri_key_valueB, &pri_key_value_lenB, pub_key_valueB, &pub_key_value_lenB) != SM2_OK)
	{
		printf("SM2GenKeyB fail\n");
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_2,&T_mpk,&pMechanismParameter));
	}

	//卡A生成随机数RA，用公钥B打包生成EA，卡B用私钥B解密(解包)EA得到RA
	rv = (*pC_GenerateKey)(hSession, &SessKeyExchangeMechanism, SessKeyExchangeTemplate1, sizeof(SessKeyExchangeTemplate1)/sizeof(CK_ATTRIBUTE), &hPartKey1);
	bRtn = Show_Result("pC_GenerateKey",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));


	rv = (*pC_WrapKey)(hSession, &SM2WrapMechanism, 0, hPartKey1, pbWrappedKey, &ulWrappedKeyLen);
	bRtn = Show_Result("pC_WrapKey",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));

	if (SM2Decrypt(pbWrappedKey, ulWrappedKeyLen, pri_key_valueB, pri_key_value_lenB, RA, &RALen) != SM2_OK)
	{
		printf("SM2Decrypt 1 fail\n");
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_2,&T_mpk,&pMechanismParameter)); 
	}

	//卡B生成随机数RB，用公钥A打包（加密）成EB，卡A用私钥A解密(解包)EB得到RB	
	RandomGenerate(RB, TT_R_LEN);

	if (SM2Encrypt(RB, TT_R_LEN, pSM2PublicKey, nSM2PublicKeyLen, WrappedKeyB, &WrappedKeyBLen) != SM2_OK)
	{
		printf("SM2Encrypt fail\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "SM2Encrypt fail<br>");
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_2,&T_mpk,&pMechanismParameter));
	}

	rv = (*pC_UnwrapKey)(hSession, &SM2UnwrapMechanism, 0, WrappedKeyB, WrappedKeyBLen, SessKeyExchangeTemplate2, sizeof(SessKeyExchangeTemplate2)/sizeof(CK_ATTRIBUTE), &hPartKey2);
	bRtn = Show_Result("pC_UnwrapKey",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));

	//使用部分密钥RA（KEY）和部分密钥RB（KEY），异或（协商）生成密钥R（KEY）
	bRtn = Generate_R_for_TT(RA, RB, R, TT_R_LEN);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));

	//keyHandles[0] = hPartKey1;
	//keyHandles[1] = hPartKey2;

	memset(pMechanismParameter, 0, DeriveKeyMechanism.ulParameterLen);
	memcpy(pMechanismParameter, &hPartKey1, sizeof(CK_OBJECT_HANDLE));
	memcpy(pMechanismParameter+sizeof(CK_OBJECT_HANDLE),&hPartKey2, sizeof(CK_OBJECT_HANDLE));
	DeriveKeyMechanism.pParameter = pMechanismParameter;
	rv = (*pC_DeriveKey)(hSession, &DeriveKeyMechanism, NULL_PTR, SessKeyDeriveTemplate, sizeof(SessKeyDeriveTemplate)/sizeof(CK_ATTRIBUTE), &hSessKey);
	bRtn = Show_Result("pC_DeriveKey",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));

	if(CardVersionData <= CARD_VERSION_413)
	{
		printf("Test Version 4.x not support get IV\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Test Version 4.x not support get IV<br>");
	}
	else if(CardVersionData <= CARD_VERSION_501)
	{
		bRtn = Result_Compare(DeriveKeyMechanism.pParameter+8, TT_R_LEN/2, R+TT_R_LEN/2, TT_R_LEN/2, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));
	}
	else
	{
		printf("CardVersionData <%x> Error.\n",CardVersionData);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "CardVersionData <%x> Error.<br>",CardVersionData);
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_2,&T_mpk,&pMechanismParameter));
	}

	//特通公钥A0加密R（KEY）得到M，私钥A0解密M得到RR
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetExchangeSessKey, &ExtOut_GetExchangeSessKey, NULL);
	bRtn = Show_Result("pC_CryptoExtend2",RUN_CORRECT, "normal", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));

	if (SM2Decrypt(pExchangeSessKey, nExchangeSessKeyLen, pri_key_valueT, pri_key_value_lenT, RR, &RRLen) != SM2_OK)
	{
		//printf("SM2Decrypt should fail\n");
		RV_FALSE_RETURN(Rv_False_Free_Memory(TRUE, FREE_2,&T_mpk,&pMechanismParameter));
	}
	else
	{
		RV_FALSE_RETURN(Rv_False_Free_Memory(FALSE, FREE_2,&T_mpk,&pMechanismParameter));
	}
	

#endif

	bRtn = Leave_Function("test_TTV4V5", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_2,&T_mpk,&pMechanismParameter));

	
	return Free_Memory(FREE_2,&T_mpk,&pMechanismParameter);
}

CK_RV Set_OTP_Local(unsigned char *pbDefaultOTPPin, unsigned char nOTPPinLen, unsigned char *pSOPin, unsigned char nSOPinLen)
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;	
	CK_EXTEND_IN ExtIn_ResetOTPPin = {CK_EXTEND_RESET_OTPPIN, NULL, 0};
	CK_EXTEND_OUT ExtOut_ResetOTPPin = {CK_EXTEND_RESET_OTPPIN, NULL, 0};
	unsigned char* pbBuffer = NULL;

	pbBuffer = (unsigned char *)malloc(nSOPinLen+ nOTPPinLen + 2);
	ExtIn_ResetOTPPin.pParameter = pbBuffer;

	//Reset OTP Pin
	*pbBuffer = nSOPinLen;
	memcpy(pbBuffer + 1, pSOPin, nSOPinLen);
	*(pbBuffer + 1 + nSOPinLen) = nOTPPinLen;
	memcpy(pbBuffer + 2 + nSOPinLen, pbDefaultOTPPin, nOTPPinLen);
	ExtIn_ResetOTPPin.ulParameterLen = 2 + nOTPPinLen + nSOPinLen;
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_ResetOTPPin, &ExtOut_ResetOTPPin, NULL);
	bRtn = Show_Result("pC_CryptoExtend", RUN_CORRECT, "Set_OTP_Local", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&pbBuffer));

	
	return Free_Memory(FREE_1,&pbBuffer); 

}
//正确使用次数
int Get_OTP_Time_Usable()
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;	
	unsigned char nOTPPinUsableTime = 0;
	CK_EXTEND_IN ExtIn_GetOTPPinTime_Usable = {CK_EXTEND_GETOTPTIME_USABLE, NULL, 0};
	CK_EXTEND_OUT ExtOut_GetOTPPinTime_Usable = {CK_EXTEND_GETOTPTIME_USABLE, (void*)&nOTPPinUsableTime, 1};
	
	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Usable, &ExtOut_GetOTPPinTime_Usable, NULL);
	bRtn = Show_Result("pC_CryptoExtend", RUN_CORRECT, "Get_OTP_Time_Usable", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	
	
	return nOTPPinUsableTime;
}
//错误使用次数
int Get_OTP_Time_Try()
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;	
	unsigned char nOTPPinTryTime = 0;
	CK_EXTEND_IN ExtIn_GetOTPPinTime_Try = {CK_EXTEND_GETOTPTIME_TRY, NULL, 0};
	CK_EXTEND_OUT ExtOut_GetOTPPinTime_Try = {CK_EXTEND_GETOTPTIME_TRY, (void*)&nOTPPinTryTime, 1};

	rv = (*pC_CryptoExtend)(hSession, &ExtIn_GetOTPPinTime_Try, &ExtOut_GetOTPPinTime_Try, NULL);
	bRtn = Show_Result("pC_CryptoExtend", RUN_CORRECT, "Get_OTP_Time_Try", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	
	
	return nOTPPinTryTime;
}

CK_RV Set_UserPin_By_OTP(unsigned char* pbDefaultOTPPin, unsigned char nOTPPinLen, unsigned char *pUserPin, unsigned char nUserPinLen)
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;	

	CK_EXTEND_IN ExtIn_ResetUserPin = {CK_EXTEND_RESET_USERPIN, NULL, 0};
	CK_EXTEND_OUT ExtOut_ResetUserPin = {CK_EXTEND_RESET_USERPIN, NULL, 0};
	unsigned char* pbBuffer = NULL;

	pbBuffer = (unsigned char *)malloc(nUserPinLen+ nOTPPinLen + 2);
	ExtIn_ResetUserPin.pParameter = pbBuffer;

	*pbBuffer = nOTPPinLen;
	memcpy(pbBuffer + 1, pbDefaultOTPPin, nOTPPinLen);
	*(pbBuffer + 1 + nOTPPinLen) = nUserPinLen;
	memcpy(pbBuffer + 2 + nOTPPinLen, pUserPin, nUserPinLen);
	ExtIn_ResetUserPin.ulParameterLen = 2 + nOTPPinLen + nUserPinLen;

	rv = (*pC_CryptoExtend)(hSession, &ExtIn_ResetUserPin, &ExtOut_ResetUserPin, NULL);
	bRtn = Show_Result("pC_CryptoExtend", RUN_CORRECT, "Set_UserPin_OTP", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&pbBuffer));
	
	
	return Free_Memory(FREE_1,&pbBuffer); 
}


CK_RV test_check_OTP(unsigned char* pbDefaultOTPPin, unsigned char nOTPPinLen)
{

	CK_RV bRtn = FALSE;
	CK_RV rv = 0;
	unsigned char pUserPin[8] = {0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,0x38};
	unsigned char nUserPinLen = sizeof(pUserPin);

	bRtn = Set_UserPin_By_OTP(pbDefaultOTPPin, nOTPPinLen, pUserPin, nUserPinLen);

	//after reset userpin successful, then execute login
	if(TRUE == bRtn)
	{
		rv=(*pC_Login)(hSession,CKU_USER,pUserPin,8);
		if(rv!=CKR_OK)
		{
			if(rv==0x00000100)
			{
				printf("Already logged in,go on.\n");
			}
			else
			{
				printf("UserPin login failed,pC_Login. rv=0x%08x.\n",rv);
				return FALSE;
			}
		}
	}
	// end

	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	return Free_Memory(FREE_0);
}
CK_RV logout_and_lockpin()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;
	int i = 0;
	//rv=(*pC_Logout)(hSession);
	//bRtn = Show_Result("pC_Logout", RUN_CORRECT, "logout_and_lockpin", rv, Testresult[nItemNumb], &nResultLength);
	//RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	for(i = 0; i < 7; ++i)
	{
		rv=(*pC_Login)(hSession,CKU_USER,"1234567890",10);
		bRtn = Show_Result("pC_Login", RUN_INCORRECT, "logout_and_lockpin", rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}

	
	return Free_Memory(FREE_0);
}


//记得测试opt长度变化，重置user时userpin长度变化
CK_RV test_OTPV4V5()
{

	CK_RV rv=0;
	CK_RV bRtn = FALSE;
	unsigned int modTemp = 0;
	BYTE Version_otp[REMOTE_DATA_VERSION_LEN] = { 0 };
	BYTE IV_otp[IV_LEN] = { 0 };

	BYTE * T_otp = NULL;
	unsigned short T_otpLen = 0;
	unsigned char *pbBuffer = NULL;
	unsigned char pSOPin[8] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	unsigned char nSOPinLen = sizeof(pSOPin);
	//for Reset User Pin
	CK_EXTEND_IN ExtIn_ResetUserPin = {CK_EXTEND_RESET_USERPIN, NULL, 0};
	CK_EXTEND_OUT ExtOut_ResetUserPin = {CK_EXTEND_RESET_USERPIN, NULL, 0};

	//for Reset OTP Pin
	unsigned char pbDefaultOTPPin[8] = {0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38};
	unsigned char nOTPPinLen = sizeof(pbDefaultOTPPin);

	bRtn = Enter_Function("test_OTPV4V5", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = Get_Update_Data_Version(CKO_REMOTE_OTP, Version_otp);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	
	bRtn = Set_OTP_Local(pbDefaultOTPPin, nOTPPinLen, pSOPin, nSOPinLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//bRtn = logout_and_lockpin();
	//RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_otp));
	
	bRtn = test_check_OTP(pbDefaultOTPPin, nOTPPinLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_otp));

	bRtn =Get_Update_Data_Version(CKO_REMOTE_OTP, Version_otp);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));


	//生成在线更新特通的T
	RandomGenerate(IV_otp, IV_LEN);	
	//memset(Version_otp, 0x01, REMOTE_DATA_VERSION_LEN);

	modTemp = 8 - nOTPPinLen;
	T_otpLen = 8 + nOTPPinLen + modTemp%16;	
	T_otp = (BYTE*)malloc(sizeof(BYTE) * T_otpLen);
	bRtn = Generate_T(pbDefaultOTPPin, nOTPPinLen, IV_otp, pSOPin, nSOPinLen, Version_otp, T_otp, &T_otpLen);	
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_otp));

	bRtn = Remote_Set_Data(T_otp, T_otpLen, IV_otp,CKO_REMOTE_OTP);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_otp));

	bRtn = Get_Update_Data_Version(CKO_REMOTE_OTP, Version_otp);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_otp));
	
	//bRtn = logout_and_lockpin();
	//RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_otp));

	bRtn = test_check_OTP(pbDefaultOTPPin, nOTPPinLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_otp));

#ifdef TT_INIT_TOKEN
	bRtn = Init_Token(pSOPin, nSOPinLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_otp));

	bRtn =Get_Update_Data_Version(CKO_REMOTE_OTP, Version_otp);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_otp));

	bRtn = test_check_OTP("12345678", 8);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_otp));
#endif

	bRtn = Leave_Function("test_OTPV4V5", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_otp));

	
	return Free_Memory(FREE_1,&T_otp);
}




CK_RV Set_BK_Local(unsigned char* pBaseKey, unsigned char pBaseKeyLen, unsigned char* pSOPin, unsigned char nSOPinLen)
{

	CK_RV bRtn = FALSE;
	CK_RV rv = 0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_SM4;
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID7;
	CK_OBJECT_HANDLE  hBaseKey = NULL_PTR;

	CK_ATTRIBUTE BaseKeyTemplate[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(ZUCkeyType)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE, pBaseKey, pBaseKeyLen},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(CK_BYTE)}
	};

	bRtn = user_to_so();
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = pC_CreateObject(hSession, BaseKeyTemplate, sizeof(BaseKeyTemplate)/sizeof(CK_ATTRIBUTE), &hBaseKey);
	bRtn = Show_Result("pC_CreateObject", RUN_CORRECT, "Set_BK_Local", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = so_to_user();
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	
	return Free_Memory(FREE_0);
}

CK_RV test_check_BK(unsigned char* pBaseKey, unsigned char pBaseKeyLen)
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE ZUCkeyType = CKK_SM4;
	CK_BYTE nSessKeyID_Enc = CK_SESSKEY_PRESET_ID1;
	CK_BYTE nSessKeyID_Dec = CK_SESSKEY_PRESET_ID7;	
	unsigned char	SM4iv[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};

	CK_ATTRIBUTE ZUCkeyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(ZUCkeyType)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,pBaseKey,pBaseKeyLen},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(nSessKeyID_Enc)}
	};

	CK_ATTRIBUTE ZUCkeyTemplate_Dec[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE, &ZUCkeyType, sizeof(ZUCkeyType)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SESSKEY_ID, &nSessKeyID_Dec, sizeof(nSessKeyID_Dec)}
	};

	CK_MECHANISM SM4mechanism = {CKM_SM4_ECB, SM4iv, sizeof(SM4iv)};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;
	CK_OBJECT_HANDLE hBaseKey = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);
	CK_BYTE inresultdata[256] = {0};
	CK_ULONG inresultdatalen=sizeof(inresultdata);
	CK_BYTE outresultdata[256] = {0};
	CK_ULONG outresultdatalen=sizeof(outresultdata);

	//bk被设定为无法加解密，屏蔽
	return Free_Memory(FREE_0);
	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Enc, sizeof(ZUCkeyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	bRtn = Show_Result("pC_CreateObject hKey_Enc", RUN_CORRECT, "test_check_BK", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Dec, sizeof(ZUCkeyTemplate_Dec)/sizeof(CK_ATTRIBUTE), &hBaseKey);
	bRtn = Show_Result("pC_CreateObject hBaseKey", RUN_CORRECT, "test_check_BK", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = (*pC_EncryptInit)(hSession, &SM4mechanism, hKey_Enc);
	bRtn = Show_Result("pC_EncryptInit", RUN_CORRECT, "test_check_BK", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = (*pC_DecryptInit)(hSession, &SM4mechanism, hBaseKey);
	bRtn = Show_Result("pC_DecryptInit", RUN_CORRECT, "test_check_BK", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));


	indatalen = sizeof(indata);
	memset(indata, 0, indatalen);

	//将IV拼接到明文之前
	indatalen =  32;
	RandomGenerate(indata, indatalen);
	memset(inresultdata, 0, inresultdatalen);
	//加密运算
	rv = (*pC_Encrypt)(hSession, indata, indatalen, inresultdata, &inresultdatalen);
	bRtn = Show_Result("pC_Encrypt", RUN_CORRECT, "test_check_BK", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	memcpy(outdata,inresultdata, inresultdatalen);	
	outdatalen = inresultdatalen;

	rv = (*pC_Decrypt)(hSession, outdata, outdatalen, outresultdata, &outresultdatalen);
	bRtn = Show_Result("pC_Decrypt", RUN_CORRECT, "test_check_BK", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = Result_Compare(outresultdata, outresultdatalen, indata,indatalen, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	
	return Free_Memory(FREE_0);

}


CK_RV test_BKV4V5()
{
	CK_RV rv=0;
	CK_RV bRtn = FALSE;
	unsigned int modTemp = 0;
	BYTE Version_BK[REMOTE_DATA_VERSION_LEN] = { 0 };
	BYTE IV_BK[IV_LEN] = { 0 };

	BYTE * T_BK = NULL;
	unsigned short T_BKLen = 0;
	unsigned char *pbBuffer = NULL;
	unsigned char pSOPin[8] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	unsigned char nSOPinLen = sizeof(pSOPin);
	unsigned char pBaseKey[16] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	unsigned char pBaseKeyLen = sizeof(pBaseKey);
	unsigned char pBaseKeytmp[16] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};

	bRtn = Enter_Function("test_BKV4V5", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

#ifdef OPEN_LOGOUT_CLOSESSION
	bRtn = Get_Update_Data_Version(CKO_REMOTE_SECRET_KEY, Version_BK);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	
	bRtn = Set_BK_Local(pBaseKey, pBaseKeyLen, pSOPin, nSOPinLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	
	bRtn = test_check_BK(pBaseKey, pBaseKeyLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
#endif
	bRtn = Get_Update_Data_Version(CKO_REMOTE_SECRET_KEY, Version_BK);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));


	RandomGenerate(IV_BK, IV_LEN);	
	//memset(Version_BK, 0x03, REMOTE_DATA_VERSION_LEN);
	memset(pBaseKey, 0x08,pBaseKeyLen);
	modTemp = 8 - pBaseKeyLen;
	T_BKLen = 8 + pBaseKeyLen + modTemp%16;	
	T_BK = (BYTE*)malloc(sizeof(BYTE) * T_BKLen);
	bRtn = Generate_T(pBaseKey, pBaseKeyLen, IV_BK, pSOPin, nSOPinLen, Version_BK, T_BK, &T_BKLen);	
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_BK));

	bRtn = Remote_Set_Data(T_BK, T_BKLen, IV_BK,CKO_REMOTE_SECRET_KEY);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_BK));

	bRtn = Get_Update_Data_Version(CKO_REMOTE_SECRET_KEY, Version_BK);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_BK));

	if(CardVersionData <= CARD_VERSION_413)
	{
		memcpy(pBaseKey,pBaseKeytmp,pBaseKeyLen);
	}
	bRtn = test_check_BK(pBaseKey, pBaseKeyLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_BK));
	

#ifdef TT_INIT_TOKEN
	bRtn = Init_Token(pSOPin, nSOPinLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_BK));

	bRtn =Get_Update_Data_Version(CKO_REMOTE_SECRET_KEY, Version_BK);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_BK));

	memset(pBaseKey,0x00,pBaseKeyLen);
	bRtn = test_check_BK(pBaseKey, pBaseKeyLen);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_BK));
#endif
	bRtn = Leave_Function("test_BKV4V5", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_1,&T_BK));


	
	return Free_Memory(FREE_1,&T_BK);
}



CK_RV CreateKeyToBeWrapped(CK_KEY_TYPE KeyToBeWrappedType, CK_BBOOL KeyToBeWrappedTokenValue, CK_BBOOL ExtractableValue, CK_BBOOL WrapWithTrustedValue, unsigned char* KeyToBeWrappedValue, unsigned int KeyToBeWrappedValueLen, CK_OBJECT_HANDLE_PTR phKeyToBeWrapped)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;	
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID0;
	CK_OBJECT_HANDLE hKey = NULL_PTR;

	CK_ATTRIBUTE KeyTemplate[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &KeyToBeWrappedTokenValue, sizeof(KeyToBeWrappedTokenValue)},
		{CKA_KEY_TYPE, &KeyToBeWrappedType, sizeof(KeyToBeWrappedType)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE, KeyToBeWrappedValue,KeyToBeWrappedValueLen},
		{CKA_EXTRACTABLE, &ExtractableValue,sizeof(ExtractableValue)},
		{CKA_WRAP_WITH_TRUSTED, &WrapWithTrustedValue,sizeof(WrapWithTrustedValue)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(nSessKeyID)}
	};
	int Run_Flag = RUN_CORRECT ;

	
	if((KeyToBeWrappedTokenValue == TRUE)&&((ExtractableValue ==TRUE)||(WrapWithTrustedValue == TRUE)))
	{
		Run_Flag = RUN_INCORRECT;
	}

	if((KeyToBeWrappedTokenValue == FALSE)&&(ExtractableValue ==FALSE)&&(WrapWithTrustedValue == TRUE))
	{
		Run_Flag = RUN_INCORRECT;
	}
	
	rv = pC_CreateObject(hSession, KeyTemplate, sizeof(KeyTemplate)/sizeof(CK_ATTRIBUTE), phKeyToBeWrapped);
	bRtn = Show_Result("pC_CreateObject", Run_Flag, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	
	return Free_Memory(FREE_0);
}

CK_RV GetWrappedKeyValue(CK_OBJECT_HANDLE_PTR phWrappedKey)
{
	bool bRtn = false;
	CK_RV rv=0;
	unsigned char	KeyValGet[16] = {0x00};
	CK_ATTRIBUTE KeyTemplateGet[] = 
	{
		{CKA_VALUE,KeyValGet,sizeof(KeyValGet)}
	};

	rv = pC_GetAttributeValue(hSession, *phWrappedKey, KeyTemplateGet,sizeof(KeyTemplateGet)/sizeof(CK_ATTRIBUTE));
	bRtn = Show_Result("pC_GetAttributeValue",RUN_INCORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	
	
	return Free_Memory(FREE_0);
}

CK_RV CreateWrappingKeyUser(CK_KEY_TYPE WrappingKeyType, CK_BBOOL WrappingKeyTokenValue, CK_OBJECT_HANDLE_PTR phWrappingKey,CK_OBJECT_HANDLE_PTR phDecryptKey)
{
	CK_RV rv=0;
	bool bRtn=false;	
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;	
	CK_OBJECT_CLASS pubclass = CKO_PUBLIC_KEY;
	CK_OBJECT_CLASS priclass = CKO_PRIVATE_KEY;
	CK_UINT modulusBits = 256;


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

	CK_ATTRIBUTE publicKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(pubclass)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_WRAP, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&WrappingKeyType,sizeof(WrappingKeyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(modulusBits)},
		{CKA_ECC_X_COORDINATE, temp_pub_x, sizeof(temp_pub_x)},
		{CKA_ECC_Y_COORDINATE, temp_pub_y, sizeof(temp_pub_y)},
	};

	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(priclass)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_UNWRAP, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&WrappingKeyType,sizeof(WrappingKeyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(modulusBits)},
		{CKA_ECC_PRIVATE, temp_prv, sizeof(temp_prv)},
	};

	CK_BYTE nSessKeyID1 = CK_SESSKEY_PRESET_ID4;
	CK_BYTE nSessKeyID2 = CK_SESSKEY_PRESET_ID5;
	CK_BYTE     temp_sm4_key[] = { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
	//CK_BYTE     temp_sm4_key[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_ATTRIBUTE SM4keyTemplate1[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &WrappingKeyTokenValue, sizeof(WrappingKeyTokenValue)},
		{CKA_KEY_TYPE, &WrappingKeyType, sizeof(WrappingKeyType)},
		{CKA_WRAP, &ttrue, sizeof(ttrue)},
		//{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE, temp_sm4_key, sizeof(temp_sm4_key)},
		{CKA_SESSKEY_ID, &nSessKeyID1, sizeof(nSessKeyID1)}
	};

	CK_ATTRIBUTE SM4keyTemplate2[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &WrappingKeyTokenValue, sizeof(WrappingKeyTokenValue)},
		{CKA_KEY_TYPE, &WrappingKeyType, sizeof(WrappingKeyType)},
		//{CKA_UNWRAP, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE, temp_sm4_key, sizeof(temp_sm4_key)},
		{CKA_SESSKEY_ID, &nSessKeyID2, sizeof(nSessKeyID2)}
	};

	int Run_Flag = RUN_CORRECT ;

	if(WrappingKeyTokenValue == FALSE)
	{
		Run_Flag = RUN_INCORRECT;
	}

	if(WrappingKeyType == CKK_ECC)
	{
		rv = (*pC_CreateObject)(hSession, publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE), phWrappingKey);
		bRtn = Show_Result("phWrappingKey",RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		rv = (*pC_CreateObject)(hSession, privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE), phDecryptKey);
		bRtn = Show_Result("phDecryptKey",RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}
	else
	{
		rv = pC_CreateObject(hSession, SM4keyTemplate1, sizeof(SM4keyTemplate1)/sizeof(CK_ATTRIBUTE), phWrappingKey);
		bRtn = Show_Result("phWrappingKey",Run_Flag, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		rv = pC_CreateObject(hSession, SM4keyTemplate2, sizeof(SM4keyTemplate2)/sizeof(CK_ATTRIBUTE), phDecryptKey);
		bRtn = Show_Result("phDecryptKey",RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}

	
	return Free_Memory(FREE_0);
}

CK_RV CreateWrappingKeySo( CK_KEY_TYPE WrappingKeyType, CK_BBOOL WrappingKeyTokenValue, CK_OBJECT_HANDLE_PTR phWrappingKey,CK_OBJECT_HANDLE_PTR phDecryptKey)
{
	CK_RV rv=0;
	bool bRtn=false;	
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;	
	CK_OBJECT_CLASS pubclass = CKO_PUBLIC_KEY;
	CK_OBJECT_CLASS priclass = CKO_PRIVATE_KEY;
	CK_UINT modulusBits = 256;

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

	CK_ATTRIBUTE publicKeyTemplate[] = {		
		{CKA_TOKEN, &WrappingKeyTokenValue, sizeof(WrappingKeyTokenValue)},
		{CKA_CLASS, &pubclass, sizeof(pubclass)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_WRAP, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&WrappingKeyType,sizeof(WrappingKeyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(modulusBits)},
		{CKA_ECC_X_COORDINATE, temp_pub_x, sizeof(temp_pub_x)},
		{CKA_ECC_Y_COORDINATE, temp_pub_y, sizeof(temp_pub_y)},
	};

	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_TOKEN, &WrappingKeyTokenValue, sizeof(WrappingKeyTokenValue)},
		{CKA_CLASS, &priclass, sizeof(priclass)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_UNWRAP, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&WrappingKeyType,sizeof(WrappingKeyType)},
		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(modulusBits)},
		{CKA_ECC_PRIVATE, temp_prv, sizeof(temp_prv)},
	};

	CK_BYTE nSessKeyID1 = CK_SESSKEY_PRESET_ID7;
	CK_BYTE nSessKeyID2 = CK_SESSKEY_PRESET_ID5;
	CK_BYTE     temp_sm4_key[] = { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
	//CK_BYTE     temp_sm4_key[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_ATTRIBUTE SM4keyTemplate1[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &WrappingKeyTokenValue, sizeof(WrappingKeyTokenValue)},
		{CKA_KEY_TYPE, &WrappingKeyType, sizeof(WrappingKeyType)},
		{CKA_WRAP, &ttrue, sizeof(ttrue)},
		//{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE, temp_sm4_key, sizeof(temp_sm4_key)},
		{CKA_SESSKEY_ID, &nSessKeyID1, sizeof(nSessKeyID1)}
	};

	CK_ATTRIBUTE SM4keyTemplate2[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &WrappingKeyTokenValue, sizeof(WrappingKeyTokenValue)},
		{CKA_KEY_TYPE, &WrappingKeyType, sizeof(WrappingKeyType)},
		//{CKA_UNWRAP, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE, temp_sm4_key, sizeof(temp_sm4_key)},
		{CKA_SESSKEY_ID, &nSessKeyID2, sizeof(nSessKeyID2)}
	};

	int Run_Flag = RUN_CORRECT ;

	if(WrappingKeyTokenValue==FALSE)
	{
		Run_Flag = RUN_INCORRECT;
	}

	if(WrappingKeyType == CKK_ECC)
	{
		rv = (*pC_CreateObject)(hSession, publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE), phWrappingKey);
		bRtn = Show_Result("phWrappingKey",RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		rv = (*pC_CreateObject)(hSession, privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE), phDecryptKey);
		bRtn = Show_Result("phDecryptKey",RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}
	else
	{
		bRtn = user_to_so();
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
		rv = pC_CreateObject(hSession, SM4keyTemplate1, sizeof(SM4keyTemplate1)/sizeof(CK_ATTRIBUTE), phWrappingKey);
		bRtn = Show_Result("phWrappingKey",Run_Flag, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		bRtn = so_to_user();
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
		rv = pC_CreateObject(hSession, SM4keyTemplate2, sizeof(SM4keyTemplate2)/sizeof(CK_ATTRIBUTE), phDecryptKey);
		bRtn = Show_Result("phDecryptKey",RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}

	
	return Free_Memory(FREE_0);
}

CK_RV WrappingTheKeyToBeWrapped(CK_OBJECT_HANDLE_PTR phKeyToBeWrapped, CK_OBJECT_HANDLE_PTR phWrappingKey, unsigned int WrapMechanismType, unsigned char* WrappedKeyValue, unsigned int* pWrappedKeyValueLen, int Run_Flag)
{
	CK_RV rv=0;
	bool bRtn=false;		
	CK_BYTE SM4_CBC_iv[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_MECHANISM WrapMechanism = {WrapMechanismType, NULL_PTR, 0};
	
	if(WrapMechanismType==CKM_SM4_CBC)
	{
		WrapMechanism.pParameter = SM4_CBC_iv;
		WrapMechanism.ulParameterLen = sizeof(SM4_CBC_iv);
	}

	rv = (*pC_WrapKey)(hSession, &WrapMechanism, *phWrappingKey, *phKeyToBeWrapped, WrappedKeyValue, pWrappedKeyValueLen);
	bRtn = Show_Result("pC_WrapKey",Run_Flag, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	return Free_Memory(FREE_0);
}

CK_RV DecryptWrappedKey(unsigned char* WrappedKeyValue, unsigned char WrappedKeyValueLen, CK_OBJECT_HANDLE_PTR phDecryptKey, unsigned int DecrptMechanismType, unsigned char* DecryptedKeyValue, unsigned int* pDecryptedKeyValueLen)
{
	CK_RV rv=0;
	bool bRtn=false;	
	CK_BYTE SM4_CBC_iv[16]= {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
	CK_MECHANISM DecryptMechanism = {DecrptMechanismType, NULL_PTR, 0};
	
	if(DecrptMechanismType==CKM_SM4_CBC)
	{
		DecryptMechanism.pParameter = SM4_CBC_iv;
		DecryptMechanism.ulParameterLen = sizeof(SM4_CBC_iv);
	}

	rv = (*pC_DecryptInit)(hSession, &DecryptMechanism, *phDecryptKey);
	bRtn = Show_Result("pC_DecryptInit",RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = (*pC_Decrypt)(hSession,WrappedKeyValue, WrappedKeyValueLen, DecryptedKeyValue, pDecryptedKeyValueLen);
	bRtn = Show_Result("pC_Decrypt",RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	return Free_Memory(FREE_0);
}

CK_RV CheckOneTypeKeyUser(CK_KEY_TYPE KeyToBeWrappedType,CK_KEY_TYPE WrappingKeyType,unsigned int WrapMechanismType,CK_BBOOL*TokenValue,CK_BBOOL*ExtractableValue,CK_BBOOL*WrapWithTrustedValue)
{
	CK_RV bRtn = FALSE;
	CK_RV rv=0;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;

	CK_OBJECT_HANDLE hKeyToBeWrapped = NULL_PTR;
	CK_OBJECT_HANDLE hWrappingKey = NULL_PTR;
	CK_OBJECT_HANDLE hDecryptKey = NULL_PTR;

	CK_BYTE KeyToBeWrappedValue[16] = {0};
	unsigned int KeyToBeWrappedValueLen = sizeof(KeyToBeWrappedValue);

	CK_BYTE WrappedKeyValue[16 + 96] = {0};
	unsigned int WrappedKeyValueLen = sizeof(WrappedKeyValue);

	CK_BYTE DecryptedKeyValue[16] = {0};
	unsigned int DecryptedKeyValueLen = sizeof(DecryptedKeyValue);
	int i = 0;
	int Wrapping_Flag = RUN_CORRECT;

	for(i = 0; i < 8; ++i)
	{	
		Wrapping_Flag = RUN_CORRECT;
		if((TokenValue[i] == TRUE)||(WrapWithTrustedValue[i] == TRUE)||(ExtractableValue[i] == FALSE))
		{
			Wrapping_Flag = RUN_INCORRECT;
		}

		RandomGenerate(KeyToBeWrappedValue, KeyToBeWrappedValueLen);

		bRtn = CreateKeyToBeWrapped(KeyToBeWrappedType, TokenValue[i], ExtractableValue[i], WrapWithTrustedValue[i], KeyToBeWrappedValue, KeyToBeWrappedValueLen, &hKeyToBeWrapped);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
		
		if((TokenValue[i] == TRUE)&&((ExtractableValue[i] ==TRUE)||(WrapWithTrustedValue[i] == TRUE)))
		{
			continue;
		}

		//if((TokenValue[i] == FALSE)&&(ExtractableValue[i] ==FALSE)&&((WrapWithTrustedValue[i] == TRUE))
		//{
		//	continue;
		//}
			
		bRtn = GetWrappedKeyValue(&hKeyToBeWrapped);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		bRtn = CreateWrappingKeyUser(WrappingKeyType, ttrue, &hWrappingKey, &hDecryptKey);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		bRtn = WrappingTheKeyToBeWrapped(&hKeyToBeWrapped, &hWrappingKey, WrapMechanismType, WrappedKeyValue, &WrappedKeyValueLen, Wrapping_Flag);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		
		if((TokenValue[i] == TRUE)||(WrapWithTrustedValue[i] == TRUE)||(ExtractableValue[i] == FALSE))
		{
			if(WrappingKeyType == CKK_ECC)
			{
				bRtn = Destory_Useless_KeyPair(hWrappingKey,hDecryptKey,Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
			}	
			continue;
		}


		bRtn = DecryptWrappedKey(WrappedKeyValue, WrappedKeyValueLen, &hDecryptKey, WrapMechanismType, DecryptedKeyValue, &DecryptedKeyValueLen);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		if(WrappingKeyType == CKK_ECC)
		{
			bRtn = Destory_Useless_KeyPair(hWrappingKey,hDecryptKey,Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
		}

		bRtn = Result_Compare(DecryptedKeyValue, DecryptedKeyValueLen, KeyToBeWrappedValue,KeyToBeWrappedValueLen, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	
	}
	return Free_Memory(FREE_0);
}

CK_RV CheckOneTypeKeySo(CK_KEY_TYPE KeyToBeWrappedType,CK_KEY_TYPE WrappingKeyType,unsigned int WrapMechanismType, CK_BBOOL*TokenValue,CK_BBOOL*ExtractableValue,CK_BBOOL*WrapWithTrustedValue)
{
	CK_RV bRtn = FALSE;
	CK_RV rv=0;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;

	CK_OBJECT_HANDLE hKeyToBeWrapped = NULL_PTR;
	CK_OBJECT_HANDLE hWrappingKey = NULL_PTR;
	CK_OBJECT_HANDLE hDecryptKey = NULL_PTR;

	CK_BYTE KeyToBeWrappedValue[16] = {0};
	unsigned int KeyToBeWrappedValueLen = sizeof(KeyToBeWrappedValue);

	CK_BYTE WrappedKeyValue[16] = {0};
	unsigned int WrappedKeyValueLen = sizeof(WrappedKeyValue);

	CK_BYTE DecryptedKeyValue[16] = {0};
	unsigned int DecryptedKeyValueLen = sizeof(DecryptedKeyValue);
	int i = 0;
	int Wrapping_Flag = RUN_CORRECT;

	for(i = 0; i < 8; ++i)
	{	
		Wrapping_Flag = RUN_CORRECT;
		if((TokenValue[i] == TRUE)||(ExtractableValue[i] == FALSE))
		{
			Wrapping_Flag = RUN_INCORRECT;
		}

		RandomGenerate(KeyToBeWrappedValue, KeyToBeWrappedValueLen);

		bRtn = CreateKeyToBeWrapped(KeyToBeWrappedType, TokenValue[i], ExtractableValue[i], WrapWithTrustedValue[i], KeyToBeWrappedValue, KeyToBeWrappedValueLen, &hKeyToBeWrapped);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		if((TokenValue[i] == TRUE)&&((ExtractableValue[i] ==TRUE)||(WrapWithTrustedValue[i] == TRUE)))
		{
			continue;
		}

		bRtn = GetWrappedKeyValue(&hKeyToBeWrapped);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		bRtn = CreateWrappingKeySo(WrappingKeyType, ttrue, &hWrappingKey, &hDecryptKey);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		bRtn = WrappingTheKeyToBeWrapped(&hKeyToBeWrapped, &hWrappingKey, WrapMechanismType, WrappedKeyValue, &WrappedKeyValueLen, Wrapping_Flag);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		if((TokenValue[i] == TRUE)||(ExtractableValue[i] == FALSE))
		{
			continue;
		}

		bRtn = DecryptWrappedKey(WrappedKeyValue, WrappedKeyValueLen, &hDecryptKey, WrapMechanismType, DecryptedKeyValue, &DecryptedKeyValueLen);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		bRtn = Result_Compare(DecryptedKeyValue, DecryptedKeyValueLen, KeyToBeWrappedValue,KeyToBeWrappedValueLen, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	}


	//for(i = 0; i < 8; ++i)
	//{
	//	printf("i=%d\n",i);
	//	RandomGenerate(KeyToBeWrappedValue, KeyToBeWrappedValueLen);
	//	//memset(KeyToBeWrappedValue,0,KeyToBeWrappedValueLen);
	//	bRtn = CreateKeyToBeWrapped(KeyToBeWrappedType, TokenValue[i], ExtractableValue[i], WrapWithTrustedValue[i], KeyToBeWrappedValue, KeyToBeWrappedValueLen, &hKeyToBeWrapped);
	//	if(bRtn == TRUE)
	//	{
	//		bRtn = GetWrappedKeyValue(&hKeyToBeWrapped);
	//		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//		bRtn = CreateWrappingKeySo(WrappingKeyType, ttrue, &hWrappingKey, &hDecryptKey);
	//		if(bRtn == TRUE)
	//		{
	//			bRtn = WrappingTheKeyToBeWrapped(&hKeyToBeWrapped, &hWrappingKey, WrapMechanismType, WrappedKeyValue, &WrappedKeyValueLen);
	//			if(bRtn == FALSE)
	//			{
	//				//if((TokenValue[i] == TRUE)||(ExtractableValue[i] == FALSE))
	//				//{
	//					continue;
	//				//}
	//			}				

	//			bRtn = DecryptWrappedKey(WrappedKeyValue, WrappedKeyValueLen, &hDecryptKey, WrapMechanismType, DecryptedKeyValue, &DecryptedKeyValueLen);
	//			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//			bRtn = Result_Compare(DecryptedKeyValue, DecryptedKeyValueLen, KeyToBeWrappedValue,KeyToBeWrappedValueLen, Testresult[nItemNumb], &nResultLength);
	//			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	//		}
	//	}
	//}
	return Free_Memory(FREE_0);
}

CK_RV test_WrapKeyOut()
{
	CK_RV bRtn = FALSE;
	CK_RV rv=0;

	//78=>45
	CK_BBOOL TokenValue[8] =			{ CK_TRUE, CK_TRUE, CK_TRUE, CK_FALSE, CK_FALSE, CK_TRUE, CK_FALSE, CK_FALSE };
	CK_BBOOL ExtractableValue[8] =		{ CK_TRUE, CK_TRUE, CK_FALSE, CK_FALSE, CK_FALSE, CK_FALSE, CK_TRUE, CK_TRUE };
	CK_BBOOL WrapWithTrustedValue[8] =	{ CK_TRUE, CK_FALSE, CK_TRUE, CK_TRUE, CK_FALSE, CK_FALSE, CK_TRUE, CK_FALSE };

	bRtn = Enter_Function("test_WrapKeyOut", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

#ifdef OPEN_LOGOUT_CLOSESSION
	bRtn = CheckOneTypeKeySo(CKK_SM4, CKK_SM4, CKM_SM4_ECB, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = CheckOneTypeKeySo(CKK_SM4, CKK_SM4, CKM_SM4_CBC, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
#endif	

	bRtn = CheckOneTypeKeyUser(CKK_SM4, CKK_SM4, CKM_SM4_ECB, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = CheckOneTypeKeyUser(CKK_SM4, CKK_SM4, CKM_SM4_CBC, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

#ifdef OPEN_LOGOUT_CLOSESSION
	bRtn = CheckOneTypeKeySo(CKK_ZUC, CKK_SM4, CKM_SM4_ECB, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = CheckOneTypeKeySo(CKK_ZUC, CKK_SM4, CKM_SM4_CBC, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
#endif

	bRtn = CheckOneTypeKeyUser(CKK_ZUC, CKK_SM4, CKM_SM4_ECB, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = CheckOneTypeKeyUser(CKK_ZUC, CKK_SM4, CKM_SM4_CBC, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));


	bRtn = CheckOneTypeKeyUser(CKK_SM4, CKK_ECC, CKM_ECC_CALC, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = CheckOneTypeKeyUser(CKK_ZUC, CKK_ECC, CKM_ECC_CALC, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = Leave_Function("test_WrapKeyOut", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	return Free_Memory(FREE_0);
}


CK_RV GenKeyToBeWrapped(CK_KEY_TYPE KeyToBeWrappedType, CK_BBOOL KeyToBeWrappedTokenValue, CK_BBOOL ExtractableValue, CK_BBOOL WrapWithTrustedValue, CK_OBJECT_HANDLE_PTR phKeyToBeWrapped)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;	
	CK_BYTE nSessKeyID = CK_SESSKEY_PRESET_ID0;
	CK_OBJECT_HANDLE hKey = NULL_PTR;
	CK_MECHANISM mechanismGen = {NULL_PTR, NULL_PTR, 0};
	CK_ATTRIBUTE KeyTemplate[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &KeyToBeWrappedTokenValue, sizeof(KeyToBeWrappedTokenValue)},
		{CKA_KEY_TYPE, &KeyToBeWrappedType, sizeof(KeyToBeWrappedType)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_EXTRACTABLE, &ExtractableValue,sizeof(ExtractableValue)},
		{CKA_WRAP_WITH_TRUSTED, &WrapWithTrustedValue,sizeof(WrapWithTrustedValue)},
		{CKA_SESSKEY_ID, &nSessKeyID, sizeof(nSessKeyID)}
	};

	int Run_Flag = RUN_CORRECT ;


	if((KeyToBeWrappedTokenValue == TRUE)&&((ExtractableValue ==TRUE)||(WrapWithTrustedValue == TRUE)))
	{
		Run_Flag = RUN_INCORRECT;
	}

	if((KeyToBeWrappedTokenValue == FALSE)&&(ExtractableValue ==FALSE)&&(WrapWithTrustedValue == TRUE))
	{
		Run_Flag = RUN_INCORRECT;
	}

	if(KeyToBeWrappedType == CKK_SM4)
	{
		mechanismGen.mechanism = CKM_SM4_KEY_GEN;
	}
	else if(KeyToBeWrappedType == CKK_ZUC)
	{
		mechanismGen.mechanism = CKM_ZUC_KEY_GEN;
	}
	else
	{
		printf("Error:key type invalid\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"Error:key type invalid<br>");
		return Free_Memory(FREE_0);
	}
	rv = pC_GenerateKey(hSession, &mechanismGen, KeyTemplate, sizeof(KeyTemplate)/sizeof(CK_ATTRIBUTE), phKeyToBeWrapped);
	bRtn = Show_Result("pC_GenerateKey",Run_Flag, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	return Free_Memory(FREE_0);
}

CK_RV Check_GenKey_by_KeyValue(CK_KEY_TYPE KeyToBeWrappedType,unsigned char* pBaseKey, unsigned char pBaseKeyLen, CK_OBJECT_HANDLE_PTR phKeyToBeWrapped)
{
	CK_RV bRtn = FALSE;
	CK_RV rv = 0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_BYTE nSessKeyID_Enc = CK_SESSKEY_PRESET_ID1;
	unsigned char	SM4iv[16] = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};

	CK_ATTRIBUTE ZUCkeyTemplate_Enc[] = 
	{
		{CKA_CLASS, &keyClass, sizeof(keyClass)},
		{CKA_TOKEN, &ffalse, sizeof(ffalse)},
		{CKA_KEY_TYPE, &KeyToBeWrappedType, sizeof(KeyToBeWrappedType)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VALUE,pBaseKey,pBaseKeyLen},
		{CKA_SESSKEY_ID, &nSessKeyID_Enc, sizeof(nSessKeyID_Enc)}
	};

	CK_BYTE ZUC_IV[5] ={ 1, 2, 3, 4, 5};

	CK_MECHANISM mechanismInit = {CKM_SM4_ECB, NULL_PTR, 0};
	CK_OBJECT_HANDLE hKey_Enc = NULL_PTR;

	CK_BYTE indata[256] = {0};
	CK_ULONG indatalen=sizeof(indata);
	CK_BYTE outdata[256] = {0};
	CK_ULONG outdatalen=sizeof(outdata);
	CK_BYTE inresultdata[256] = {0};
	CK_ULONG inresultdatalen=sizeof(inresultdata);
	CK_BYTE outresultdata[256] = {0};
	CK_ULONG outresultdatalen=sizeof(outresultdata);

	if(KeyToBeWrappedType == CKK_SM4)
	{
		mechanismInit.mechanism = CKM_SM4_ECB;
	}
	else if(KeyToBeWrappedType == CKK_ZUC)
	{
		mechanismInit.mechanism = CKM_ZUC_CALC;
	}
	else
	{
		printf("Error:key type invalid\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "Error:key type invalid<br>");
		return Free_Memory(FREE_0);
	}

	hKey_Enc = NULL_PTR;
	rv = pC_CreateObject(hSession, ZUCkeyTemplate_Enc, sizeof(ZUCkeyTemplate_Enc)/sizeof(CK_ATTRIBUTE), &hKey_Enc);
	bRtn = Show_Result("pC_CreateObject hKey_Enc", RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = (*pC_EncryptInit)(hSession, &mechanismInit, hKey_Enc);
	bRtn = Show_Result("pC_EncryptInit", RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	rv = (*pC_DecryptInit)(hSession, &mechanismInit, *phKeyToBeWrapped);
	bRtn = Show_Result("pC_DecryptInit", RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	indatalen = sizeof(indata);
	memset(indata, 0, indatalen);

	//将IV拼接到明文之前
	if(KeyToBeWrappedType == CKK_SM4)
	{
		indatalen =  32;
		RandomGenerate(indata, indatalen);
	}
	else if(KeyToBeWrappedType == CKK_ZUC)
	{
		memcpy(indata, ZUC_IV, 5);		
		RandomGenerate(indata + 5, 32);
		indatalen =  32 + 5;
	}
	else
	{
		printf("Error:key type invalid\n");
		return Free_Memory(FREE_0);
	}

	memset(inresultdata, 0, inresultdatalen);
	//加密运算
	rv = (*pC_Encrypt)(hSession, indata, indatalen, inresultdata, &inresultdatalen);
	bRtn = Show_Result("pC_DecryptInit", RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));


	if(KeyToBeWrappedType == CKK_SM4)
	{
		memcpy(outdata,inresultdata, inresultdatalen);	
		outdatalen = inresultdatalen;
	}
	else if(KeyToBeWrappedType == CKK_ZUC)
	{
		memcpy(outdata, ZUC_IV, 5);		
		memcpy(outdata + 5,inresultdata, inresultdatalen);	
		outdatalen = inresultdatalen +5;
	}
	else
	{
		printf("Error:key type invalid\n");
		return Free_Memory(FREE_0);
	}

	rv = (*pC_Decrypt)(hSession, outdata, outdatalen, outresultdata, &outresultdatalen);
	bRtn = Show_Result("pC_DecryptInit", RUN_CORRECT, __FUNCTION__, rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	if(KeyToBeWrappedType == CKK_SM4)
	{
		bRtn = Result_Compare(outresultdata, outresultdatalen, indata,indatalen, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}
	else if(KeyToBeWrappedType == CKK_ZUC)
	{
		bRtn = Result_Compare(outresultdata, outresultdatalen, indata + 5,indatalen - 5, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}
	else
	{
		printf("Error:key type invalid\n");
		return Free_Memory(FREE_0);
	}

	return Free_Memory(FREE_0);
}

CK_RV CheckOneTypeKeyUserGen(CK_KEY_TYPE KeyToBeWrappedType,CK_KEY_TYPE WrappingKeyType,unsigned int WrapMechanismType,CK_BBOOL*TokenValue,CK_BBOOL*ExtractableValue,CK_BBOOL*WrapWithTrustedValue)
{
	CK_RV bRtn = FALSE;
	CK_RV rv=0;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;

	CK_OBJECT_HANDLE hKeyToBeWrapped = NULL_PTR;
	CK_OBJECT_HANDLE hWrappingKey = NULL_PTR;
	CK_OBJECT_HANDLE hDecryptKey = NULL_PTR;

	CK_BYTE WrappedKeyValue[16 + 96] = {0};
	unsigned int WrappedKeyValueLen = sizeof(WrappedKeyValue);

	CK_BYTE DecryptedKeyValue[16] = {0};
	unsigned int DecryptedKeyValueLen = sizeof(DecryptedKeyValue);
	int i = 0;
	int Wrapping_Flag = RUN_CORRECT;

	for(i = 0; i < 8; ++i)
	{	
		Wrapping_Flag = RUN_CORRECT;
		if((TokenValue[i] == TRUE)||(WrapWithTrustedValue[i] == TRUE)||(ExtractableValue[i] == FALSE))
		{
			Wrapping_Flag = RUN_INCORRECT;
		}

		bRtn = GenKeyToBeWrapped(KeyToBeWrappedType, TokenValue[i], ExtractableValue[i], WrapWithTrustedValue[i], &hKeyToBeWrapped);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));


		if((TokenValue[i] == TRUE)&&((ExtractableValue[i] ==TRUE)||(WrapWithTrustedValue[i] == TRUE)))
		{
			continue;
		}

		//if((TokenValue[i] == FALSE)&&(ExtractableValue[i] ==FALSE)&&(WrapWithTrustedValue[i] == TRUE))
		//{
		//	continue;
		//}

		bRtn = GetWrappedKeyValue(&hKeyToBeWrapped);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
		
		bRtn = CreateWrappingKeyUser(WrappingKeyType, ttrue, &hWrappingKey, &hDecryptKey);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
		
		bRtn = WrappingTheKeyToBeWrapped(&hKeyToBeWrapped, &hWrappingKey, WrapMechanismType, WrappedKeyValue, &WrappedKeyValueLen, Wrapping_Flag);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
		

		if((TokenValue[i] == TRUE)||(WrapWithTrustedValue[i] == TRUE)||(ExtractableValue[i] == FALSE))
		{
			if(WrappingKeyType == CKK_ECC)
			{
				bRtn = Destory_Useless_KeyPair(hWrappingKey,hDecryptKey,Testresult[nItemNumb], &nResultLength);
				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
			}	
			continue;
		}


		bRtn = DecryptWrappedKey(WrappedKeyValue, WrappedKeyValueLen, &hDecryptKey, WrapMechanismType, DecryptedKeyValue, &DecryptedKeyValueLen);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		if(WrappingKeyType == CKK_ECC)
		{
			bRtn = Destory_Useless_KeyPair(hWrappingKey,hDecryptKey,Testresult[nItemNumb], &nResultLength);
			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
		}

		bRtn = Check_GenKey_by_KeyValue(KeyToBeWrappedType, DecryptedKeyValue, DecryptedKeyValueLen, &hKeyToBeWrapped);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}

	//for(i = 0; i < 8; ++i)
	//{		
	//	printf("user i=%d\r",i);
	//	//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"so i=%d\r",i);
	//	bRtn = GenKeyToBeWrapped(KeyToBeWrappedType, TokenValue[i], ExtractableValue[i], WrapWithTrustedValue[i], &hKeyToBeWrapped);
	//	if(bRtn == TRUE)
	//	{
	//		bRtn = GetWrappedKeyValue(&hKeyToBeWrapped);
	//		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//		bRtn = CreateWrappingKeyUser(WrappingKeyType, ttrue, &hWrappingKey, &hDecryptKey);
	//		if(bRtn == TRUE)
	//		{
	//			bRtn = WrappingTheKeyToBeWrapped(&hKeyToBeWrapped, &hWrappingKey, WrapMechanismType, WrappedKeyValue, &WrappedKeyValueLen,Run_Flag);
	//			if(bRtn == FALSE)
	//			{
	//				if(WrappingKeyType == CKK_ECC)
	//				{
	//					bRtn = Destory_Useless_KeyPair(hWrappingKey,hDecryptKey,Testresult[nItemNumb], &nResultLength);
	//					RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	//				}
	//				//if((TokenValue[i] == TRUE)||(WrapWithTrustedValue[i] == TRUE)||(ExtractableValue[i] == FALSE))
	//				//{
	//				continue;
	//				//}
	//			}				

	//			bRtn = DecryptWrappedKey(WrappedKeyValue, WrappedKeyValueLen, &hDecryptKey, WrapMechanismType, DecryptedKeyValue, &DecryptedKeyValueLen);
	//			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//			if(WrappingKeyType == CKK_ECC)
	//			{
	//				bRtn = Destory_Useless_KeyPair(hWrappingKey,hDecryptKey,Testresult[nItemNumb], &nResultLength);
	//				RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	//			}

	//			bRtn = Check_GenKey_by_KeyValue(KeyToBeWrappedType, DecryptedKeyValue, DecryptedKeyValueLen, &hKeyToBeWrapped);
	//			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	//		}
	//	}
	//}
	return Free_Memory(FREE_0);
}

CK_RV CheckOneTypeKeySoGen(CK_KEY_TYPE KeyToBeWrappedType,CK_KEY_TYPE WrappingKeyType,unsigned int WrapMechanismType, CK_BBOOL*TokenValue,CK_BBOOL*ExtractableValue,CK_BBOOL*WrapWithTrustedValue)
{
	CK_RV bRtn = FALSE;
	CK_RV rv=0;
	CK_BBOOL ttrue = CK_TRUE;
	CK_BBOOL ffalse = CK_FALSE;
	CK_KEY_TYPE SM4keyType = CKK_SM4;

	CK_OBJECT_HANDLE hKeyToBeWrapped = NULL_PTR;
	CK_OBJECT_HANDLE hWrappingKey = NULL_PTR;
	CK_OBJECT_HANDLE hDecryptKey = NULL_PTR;

	CK_BYTE WrappedKeyValue[16] = {0};
	unsigned int WrappedKeyValueLen = sizeof(WrappedKeyValue);

	CK_BYTE DecryptedKeyValue[16] = {0};
	unsigned int DecryptedKeyValueLen = sizeof(DecryptedKeyValue);
	int i = 0;
	int Wrapping_Flag = RUN_CORRECT;

	for(i = 0; i < 8; ++i)
	{		
		Wrapping_Flag = RUN_CORRECT;
		if((TokenValue[i] == TRUE)||(ExtractableValue[i] == FALSE))
		{
			Wrapping_Flag = RUN_INCORRECT;
		}

		bRtn = GenKeyToBeWrapped(KeyToBeWrappedType, TokenValue[i], ExtractableValue[i], WrapWithTrustedValue[i], &hKeyToBeWrapped);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		if((TokenValue[i] == TRUE)&&((ExtractableValue[i] ==TRUE)||(WrapWithTrustedValue[i] == TRUE)))
		{
			continue;
		}

		//if((TokenValue[i] == FALSE)&&(ExtractableValue[i] ==FALSE)&&(WrapWithTrustedValue[i] == TRUE))
		//{
		//	continue;
		//}

		bRtn = GetWrappedKeyValue(&hKeyToBeWrapped);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		bRtn = CreateWrappingKeySo(WrappingKeyType, ttrue, &hWrappingKey, &hDecryptKey);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		bRtn = WrappingTheKeyToBeWrapped(&hKeyToBeWrapped, &hWrappingKey, WrapMechanismType, WrappedKeyValue, &WrappedKeyValueLen, Wrapping_Flag);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		if((TokenValue[i] == TRUE)||(ExtractableValue[i] == FALSE))
		{
			continue;
		}

		bRtn = DecryptWrappedKey(WrappedKeyValue, WrappedKeyValueLen, &hDecryptKey, WrapMechanismType, DecryptedKeyValue, &DecryptedKeyValueLen);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

		bRtn = Check_GenKey_by_KeyValue(KeyToBeWrappedType, DecryptedKeyValue, DecryptedKeyValueLen, &hKeyToBeWrapped);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	}

	//for(i = 0; i < 8; ++i)
	//{
	//	printf("so i=%d\r",i);
	//	//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"so i=%d\r",i);
	//	bRtn = GenKeyToBeWrapped(KeyToBeWrappedType, TokenValue[i], ExtractableValue[i], WrapWithTrustedValue[i], &hKeyToBeWrapped);
	//	if(bRtn == TRUE)
	//	{
	//		bRtn = GetWrappedKeyValue(&hKeyToBeWrapped);
	//		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//		bRtn = CreateWrappingKeySo(WrappingKeyType, ttrue, &hWrappingKey, &hDecryptKey);
	//		if(bRtn == TRUE)
	//		{
	//			bRtn = WrappingTheKeyToBeWrapped(&hKeyToBeWrapped, &hWrappingKey, WrapMechanismType, WrappedKeyValue, &WrappedKeyValueLen, Run_Flag);
	//			if(bRtn == FALSE)
	//			{
	//				//if((TokenValue[i] == TRUE)||(ExtractableValue[i] == FALSE))
	//				//{
	//				continue;
	//				//}
	//			}				

	//			bRtn = DecryptWrappedKey(WrappedKeyValue, WrappedKeyValueLen, &hDecryptKey, WrapMechanismType, DecryptedKeyValue, &DecryptedKeyValueLen);
	//			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	//			bRtn = Check_GenKey_by_KeyValue(KeyToBeWrappedType, DecryptedKeyValue, DecryptedKeyValueLen, &hKeyToBeWrapped);
	//			RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	//		}
	//	}
	//}
	return Free_Memory(FREE_0);
}

CK_RV test_WrapKeyOut_Gen()
{
	CK_RV bRtn = FALSE;
	CK_RV rv=0;

	//78=>45
	CK_BBOOL TokenValue[8] =			{ CK_TRUE, CK_TRUE, CK_TRUE, CK_FALSE, CK_FALSE, CK_TRUE, CK_FALSE, CK_FALSE };
	CK_BBOOL ExtractableValue[8] =		{ CK_TRUE, CK_TRUE, CK_FALSE, CK_FALSE, CK_FALSE, CK_FALSE, CK_TRUE, CK_TRUE };
	CK_BBOOL WrapWithTrustedValue[8] =	{ CK_TRUE, CK_FALSE, CK_TRUE, CK_TRUE, CK_FALSE, CK_FALSE, CK_TRUE, CK_FALSE };


	bRtn = Enter_Function("test_WrapKeyOutGen", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

#ifdef OPEN_LOGOUT_CLOSESSION
	bRtn = CheckOneTypeKeySoGen(CKK_SM4, CKK_SM4, CKM_SM4_ECB, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = CheckOneTypeKeySoGen(CKK_SM4, CKK_SM4, CKM_SM4_CBC, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
#endif

	bRtn = CheckOneTypeKeyUserGen(CKK_SM4, CKK_SM4, CKM_SM4_ECB, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = CheckOneTypeKeyUserGen(CKK_SM4, CKK_SM4, CKM_SM4_CBC, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

#ifdef OPEN_LOGOUT_CLOSESSION
	bRtn = CheckOneTypeKeySoGen(CKK_ZUC, CKK_SM4, CKM_SM4_ECB, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = CheckOneTypeKeySoGen(CKK_ZUC, CKK_SM4, CKM_SM4_CBC, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
#endif

	bRtn = CheckOneTypeKeyUserGen(CKK_ZUC, CKK_SM4, CKM_SM4_ECB, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = CheckOneTypeKeyUserGen(CKK_ZUC, CKK_SM4, CKM_SM4_CBC, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));



	bRtn = CheckOneTypeKeyUserGen(CKK_SM4, CKK_ECC, CKM_ECC_CALC, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = CheckOneTypeKeyUserGen(CKK_ZUC, CKK_ECC, CKM_ECC_CALC, TokenValue, ExtractableValue, WrapWithTrustedValue);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	bRtn = Leave_Function("test_WrapKeyOutGen", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	return Free_Memory(FREE_0);
}



CK_RV CreateKeyOnce(CK_BYTE nSessKeyID, CK_BBOOL TokenValue, CK_BBOOL EncryptValue, CK_BBOOL DecryptValue, CK_BBOOL WrapValue, CK_BBOOL UnwrapValue)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_OBJECT_HANDLE hKey = NULL_PTR;
	CK_BYTE KeyValue[16] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
	int Run_Flag = 0;
	CK_ATTRIBUTE KeyTemplate[] = 
	{
		{CKA_CLASS,			&keyClass,		sizeof(keyClass)},
		{CKA_TOKEN,			&TokenValue,	sizeof(TokenValue)},
		{CKA_KEY_TYPE,		&SM4keyType,	sizeof(SM4keyType)},
		{CKA_ENCRYPT,		&EncryptValue,	sizeof(EncryptValue)},
		{CKA_DECRYPT,		&DecryptValue,	sizeof(DecryptValue)},
		{CKA_WRAP,			&WrapValue,		sizeof(WrapValue)},
		{CKA_UNWRAP,		&UnwrapValue,	sizeof(UnwrapValue)},
		{CKA_VALUE,			KeyValue,		sizeof(KeyValue)},
		{CKA_SESSKEY_ID,	&nSessKeyID,	sizeof(nSessKeyID)}
	};

	Run_Flag = RUN_CORRECT;

	if((TokenValue == FALSE)&&(nSessKeyID == CK_SESSKEY_PRESET_ID7))
	{
		Run_Flag = RUN_INCORRECT;
	}

	if((EncryptValue == TRUE)&&(UnwrapValue == TRUE))
	{
		Run_Flag = RUN_INCORRECT;
	}

	if((DecryptValue == TRUE)&&(WrapValue == TRUE))
	{
		Run_Flag = RUN_INCORRECT;
	}

	//if((UnwrapValue == TRUE)&&(WrapValue == TRUE))
	//{
	//	Run_Flag = RUN_INCORRECT;
	//}

	rv = pC_CreateObject(hSession, KeyTemplate, sizeof(KeyTemplate)/sizeof(CK_ATTRIBUTE), &hKey);
	bRtn = Show_Result("pC_CreateObject",Run_Flag, "CreateKeyOnce", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	if(hKey != NULL_PTR)
	{
		rv = pC_DestroyObject(hSession, hKey);
		bRtn = Show_Result("pC_DestroyObject",Run_Flag, "CreateKeyOnce", rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}

	return Free_Memory(FREE_0);
}

CK_RV GenerateKeyOnce(CK_BYTE nSessKeyID ,CK_BBOOL TokenValue, CK_BBOOL EncryptValue, CK_BBOOL DecryptValue, CK_BBOOL WrapValue, CK_BBOOL UnwrapValue)
{
	bool bRtn = false;
	CK_RV rv=0;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_KEY_TYPE SM4keyType = CKK_SM4;
	CK_OBJECT_HANDLE hKey = NULL_PTR;
	CK_MECHANISM mechanismGen = {CKM_SM4_KEY_GEN, NULL_PTR, 0};
	CK_ATTRIBUTE KeyTemplate[] = 
	{
		{CKA_CLASS,			&keyClass,		sizeof(keyClass)},
		{CKA_TOKEN,			&TokenValue,	sizeof(TokenValue)},
		{CKA_KEY_TYPE,		&SM4keyType,	sizeof(SM4keyType)},
		{CKA_ENCRYPT,		&EncryptValue,	sizeof(EncryptValue)},
		{CKA_DECRYPT,		&DecryptValue,	sizeof(DecryptValue)},
		{CKA_WRAP,			&WrapValue,		sizeof(WrapValue)},
		{CKA_UNWRAP,		&UnwrapValue,	sizeof(UnwrapValue)},
		{CKA_SESSKEY_ID,	&nSessKeyID,	sizeof(nSessKeyID)}
	};
	int Run_Flag = 0;

	Run_Flag = RUN_CORRECT;

	if((TokenValue == FALSE)&&(nSessKeyID == CK_SESSKEY_PRESET_ID7))
	{
		Run_Flag = RUN_INCORRECT;
	}

	if((EncryptValue == TRUE)&&(UnwrapValue == TRUE))
	{
		Run_Flag = RUN_INCORRECT;
	}

	if((DecryptValue == TRUE)&&(WrapValue == TRUE))
	{
		Run_Flag = RUN_INCORRECT;
	}

	//if((UnwrapValue == TRUE)&&(WrapValue == TRUE))
	//{
	//	Run_Flag = RUN_INCORRECT;
	//}

	rv = pC_GenerateKey(hSession, &mechanismGen, KeyTemplate, sizeof(KeyTemplate)/sizeof(CK_ATTRIBUTE), &hKey);
	bRtn = Show_Result("pC_GenerateKey",Run_Flag, "GenerateKeyOnce", rv, Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	if(hKey != NULL_PTR)
	{
		rv = pC_DestroyObject(hSession, hKey);
		bRtn = Show_Result("pC_DestroyObject",Run_Flag, "GenerateKeyOnce", rv, Testresult[nItemNumb], &nResultLength);
		RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
	}

	return Free_Memory(FREE_0);
}

CK_RV test_CreateKey()
{
	CK_RV bRtn = FALSE;
	CK_RV rv=0;
	int i=0,j=0,n=0,m=0,p=0,q=0;
	CK_BBOOL TokenValue[8]	=	{ CK_TRUE, CK_FALSE};
	CK_BBOOL WrapValue[8]	=	{ CK_TRUE, CK_FALSE};
	CK_BBOOL EncryptValue[8] =	{ CK_TRUE, CK_FALSE};
	CK_BBOOL DecryptValue[8] =	{ CK_TRUE, CK_FALSE};
	CK_BBOOL UnwrapValue[8] =	{ CK_TRUE, CK_FALSE};

	bRtn = Enter_Function("test_CreateKey", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	q = 0;
	for(i=0;i<2;++i)
	{
		for(j=0;j<2;++j)
		{
			for(m=0;m<2;++m)
			{
				for(n=0;n<2;++n)
				{
					for(p=0;p<2;++p)
					{
						printf("%*c\r",79,0x20);
						printf("Token=%x, Encrypt=%x, Decrypt=%x, Wrap=%x, Unwrap=%x, Count=%x.\r",TokenValue[p], EncryptValue[n], DecryptValue[m], WrapValue[j], UnwrapValue[i],q);
						fflush(stdout);
						
						//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"TokenValue = %x,EncryptValue = %x,DecryptValue = %x,WrapValue = %x,UnwrapValue = %x,count =%x\r",TokenValue[p], EncryptValue[n], DecryptValue[m], WrapValue[j], UnwrapValue[i],q);
						bRtn = CreateKeyOnce(CK_SESSKEY_PRESET_ID0, TokenValue[p], EncryptValue[n], DecryptValue[m], WrapValue[j], UnwrapValue[i]);
						RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

						bRtn = GenerateKeyOnce(CK_SESSKEY_PRESET_ID0, TokenValue[p], EncryptValue[n], DecryptValue[m], WrapValue[j], UnwrapValue[i]);
						RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));						
						++q;
					}
				}
			}
		}
	}

#ifdef OPEN_LOGOUT_CLOSESSION
	bRtn = user_to_so();
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	q = 0;
	for(i=0;i<2;++i)
	{
		for(j=0;j<2;++j)
		{
			for(m=0;m<2;++m)
			{
				for(n=0;n<2;++n)
				{
					for(p=0;p<2;++p)
					{
						printf("TokenValue = %x,EncryptValue = %x,DecryptValue = %x,WrapValue = %x,UnwrapValue = %x,count =%x\r",TokenValue[p], EncryptValue[n], DecryptValue[m], WrapValue[j], UnwrapValue[i],q);
						//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "TokenValue = %x,EncryptValue = %x,DecryptValue = %x,WrapValue = %x,UnwrapValue = %x,count =%x\r",TokenValue[p], EncryptValue[n], DecryptValue[m], WrapValue[j], UnwrapValue[i],q);
						bRtn = CreateKeyOnce(CK_SESSKEY_PRESET_ID7, TokenValue[p], EncryptValue[n], DecryptValue[m], WrapValue[j], UnwrapValue[i]);
						RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

						bRtn = GenerateKeyOnce(CK_SESSKEY_PRESET_ID7, TokenValue[p], EncryptValue[n], DecryptValue[m], WrapValue[j], UnwrapValue[i]);
						RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));
						++q;
					}
				}
			}
		}
	}

	bRtn = so_to_user();
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

#endif
	bRtn = Leave_Function("test_CreateKey", Testresult[nItemNumb], &nResultLength);
	RV_FALSE_RETURN(Rv_False_Free_Memory(bRtn, FREE_0));

	return Free_Memory(FREE_0);
}