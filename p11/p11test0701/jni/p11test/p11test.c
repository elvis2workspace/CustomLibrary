// p11test.cpp : Defines the entry point for the console application.
//#include <stdlib.h>
#include <assert.h>
//#include <vector>
#include <general.h>
#include "Encrypt.h"
#include "ErrorTest.h"
#include "Digest.h"
#include "GenKeyPair.h"
#include "KeyManage.h"
#include "pin.h"
#include "auth.h"
#include "object.h"
#include "GeneralCall.h"
#include "mxml_for_p11.h"
#include <errno.h>
#include "Utils_c.h"
#include <stdio.h>


#if defined(__ANDROID__)
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#endif /*__ANDROID__*/

#if defined(__linux__)
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <execinfo.h>
#include <pthread.h>

void dump(int signo)
{
        char buf[1024];
        char cmd[1024];
        FILE *fh;

        snprintf(buf, sizeof(buf), "/proc/%d/cmdline", getpid());
        if(!(fh = fopen(buf, "r")))
                exit(0);
        if(!fgets(buf, sizeof(buf), fh))
                exit(0);
        fclose(fh);
        if(buf[strlen(buf) - 1] == '\n')
                buf[strlen(buf) - 1] = '\0';
        snprintf(cmd, sizeof(cmd), "gdb %s %d", buf, getpid());
        system(cmd);

        exit(0);
}
#endif



int TestMonopolize()
{
	int i = 0;
	bool brtn = false;
	int im=0;



	//获取各种信息
	brtn=test_geteveryinfo();
	if(brtn!=true)
	{
		return 0;
	}

	for(i = 0; i <= 10; ++i)
	{

		 char isSelf[2] = {0};
		 int iret = 0;
		 printf("是否进入独占(Y/N)？\n");
		 scanf("%s",isSelf);

		 if(isSelf[0] == 'y' || isSelf[0] == 'Y')
		 {
			 iret =  pC_Monopolize_Enable();
			 if(iret == 0)
		  		 printf("Monopolize_Enable Sucess\n");
			 if(iret  == 0xA0005000 )
				 printf("Monopolize_Enable return SERVER_MONOPOLIZING_ERROR\n");
			 if(iret  == 0xA0006000 )
				 printf("Monopolize_Enable return SERVER_PERMISSION_DENIED \n");

			 iret =  pC_Monopolize_Enable();
			 if(iret == 0)
		  		 printf("Monopolize_Enable Sucess\n");
			 if(iret  == 0xA0005000 )
				 printf("Monopolize_Enable return SERVER_MONOPOLIZING_ERROR\n");
			 if(iret  == 0xA0006000 )
				 printf("Monopolize_Enable return SERVER_PERMISSION_DENIED \n");

			// sleep(30);

		 }

		for ( im=0; im<1; im++)
		{
			brtn=test_object_cert();
			/*sleep(3);*/
			if(!brtn)
			{
				return 0;
			}
		}




	
		iret =  pC_Monopolize_Disable();
		if(iret == 0)
			printf("pC_Monopolize_Disable Sucess\n");
		if(iret  == 0xA0006000 )
			printf("pC_Monopolize_Disable SERVER_MONOPOLIZING_ERROR \n");
		if(iret  == 0xA0007000 )
			printf("pC_Monopolize_Disable SERVER_ALREADY_DISMONOPOLIZE \n");

		iret =  pC_Monopolize_Disable();
		if(iret == 0)
			printf("pC_Monopolize_Disable Sucess\n");
		if(iret  == 0xA0006000 )
			printf("pC_Monopolize_Disable SERVER_MONOPOLIZING_ERROR \n");
		if(iret  == 0xA0007000 )
			printf("pC_Monopolize_Disable SERVER_ALREADY_DISMONOPOLIZE \n");


	////	//随机数测试
	//	brtn=test_random();
	//	if(!brtn)
	//	{
	//		return 0;
	//	}
	}

	return 1;
}


CK_RV Test_All(int looptime,int datalen )
{
	CK_RV xnRtn = FALSE;

		xnRtn = test_getinfo();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn = xtest_Flash_Normal();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		
		xnRtn = TestUnwrapAll();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn =test_UnwrapKey();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		
		xnRtn =test_CreateKey();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn = test_WrapKeyOut_Gen();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn = test_WrapKeyOut();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn = test_TTV4V5();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn = test_BKV4V5();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}



		xnRtn = xtest_ZUCPerformance_Gen(looptime,datalen);
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn = xtest_SM4OFB_Speed_Gen(looptime,datalen);
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn = xtest_SM4ECB_Speed_Gen(looptime,datalen);
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn = xtest_SM4CBC_Speed_Gen(looptime,datalen);
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}


		xnRtn=xtest_ZUCEncrypt();	
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn=xtest_ZUCDecrypt();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn=xtest_SM3Encrypt();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}


		xnRtn=xtest_SM2PointMul1();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn=xtest_SM2PointMul2();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn=xtest_SM2KeyCoordinate();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn=xtest_SM3KDF();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn=xtest_SM4ECB_Encrypt();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn=xtest_SM4ECB_Decrypt();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		printf("Start OFB Test...\n");
		xnRtn = test_SM4RAMnoValue(CKM_SM4_OFB, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1, 9);//RAM密钥
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn = test_SM4FLASHValue(CKM_SM4_OFB, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1, 9);//FLASH密钥
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn = test_SM4FLASHnoValue(CKM_SM4_OFB, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1, 9);//FLASH预设密钥
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		printf("Start ECB Test...\n");
		xnRtn = test_SM4RAMnoValue(CKM_SM4_ECB, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1, 10);//RAM密钥
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn = test_SM4FLASHValue(CKM_SM4_ECB, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1, 10);//FLASH密钥
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn = test_SM4FLASHnoValue(CKM_SM4_ECB, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1, 10);//FLASH预设密钥
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		printf("Start CBC Test...\n");
		xnRtn = test_SM4RAMnoValue(CKM_SM4_CBC, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1, 11);//RAM密钥
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn = test_SM4FLASHValue(CKM_SM4_CBC, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1, 11);//FLASH密钥
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn = test_SM4FLASHnoValue(CKM_SM4_CBC, CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1, 11);//FLASH预设密钥
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		printf("Start ZUC Test (UserPin)...\n");
		xnRtn=test_ZUCRAM(CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1, 13);//RAM密钥
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn=test_ZUCFLASHValue(CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1,13);//FLASH密钥
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn=test_ZUCFLASHnoValue(CK_SESSKEY_PRESET_ID0, CK_SESSKEY_PRESET_ID1, 13);//FLASH预设密钥
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn = xtest_ZUCPerformance(looptime,datalen);			
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn = xtest_ZUCHashPerformance_withkey(looptime,datalen);
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn=xtest_SM2GenPerformance(looptime,datalen);			
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn=xtest_SM2ImportPerformance(looptime,datalen);
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn=xtest_SM3Performance(looptime,datalen);
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn=xtest_SM4OFB_Speed(looptime,datalen);	
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn=xtest_SM4ECB_Speed(looptime,datalen);
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn=xtest_SM4CBC_Speed(looptime,datalen);
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn=xtest_SM3KDF_Speed();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn= xtest_SM2PointMul_speed(looptime);
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn= xtest_GenerateKeyPair_Performance(looptime);
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn = xtest_SM2KeyCoordinate_Performace();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn = test_keystate();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn = xtest_SM3KDF_Object();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn = xtest_SM3KDF_OFF();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		
		xnRtn = test_Flash_SymmetricKey();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}


		xnRtn = xtest_SM4ECB_Create_Object();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn = xtest_FindSecretKEY();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn=etest_ZUCEncrypt();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn=etest_ZUCDecrypt();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn=etest_SM2GenEnDe();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}
		xnRtn=etest_SM2ImpSiVr();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		xnRtn=test_object_cert();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}	

		xnRtn = test_OTPV4V5();
		if(xnRtn==FALSE)
		{
			return xnRtn;
		}

		return TRUE;
	
}

/*CK_RV xTestChoose(int xSwitch,int looptime,int datalen)
{
	CK_RV xnRtn = FALSE;
	int i = 0;
	FILE *fp = NULL;
	int j = 0;
	struct timeval tv;
	struct tm *t = 0;

	looptime=1;
	datalen=32;	
	xSwitch=101;


	
for(i=101;i<149;i++)
{
	switch(i)
	{
		case 101:
			nResultLength = 0;
			nItemNumb = 1;
			xnRtn = xtest_ZUCEncrypt();			
			break;
		case 102:
			nResultLength = 0;
			nItemNumb = 2;
			xnRtn = xtest_ZUCDecrypt();
			break;
		case 103:
			nResultLength = 0;
			nItemNumb = 3;
			xnRtn = xtest_SM3Encrypt();
			break;
		case 104:
			nResultLength = 0;
			nItemNumb = 4;
			xnRtn = xtest_SM2PointMul1();
			break;
		case 105:
			nResultLength = 0;
			nItemNumb = 5;
			xnRtn = xtest_SM2PointMul2();
			break;
		case 106:
			nResultLength = 0;
			nItemNumb = 6;
			xnRtn = xtest_SM2KeyCoordinate();
			break;
		case 107:
			nResultLength = 0;
			nItemNumb = 7;
			xnRtn = xtest_SM3KDF();
			break;
		case 108:
			nResultLength = 0;
			nItemNumb = 8;
			xnRtn = xtest_SM4ECB_Encrypt();
			break;
		case 109:
			nResultLength = 0;
			nItemNumb = 9;
			xnRtn = xtest_SM4ECB_Decrypt();
			break;
		case 110:
			nResultLength = 0;
			nItemNumb = 10;
			xnRtn = xtest_SM4_KEY(CKM_SM4_OFB);
			break;
		case 111:
			nResultLength = 0;
			nItemNumb = 11;
			xnRtn = xtest_SM4_KEY(CKM_SM4_ECB);
			break;
		case 112:
			nResultLength = 0;
			nItemNumb = 12;
			xnRtn = xtest_SM4_KEY(CKM_SM4_CBC);
			break;
		case 113:
			nResultLength = 0;
			nItemNumb = 13;
			xnRtn = xtest_ZUC_KEY();		
			break;
		case 114:
			nResultLength = 0;
			nItemNumb = 14;
			xnRtn = test_WrapKeyOut();
			break;
		case 115:
			nResultLength = 0;
			nItemNumb = 15;
			xnRtn = test_WrapKeyOut_Gen();
			break;
		case 116:
			nResultLength = 0;
			nItemNumb = 16;
			xnRtn = TestUnwrapAll();
			break;
		case 117:
			nResultLength = 0;
			nItemNumb = 17;
			xnRtn = test_TTV4V5();
			break;
		case 118:
			nResultLength = 0;
			nItemNumb = 18;
			xnRtn = test_OTPV4V5();
			break;
		case 119:
			nResultLength = 0;
			nItemNumb = 19;
			xnRtn = test_BKV4V5();
			break;

		case 120:	
			nResultLength = 0;
			nItemNumb = 20;
			xnRtn = xtest_ZUCPerformance(looptime, datalen);			
			break;
		case 121:
			nResultLength = 0;
			nItemNumb = 21;
			xnRtn = xtest_ZUCPerformance_Gen(looptime,datalen);
			break;
		case 122:
			nResultLength = 0;
			nItemNumb = 22;
			xnRtn = xtest_ZUCHashPerformance_withkey(looptime, datalen);
			break;
		case 123:	
			nResultLength = 0;
			nItemNumb = 23;
			xnRtn = xtest_SM2GenPerformance(looptime, datalen);	
			break;
		case 124:
			nResultLength = 0;
			nItemNumb = 24;
			xnRtn = xtest_SM2ImportPerformance(looptime, datalen);
			break;
		case 125:
			nResultLength = 0;
			nItemNumb = 25;
			xnRtn = xtest_SM3Performance(looptime, datalen);
			break;
		case 126:
			nResultLength = 0;
			nItemNumb = 26;
			xnRtn = xtest_SM4OFB_Speed(looptime, datalen);			
			break;

		case 127:
			nResultLength = 0;
			nItemNumb = 27;
			xnRtn = xtest_SM4OFB_Speed_Gen(looptime,datalen);
			break;


		case 128:
			nResultLength = 0;
			nItemNumb = 28;
			xnRtn = xtest_SM4ECB_Speed(looptime, datalen);
			break;
		case 129:
			nResultLength = 0;
			nItemNumb = 29;
			xnRtn = xtest_SM4ECB_Speed_Gen(looptime,datalen);
			break;
		case 130:
			nResultLength = 0;
			nItemNumb = 30;
			xnRtn = xtest_SM4CBC_Speed(looptime, datalen);
			break;
		case 131:
			nResultLength = 0;
			nItemNumb = 31;
			xnRtn = xtest_SM4CBC_Speed_Gen(looptime,datalen);
			break;
		case 132:
			nResultLength = 0;
			nItemNumb = 32;
			xnRtn = xtest_SM3KDF_Speed();
			break;
		case 133:
			nResultLength = 0;
			nItemNumb = 33;
			xnRtn = xtest_SM2PointMul_speed(looptime);
			break;
		case 134:
			nResultLength = 0;
			nItemNumb = 34;
			xnRtn = xtest_GenerateKeyPair_Performance(looptime);
			break;
			//case 125:
			//nResultLength = 0;
			//nItemNumb = 25;
			//xnRtn = xtest_GenerateKey_Performace(looptime);
			//break;			
		case 135:
			nResultLength = 0;
			nItemNumb = 35;
			xnRtn = xtest_SM2KeyCoordinate_Performace();
			break;
		case 136:
			nResultLength = 0;
			nItemNumb = 36;
			xnRtn = test_getinfo();
			break;
		case 137:
			nResultLength = 0;
			nItemNumb = 37;
			xnRtn = test_keystate();
			break;
		case 138:
			nResultLength = 0;
			nItemNumb = 38;
			xnRtn = xtest_SM3KDF_Object();
			break;
		case 139:
			nResultLength = 0;
			nItemNumb = 39;
			xnRtn = xtest_SM3KDF_OFF();
			break;
		case 140:
			nResultLength = 0;
			nItemNumb = 40;
			xnRtn = xtest_Flash_Normal();
			break;
		case 141:
			nResultLength = 0;
			nItemNumb = 41;
			xnRtn = test_otp();
			break;
		case 142:
			nResultLength = 0;
			nItemNumb = 42;
			xnRtn = test_UnwrapKey();
			break;
		case 143:
			nResultLength = 0;
			nItemNumb = 43;
			xnRtn = test_CreateKey();
			break;
		case 144:
			nResultLength = 0;
			nItemNumb = 44;
			xnRtn = xtest_FindKeyObjectAndDestory();
			break;
		case 145:
			nResultLength = 0;
			nItemNumb = 45;
			xnRtn = xtest_GenerateKeyPairAndOperateDate();
			break;

		case 146:
			nResultLength = 0;
			nItemNumb = 46;
			xnRtn = xtest_SetDestroyKeyIncorrect();
			break;
		case 147:
			nResultLength = 0;
			nItemNumb = 47;
			xnRtn = xtest_SetDestroyKeyCorrect();
			break;
		case 148://destroy key ==destroy card
			nResultLength = 0;
			nItemNumb = 48;
			//xnRtn = xtest_DestroyKey();
			break;

		case 149:
			nResultLength = 0;
			nItemNumb = 49;
			xnRtn=etest_ZUCEncrypt();
			break;
		case 150:
			nResultLength = 0;
			nItemNumb = 50;
			xnRtn=etest_ZUCDecrypt();
			break;
		case 151:
			nResultLength = 0;
			nItemNumb = 51;
			xnRtn=etest_SM2GenEnDe();
			break;
		case 152:
			nResultLength = 0;
			nItemNumb = 52;
			xnRtn=etest_SM2ImpSiVr();
			break;
		case 153:
			nResultLength = 0;
			nItemNumb = 53;
			xnRtn=etest_SM3Digest();
			break;
		case 154:
			nResultLength = 0;
			nItemNumb = 54;
			xnRtn=etest_SM4();
			break;

		case 166:
			printf("666666666\n");
			nResultLength = 0;
			nItemNumb = 66;
			xnRtn=xtest_SM2ImportDataVerify0606();
			break;
		case 167:
			printf("67676767\n");
			nResultLength = 0;
			nItemNumb = 67;
			xnRtn=xtest_ZUCEncrypt0607();

			printf("67676767222\n");
			nResultLength = 0;
			nItemNumb = 67;
			xnRtn=xtest_ZUCEncrypt0607();
			break;
		
	
			
		case 188:
			nResultLength = 0;
			nItemNumb = 88;
			xnRtn=xtest_FindPublicPrivateKey();
			break;
		case 189:
			nResultLength = 0;
			nItemNumb = 89;
			xnRtn=xtest_xxxxSM2ImportPerformance();
			break;	
		case 190:
			nResultLength = 0;
			nItemNumb = 90;
			xnRtn=xtest_object_data_create();
			break;
		case 191:
			nResultLength = 0;
			nItemNumb = 91;
			xnRtn=xtest_FindData();
			break;

		case 192:
			nResultLength = 0;
			nItemNumb = 92;
			xnRtn=xtest_FindDataObjectAndDestory();
			break;

			
			
		case 800:
			//xnRtn=test_object_cert();
			break;	

		case 777:

			for(i=0;i<10000;i++)
			{
				gettimeofday(&tv, 0);
				t = localtime(&(tv.tv_sec));
				printf("iiiiiiiiiiiiiiiii=%d\n",i);
				printf( " Time: %04d-%02d-%02d %02d:%02d:%02d:%03ld\n", t->tm_year+1900,t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec/1000);
				//xnRtn = Test_All(looptime,datalen);
				if(xnRtn == FALSE)
				{
					break;
				}
			}
			break;
		default:
			printf("Error：Choose not define!\n");
			break;
	}

					if(xnRtn == FALSE)
				{
					break;
				}
	}
	printf("########################################\n");

	return(xnRtn);
}*/

const char* pcTestFile ="/sdcard/XMLFile/Testinfo.txt";


CK_RV xTestChoose(int xSwitch,int looptime,int datalen)
{
	CK_RV xnRtn = FALSE;
	int i = 0;
	FILE *fp = NULL;
	char str[256];
	int j = 0;
	struct timeval tv;
	struct tm *t = 0;


	//looptime=1;
	//datalen=32;	
	//xSwitch=800;

	printf("****************************************\n");
	//remove(pcZucFile);
	FILE *_fp=fopen(pcTestFile,"a");
	
	//for(j=101;j<155;j++)
	//{
	switch(xSwitch)
	{
		case 101:
			//FILE *_fp=fopen(pcZucFile,"a");
			nResultLength = 0;
			nItemNumb = 1;
			xnRtn = xtest_ZUCEncrypt();		
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/

			break;
		case 102:
			nResultLength = 0;
			nItemNumb = 2;
			xnRtn = xtest_ZUCDecrypt();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 103:
			nResultLength = 0;
			nItemNumb = 3;
			xnRtn = xtest_SM3Encrypt();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 104:
			nResultLength = 0;
			nItemNumb = 4;
			xnRtn = xtest_SM2PointMul1();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 105:
			nResultLength = 0;
			nItemNumb = 5;
			xnRtn = xtest_SM2PointMul2();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 106:
			nResultLength = 0;
			nItemNumb = 6;
			xnRtn = xtest_SM2KeyCoordinate();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 107:
			nResultLength = 0;
			nItemNumb = 7;
			xnRtn = xtest_SM3KDF();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 108:
			nResultLength = 0;
			nItemNumb = 8;
			xnRtn = xtest_SM4ECB_Encrypt();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 109:
			nResultLength = 0;
			nItemNumb = 9;
			xnRtn = xtest_SM4ECB_Decrypt();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 110:
			nResultLength = 0;
			nItemNumb = 10;
			xnRtn = xtest_SM4_KEY(CKM_SM4_OFB);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 111:
			nResultLength = 0;
			nItemNumb = 11;
			xnRtn = xtest_SM4_KEY(CKM_SM4_ECB);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 112:
			nResultLength = 0;
			nItemNumb = 12;
			xnRtn = xtest_SM4_KEY(CKM_SM4_CBC);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 113:
			nResultLength = 0;
			nItemNumb = 13;
			xnRtn = xtest_ZUC_KEY();	
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 114:
			nResultLength = 0;
			nItemNumb = 14;
			xnRtn = test_WrapKeyOut();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 115:
			nResultLength = 0;
			nItemNumb = 15;
			xnRtn = test_WrapKeyOut_Gen();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 116:
			nResultLength = 0;
			nItemNumb = 16;
			xnRtn = TestUnwrapAll();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 117:
			nResultLength = 0;
			nItemNumb = 17;
			xnRtn = test_TTV4V5();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 118:
			nResultLength = 0;
			nItemNumb = 18;
			xnRtn = test_OTPV4V5();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 119:
			nResultLength = 0;
			nItemNumb = 19;
			xnRtn = test_BKV4V5();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;

		case 120:	
			nResultLength = 0;
			nItemNumb = 20;
			xnRtn = xtest_ZUCPerformance(looptime, datalen);	
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 121:
			nResultLength = 0;
			nItemNumb = 21;
			xnRtn = xtest_ZUCPerformance_Gen(looptime,datalen);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 122:
			nResultLength = 0;
			nItemNumb = 22;
			xnRtn = xtest_ZUCHashPerformance_withkey(looptime, datalen);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 123:	
			nResultLength = 0;
			nItemNumb = 23;
			xnRtn = xtest_SM2GenPerformance(looptime, datalen);	
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 124:
			nResultLength = 0;
			nItemNumb = 24;
			xnRtn = xtest_SM2ImportPerformance(looptime, datalen);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 125:
			nResultLength = 0;
			nItemNumb = 25;
			xnRtn = xtest_SM3Performance(looptime, datalen);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 126:
			nResultLength = 0;
			nItemNumb = 26;
			xnRtn = xtest_SM4OFB_Speed(looptime, datalen);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;

		case 127:
			nResultLength = 0;
			nItemNumb = 27;
			xnRtn = xtest_SM4OFB_Speed_Gen(looptime,datalen);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;


		case 128:
			nResultLength = 0;
			nItemNumb = 28;
			xnRtn = xtest_SM4ECB_Speed(looptime, datalen);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 129:
			nResultLength = 0;
			nItemNumb = 29;
			xnRtn = xtest_SM4ECB_Speed_Gen(looptime,datalen);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 130:
			nResultLength = 0;
			nItemNumb = 30;
			xnRtn = xtest_SM4CBC_Speed(looptime, datalen);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 131:
			nResultLength = 0;
			nItemNumb = 31;
			xnRtn = xtest_SM4CBC_Speed_Gen(looptime,datalen);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 132:
			nResultLength = 0;
			nItemNumb = 32;
			xnRtn = xtest_SM3KDF_Speed();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 133:
			nResultLength = 0;
			nItemNumb = 33;
			xnRtn = xtest_SM2PointMul_speed(looptime);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 134:
			nResultLength = 0;
			nItemNumb = 34;
			xnRtn = xtest_GenerateKeyPair_Performance(looptime);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
			//case 125:
			//nResultLength = 0;
			//nItemNumb = 25;
			//xnRtn = xtest_GenerateKey_Performace(looptime);
			//break;			
		case 135:
			nResultLength = 0;
			nItemNumb = 35;
			xnRtn = xtest_SM2KeyCoordinate_Performace();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 136:
			nResultLength = 0;
			nItemNumb = 36;
			xnRtn = test_getinfo();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 137:
			nResultLength = 0;
			nItemNumb = 37;
			xnRtn = test_keystate();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 138:
			nResultLength = 0;
			nItemNumb = 38;
			xnRtn = xtest_SM3KDF_Object();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 139:
			nResultLength = 0;
			nItemNumb = 39;
			xnRtn = xtest_SM3KDF_OFF();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 140:
			nResultLength = 0;
			nItemNumb = 40;
			xnRtn = xtest_Flash_Normal();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 141:
			nResultLength = 0;
			nItemNumb = 41;
			xnRtn = test_otp();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 142:
			nResultLength = 0;
			nItemNumb = 42;
			xnRtn = test_UnwrapKey();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 143:
			nResultLength = 0;
			nItemNumb = 43;
			xnRtn = test_CreateKey();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 144:
			nResultLength = 0;
			nItemNumb = 44;
			xnRtn = xtest_FindKeyObjectAndDestory();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 145:
			nResultLength = 0;
			nItemNumb = 45;
			xnRtn = xtest_GenerateKeyPairAndOperateDate();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;

		case 146:
			nResultLength = 0;
			nItemNumb = 46;
			xnRtn = xtest_SetDestroyKeyIncorrect();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 147:
			nResultLength = 0;
			nItemNumb = 47;
			xnRtn = xtest_SetDestroyKeyCorrect();
			//xnRtn = test_logout();
			//xnRtn = test_login(CKU_SO);
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 148://destroy key ==destroy card
			nResultLength = 0;
			nItemNumb = 48;
			xnRtn = xtest_DestroyKey();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;

		case 149:
			nResultLength = 0;
			nItemNumb = 49;
			xnRtn=etest_ZUCEncrypt();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 150:
			nResultLength = 0;
			nItemNumb = 50;
			xnRtn=etest_ZUCDecrypt();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 151:
			nResultLength = 0;
			nItemNumb = 51;
			xnRtn=etest_SM2GenEnDe();
			sprintf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 152:
			nResultLength = 0;
			nItemNumb = 52;
			xnRtn=etest_SM2ImpSiVr();
			printf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 153:
			nResultLength = 0;
			nItemNumb = 53;
			xnRtn=etest_SM3Digest();
			printf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;
		case 154:
			nResultLength = 0;
			nItemNumb = 54;
			xnRtn=etest_SM4();
			printf(str, "%d %d\n",nItemNumb, xnRtn);
			fputs(str,_fp);/*将字符串写入文件*/
			break;


		case 800:
			xnRtn=test_object_cert();
			break;	

		case 777:

			for(i=0;i<10000;i++)
			{
				gettimeofday(&tv, 0);
				t = localtime(&(tv.tv_sec));
				printf("iiiiiiiiiiiiiiiii=%d\n",i);
				printf( " Time: %04d-%02d-%02d %02d:%02d:%02d:%03ld\n", t->tm_year+1900,t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec/1000);
				//xnRtn = Test_All(looptime,datalen);
				if(xnRtn == FALSE)
				{
					break;
				}
			}
			break;
		default:
			printf("Error：Choose not define!\n");
			break;
	}
			/*	if(xnRtn == FALSE)
				{
					break;
				}
	*/
	fclose(_fp);
	printf("########################################\n");

	return(xnRtn);
}


const char * pathname=dllname_my;

CK_RV HardSoftShift_SetPin(int hard_soft)
{
	printf("************************\n");
	if(hard_soft==0)
	{
		printf("choose hard library.\n");
		pathname=dllname_my;
		strcpy((char *)userpin,"88888888");
		strcpy((char *)adminpin,"12345678");
	}
	else if(hard_soft==1)
	{
		printf("choose soft library.\n");
		pathname=dllname_soft;
		strcpy((char *)userpin,"123456"/*"abcdefabcdefabcdefabcdefabcdef"*/);
	}
	else
	{
		printf("choose library error.\n");
		return FALSE;
	}
	return TRUE;
}

//#define TEST_CHANGE_PIN
//WriteCount控制是打印流程信息：0为进入用例失败，打印异常载入信息；其它为进入用例成功，打印正常载入信息和各个用例信息
int LeaveMain(int WriteCount)
{
	functionpop(0);

	if(WriteCount == 0)
	{		
		Testchoose[0] = 100;
		Test[0] = 0;
	}
	else
	{
		Testchoose[0] = 100;
		Test[0] = 1;
	}

	WriteXml(WriteCount);
	return 0;	
}

CK_RV TestLoop(unsigned int* chooseitem, unsigned int* itemdatalen, unsigned int* itemlooptime, unsigned int itemcount)
{
	CK_RV xnRtn=0;
	unsigned int Choose = 0;
	unsigned int LoopTime = 10;
	unsigned int DataLen = 16;
	unsigned int i = 0;

	for(i= 0; i<itemcount; i++)
	{
		Choose = chooseitem[i];
		LoopTime = itemlooptime[i];
		DataLen = itemdatalen[i];

		printf("Choose=%d\n",Choose);
		Testchoose[i+1]=Choose;
		xnRtn=xTestChoose(Choose,LoopTime,DataLen);	

		if(xnRtn == TRUE)
		{
			Test[Choose-100] = 1;
		}
		else
		{
			Test[Choose-100] = 0;
		}	
	}
	return TRUE;
}



int main()
{

	CK_RV nRtn=0;
	CK_RV xnRtn = FALSE;
	CK_USER_TYPE userType=CKU_USER, soType=CKU_SO;

	unsigned int chooseitem[100] = {0};
	unsigned int itemdatalen[100] = {0};
	unsigned int itemlooptime[100] = {0};
	unsigned int itemcount = 0;
	int i=0;

	CK_RV rv=0;
	CK_ULONG ulCountUser=0;
	CK_ATTRIBUTE attru = {0x5d6c0007, &ulCountUser, sizeof(CK_ULONG)};

#if defined(__linux__)
	signal(SIGSEGV, &dump);
	//dummy_function();
	printf("_SC_THREAD_PROCESS_SHARED=%d.\n",sysconf(_SC_THREAD_PROCESS_SHARED));
#endif

	nRtn = ReadXml(chooseitem, itemdatalen, itemlooptime, &itemcount);
	if(nRtn != TRUE)
	{
		return LeaveMain(0);
	}

	//chooseitem[0]=101;
	//itemdatalen[0]=32;
	//itemlooptime[0]=100;
	//itemcount=1;
	//0-hard ,1-soft,others-error
	nRtn = HardSoftShift_SetPin(0);
	if(nRtn!=TRUE)
	{
		return LeaveMain(0);
	}

	nResultLength = 0;
	nItemNumb = 0;

	//载入动态库
	nRtn = test_loadlibrary();
	if(nRtn!=TRUE)
	{
		return LeaveMain(0);
	}

	nRtn = functionpush(test_freelibrary, "test_freelibrary");
	if(nRtn!=TRUE)
	{
		return LeaveMain(0);
	}
	//打开设备
	nRtn=test_opendevice();
	if(nRtn!=TRUE)
	{
		return LeaveMain(0);
	}

	nRtn = functionpush(test_closedevice, "test_closedevice");
	if(nRtn!=TRUE)
	{
		return LeaveMain(0);
	}

	nRtn=test_login(userType);
	if(nRtn!=TRUE)
	{
		return LeaveMain(0);
	}

	nRtn = functionpush(test_logout, "test_logout");
	if(nRtn!=TRUE)
	{
		return LeaveMain(0);
	}

	printf("itemcount=%d\n",itemcount);
	nRtn = TestLoop(chooseitem, itemdatalen, itemlooptime, itemcount);
	if(nRtn!=TRUE)
	{
		return LeaveMain(itemcount);
	}

	return LeaveMain(itemcount);	
}

