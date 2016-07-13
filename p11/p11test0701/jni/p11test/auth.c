
#include "general.h"

/*认证*/
CK_RV test_login(CK_USER_TYPE userType)
{
	CK_RV rv=0;
	int nResultLength =0;
	//CK_UTF8CHAR pin[]="12345679";
	CK_UTF8CHAR_PTR pin=NULL;
	const char* Pinlist[] = {"SoPin", "UserPin"};

	printf("enter test_login.\n");
	nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"enter test_login.<br>") ;

	if (userType==CKU_USER)
	{
		pin=userpin;
		//pin = "88888888";
	}
	else if (userType==CKU_SO)
	{
		pin=adminpin;
	}
	else
	{
		printf("test_login failed,parameter is wrong.\n");
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_login failed,parameter is wrong.<br>") ;
		return FALSE;
	}	

	rv=(*pC_Login)(hSession,userType,pin,strlen((char*)pin));

	printf("Login Pin is:  %s=%s\n", Pinlist[userType], pin);
	if(rv!=CKR_OK)
	{
		if(rv==0x00000100)
		{
			printf("test_login already logged in,go on.\n");
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength, "test_login already logged in,go on.<br>") ;
		}
		else
		{
			printf("test_login failed,pC_Login. rv=0x%08x.\n",rv);	
			nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_login failed,pC_Login. rv=0x%08x.<br>",rv) ;
			return FALSE;
		}
	}
	printf("Login Pin OK\n");
	return TRUE;
}
/*注销认证*/
CK_RV test_logout()
{
	CK_RV rv=0;

	printf("enter test_logout.\n");
	
	rv=(*pC_Logout)(hSession);
	if(rv!=CKR_OK)
	{
		printf("test_logout failed,pC_Logout. rv=0x%08x.\n",rv);
		nResultLength += sprintf(Testresult[nItemNumb] + nResultLength,"test_logout failed,pC_Logout. rv=0x%08x.<br>",rv) ;
		return FALSE;
	}

	return TRUE;
}
