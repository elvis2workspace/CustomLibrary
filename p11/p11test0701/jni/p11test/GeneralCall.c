#include <stdio.h>
#include <stdarg.h>
#include "general.h"
#include "GeneralCall.h"

#ifdef __ANDROID__
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#endif

int Func_Num = 0;
void* StackMemory[10][9] = {0};
F_pointer FunctionList[10] = {0};
char* FunctionNameList[10] = {0};

//unsigned int generalcall(F f, char* f_name, unsigned int nullpos, ...) //nullpos is position of null parameter, begins with 1, if it is 0, no null parameter in the f function.
unsigned int generalcall(F_pointer f, unsigned int nullpos, ...) //nullpos is position of null parameter, begins with 1, if it is 0, no null parameter in the f function.
{
	va_list arg_ptr;
	void* p[12] = { 0 };
	int i = 0;
	unsigned int rv = 0;
	va_start(arg_ptr, nullpos);
	
	for (i = 0; i < 12; ++i)
	{
		p[i] = va_arg(arg_ptr, void*);
	}

	if (nullpos != 0)
	{
		p[nullpos - 1] = 0;
	}
	rv = f(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8],p[9],p[10],p[11]);
	va_end(arg_ptr);	
	return rv;
}




CK_RV functionpush(F_pointer func_push, char* func_name,...) //nullpos is position of null parameter, begins with 1, if it is 0, no null parameter in the f function.
{
	va_list arg_ptr;
	int i = 0;
	unsigned int rv = 0;

	if(Func_Num < 0)
	{
		printf("Error:Func_Num less than 0.\n");
		return FALSE;
	}

	if(Func_Num == 10)
	{
		printf("Error:StackMemory is full.\n");
		return FALSE;
	}

	va_start(arg_ptr, func_name);

	FunctionList[Func_Num] = func_push;
	FunctionNameList[Func_Num] = func_name;
	
	for (i = 0; i < 9; ++i)
	{
		StackMemory[Func_Num][i] = va_arg(arg_ptr, void*);		
	}

	va_end(arg_ptr);
	++Func_Num;	
	return TRUE;
}


CK_RV functionpop(int pop_num) 
{
	unsigned int rv = 0;
	int func_addr = 0;

	if(Func_Num < 0)
	{
		printf("Error:Func_Num less than 0.\n");
		return FALSE;
	}

	if(Func_Num == 0)
	{
		printf("Error:Func_Num is 0, no func in stack.\n");
		return FALSE;
	}

	if(pop_num < 0)
	{
		printf("Error:pop_num less than 0.\n");
		return FALSE;
	}
	else if(pop_num == 0)
	{
		do 
		{
			func_addr = Func_Num - 1;
			rv = generalcall(FunctionList[func_addr], 0, StackMemory[func_addr][0], StackMemory[func_addr][1],
				StackMemory[func_addr][2],StackMemory[func_addr][3],StackMemory[func_addr][4],
				StackMemory[func_addr][5],StackMemory[func_addr][6],StackMemory[func_addr][7],
				StackMemory[func_addr][8]);
			--Func_Num;
		} while (Func_Num > 0);
	}
	else
	{
		do 
		{
			func_addr = Func_Num - 1;
			rv = generalcall(FunctionList[func_addr], 0, StackMemory[func_addr][0], StackMemory[func_addr][1],
				StackMemory[func_addr][2],StackMemory[func_addr][3],StackMemory[func_addr][4],
				StackMemory[func_addr][5],StackMemory[func_addr][6],StackMemory[func_addr][7],
				StackMemory[func_addr][8]);	
			--Func_Num;
			--pop_num;			
		} while (pop_num > 0);	
	}
	return TRUE;
}



//
//int mainxx()
//{
//	F_pointer f[] = { (F_pointer)f0, (F_pointer)f1, (F_pointer)f2, (F_pointer)f3, (F_pointer)f1_1, (F_pointer)f2_1, (F_pointer)f3_1 };
//	int i = 0, j = 0;
//	for (i = 0; i < sizeof(f) / sizeof(f[0]); ++i)
//	{
//		generalcall(f[i], 0, 4, 7, 8);
//	}
//
//	for (i = 4; i < 7; ++i)
//	{
//		for (j = 1; j < i - 2; ++j)
//		{
//			generalcall(f[i], j, 1, 2, 3, 4, 5, 6, 7, 8);
//		}
//	}
//
//	getchar();
//	return 0;
//}

