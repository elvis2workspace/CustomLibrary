#include <stdlib.h>
#include <string.h>
#include "general.h"
#include "Encrypt.h"
#define ASSERT(x) assert(x)


/*π˛œ£≤‚ ‘*/
bool test_digest()
{
	CK_RV rv=0;
	int i=0,j=0, t1=0, t2=0, iSumTime = 0;

	CK_BYTE srcData[128]={0};
	for(i=0;i<sizeof(srcData)/4;i++)
	{
		memcpy(&srcData[i*4],"abcd",4);
	}
	//const unsigned char pszCorrectResult_SM3[]={0xde, 0xbe, 0x9f, 0xf9, 0x22, 0x75, 0xb8, 0xa1, 0x38, 0x60, 0x48, 0x89, 0xc1, 0x8e, 0x5a, 0x4d, 0x6f, 0xdb, 0x70, 0xe5, 0x38, 0x7e, 0x57, 0x65, 0x29, 0x3d, 0xcb, 0xa3, 0x9c, 0x0c, 0x57, 0x32};//64
	const unsigned char pszCorrectResult_SM3[]={0x90, 0xd5, 0x2a, 0x2e, 0x85, 0x63, 0x1a, 0x8d, 0x60, 0x35, 0x26, 0x26, 0x26, 0x94, 0x1f, 0xa1, 0x1b, 0x85, 0xce, 0x57, 0x0c, 0xec, 0x1e, 0x3e, 0x99, 0x1e, 0x2d, 0xd7, 0xed, 0x25, 0x81, 0x48};//128
	//const unsigned char pszCorrectResult_SM3[]={0x59, 0x85, 0xbf, 0x39, 0x20, 0x6b, 0x50, 0xe6, 0x70, 0x1a, 0x2f, 0x04, 0x90, 0x57, 0x62, 0xb9, 0xd5, 0xc2, 0xb1, 0xfb, 0xdd, 0x62, 0x6b, 0x78, 0x96, 0x2a, 0xaa, 0x94, 0x4f, 0x7c, 0x1b, 0xc5};//512
	//const unsigned char pszCorrectResult_SM3[]={0x19, 0x35, 0x2a, 0x2a, 0xd9, 0x53, 0x80, 0x68, 0x94, 0x36, 0xf1, 0xc2, 0x64, 0x39, 0x6e, 0xe6, 0x82, 0x9a, 0x67, 0x1f, 0x37, 0xcb, 0x8a, 0xb0, 0x02, 0x45, 0x48, 0xe6, 0x16, 0xd6, 0xa1, 0x65};//1024
	//const unsigned char pszCorrectResult_SM3[]={0x3b, 0xe4, 0x6e, 0xac, 0xa6, 0x02, 0x74, 0x74, 0x00, 0x68, 0xe1, 0xff, 0xb8, 0x11, 0xf6, 0x67, 0x37, 0x7c, 0x23, 0x8f, 0x38, 0x24, 0x14, 0x7d, 0xd7, 0x85, 0x42, 0x12, 0x5a, 0x80, 0xd7, 0x54};//1504

	CK_BYTE digData[100];
	CK_ULONG ulDigLen=sizeof(digData);

	CK_MECHANISM md5mechanism={CKM_MD5,NULL_PTR,0};
	CK_MECHANISM sha1mechanism={CKM_SHA_1,NULL_PTR,0};
	CK_MECHANISM sm3mechanism={CKM_HASH_SM3,NULL_PTR,0};
	CK_MECHANISM hashcustommechanism={CKM_HASH_CUSTOM,NULL_PTR,0};
	CK_MECHANISM zucmechanism = {CKM_HASH_ZUC_CALC, NULL_PTR, 0};

	//for westone
	HASHPARM hashParam = {((16*8)<<8)+((16*8)>>8), \
	{0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38}, \
	{0xD6, 0xA7, 0x78, 0x8C, 0x41, 0xC3, 0xA3, 0x66, 0x18, 0xE8, 0x02, 0x1E, 0xE1, 0xEB, 0x27, 0x2F, 0xA7, 0x24, 0x7F, 0x77, 0xF4, 0x7F, 0x40, 0xBE, 0x6A, 0x31, 0x02, 0x48, 0x41, 0xA0, 0x14, 0xC6, \
	0x39, 0xE2, 0xF0, 0xA4, 0x69, 0x22, 0xD3, 0x56, 0xBD, 0xB0, 0x39, 0x12, 0x26, 0xA9, 0xD1, 0x47, 0x40, 0x84, 0x3D, 0xFE, 0x57, 0x68, 0x0D, 0x28, 0x87, 0x44, 0xB7, 0xD8, 0x20, 0xE0, 0x32, 0xBB}};
	CK_MECHANISM sm3mechanism_westone={CKM_HASH_SM3,&hashParam,sizeof(HASHPARM)};

	CK_MECHANISM  *mechanism[7]={&md5mechanism,&sha1mechanism,&sm3mechanism,&hashcustommechanism, &sm3mechanism_westone, &zucmechanism};
	const unsigned char pszCorrectResult_SM3_westone[]={0x16, 0x83, 0x4d, 0x1d, 0x74, 0x5c, 0xa1, 0x09, 0x4a, 0x62, 0x67, 0x53, 0xa1, 0x65, 0x27, 0x34, 0x7d, 0x98, 0xe4, 0x03, 0x3b, 0x07, 0xa9, 0x1b, 0x58, 0x0f, 0x85, 0xd6, 0xd7, 0xe8, 0xd1, 0x5c};

	printf("enter test_digest.\n");

	for(i=0;i<6;i++)
	{
		iSumTime = 0;
		for (j=0; j<1; j++)
		{
			rv=(*pC_DigestInit)(hSession,mechanism[i]);
			if(rv!=CKR_OK)
			{
				printf("test_digest failed,pC_DigestInit. rv=0x%08x. i=%d.\n",rv,i);
				return FALSE;
			}
		
			ulDigLen=sizeof(digData);

			//t1 = PlatformAPI::GetTickCount();
			rv=(*pC_Digest)(hSession,srcData,sizeof(srcData),digData,&ulDigLen);
			if(rv!=CKR_OK)
			{
				printf("test_digest failed,pC_Digest. rv=0x%08x. i=%d.\n",rv,i);
				return FALSE;
			}
			//t2 = PlatformAPI::GetTickCount();
			//iSumTime += t2 - t1;

			if(i != 2)
			{
				break;
			}
		}
		
		//printf("Digest Speed of Alg %d is: %.3f ms per %d bytes.\n", i, iSumTime/1024.0, (int)sizeof(srcData));
		if(i == 2)
		{
			//if(CheckResult(pszCorrectResult_SM3,digData,ulDigLen)==false)
			if (memcmp(pszCorrectResult_SM3, digData, ulDigLen))
			{
				printf("test_digest failed.SM3 Result is wrong!!! \n");
				return FALSE;
			}
		}

		if (i == 4)
		{
			//if(CheckResult(pszCorrectResult_SM3_westone,digData,ulDigLen)==false)
			if (memcmp(pszCorrectResult_SM3_westone, digData, ulDigLen))
			{
				printf("test_digest failed.SM3 Result is wrong!!! \n");
				return FALSE;
			}
		}
		
		rv=(*pC_DigestInit)(hSession,mechanism[i]);
		if(rv!=CKR_OK)
		{
			printf("test_digest failed,pC_DigestInit. rv=0x%08x. i=%d.\n",rv,i);
			return FALSE;
		}

		rv=(*pC_DigestUpdate)(hSession,srcData,sizeof(srcData)/2);
		if(rv!=CKR_OK)
		{
			printf("test_digest failed,pC_DigestUpdate. rv=0x%08x. i=%d.\n",rv,i);
			return FALSE;
		}

		rv=(*pC_DigestUpdate)(hSession,srcData+sizeof(srcData)/2,sizeof(srcData)-sizeof(srcData)/2);
		if(rv!=CKR_OK)
		{
			printf("test_digest failed,pC_DigestUpdate. rv=0x%08x. i=%d.\n",rv,i);
			return FALSE;
		}
		
		ulDigLen=sizeof(digData);
		rv=(*pC_DigestFinal)(hSession,digData,&ulDigLen);
		if(rv!=CKR_OK)
		{
			printf("test_digest failed,pC_DigestFinal. rv=0x%08x. i=%d.\n",rv,i);
			return FALSE;
		}

		if(i == 2)
		{
			//if(CheckResult(pszCorrectResult_SM3,digData,ulDigLen)==false)
			if (memcmp(pszCorrectResult_SM3, digData, ulDigLen))
			{
				printf("test_digest failed.SM3 Result is wrong!!! \n");
				return FALSE;
			}
		}

		if (i == 4)
		{
			//if(CheckResult(pszCorrectResult_SM3_westone,digData,ulDigLen)==false)
			if (memcmp(pszCorrectResult_SM3_westone, digData, ulDigLen))
			{
				printf("test_digest failed.SM3 Result is wrong!!! \n");
				return FALSE;
			}
		}
	}

	return TRUE;
}

/*π˛œ£key≤‚ ‘*/
bool test_digestkey()
{
	CK_RV rv=0;
	CK_BYTE digData[100];
	CK_ULONG ulDigLen=sizeof(digData);
	CK_BBOOL ttrue = CK_TRUE;
	int i=0,j=0;
	CK_OBJECT_HANDLE hKey=0;
	
	CK_MECHANISM md5mechanism={CKM_MD5,NULL_PTR,0};
	CK_MECHANISM sha1mechanism={CKM_SHA_1,NULL_PTR,0};
	CK_MECHANISM sm3mechanism={CKM_HASH_SM3,NULL_PTR,0};
	CK_MECHANISM hashcustommechanism={CKM_HASH_CUSTOM,NULL_PTR,0};
	
	CK_MECHANISM  *mechanism[7]={&md5mechanism,&sha1mechanism,&sm3mechanism,&hashcustommechanism};

	CK_MECHANISM DESmechanismkeygen = {
		CKM_DES_KEY_GEN, NULL_PTR, 0
	};
	CK_ATTRIBUTE DESkeytemplate[] = {
		{CKA_ENCRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_WRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)}
	};	

	CK_MECHANISM DES2mechanismkeygen = {
		CKM_DES2_KEY_GEN, NULL_PTR, 0
	};
	CK_ATTRIBUTE DES2keytemplate[] = {
		{CKA_ENCRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_WRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)}
	};

	CK_MECHANISM DES3mechanismkeygen = {
		CKM_DES3_KEY_GEN, NULL_PTR, 0
	};
	CK_ATTRIBUTE DES3keytemplate[] = {
		{CKA_ENCRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_WRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)}
	};	
	
	CK_MECHANISM SM1mechanismkeygen = {
		CKM_SM1_KEY_GEN, NULL_PTR, 0
	};
	CK_ATTRIBUTE SM1keytemplate[] = {
		{CKA_ENCRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_WRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)}
	};	
	
	CK_MECHANISM SSF33mechanismkeygen = {
		CKM_SSF33_KEY_GEN, NULL_PTR, 0
	};
	CK_ATTRIBUTE SSF33keytemplate[] = {
		{CKA_ENCRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_WRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)}
	};
	
	CK_MECHANISM  *mechanismkeygen[7]={&DESmechanismkeygen,&DES2mechanismkeygen,&DES3mechanismkeygen,&SM1mechanismkeygen,
							&SSF33mechanismkeygen};
	CK_ATTRIBUTE *ptemplate[7]={DESkeytemplate,DES2keytemplate,DES3keytemplate,SM1keytemplate,SSF33keytemplate};
	CK_ULONG      ulCount[7]={sizeof(DESkeytemplate)/sizeof(CK_ATTRIBUTE),sizeof(DES2keytemplate)/sizeof(CK_ATTRIBUTE),
							sizeof(DES3keytemplate)/sizeof(CK_ATTRIBUTE),sizeof(SM1keytemplate)/sizeof(CK_ATTRIBUTE),
							sizeof(SSF33keytemplate)/sizeof(CK_ATTRIBUTE)};

	printf("enter test_digestkey.\n");

	for(i=0;i<3;i++)
	{
		for(j=0;j<5;j++)
		{
			rv=(*pC_DigestInit)(hSession,mechanism[i]);
			if(rv!=CKR_OK)
			{
				printf("test_digestkey failed,pC_DigestInit. rv=0x%08x. i=%d,j=%d.\n",rv,i,j);
				return FALSE;
			}
			
			rv=(*pC_GenerateKey)(hSession,mechanismkeygen[j],ptemplate[j],ulCount[j],&hKey);
			if(rv!=CKR_OK)
			{
				printf("test_digestkey failed,pC_GenerateKey. rv=0x%08x. i=%d,j=%d.\n",rv,i,j);
				return FALSE;
			}

			rv=(*pC_DigestKey)(hSession,hKey);
			if(rv!=CKR_OK)
			{
				printf("test_digestkey failed,pC_DigestKey. rv=0x%08x. i=%d,j=%d.\n",rv,i,j);
				return FALSE;
			}

			ulDigLen=sizeof(digData);
			rv=(*pC_DigestFinal)(hSession,digData,&ulDigLen);
			if(rv!=CKR_OK)
			{
				printf("test_digestkey failed,pC_DigestFinal. rv=0x%08x. i=%d,j=%d.\n",rv,i,j);
				return FALSE;
			}
		}
	}
	
	return TRUE;
}




