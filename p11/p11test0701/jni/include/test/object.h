
#ifndef OBJECT_H_
#define OBJECT_H_



#ifdef __cplusplus
extern "C" {
#endif


/*删除对象*/
bool test_deleteobject(CK_OBJECT_HANDLE hobj);
/*对象(创建、查找、删除)*/
bool test_objects();

void test_object_attribute();

void test_object_attribute_read();

bool test_data_objects();



CK_RV test_object_westone();

/*测试最多可支持多少对象*/
bool test_object_max_count();

/*测试导入导出证书对象*/
bool test_object_cert();

bool test_object_cert_d_c_r_d();
bool test_object_data_d_c_r_d();
/*测试对称密钥SM4对象*/
bool test_Flash_SymmetricKey();

#ifdef __cplusplus
}
#endif

#endif