
#include "general.h"
#include "pin.h"

/*pin测试*/
CK_RV test_pin()
{
	CK_RV rv=0;

	//CK_UTF8CHAR_PTR pOldPin=(CK_UTF8CHAR_PTR)"12345678";
	//CK_ULONG ulOldLen=8;
	//CK_UTF8CHAR_PTR pNewPin=(CK_UTF8CHAR_PTR)"12345678";
	//CK_ULONG ulNewLen=8;
	
	printf("\nenter test_pin.\n");
	
	printf(" test user pin.\n");

	rv=(*pC_Login)(hSession,CKU_USER,userpin,strlen((char*)userpin));
	if(rv!=CKR_OK)
	{
		printf("test_pin failed,pC_Login. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv=(*pC_SetPIN)(hSession,userpin,strlen((char*)userpin),(CK_UTF8CHAR_PTR)/*"123456"*/"abcdefabcdefabcdefabcdefabcdef",30);
	if(rv!=CKR_OK)
	{
		printf("test_pin failed,pC_SetPIN. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv=(*pC_Logout)(hSession);
	if(rv!=CKR_OK)
	{
		printf("test_pin failed,pC_Logout. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv=(*pC_Login)(hSession,CKU_USER,(CK_UTF8CHAR_PTR)/*"123456"*/"abcdefabcdefabcdefabcdefabcdef",30);
	if(rv!=CKR_OK)
	{
		printf("test_pin failed,pC_Login. rv=0x%08x.\n",rv);
		return FALSE;
	}

	rv=(*pC_Logout)(hSession);
	if(rv!=CKR_OK)
	{
		printf("test_pin failed,pC_Logout. rv=0x%08x.\n",rv);
		return FALSE;
	}
	
	printf(" test admin pin: %s, len=%d.\n", adminpin, (int)strlen((char*)adminpin));
	rv=(*pC_Login)(hSession,CKU_SO,adminpin,strlen((char*)adminpin));
	if(rv!=CKR_OK)
	{
		printf("test_pin failed,pC_Login. rv=0x%08x.\n",rv);
		return FALSE;
	}
	
	printf("userpin=%s, len=%d\n", userpin, (int)strlen((char*)userpin));
	rv=(*pC_InitPIN)(hSession,userpin,strlen((char*)userpin));
	if(rv!=CKR_OK)
	{
		printf("test_pin failed,pC_InitPIN. rv=0x%08x.\n",rv);
		return FALSE;
	}
	
	rv=(*pC_SetPIN)(hSession,adminpin,strlen((char*)adminpin), adminpin, (int)strlen((char*)adminpin));
	if(rv!=CKR_OK)
	{
		printf("test_pin failed,pC_SetPIN. rv=0x%08x.\n",rv);
		return FALSE;
	}
	
	rv=(*pC_Logout)(hSession);
	if(rv!=CKR_OK)
	{
		printf("test_pin failed,pC_Logout. rv=0x%08x.\n",rv);
		return FALSE;
	}

	//测试错误的管理员PIN
	printf(" test wrong-admin pin.");
	char l_c=adminpin[0];
	adminpin[0]='9';
	printf("AdminPin is %s.\n",(char*)adminpin);

	rv=(*pC_Login)(hSession,CKU_SO,adminpin,strlen((char*)adminpin));
	if(rv==CKR_OK)
	{
		printf("test wrong-admin pin failed.\n");
		return FALSE;
	}
	adminpin[0]=l_c;

	return TRUE;
}