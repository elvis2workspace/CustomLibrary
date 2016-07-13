
#ifndef AUTH_H_
#define AUTH_H_
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*认证*/
CK_RV test_login(CK_USER_TYPE userType);
/*注销认证*/
CK_RV test_logout();

#ifdef __cplusplus
}
#endif

#endif