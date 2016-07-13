
#include <stdlib.h>
#include <string.h>
#include "general.h"
#include "Encrypt.h"
#define ASSERT(x) assert(x)
#include "GenKeyPair.h"



/*产生ECC密钥对*/
bool test_ECC_GenKeyPair(CK_OBJECT_HANDLE *phPublicKey,CK_OBJECT_HANDLE *phPrivateKey)
{
	CK_RV rv=0;

	CK_MECHANISM      mechanism = {CKM_ECC_KEY_PAIR_GEN, NULL_PTR, 0};
	/*CK_ULONG*/CK_UINT     modulusBits = 256;
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

	printf("enter test_ECC_GenKeyPair.\n");

	//产生密钥对
	rv = (*pC_GenerateKeyPair)(hSession, &mechanism,
						  publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  phPublicKey, phPrivateKey);
	if(rv!=CKR_OK)
	{
		printf("test_ECC_GenKeyPair failed,pC_GenerateKeyPair. rv=0x%08x.\n",rv);
		return FALSE;
	}

	return TRUE;
}


/*导入ECC密钥对*/
//bool test_ECC_importKeyPair(CK_OBJECT_HANDLE *phPublicKey,CK_OBJECT_HANDLE *phPrivateKey)
//{
//	CK_RV rv=0;
//
//	CK_BYTE     temp_pub_x[32]={
//		0xec,0x91,0x81,0x8d,0xe0,0xb7,0x01,0x21,0x73,0xf5,0x1c,0x33,0x75,0x43,0x6e,0x43,
//			0xb6,0xa9,0xa2,0x6a,0xbd,0x6d,0xbc,0xb7,0x9f,0x85,0x1c,0xde,0xaf,0x7a,0x0f,0x6c
//	};
//	CK_BYTE     temp_pub_y[32]={
//		0xcb,0xf4,0xb5,0xa1,0x5f,0xb8,0x7e,0x60,0xfc,0x0b,0x3a,0x92,0x3d,0x12,0xe8,0x66,
//			0x36,0x4a,0x93,0x5f,0xfb,0x30,0x84,0x2b,0xc9,0x13,0x9e,0xbd,0x2d,0xdc,0xe9,0x61
//	};
//	CK_BYTE     temp_prv[32]={
//		0xc5,0x6a,0x2b,0x58,0xa0,0x94,0xef,0x24,0x41,0x03,0x79,0x45,0xba,0xb1,0x39,0x8c,
//			0xc0,0xdf,0x9f,0xc4,0xf9,0x9e,0x9a,0x60,0x2c,0xd8,0x6f,0xc2,0xc3,0x88,0xad,0x0c
//	};
//
////	CK_BYTE     temp_prv[32] = {0x65, 0x83, 0x2D, 0x7C, 0x7B, 0x27, 0x2E, 0x98, 0x57, 0xD1, 0x08, 0x4F, 0x3A, 0x75, 0x54, 0x0D, 0x40, 0xC2, 0x04, 0x23, 0x3A, 0x6A, 0x65, 0x6A, 0x15, 0x9D, 0x59, 0x52, 0x2D, 0x68, 0x4E, 0x3E};
////	CK_BYTE     temp_pub_x[32] = {0xD6, 0xA7, 0x78, 0x8C, 0x41, 0xC3, 0xA3, 0x66, 0x18, 0xE8, 0x02, 0x1E, 0xE1, 0xEB, 0x27, 0x2F, 0xA7, 0x24, 0x7F, 0x77, 0xF4, 0x7F, 0x40, 0xBE, 0x6A, 0x31, 0x02, 0x48, 0x41, 0xA0, 0x14, 0xC6};
////	CK_BYTE     temp_pub_y[32] = {0x39, 0xE2, 0xF0, 0xA4, 0x69, 0x22, 0xD3, 0x56, 0xBD, 0xB0, 0x39, 0x12, 0x26, 0xA9, 0xD1, 0x47, 0x40, 0x84, 0x3D, 0xFE, 0x57, 0x68, 0x0D, 0x28, 0x87, 0x44, 0xB7, 0xD8, 0x20, 0xE0, 0x32, 0xBB};
//
//	
//	CK_BBOOL     ttrue = CK_TRUE;
//	CK_KEY_TYPE  keyType=CKK_ECC;
//	CK_OBJECT_CLASS pubclass=CKO_PUBLIC_KEY,priclass=CKO_PRIVATE_KEY;
//	CK_UINT     modulusBits = 256;
//
//	CK_BYTE      id[] = {0x01,0x01,0x02,0x03};
//
//	CK_ATTRIBUTE publicKeyTemplate[] = {		
//		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
//		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
//		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
//		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
//		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
//		{CKA_WRAP, &ttrue, sizeof(true)},
//		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
//		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
//		{CKA_ECC_X_COORDINATE, temp_pub_x, sizeof(temp_pub_x)},
//		{CKA_ECC_Y_COORDINATE, temp_pub_y, sizeof(temp_pub_y)},
//		{CKA_ID, id, sizeof(id)}
//	};
//
//	CK_ATTRIBUTE privateKeyTemplate[] = {
//		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
//		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
//		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
//		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
//		{CKA_SIGN, &ttrue, sizeof(ttrue)},
//		{CKA_UNWRAP, &ttrue, sizeof(true)},
//		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
//		{CKA_ECC_BITS_LEN, &modulusBits, sizeof(CK_UINT)},
//		{CKA_ECC_PRIVATE, temp_prv, sizeof(temp_prv)},
//		{CKA_ID, id, sizeof(id)}
//	};
//
//	CK_ATTRIBUTE pubFindKeyTemplate[] = {
//		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
//		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
//		{CKA_ID, id, sizeof(id)}
//	};
//
//	CK_ATTRIBUTE prvFindKeyTemplate[] = {
//		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
//		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
//		{CKA_ID, id, sizeof(id)}
//	};
//
//	CK_BYTE get_pub_x[32] = {0};
//	CK_BYTE get_pub_y[32] = {0};
//	CK_ATTRIBUTE pubGetKeyTemplate[] = {
//		{CKA_ECC_X_COORDINATE, get_pub_x, sizeof(get_pub_x)},
//		{CKA_ECC_Y_COORDINATE, get_pub_y, sizeof(get_pub_y)}
//	};
//
//	CK_BYTE get_prv[32] = {0};
//	CK_ATTRIBUTE prvGetKeyTemplate[] = {
//		{CKA_ECC_PRIVATE, get_prv, sizeof(get_prv)}
//	};
//
//	CK_OBJECT_HANDLE hObject = NULL_PTR;
//	CK_ULONG ulObjectCount = 0;
//
//	unsigned int t1 = 0, t2 = 0;
//
//	printf("enter test_ECC_importKeyPair.\n");
//
//	//创建公钥对象
//	//t1 = PlatformAPI::GetTickCount();
//	rv = (*pC_CreateObject)(hSession, 
//						  publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
//						  phPublicKey);
//	if(rv!=CKR_OK)
//	{
//		printf("test_ECC_importKeyPair failed,pC_CreateObject,0. rv=0x%08x.\n",rv);
//		return FALSE;
//	}
//	//t2 = PlatformAPI::GetTickCount();
//	printf("Import Public Key:\t%dms.\n", t2-t1);
//	
//
//	//创建私钥对象
//	rv = (*pC_CreateObject)(hSession, 
//						  privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
//						  phPrivateKey);
//	if(rv!=CKR_OK)
//	{
//		printf("test_ECC_importKeyPair failed,pC_CreateObject,1. rv=0x%08x.\n",rv);
//		return FALSE;
//	}
//
//	//查找私钥对象	
//	hObject = NULL_PTR;
//	ulObjectCount = 0;
//	rv = (*pC_FindObjectsInit)(hSession, prvFindKeyTemplate, sizeof(prvFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
//	if(rv!=CKR_OK)
//	{
//		printf("test_ECC_importKeyPair failed,pC_FindObjectsInit,1. rv=0x%08x.\n",rv);
//		return FALSE;
//	}
//
//	rv = (*pC_FindObjects)(hSession, &hObject, 1, &ulObjectCount);
//	if(rv!=CKR_OK)
//	{
//		printf("test_ECC_importKeyPair failed,pC_FindObjects,1. rv=0x%08x.\n",rv);
//		return FALSE;
//	}
//
//	rv = (*pC_FindObjectsFinal)(hSession);
//	if(rv!=CKR_OK)
//	{
//		printf("test_ECC_importKeyPair failed,pC_FindObjectsFinal,1. rv=0x%08x.\n",rv);
//		return FALSE;
//	}
//
//	//查找公钥对象，并读取公钥对象属性
//	hObject = NULL_PTR;
//	ulObjectCount = 0;
//	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate, sizeof(pubFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
//	if(rv!=CKR_OK)
//	{
//		printf("test_ECC_importKeyPair failed,pC_FindObjectsInit,2. rv=0x%08x.\n",rv);
//		return FALSE;
//	}
//
//	rv = (*pC_FindObjects)(hSession, &hObject, 1, &ulObjectCount);
//	if(rv!=CKR_OK)
//	{
//		printf("test_ECC_importKeyPair failed,pC_FindObjects,2. rv=0x%08x.\n",rv);
//		return FALSE;
//	}
//
//	rv = (*pC_FindObjectsFinal)(hSession);
//	if(rv!=CKR_OK)
//	{
//		printf("test_ECC_importKeyPair failed,pC_FindObjectsFinal,2. rv=0x%08x.\n",rv);
//		return FALSE;
//	}
//
//	rv = (*pC_GetAttributeValue)(hSession, hObject, pubGetKeyTemplate, sizeof(pubGetKeyTemplate)/sizeof(CK_ATTRIBUTE));
//	if(rv!=CKR_OK)
//	{
//		printf("test_ECC_importKeyPair failed,pC_GetAttributeValue,2. rv=0x%08x.\n",rv);
//		return FALSE;
//	}
//	
//	return TRUE;
//}
/*导入ECC密钥对*/
bool test_ECC_importKeyPair(CK_OBJECT_HANDLE *phPublicKey,CK_OBJECT_HANDLE *phPrivateKey)
{
	CK_RV rv=0;

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
	CK_BBOOL     ffalse = CK_FALSE;
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
		{CKA_WRAP, &ttrue, sizeof(ttrue)},
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
		{CKA_UNWRAP, &ttrue, sizeof(ttrue)},
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


	//printf("enter test_ECC_importKeyPair.\n");
	//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","enter test_ECC_importKeyPair.<br>");
	//printf("enter test_ECC_importKeyPairxxxx.\n");

	//创建公钥对象
	rv = (*pC_CreateObject)(hSession, 
						  publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  phPublicKey);
	if(rv!=CKR_OK)
	{
		printf("test_ECC_importKeyPair failed,pC_CreateObject,0. rv=0x%08x.\n",rv);
		return FALSE;
	}
	//创建私钥对象
	rv = (*pC_CreateObject)(hSession, 
						  privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  phPrivateKey);
	if(rv!=CKR_OK)
	{
		printf("test_ECC_importKeyPair failed,pC_CreateObject,1. rv=0x%08x.\n",rv);
		return FALSE;
	}

	//查找私钥对象	
	hObject = NULL_PTR;
	ulObjectCount = 0;
	rv = (*pC_FindObjectsInit)(hSession, prvFindKeyTemplate, sizeof(prvFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	if(rv!=CKR_OK)
	{
		printf("test_ECC_importKeyPair failed,pC_FindObjectsInit,1. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv = (*pC_FindObjects)(hSession, &hObject, 1, &ulObjectCount);
	if(rv!=CKR_OK)
	{
		printf("test_ECC_importKeyPair failed,pC_FindObjects,1. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv = (*pC_FindObjectsFinal)(hSession);
	if(rv!=CKR_OK)
	{
		printf("test_ECC_importKeyPair failed,pC_FindObjectsFinal,1. rv=0x%08x.\n",rv);
		return FALSE;
	}

	//查找公钥对象，并读取公钥对象属性
	hObject = NULL_PTR;
	ulObjectCount = 0;
	rv = (*pC_FindObjectsInit)(hSession, pubFindKeyTemplate, sizeof(pubFindKeyTemplate)/sizeof(CK_ATTRIBUTE));
	if(rv!=CKR_OK)
	{
		printf("test_ECC_importKeyPair failed,pC_FindObjectsInit,2. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv = (*pC_FindObjects)(hSession, &hObject, 1, &ulObjectCount);
	if(rv!=CKR_OK)
	{
		printf("test_ECC_importKeyPair failed,pC_FindObjects,2. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv = (*pC_FindObjectsFinal)(hSession);
	if(rv!=CKR_OK)
	{
		printf("test_ECC_importKeyPair failed,pC_FindObjectsFinal,2. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv = (*pC_GetAttributeValue)(hSession, hObject, pubGetKeyTemplate, sizeof(pubGetKeyTemplate)/sizeof(CK_ATTRIBUTE));
	if(rv!=CKR_OK)
	{
		printf("test_ECC_importKeyPair failed,pC_GetAttributeValue,2. rv=0x%08x.\n",rv);
		return FALSE;
	}
	//printf("enter test_ECC_importKeyPair.\n");
	//nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "%s","enter test_ECC_importKeyPair.<br>");
	
	return TRUE;
}
/*产生RSA密钥对*/
bool test_RSA_GenKeyPair(CK_OBJECT_HANDLE *phPublicKey,CK_OBJECT_HANDLE *phPrivateKey)
{
	CK_RV rv=0;

	CK_MECHANISM      mechanism = {CKM_RSA_PKCS_KEY_PAIR_GEN, NULL_PTR, 0};
	CK_UINT     modulusBits = 1024;
	CK_BYTE      subject[] = {0x00,0x01,0x02};
	CK_BYTE      id[] = {0x00,0x01,0x02,0x03};
	CK_BBOOL     ttrue = CK_TRUE;
	CK_KEY_TYPE  keyType=CKK_RSA;
	CK_ATTRIBUTE publicKeyTemplate[] = {
		{CKA_MODULUS_BITS, &modulusBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_SUBJECT, subject, sizeof(subject)},
		{CKA_ID, id, sizeof(id)},
		{CKA_WRAP, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)}
	};
	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_MODULUS_BITS, &modulusBits, sizeof(CK_UINT)},
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_SUBJECT, subject, sizeof(subject)},
		{CKA_ID, id, sizeof(id)},
		{CKA_SENSITIVE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_UNWRAP, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)}
	};

	printf("enter test_RSA_GenKeyPair.\n");

	//产生密钥对
	rv = (*pC_GenerateKeyPair)(hSession, &mechanism,
						  publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  phPublicKey, phPrivateKey);
	if(rv!=CKR_OK)
	{
		printf("test_RSA_GenKeyPair failed,pC_GenerateKeyPair. rv=0x%08x.\n",rv);
		return FALSE;
	}

	return TRUE;
}

/*导入RSA密钥对*/
bool test_RSA_importKeyPair(CK_OBJECT_HANDLE *phPublicKey,CK_OBJECT_HANDLE *phPrivateKey)
{
	CK_RV rv=0;

	CK_BYTE     Modulus[128]={
		0xa2,0xfd,0x11,0x93,0xf5,0x72,0x1c,0x8c,0xa8,0xe8,0x59,0xf2,0x2b,0xfa,0x51,0x16,
		0x5b,0x91,0xd9,0x75,0xbb,0x2b,0x24,0x4d,0x77,0xa3,0xc8,0xee,0x18,0x76,0x41,0x0b, 
		0x77,0x32,0x10,0x6a,0xd7,0x52,0x0c,0x31,0x4a,0x5a,0x50,0xe7,0xd6,0xaf,0x86,0xf3,  
		0xf3,0x18,0x5a,0xdc,0xaf,0x06,0xad,0x3c,0x72,0x23,0x7e,0x10,0xd1,0x26,0x9e,0x27,  
		0x24,0xbe,0xe4,0x8c,0x1b,0x2a,0x31,0xfb,0x68,0x43,0x68,0x71,0x65,0xdf,0xa9,0x0e,  
		0x9c,0x30,0x15,0x78,0x7b,0x93,0x60,0x43,0xc9,0xbd,0xcc,0x14,0xc4,0xbe,0x19,0xa5,  
		0x50,0xf6,0x49,0xb7,0x73,0x3d,0xb8,0x45,0x75,0x39,0x08,0x2e,0x0c,0x7a,0x48,0x4d,  
		0x58,0x76,0x83,0x97,0xba,0x86,0xa4,0x48,0xe1,0xcd,0xd0,0x1d,0xa1,0x56,0x4a,0x35 
	};
	CK_BYTE     PublicExponent[4]={0x00,0x01,0x00,0x01};
	CK_BYTE     PrivateExponent[128]={
		0x23,0x25,0xfa,0x05,0x73,0x57,0xe1,0x6a,0x3f,0xfe,0xa3,0x6f,0x03,0x26,0xdf,0x17,  
		0xb8,0x35,0x92,0xb4,0xbd,0xe1,0x46,0xd9,0xe1,0xe4,0x13,0x2f,0xad,0x5c,0xcb,0x18,  
		0x22,0xe3,0x4c,0x01,0x0e,0x60,0x26,0x3a,0xf6,0xf2,0xd5,0x21,0xb9,0xfd,0x6e,0x57,  
		0xeb,0x63,0xa4,0x2c,0x95,0x1e,0x16,0x7b,0xc3,0x8d,0x7d,0xe1,0x38,0x89,0x60,0x6f,  
		0x6e,0x6d,0xe7,0x01,0x69,0x01,0x96,0x31,0x3c,0x7d,0xa9,0x09,0x4a,0xdd,0xdf,0xbc,  
		0x2c,0x24,0xc4,0x1d,0xc4,0x6a,0x14,0x80,0x01,0x75,0x4f,0x6c,0x52,0x1a,0xc5,0xfd,  
		0xb0,0x40,0x71,0x65,0xc2,0xbb,0x0e,0x8a,0x43,0x61,0x9a,0x9b,0x99,0x74,0x75,0xca,  
		0xb5,0x0f,0xac,0x83,0x5d,0xfc,0xee,0xf4,0xa6,0xda,0xf7,0x3c,0x51,0xe3,0xd5,0xfd  
	};
	CK_BYTE     Prime1[64]={
		0xd1,0xe4,0x30,0x53,0x0e,0xaa,0xcd,0xcf,0x7e,0x68,0x74,0x1b,0x0c,0x35,0x18,0xe2,  
		0x03,0xbb,0x05,0xdd,0x31,0xbc,0x76,0x49,0x6e,0xf8,0x3f,0x02,0x1e,0x1e,0x61,0x0e,  
		0x4a,0x4f,0x64,0x3b,0x1c,0xb0,0x73,0xcf,0x0c,0xe0,0x8b,0xfe,0x54,0xfe,0xa3,0x2b,  
		0xfa,0xc7,0x74,0x4b,0xd0,0x1f,0x7d,0x20,0x79,0xda,0xff,0x2c,0x1c,0x47,0x4e,0xd7  
	};
	CK_BYTE     Prime2[64]={
		0xc6,0xcb,0x2c,0x38,0x52,0xf5,0xb0,0x16,0xc7,0xd6,0xfa,0x5e,0xb8,0x73,0xaf,0xa3,  
		0x01,0xae,0x34,0x6c,0x05,0x35,0xaf,0xa1,0xe3,0x6a,0x4b,0xf6,0x05,0x92,0xe9,0xb9,  
		0x4f,0x89,0xf0,0x67,0x8f,0x36,0xee,0xd5,0x65,0x9c,0xe6,0x87,0x21,0xca,0x73,0xf7,  
		0x86,0xaf,0xaa,0x4b,0x4f,0x0e,0x12,0x35,0x37,0x3f,0x74,0x0d,0x31,0xf3,0x49,0xd3  
	};
	CK_BYTE     Prime1Exponent[64]={
		0x7f,0x9d,0x95,0x80,0x3a,0x8f,0xc1,0x79,0x22,0x6b,0x92,0x04,0x8c,0x04,0x90,0xaf,  
		0x83,0x03,0x28,0x8a,0x97,0xdd,0xf5,0xb1,0xaa,0x66,0x07,0xb1,0x5a,0xec,0xaf,0xa8,  
		0x47,0xa8,0x43,0xd1,0xe9,0x1c,0xa4,0x1d,0x44,0xc0,0x16,0x7e,0x39,0xe1,0x65,0x06,  
		0xad,0x80,0xae,0x06,0x16,0x71,0xbe,0xdb,0x8b,0x1b,0x8b,0x40,0x9f,0xa4,0xff,0x9b  
	};
	CK_BYTE     Prime2Exponent[64]={
		0xb3,0xff,0xd1,0x99,0x6a,0x5c,0xb6,0x45,0xda,0x4b,0x28,0xc6,0xcd,0x83,0x4a,0x9c,  
		0x71,0x70,0x18,0x5d,0x5a,0xd6,0x42,0xa2,0x76,0x35,0x90,0x74,0x42,0x95,0xc4,0xf5,  
		0x9b,0x34,0x8e,0x2e,0x66,0xf9,0xd9,0x85,0xf6,0x0c,0xfc,0x9b,0x8d,0x18,0x30,0xab,  
		0x07,0x14,0x56,0x40,0xfc,0x63,0x04,0x4f,0x95,0x27,0xea,0x5f,0xec,0x43,0x9b,0x2b  
	};
	CK_BYTE     Coefficient[64]={
		0x13,0xa5,0x71,0x23,0xb3,0x9e,0x90,0x42,0x3a,0xd5,0x45,0x68,0x70,0x9d,0x79,0x0e,  
		0xed,0x07,0x3a,0xe8,0x4d,0xab,0xe9,0x8b,0x3e,0x03,0xcc,0x73,0x5d,0x48,0x6b,0xcb,  
		0xf5,0x95,0x95,0x37,0x7c,0x37,0x0d,0x1f,0x64,0xe3,0x4c,0x49,0xd1,0x55,0x4d,0x13,  
		0x2b,0x94,0x50,0x71,0xd5,0xaf,0xfe,0x34,0xc5,0x4e,0xfa,0xcf,0x78,0x96,0x7b,0x70 
	};

	
	CK_BBOOL     ttrue = CK_TRUE;
	CK_KEY_TYPE  keyType=CKK_RSA;
	CK_OBJECT_CLASS pubclass=CKO_PUBLIC_KEY,priclass=CKO_PRIVATE_KEY;
	CK_UINT     modulusBits = 1024;

	CK_ATTRIBUTE publicKeyTemplate[] = {		
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &pubclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_VERIFY, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_MODULUS_BITS, &modulusBits, sizeof(CK_UINT)},
		{CKA_MODULUS, Modulus, sizeof(Modulus)},
		{CKA_PUBLIC_EXPONENT, PublicExponent, sizeof(PublicExponent)}
	};

	CK_ATTRIBUTE privateKeyTemplate[] = {
		{CKA_TOKEN, &ttrue, sizeof(ttrue)},
		{CKA_CLASS, &priclass, sizeof(CK_OBJECT_CLASS)},
		{CKA_PRIVATE, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)},
		{CKA_SIGN, &ttrue, sizeof(ttrue)},
		{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
		{CKA_MODULUS_BITS, &modulusBits, sizeof(CK_UINT)},
		{CKA_PRIVATE_EXPONENT, PrivateExponent, sizeof(PrivateExponent)},
		{CKA_PRIME_1, Prime1, sizeof(Prime1)},
		{CKA_PRIME_2, Prime2, sizeof(Prime2)},
		{CKA_EXPONENT_1, Prime1Exponent, sizeof(Prime1Exponent)},
		{CKA_EXPONENT_2, Prime2Exponent, sizeof(Prime2Exponent)},
		{CKA_COEFFICIENT, Coefficient, sizeof(Coefficient)},
	};

	printf("enter test_RSA_importKeyPair.\n");

	//创建公钥对象
	rv = (*pC_CreateObject)(hSession, 
						  publicKeyTemplate, sizeof(publicKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  phPublicKey);
	if(rv!=CKR_OK)
	{
		printf("test_RSA_importKeyPair failed,pC_CreateObject,0. rv=0x%08x.\n",rv);
		return FALSE;
	}
	

	//创建私钥对象
	rv = (*pC_CreateObject)(hSession, 
						  privateKeyTemplate, sizeof(privateKeyTemplate)/sizeof(CK_ATTRIBUTE),
						  phPrivateKey);
	if(rv!=CKR_OK)
	{
		printf("test_RSA_importKeyPair failed,pC_CreateObject,1. rv=0x%08x.\n",rv);
		return FALSE;
	}
	
	return TRUE;
}