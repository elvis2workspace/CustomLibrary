
#include "general.h"
#include "Encrypt.h"

#define ASSERT(x) assert(x)



/*key wrap≤‚ ‘*/
bool test_keymanager()
{
	CK_RV rv=0;	
	CK_OBJECT_HANDLE hKey=0,hKey2=0,hKey3=0;;
	CK_BBOOL ttrue = CK_TRUE;
	CK_OBJECT_CLASS keyClass = CKO_SECRET_KEY;
	CK_BYTE attVal[128]={0};
	CK_ULONG ulAttValLen=sizeof(attVal);
	CK_BYTE valGet[128];
	CK_ULONG ulValGetLen=sizeof(valGet);
	CK_BYTE data[1024]={0};
	CK_ULONG ulDataLen=sizeof(data);
	int i=0,j=0;

	CK_BBOOL bToken = CK_FALSE, bSensitive = CK_FALSE, bExtractable = CK_FALSE;
	CK_ATTRIBUTE attrGroup[3] = {
		{CKA_TOKEN, &bToken, sizeof(bToken)},
		{CKA_SENSITIVE, &bSensitive, sizeof(bSensitive)},
		{CKA_EXTRACTABLE, &bExtractable, sizeof(bExtractable)}};

	CK_MECHANISM DESmechanismkeygen = {
		CKM_DES_KEY_GEN, NULL_PTR, 0
	};
	CK_ATTRIBUTE DESkeytemplate[] = {
		{CKA_ENCRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_WRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_EXTRACTABLE, &ttrue, sizeof(ttrue)}
	};	
	CK_MECHANISM DESmechanism = {
		CKM_DES_ECB, NULL_PTR, 0
	};
	CK_KEY_TYPE DESkeyType = CKK_DES;
	CK_ATTRIBUTE DEStemplate2[] = {
		{CKA_CLASS, &keyClass, sizeof(CK_OBJECT_CLASS)},
		{CKA_KEY_TYPE, &DESkeyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)}
	};

	CK_MECHANISM DES2mechanismkeygen = {
		CKM_DES2_KEY_GEN, NULL_PTR, 0
	};
	CK_ATTRIBUTE DES2keytemplate[] = {
		{CKA_ENCRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_WRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_EXTRACTABLE, &ttrue, sizeof(ttrue)}
	};
	CK_MECHANISM DES2mechanism = {
		CKM_DES3_ECB, NULL_PTR, 0
	};
	CK_KEY_TYPE DES2keyType = CKK_DES2;
	CK_ATTRIBUTE DES2template2[] = {
		{CKA_CLASS, &keyClass, sizeof(CK_OBJECT_CLASS)},
		{CKA_KEY_TYPE, &DES2keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)}
	};

	CK_MECHANISM DES3mechanismkeygen = {
		CKM_DES3_KEY_GEN, NULL_PTR, 0
	};
	CK_ATTRIBUTE DES3keytemplate[] = {
		{CKA_ENCRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_WRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_EXTRACTABLE, &ttrue, sizeof(ttrue)}
	};	
	CK_MECHANISM DES3mechanism = {
		CKM_DES3_ECB, NULL_PTR, 0
	};
	CK_KEY_TYPE DES3keyType = CKK_DES3;
	CK_ATTRIBUTE DES3template2[] = {
		{CKA_CLASS, &keyClass, sizeof(CK_OBJECT_CLASS)},
		{CKA_KEY_TYPE, &DES3keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)}
	};

	CK_MECHANISM SM1mechanismkeygen = {
		CKM_SM1_KEY_GEN, NULL_PTR, 0
	};
	CK_ATTRIBUTE SM1keytemplate[] = {
		{CKA_ENCRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_WRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_EXTRACTABLE, &ttrue, sizeof(ttrue)}
	};	
	CK_MECHANISM SM1mechanism = {
		CKM_SM1_ECB, NULL_PTR, 0
	};
	CK_KEY_TYPE SM1keyType = CKK_SM1;
	CK_ATTRIBUTE SM1template2[] = {
		{CKA_CLASS, &keyClass, sizeof(CK_OBJECT_CLASS)},
		{CKA_KEY_TYPE, &SM1keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)}
	};

	CK_MECHANISM SSF33mechanismkeygen = {
		CKM_SSF33_KEY_GEN, NULL_PTR, 0
	};
	CK_ATTRIBUTE SSF33keytemplate[] = {
		{CKA_ENCRYPT, &ttrue, sizeof(CK_BBOOL)},
		{CKA_WRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_UNWRAP, &ttrue, sizeof(CK_BBOOL)},
		{CKA_EXTRACTABLE, &ttrue, sizeof(ttrue)}
	};
	CK_MECHANISM SSF33mechanism = {
		CKM_SSF33_ECB, NULL_PTR, 0
	};
	CK_KEY_TYPE SSF33keyType = CKK_SSF33;
	CK_ATTRIBUTE SSF33template2[] = {
		{CKA_CLASS, &keyClass, sizeof(CK_OBJECT_CLASS)},
		{CKA_KEY_TYPE, &SSF33keyType, sizeof(CK_KEY_TYPE)},
		{CKA_ENCRYPT, &ttrue, sizeof(ttrue)},
		{CKA_DECRYPT, &ttrue, sizeof(ttrue)}
	};

	CK_MECHANISM  *mechanismkeygen[7]={&DESmechanismkeygen,&DES2mechanismkeygen,&DES3mechanismkeygen,&SM1mechanismkeygen,
							&SSF33mechanismkeygen};
	CK_ATTRIBUTE *ptemplate[7]={DESkeytemplate,DES2keytemplate,DES3keytemplate,SM1keytemplate,SSF33keytemplate};
	CK_ULONG      ulCount[7]={sizeof(DESkeytemplate)/sizeof(CK_ATTRIBUTE),sizeof(DES2keytemplate)/sizeof(CK_ATTRIBUTE),
							sizeof(DES3keytemplate)/sizeof(CK_ATTRIBUTE),sizeof(SM1keytemplate)/sizeof(CK_ATTRIBUTE),
							sizeof(SSF33keytemplate)/sizeof(CK_ATTRIBUTE)};
	CK_MECHANISM  *mechanism[7]={&DESmechanism,&DES2mechanism,&DES3mechanism,&SM1mechanism,&SSF33mechanism};
	CK_ATTRIBUTE *ptemplate2[7]={DEStemplate2,DES2template2,DES3template2,SM1template2,SSF33template2};
	CK_ULONG      ulCount2[7]={sizeof(DEStemplate2)/sizeof(CK_ATTRIBUTE),sizeof(DES2template2)/sizeof(CK_ATTRIBUTE),
							sizeof(DES3template2)/sizeof(CK_ATTRIBUTE),sizeof(SM1template2)/sizeof(CK_ATTRIBUTE),
							sizeof(SSF33template2)/sizeof(CK_ATTRIBUTE)};

	printf("enter test_keymanager.\n");

	for(i=0;i<4;i++)
	{
		rv=(*pC_GenerateKey)(hSession,mechanismkeygen[i],ptemplate[i],ulCount[i],&hKey);
		if(rv!=CKR_OK)
		{
			printf("test_keymanager failed,pC_GenerateKey. rv=0x%08x. i=%d,j=%d.\n",rv,i,j);
			return FALSE;
		}

		for(j=0;j<5;j++)
		{
			if((i>2)&&((j==0)||(j==2)))
			{
				continue;
			}

			rv=(*pC_GenerateKey)(hSession,mechanismkeygen[j],ptemplate[j],ulCount[j],&hKey2);
			if(rv!=CKR_OK)
			{
				printf("test_keymanager failed,pC_GenerateKey. rv=0x%08x. i=%d,j=%d.\n",rv,i,j);
				return FALSE;
			}

			rv = (*pC_GetAttributeValue)(hSession, hKey2, attrGroup, sizeof(attrGroup)/sizeof(CK_ATTRIBUTE));
			if (rv!=CKR_OK)
			{
				printf("test_keymanager failed,pC_GetAttributeValue. rv=0x%08x. i=%d,j=%d.\n",rv,i,j);
				return FALSE;
			}
			
			ulDataLen=sizeof(data);
			rv=(*pC_WrapKey)(hSession,mechanism[i],hKey,hKey2,data,&ulDataLen);
			if(rv!=CKR_OK)
			{
				printf("test_keymanager failed,pC_WrapKey. rv=0x%08x. i=%d,j=%d.\n",rv,i,j);
				return FALSE;
			}

			rv=(*pC_UnwrapKey)(hSession,mechanism[i],hKey,data,ulDataLen,ptemplate2[j],ulCount2[j],&hKey3);
			if(rv!=CKR_OK)
			{
				printf("test_keymanager failed,pC_UnwrapKey. rv=0x%08x. i=%d,j=%d.\n",rv,i,j);
				return FALSE;
			}
		}	
	}


	return TRUE;
}

