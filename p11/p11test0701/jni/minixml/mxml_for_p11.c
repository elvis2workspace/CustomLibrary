#include "config_mini.h"
#include "mxml.h"
#include "general.h"

#if defined(__ANDROID__)
#include <fcntl.h>
#endif /*__ANDROID__*/

#if defined(__linux__)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif





//#define INPUT_PIN
int InputPin(int argc, char *argv[])
{
	if(argc < 7)
		return 1;

	strcpy(czAppName,argv[1]);
	printf("Input Application name:%s\n", czAppName);

	sscanf(argv[2],"%d",&ilevel);
	printf("Input security level 0~255:%d\n",ilevel);


	memset(userpin,0,sizeof(userpin));
	strcpy((char *)userpin,argv[3]);
	strcpy((char *)userpin,"12345678");
	printf("Please input the user pin(%d~%d):%s\n",MIN_PIN_LEN,MAX_PIN_LEN,userpin);

	memset(adminpin,0,sizeof(adminpin));
	strcpy((char*)adminpin,argv[4]);
	printf("Please input the admin pin(%d~%d): %s\n",MIN_PIN_LEN,MAX_PIN_LEN,adminpin);

	printf("Input option 0~255£º%s\n",argv[5]);

	printf("Input looptime£º%s\n",argv[6]);

	return 0;
}

unsigned int XmlStr2Int(const char * readstrdata)
{
	int readstrlen = 0;
	char readstrdatabuf = 0x00;
	unsigned int readnumber = 0;
	unsigned int readfactor = 1;
	int i = 0;
	
	if(strcmp(readstrdata,"null") == 0)
	{
		return 0;
	}

	readstrlen = strlen(readstrdata);

	for(i = readstrlen-1; i >= 0; --i)
	{
		readstrdatabuf = *(readstrdata+i)-0x30;
		readnumber += ((unsigned int)readstrdatabuf)*readfactor;
		readfactor *= 10;
	}

	if(readnumber <= 0)
	{
		printf("Error:read number should bigger then 0\n");
		return 0;
	}

	return readnumber;
}

unsigned int XmlStr2IntDelateBytes(const char * readstrdata)
{
	int readstrlen = 0;
	char readstrdatabuf = 0x00;
	unsigned int readnumber = 0;
	unsigned int readfactor = 1;
	int i = 0;

	if(strcmp(readstrdata,"null") == 0)
	{
		return 0;
	}

	readstrlen = strlen(readstrdata);
	readstrlen -= 5;

	for(i=readstrlen-1;i>=0; --i)
	{			
		readstrdatabuf = *(readstrdata+i)-0x30;
		readnumber += readstrdatabuf*readfactor;
		readfactor *= 10;
	}

	if(readnumber <= 0)
	{
		printf("Error:read number should bigger then 0\n");
		return 0;
	}

	return readnumber;
}


/*
 * 'type_cb()' - XML data type callback for mxmlLoadFile()...
 */

mxml_type_t				/* O - Data type */
type_cb(mxml_node_t *node)		/* I - Element node */
{
  const char	*type;			/* Type string */


 /*
  * You can lookup attributes and/or use the element name, hierarchy, etc...
  */

  if ((type = mxmlElementGetAttr(node, "type")) == NULL)
    type = node->value.element.name;

  if (strstr(type, "pin"))
    return (MXML_TEXT);
  else
    return (MXML_INTEGER);
}

CK_RV ReadXml(unsigned int*chooseitem, unsigned int*itemdatalen, unsigned int* itemlooptime, unsigned int* itemcount)
{
	const char*  mxmlreadfile= "/sdcard/XMLFile/data.xml";
	const char*  mxmlwritefile= "/sdcard/XMLFile/Result.xml";
	unsigned int readcnt = 0;
	const char* readbuf_ch = NULL;
	FILE *fp;
	mxml_node_t *tree;
	mxml_node_t *xml;   
	mxml_node_t *root;  
	mxml_node_t *test;  
	mxml_node_t *data; 
	const char* readstrdata =0;

	printf("enter ReadXml\n");

	fp = fopen(mxmlreadfile, "r");
	if(fp == NULL)
	{
		printf("open the info.xml error!\n");
		return FALSE;
	}

	tree = mxmlLoadFile(NULL, fp,type_cb);

	if(tree == NULL)
	{
		printf("Load file error!\n");
		return FALSE;
	}

	root = mxmlFindElement(tree, tree, "root",NULL, NULL,MXML_DESCEND);
	if(root == NULL)
	{
		printf("can not find element root!\n");
		return FALSE;
	}

	test = mxmlFindElement(root, tree, "Test",NULL, NULL,MXML_DESCEND);
	if(test == NULL)
	{
		printf("can not find element test!\n");
		return FALSE;
	}

	//for(test = mxmlFindElement(root, tree, "Test","id", NULL,MXML_DESCEND);
	//	test != NULL;
	//	test = mxmlFindElement(test, tree, "Test","id", NULL,MXML_DESCEND))
		for(test = mxmlFindElement(root, tree, "Test",NULL, NULL,MXML_DESCEND);
			test != NULL;
			test = mxmlFindElement(test, tree, "Test",NULL, NULL,MXML_DESCEND))
	{

		//printf(" id:%s \n",mxmlElementGetAttr(test,"id"));

		data = mxmlFindElement(test, tree, "Testname",NULL, NULL,MXML_DESCEND);
		if(data == NULL)
		{
			printf("can not find element data!\n");
			return FALSE;
		}
		chooseitem[readcnt] = mxmlGetInteger(data);		
		printf("chooseitem[%d]=%d\n",readcnt,chooseitem[readcnt]);

		data = mxmlFindElement(data, tree, "Userpin",NULL, NULL,MXML_DESCEND);
		if(data == NULL)
		{
			printf("can not find element data!\n");
			return FALSE;
		}
		readbuf_ch=mxmlGetText(data,NULL);
		strcpy((char *)userpin,readbuf_ch/*"abcdefabcdefabcdefabcdefabcdef"*/);
		printf("userpin is %s\n",userpin);

		data = mxmlFindElement(data, tree, "Sopin",NULL, NULL,MXML_DESCEND);
		if(data == NULL)
		{
			printf("can not find element data!\n");
			return FALSE;
		}
		readbuf_ch=mxmlGetText(data,NULL);
		strcpy((char *)adminpin,readbuf_ch/*"abcdefabcdefabcdefabcdefabcdef"*/);
		printf("adminpin is %s\n",adminpin);


		data = mxmlFindElement(data, tree, "Looptimes",NULL, NULL,MXML_DESCEND);
		if(data == NULL)
		{
			itemlooptime[readcnt] = 0;	
		}
		else
		{
			itemlooptime[readcnt] = mxmlGetInteger(data);	
		}
		
		printf("itemlooptime[%d]=%d\n",readcnt,itemlooptime[readcnt]);

		data = mxmlFindElement(data, tree, "Datalen",NULL, NULL,MXML_DESCEND);
		if(data == NULL)
		{
			itemdatalen[readcnt] = 0;
		}
		else
		{
			itemdatalen[readcnt] = mxmlGetInteger(data);
		}
	
		printf("itemdatalen[%d]=%d\n",readcnt,itemdatalen[readcnt]);
		readcnt++;
	}
	if(readcnt <= 0)
	{
		return FALSE;
	}

	*itemcount = readcnt;

	mxmlDelete(tree); 
	fclose(fp);

	printf("leave ReadXml\n");
	return TRUE;
}


CK_RV WriteXml(int CNT)
{
	int i = 0;
	FILE *fp;
	mxml_node_t *tree = NULL;
	mxml_node_t *xml = NULL;   
	mxml_node_t *root = NULL;  
	mxml_node_t *test[100] = {NULL};  
	mxml_node_t *testname[100] = {NULL};  
	mxml_node_t *testresut[100] = {NULL};  
	mxml_node_t *testinfo[100] = {NULL};  
	//mxml_node_t *data[100] = NULL;
	mxml_node_t *xmlresult1[100] = {NULL};
	mxml_node_t *xmlresult2[100] = {NULL};
	mxml_node_t *xmlresult3[100] = {NULL};

	char i_char = 0x00;
	char temp[5];
	char tmp[4]={0x00};

	const char* pcLogFile = "/sdcard/XMLFile/Result.xml";

	fp = fopen(pcLogFile, "w");
	xml = mxmlNewXML("1.0");

	tree= xml;
	root = mxmlNewElement(xml, "root");
	

	for(i = 0; i < CNT + 1; ++i)
	{	
		if(i>=9)
		{
			i_char = 0x00;
			i_char = (char)(i/10)+0x30;
			memcpy(tmp,&i_char,1);
			i_char = 0x00;
			i_char = (char)(i%10)+0x30;
			memcpy(tmp+1,&i_char,1);
		}
		else
		{
			i_char = 0x00;
			i_char = (char)i+0x30;
			memcpy(tmp,&i_char,1);
		}
		test[i] = mxmlNewElement(root, "Test");
		//mxmlElementSetAttr(test,"id",tmp);

		testname[i] = mxmlNewElement(test[i], "Testname");
		sprintf(temp, "%d ",Testchoose[i]);
		xmlresult1[i] = mxmlNewText(testname[i], 0, temp);

		testresut[i] = mxmlNewElement(test[i], "Result");
		sprintf(temp, "%d ",Test[Testchoose[i]-100]);
		xmlresult2[i] = mxmlNewText(testresut[i], 0, temp);

		if((Test[Testchoose[i]-100]==1)&&((Testchoose[i]<120)||(Testchoose[i]>135)))
		{
			memset(Testresult[Testchoose[i]-100], 0, strlen(Testresult[Testchoose[i]-100]));
		}

		testinfo[i] = mxmlNewElement(test[i], "info");
		xmlresult3[i] = mxmlNewText(testinfo[i], 0, Testresult[Testchoose[i]-100]);

	}

	mxmlSaveFile(tree, fp, MXML_NO_CALLBACK);
	fclose(fp);

	if(xml != NULL)
	{
		free(xml);
		xml = NULL;
	}

	if(root != NULL)
	{
		free(root);
		root = NULL;
	}

	for(i = 0; i < CNT + 1; ++i)
	{	
		if(test[i] != NULL)
		{
			free(test[i]);
			test[i] = NULL;
		}

		if(testname[i] != NULL)
		{
			free(testname[i]);
			testname[i] = NULL;
		}

		if(testresut[i] != NULL)
		{
			free(testresut[i]);
			testresut[i] = NULL;
		}

		if(testinfo[i] != NULL)
		{
			free(testinfo[i]);
			testinfo[i] = NULL;
		}

		if(xmlresult1[i] != NULL)
		{
			free(xmlresult1[i]);
			xmlresult1[i] = NULL;
		}

		if(xmlresult2[i] != NULL)
		{
			free(xmlresult2[i]);
			xmlresult2[i] = NULL;
		}

		if(xmlresult3[i] != NULL)
		{
			free(xmlresult3[i]);
			xmlresult3[i] = NULL;
		}
		
	}

	return TRUE;
}

//
//FILE *fpWriteXml;
//mxml_node_t *treeWriteXml;
//mxml_node_t *xmlWriteXml;   
//mxml_node_t *rootWriteXml;  
//mxml_node_t *testWriteXml;  
//mxml_node_t *dataWriteXml;
//int cntWriteXml = 0;
//
//CK_RV WriteXmlOpen(int CNT)
//{
//
//	const char* pcLogFile = "/sdcard/XMLFile/Result.xml";
//	cntWriteXml = 0;
//	fpWriteXml = fopen(pcLogFile, "w");
//	xmlWriteXml = mxmlNewXML("1.0");
//
//	treeWriteXml= xmlWriteXml;
//	rootWriteXml = mxmlNewElement(xmlWriteXml, "root");
//
//	return TRUE;
//}
//CK_RV WriteXmlStep(int CNT)
//{
//
//
//
//
//
//	i_char = 0x00;
//	i_char = (char)(i+1)+0x30;
//	memcpy(tmp,&i_char,1);
//	testWriteXml = mxmlNewElement(rootWriteXml, "Test");
//	mxmlElementSetAttr(testWriteXml,"id",tmp);
//	dataWriteXml = mxmlNewElement(testWriteXml, "Testname");
//	sprintf(temp, "%d ",Testchoose[i]);
//	mxmlNewText(dataWriteXml, 0, temp);
//
//	dataWriteXml = mxmlNewElement(testWriteXml, "Result");
//	sprintf(temp, "%d ",Test[Testchoose[i]-101]);
//	mxmlNewText(dataWriteXml, 0, temp);
//
//	dataWriteXml = mxmlNewElement(testWriteXml, "info");
//	mxmlNewText(dataWriteXml, 0, Testresult[nItemNumb]);
//
//
//	return TRUE;
//}
//
//CK_RV WriteXmlClose(int CNT)
//{
//	int i = 0;
//	FILE *fp;
//	mxml_node_t *tree;
//	mxml_node_t *xml;   
//	mxml_node_t *root;  
//	mxml_node_t *test;  
//	mxml_node_t *data;
//
//	char i_char = 0x00;
//	char temp[5];
//	char tmp[2]={0};
//
//	const char* pcLogFile = "/sdcard/XMLFile/Result.xml";
//
//	fp = fopen(pcLogFile, "w");
//	xml = mxmlNewXML("1.0");
//
//	tree= xml;
//	root = mxmlNewElement(xml, "root");
//
//	for(i = 0; i < CNT; ++i)
//	{
//		i_char = 0x00;
//		i_char = (char)(i+1)+0x30;
//		memcpy(tmp,&i_char,1);
//		test = mxmlNewElement(root, "Test");
//		mxmlElementSetAttr(test,"id",tmp);
//		data = mxmlNewElement(test, "Testname");
//		sprintf(temp, "%d ",Testchoose[i]);
//		mxmlNewText(data, 0, temp);
//
//		data = mxmlNewElement(test, "Result");
//		sprintf(temp, "%d ",Test[Testchoose[i]-101]);
//		mxmlNewText(data, 0, temp);
//
//		data = mxmlNewElement(test, "info");
//		mxmlNewText(data, 0, Testresult[nItemNumb]);
//	}
//
//	mxmlSaveFile(tree, fp, MXML_NO_CALLBACK);
//	fclose(fp);
//	return TRUE;
//}