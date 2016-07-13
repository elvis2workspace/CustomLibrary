#ifndef ERRORTEST_H_
#define ERRORTEST_H_

#ifdef __cplusplus
extern "C" {
#endif


CK_RV etest_ZUCEncrypt();
CK_RV etest_ZUCDecrypt();
CK_RV etest_SM2GenEnDe();
CK_RV etest_SM2ImpSiVr();
CK_RV etest_SM3Digest();
CK_RV etest_SM4();
CK_RV test_TT();
CK_RV free_memory_test();


unsigned short do_crc(unsigned short reg_init, unsigned char *message, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
