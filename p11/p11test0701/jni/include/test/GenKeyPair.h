#ifndef  GENKEYPAIR_H_
#define  GENKEYPAIR_H_

#ifdef __cplusplus
extern "C" {
#endif

#if 0
void Test_GenKeyPair();
#else
/*产生ECC密钥对*/
bool test_ECC_GenKeyPair(CK_OBJECT_HANDLE *phPublicKey,CK_OBJECT_HANDLE *phPrivateKey);
/*导入ECC密钥对*/
bool test_ECC_importKeyPair(CK_OBJECT_HANDLE *phPublicKey,CK_OBJECT_HANDLE *phPrivateKey);
/*产生RSA密钥对*/
bool test_RSA_GenKeyPair(CK_OBJECT_HANDLE *phPublicKey,CK_OBJECT_HANDLE *phPrivateKey);
/*导入RSA密钥对*/
bool test_RSA_importKeyPair(CK_OBJECT_HANDLE *phPublicKey,CK_OBJECT_HANDLE *phPrivateKey);
#endif


#ifdef __cplusplus
}
#endif

#endif
