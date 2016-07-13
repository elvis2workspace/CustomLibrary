#ifndef GENERAL_CALL_H_
#define GENERAL_CALL_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int (*F_pointer)();

//extern int Func_Num;
//extern void* StackMemory[10][9];
//extern F_pointer FunctionList[10];
//extern char* FunctionNameList[10];

unsigned int generalcall(F_pointer f, unsigned int nullpos, ...);
CK_RV functionpush(F_pointer func_push, char* func_name,...);
CK_RV functionpop(int pop_num);

#ifdef __cplusplus
}
#endif

#endif