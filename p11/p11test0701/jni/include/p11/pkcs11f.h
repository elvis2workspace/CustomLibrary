/* pkcs11f.h include file for PKCS #11. */
/* $Revision: 1.4 $ */

/* License to copy and use this software is granted provided that it is
 * identified as "RSA Security Inc. PKCS #11 Cryptographic Token Interface
 * (Cryptoki)" in all material mentioning or referencing this software.

 * License is also granted to make and use derivative works provided that
 * such works are identified as "derived from the RSA Security Inc. PKCS #11
 * Cryptographic Token Interface (Cryptoki)" in all material mentioning or 
 * referencing the derived work.

 * RSA Security Inc. makes no representations concerning either the 
 * merchantability of this software or the suitability of this software for
 * any particular purpose. It is provided "as is" without express or implied
 * warranty of any kind.
 */

/* This header file contains pretty much everything about all the */
/* Cryptoki function prototypes.  Because this information is */
/* used for more than just declaring function prototypes, the */
/* order of the functions appearing herein is important, and */
/* should not be altered. */

/* General-purpose */

/* C_Initialize initializes the Cryptoki library. */
CK_PKCS11_FUNCTION_INFO(C_Initialize)
#ifdef CK_NEED_ARG_LIST
(
  CK_VOID_PTR   pInitArgs  /* if this is not NULL_PTR, it gets
                            * cast to CK_C_INITIALIZE_ARGS_PTR
                            * and dereferenced */
);
#endif


/* C_Finalize indicates that an application is done with the
 * Cryptoki library. */
CK_PKCS11_FUNCTION_INFO(C_Finalize)
#ifdef CK_NEED_ARG_LIST
(
  CK_VOID_PTR   pReserved  /* reserved.  Should be NULL_PTR */
);
#endif


/* C_GetInfo returns general information about Cryptoki. */
CK_PKCS11_FUNCTION_INFO(C_GetInfo)
#ifdef CK_NEED_ARG_LIST
(
  CK_INFO_PTR   pInfo  /* location that receives information */
);
#endif


/* C_GetFunctionList returns the function list. */
CK_PKCS11_FUNCTION_INFO(C_GetFunctionList)
#ifdef CK_NEED_ARG_LIST
(
  CK_FUNCTION_LIST_PTR_PTR ppFunctionList  /* receives pointer to
                                            * function list */
);
#endif



/* Slot and token management */

/* C_GetSlotList obtains a list of slots in the system. */
CK_PKCS11_FUNCTION_INFO(C_GetSlotList)
#ifdef CK_NEED_ARG_LIST
(
  CK_BBOOL       tokenPresent,  /* only slots with tokens? */
  CK_SLOT_ID_PTR pSlotList,     /* receives array of slot IDs */
  CK_ULONG_PTR   pulCount       /* receives number of slots */
);
#endif


/* C_GetSlotInfo obtains information about a particular slot in
 * the system. */
CK_PKCS11_FUNCTION_INFO(C_GetSlotInfo)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID       slotID,  /* the ID of the slot */
  CK_SLOT_INFO_PTR pInfo    /* receives the slot information */
);
#endif


/* C_GetTokenInfo obtains information about a particular token
 * in the system. */
CK_PKCS11_FUNCTION_INFO(C_GetTokenInfo)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID        slotID,  /* ID of the token's slot */
  CK_TOKEN_INFO_PTR pInfo    /* receives the token information */
);
#endif


/* C_GetMechanismList obtains a list of mechanism types
 * supported by a token. */
CK_PKCS11_FUNCTION_INFO(C_GetMechanismList)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID            slotID,          /* ID of token's slot */
  CK_MECHANISM_TYPE_PTR pMechanismList,  /* gets mech. array */
  CK_ULONG_PTR          pulCount         /* gets # of mechs. */
);
#endif


/* C_GetMechanismInfo obtains information about a particular
 * mechanism possibly supported by a token. */
CK_PKCS11_FUNCTION_INFO(C_GetMechanismInfo)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID            slotID,  /* ID of the token's slot */
  CK_MECHANISM_TYPE     type,    /* type of mechanism */
  CK_MECHANISM_INFO_PTR pInfo    /* receives mechanism info */
);
#endif


/* C_InitToken initializes a token. */
CK_PKCS11_FUNCTION_INFO(C_InitToken)
#ifdef CK_NEED_ARG_LIST
/* pLabel changed from CK_CHAR_PTR to CK_UTF8CHAR_PTR for v2.10 */
(
  CK_SLOT_ID      slotID,    /* ID of the token's slot */
  CK_UTF8CHAR_PTR pPin,      /* the SO's initial PIN */
  CK_ULONG        ulPinLen,  /* nResultLength in bytes of the PIN */
  CK_UTF8CHAR_PTR pLabel     /* 32-byte token label (blank padded) */
);
#endif


/* C_InitPIN initializes the normal user's PIN. */
CK_PKCS11_FUNCTION_INFO(C_InitPIN)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,  /* the session's handle */
  CK_UTF8CHAR_PTR   pPin,      /* the normal user's PIN */
  CK_ULONG          ulPinLen   /* nResultLength in bytes of the PIN */
);
#endif


/* C_SetPIN modifies the PIN of the user who is logged in. */
CK_PKCS11_FUNCTION_INFO(C_SetPIN)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,  /* the session's handle */
  CK_UTF8CHAR_PTR   pOldPin,   /* the old PIN */
  CK_ULONG          ulOldLen,  /* nResultLength of the old PIN */
  CK_UTF8CHAR_PTR   pNewPin,   /* the new PIN */
  CK_ULONG          ulNewLen   /* nResultLength of the new PIN */
);
#endif



/* Session management */

/* C_OpenSession opens a session between an application and a
 * token. */
CK_PKCS11_FUNCTION_INFO(C_OpenSession)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID            slotID,        /* the slot's ID */
  CK_FLAGS              flags,         /* from CK_SESSION_INFO */
  CK_VOID_PTR           pApplication,  /* passed to callback */
  CK_NOTIFY             Notify,        /* callback function */
  CK_SESSION_HANDLE_PTR phSession      /* gets session handle */
);
#endif


/* C_CloseSession closes a session between an application and a
 * token. */
CK_PKCS11_FUNCTION_INFO(C_CloseSession)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession  /* the session's handle */
);
#endif


/* C_CloseAllSessions closes all sessions with a token. */
CK_PKCS11_FUNCTION_INFO(C_CloseAllSessions)
#ifdef CK_NEED_ARG_LIST
(
  CK_SLOT_ID     slotID  /* the token's slot */
);
#endif


/* C_GetSessionInfo obtains information about the session. */
CK_PKCS11_FUNCTION_INFO(C_GetSessionInfo)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE   hSession,  /* the session's handle */
  CK_SESSION_INFO_PTR pInfo      /* receives session info */
);
#endif


/* C_GetOperationState obtains the state of the cryptographic operation
 * in a session. */
CK_PKCS11_FUNCTION_INFO(C_GetOperationState)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,             /* session's handle */
  CK_BYTE_PTR       pOperationState,      /* gets state */
  CK_ULONG_PTR      pulOperationStateLen  /* gets state nResultLength */
);
#endif


/* C_SetOperationState restores the state of the cryptographic
 * operation in a session. */
CK_PKCS11_FUNCTION_INFO(C_SetOperationState)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,            /* session's handle */
  CK_BYTE_PTR      pOperationState,      /* holds state */
  CK_ULONG         ulOperationStateLen,  /* holds state nResultLength */
  CK_OBJECT_HANDLE hEncryptionKey,       /* en/decryption key */
  CK_OBJECT_HANDLE hAuthenticationKey    /* sign/verify key */
);
#endif


/* C_Login logs a user into a token. */
CK_PKCS11_FUNCTION_INFO(C_Login)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,  /* the session's handle */
  CK_USER_TYPE      userType,  /* the user type */
  CK_UTF8CHAR_PTR   pPin,      /* the user's PIN */
  CK_ULONG          ulPinLen   /* the nResultLength of the PIN */
);
#endif


/* C_Logout logs a user out from a token. */
CK_PKCS11_FUNCTION_INFO(C_Logout)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession  /* the session's handle */
);
#endif



/* Object management */

/* C_CreateObject creates a new object. */
CK_PKCS11_FUNCTION_INFO(C_CreateObject)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,    /* the session's handle */
  CK_ATTRIBUTE_PTR  pTemplate,   /* the object's template */
  CK_ULONG          ulCount,     /* attributes in template */
  CK_OBJECT_HANDLE_PTR phObject  /* gets new object's handle. */
);
#endif


/* C_CopyObject copies an object, creating a new object for the
 * copy. */
CK_PKCS11_FUNCTION_INFO(C_CopyObject)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,    /* the session's handle */
  CK_OBJECT_HANDLE     hObject,     /* the object's handle */
  CK_ATTRIBUTE_PTR     pTemplate,   /* template for new object */
  CK_ULONG             ulCount,     /* attributes in template */
  CK_OBJECT_HANDLE_PTR phNewObject  /* receives handle of copy */
);
#endif


/* C_DestroyObject destroys an object. */
CK_PKCS11_FUNCTION_INFO(C_DestroyObject)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,  /* the session's handle */
  CK_OBJECT_HANDLE  hObject    /* the object's handle */
);
#endif


/* C_GetObjectSize gets the size of an object in bytes. */
CK_PKCS11_FUNCTION_INFO(C_GetObjectSize)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,  /* the session's handle */
  CK_OBJECT_HANDLE  hObject,   /* the object's handle */
  CK_ULONG_PTR      pulSize    /* receives size of object */
);
#endif


/* C_GetAttributeValue obtains the value of one or more object
 * attributes. */
CK_PKCS11_FUNCTION_INFO(C_GetAttributeValue)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,   /* the session's handle */
  CK_OBJECT_HANDLE  hObject,    /* the object's handle */
  CK_ATTRIBUTE_PTR  pTemplate,  /* specifies attrs; gets vals */
  CK_ULONG          ulCount     /* attributes in template */
);
#endif


/* C_SetAttributeValue modifies the value of one or more object
 * attributes */
CK_PKCS11_FUNCTION_INFO(C_SetAttributeValue)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,   /* the session's handle */
  CK_OBJECT_HANDLE  hObject,    /* the object's handle */
  CK_ATTRIBUTE_PTR  pTemplate,  /* specifies attrs and values */
  CK_ULONG          ulCount     /* attributes in template */
);
#endif


/* C_FindObjectsInit initializes a search for token and session
 * objects that match a template. */
CK_PKCS11_FUNCTION_INFO(C_FindObjectsInit)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,   /* the session's handle */
  CK_ATTRIBUTE_PTR  pTemplate,  /* attribute values to match */
  CK_ULONG          ulCount     /* attrs in search template */
);
#endif


/* C_FindObjects continues a search for token and session
 * objects that match a template, obtaining additional object
 * handles. */
CK_PKCS11_FUNCTION_INFO(C_FindObjects)
#ifdef CK_NEED_ARG_LIST
(
 CK_SESSION_HANDLE    hSession,          /* session's handle */
 CK_OBJECT_HANDLE_PTR phObject,          /* gets obj. handles */
 CK_ULONG             ulMaxObjectCount,  /* max handles to get */
 CK_ULONG_PTR         pulObjectCount     /* actual # returned */
);
#endif


/* C_FindObjectsFinal finishes a search for token and session
 * objects. */
CK_PKCS11_FUNCTION_INFO(C_FindObjectsFinal)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession  /* the session's handle */
);
#endif



/* Encryption and decryption */

/* C_EncryptInit initializes an encryption operation. */
CK_PKCS11_FUNCTION_INFO(C_EncryptInit)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,    /* the session's handle */
  CK_MECHANISM_PTR  pMechanism,  /* the encryption mechanism */
  CK_OBJECT_HANDLE  hKey         /* handle of encryption key */
);
#endif


/* C_Encrypt encrypts single-part data. */
CK_PKCS11_FUNCTION_INFO(C_Encrypt)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,            /* session's handle */
  CK_BYTE_PTR       pData,               /* the plaintext data */
  CK_ULONG          ulDataLen,           /* bytes of plaintext */
  CK_BYTE_PTR       pEncryptedData,      /* gets ciphertext */
  CK_ULONG_PTR      pulEncryptedDataLen  /* gets c-text size */
);
#endif


/* C_EncryptUpdate continues a multiple-part encryption
 * operation. */
CK_PKCS11_FUNCTION_INFO(C_EncryptUpdate)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,           /* session's handle */
  CK_BYTE_PTR       pPart,              /* the plaintext data */
  CK_ULONG          ulPartLen,          /* plaintext data len */
  CK_BYTE_PTR       pEncryptedPart,     /* gets ciphertext */
  CK_ULONG_PTR      pulEncryptedPartLen /* gets c-text size */
);
#endif


/* C_EncryptFinal finishes a multiple-part encryption
 * operation. */
CK_PKCS11_FUNCTION_INFO(C_EncryptFinal)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,                /* session handle */
  CK_BYTE_PTR       pLastEncryptedPart,      /* last c-text */
  CK_ULONG_PTR      pulLastEncryptedPartLen  /* gets last size */
);
#endif


/* C_DecryptInit initializes a decryption operation. */
CK_PKCS11_FUNCTION_INFO(C_DecryptInit)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,    /* the session's handle */
  CK_MECHANISM_PTR  pMechanism,  /* the decryption mechanism */
  CK_OBJECT_HANDLE  hKey         /* handle of decryption key */
);
#endif


/* C_Decrypt decrypts encrypted data in a single part. */
CK_PKCS11_FUNCTION_INFO(C_Decrypt)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,           /* session's handle */
  CK_BYTE_PTR       pEncryptedData,     /* ciphertext */
  CK_ULONG          ulEncryptedDataLen, /* ciphertext nResultLength */
  CK_BYTE_PTR       pData,              /* gets plaintext */
  CK_ULONG_PTR      pulDataLen          /* gets p-text size */
);
#endif


/* C_DecryptUpdate continues a multiple-part decryption
 * operation. */
CK_PKCS11_FUNCTION_INFO(C_DecryptUpdate)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,            /* session's handle */
  CK_BYTE_PTR       pEncryptedPart,      /* encrypted data */
  CK_ULONG          ulEncryptedPartLen,  /* input nResultLength */
  CK_BYTE_PTR       pPart,               /* gets plaintext */
  CK_ULONG_PTR      pulPartLen           /* p-text size */
);
#endif


/* C_DecryptFinal finishes a multiple-part decryption
 * operation. */
CK_PKCS11_FUNCTION_INFO(C_DecryptFinal)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,       /* the session's handle */
  CK_BYTE_PTR       pLastPart,      /* gets plaintext */
  CK_ULONG_PTR      pulLastPartLen  /* p-text size */
);
#endif



/* Message digesting */

/* C_DigestInit initializes a message-digesting operation. */
CK_PKCS11_FUNCTION_INFO(C_DigestInit)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,   /* the session's handle */
  CK_MECHANISM_PTR  pMechanism  /* the digesting mechanism */
);
#endif


/* C_Digest digests data in a single part. */
CK_PKCS11_FUNCTION_INFO(C_Digest)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,     /* the session's handle */
  CK_BYTE_PTR       pData,        /* data to be digested */
  CK_ULONG          ulDataLen,    /* bytes of data to digest */
  CK_BYTE_PTR       pDigest,      /* gets the message digest */
  CK_ULONG_PTR      pulDigestLen  /* gets digest nResultLength */
);
#endif


/* C_DigestUpdate continues a multiple-part message-digesting
 * operation. */
CK_PKCS11_FUNCTION_INFO(C_DigestUpdate)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,  /* the session's handle */
  CK_BYTE_PTR       pPart,     /* data to be digested */
  CK_ULONG          ulPartLen  /* bytes of data to be digested */
);
#endif


/* C_DigestKey continues a multi-part message-digesting
 * operation, by digesting the value of a secret key as part of
 * the data already digested. */
CK_PKCS11_FUNCTION_INFO(C_DigestKey)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,  /* the session's handle */
  CK_OBJECT_HANDLE  hKey       /* secret key to digest */
);
#endif


/* C_DigestFinal finishes a multiple-part message-digesting
 * operation. */
CK_PKCS11_FUNCTION_INFO(C_DigestFinal)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,     /* the session's handle */
  CK_BYTE_PTR       pDigest,      /* gets the message digest */
  CK_ULONG_PTR      pulDigestLen  /* gets byte count of digest */
);
#endif



/* Signing and MACing */

/* C_SignInit initializes a signature (private key encryption)
 * operation, where the signature is (will be) an appendix to
 * the data, and plaintext cannot be recovered from the
 *signature. */
CK_PKCS11_FUNCTION_INFO(C_SignInit)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,    /* the session's handle */
  CK_MECHANISM_PTR  pMechanism,  /* the signature mechanism */
  CK_OBJECT_HANDLE  hKey         /* handle of signature key */
);
#endif


/* C_Sign signs (encrypts with private key) data in a single
 * part, where the signature is (will be) an appendix to the
 * data, and plaintext cannot be recovered from the signature. */
CK_PKCS11_FUNCTION_INFO(C_Sign)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,        /* the session's handle */
  CK_BYTE_PTR       pData,           /* the data to sign */
  CK_ULONG          ulDataLen,       /* count of bytes to sign */
  CK_BYTE_PTR       pSignature,      /* gets the signature */
  CK_ULONG_PTR      pulSignatureLen  /* gets signature nResultLength */
);
#endif


/* C_SignUpdate continues a multiple-part signature operation,
 * where the signature is (will be) an appendix to the data, 
 * and plaintext cannot be recovered from the signature. */
CK_PKCS11_FUNCTION_INFO(C_SignUpdate)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,  /* the session's handle */
  CK_BYTE_PTR       pPart,     /* the data to sign */
  CK_ULONG          ulPartLen  /* count of bytes to sign */
);
#endif


/* C_SignFinal finishes a multiple-part signature operation, 
 * returning the signature. */
CK_PKCS11_FUNCTION_INFO(C_SignFinal)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,        /* the session's handle */
  CK_BYTE_PTR       pSignature,      /* gets the signature */
  CK_ULONG_PTR      pulSignatureLen  /* gets signature nResultLength */
);
#endif


/* C_SignRecoverInit initializes a signature operation, where
 * the data can be recovered from the signature. */
CK_PKCS11_FUNCTION_INFO(C_SignRecoverInit)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,   /* the session's handle */
  CK_MECHANISM_PTR  pMechanism, /* the signature mechanism */
  CK_OBJECT_HANDLE  hKey        /* handle of the signature key */
);
#endif


/* C_SignRecover signs data in a single operation, where the
 * data can be recovered from the signature. */
CK_PKCS11_FUNCTION_INFO(C_SignRecover)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,        /* the session's handle */
  CK_BYTE_PTR       pData,           /* the data to sign */
  CK_ULONG          ulDataLen,       /* count of bytes to sign */
  CK_BYTE_PTR       pSignature,      /* gets the signature */
  CK_ULONG_PTR      pulSignatureLen  /* gets signature nResultLength */
);
#endif



/* Verifying signatures and MACs */

/* C_VerifyInit initializes a verification operation, where the
 * signature is an appendix to the data, and plaintext cannot
 *  cannot be recovered from the signature (e.g. DSA). */
CK_PKCS11_FUNCTION_INFO(C_VerifyInit)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,    /* the session's handle */
  CK_MECHANISM_PTR  pMechanism,  /* the verification mechanism */
  CK_OBJECT_HANDLE  hKey         /* verification key */ 
);
#endif


/* C_Verify verifies a signature in a single-part operation, 
 * where the signature is an appendix to the data, and plaintext
 * cannot be recovered from the signature. */
CK_PKCS11_FUNCTION_INFO(C_Verify)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,       /* the session's handle */
  CK_BYTE_PTR       pData,          /* signed data */
  CK_ULONG          ulDataLen,      /* nResultLength of signed data */
  CK_BYTE_PTR       pSignature,     /* signature */
  CK_ULONG          ulSignatureLen  /* signature nResultLength*/
);
#endif


/* C_VerifyUpdate continues a multiple-part verification
 * operation, where the signature is an appendix to the data, 
 * and plaintext cannot be recovered from the signature. */
CK_PKCS11_FUNCTION_INFO(C_VerifyUpdate)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,  /* the session's handle */
  CK_BYTE_PTR       pPart,     /* signed data */
  CK_ULONG          ulPartLen  /* nResultLength of signed data */
);
#endif


/* C_VerifyFinal finishes a multiple-part verification
 * operation, checking the signature. */
CK_PKCS11_FUNCTION_INFO(C_VerifyFinal)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,       /* the session's handle */
  CK_BYTE_PTR       pSignature,     /* signature to verify */
  CK_ULONG          ulSignatureLen  /* signature nResultLength */
);
#endif


/* C_VerifyRecoverInit initializes a signature verification
 * operation, where the data is recovered from the signature. */
CK_PKCS11_FUNCTION_INFO(C_VerifyRecoverInit)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,    /* the session's handle */
  CK_MECHANISM_PTR  pMechanism,  /* the verification mechanism */
  CK_OBJECT_HANDLE  hKey         /* verification key */
);
#endif


/* C_VerifyRecover verifies a signature in a single-part
 * operation, where the data is recovered from the signature. */
CK_PKCS11_FUNCTION_INFO(C_VerifyRecover)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,        /* the session's handle */
  CK_BYTE_PTR       pSignature,      /* signature to verify */
  CK_ULONG          ulSignatureLen,  /* signature nResultLength */
  CK_BYTE_PTR       pData,           /* gets signed data */
  CK_ULONG_PTR      pulDataLen       /* gets signed data len */
);
#endif



/* Dual-function cryptographic operations */

/* C_DigestEncryptUpdate continues a multiple-part digesting
 * and encryption operation. */
CK_PKCS11_FUNCTION_INFO(C_DigestEncryptUpdate)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,            /* session's handle */
  CK_BYTE_PTR       pPart,               /* the plaintext data */
  CK_ULONG          ulPartLen,           /* plaintext nResultLength */
  CK_BYTE_PTR       pEncryptedPart,      /* gets ciphertext */
  CK_ULONG_PTR      pulEncryptedPartLen  /* gets c-text nResultLength */
);
#endif


/* C_DecryptDigestUpdate continues a multiple-part decryption and
 * digesting operation. */
CK_PKCS11_FUNCTION_INFO(C_DecryptDigestUpdate)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,            /* session's handle */
  CK_BYTE_PTR       pEncryptedPart,      /* ciphertext */
  CK_ULONG          ulEncryptedPartLen,  /* ciphertext nResultLength */
  CK_BYTE_PTR       pPart,               /* gets plaintext */
  CK_ULONG_PTR      pulPartLen           /* gets plaintext len */
);
#endif


/* C_SignEncryptUpdate continues a multiple-part signing and
 * encryption operation. */
CK_PKCS11_FUNCTION_INFO(C_SignEncryptUpdate)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,            /* session's handle */
  CK_BYTE_PTR       pPart,               /* the plaintext data */
  CK_ULONG          ulPartLen,           /* plaintext nResultLength */
  CK_BYTE_PTR       pEncryptedPart,      /* gets ciphertext */
  CK_ULONG_PTR      pulEncryptedPartLen  /* gets c-text nResultLength */
);
#endif


/* C_DecryptVerifyUpdate continues a multiple-part decryption and
 * verify operation. */
CK_PKCS11_FUNCTION_INFO(C_DecryptVerifyUpdate)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,            /* session's handle */
  CK_BYTE_PTR       pEncryptedPart,      /* ciphertext */
  CK_ULONG          ulEncryptedPartLen,  /* ciphertext nResultLength */
  CK_BYTE_PTR       pPart,               /* gets plaintext */
  CK_ULONG_PTR      pulPartLen           /* gets p-text nResultLength */
);
#endif



/* Key management */

/* C_GenerateKey generates a secret key, creating a new key
 * object. */
CK_PKCS11_FUNCTION_INFO(C_GenerateKey)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,    /* the session's handle */
  CK_MECHANISM_PTR     pMechanism,  /* key generation mech. */
  CK_ATTRIBUTE_PTR     pTemplate,   /* template for new key */
  CK_ULONG             ulCount,     /* # of attrs in template */
  CK_OBJECT_HANDLE_PTR phKey        /* gets handle of new key */
);
#endif


/* C_GenerateKeyPair generates a public-key/private-key pair, 
 * creating new key objects. */
CK_PKCS11_FUNCTION_INFO(C_GenerateKeyPair)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,                    /* session
                                                     * handle */
  CK_MECHANISM_PTR     pMechanism,                  /* key-gen
                                                     * mech. */
  CK_ATTRIBUTE_PTR     pPublicKeyTemplate,          /* template
                                                     * for pub.
                                                     * key */
  CK_ULONG             ulPublicKeyAttributeCount,   /* # pub.
                                                     * attrs. */
  CK_ATTRIBUTE_PTR     pPrivateKeyTemplate,         /* template
                                                     * for priv.
                                                     * key */
  CK_ULONG             ulPrivateKeyAttributeCount,  /* # priv.
                                                     * attrs. */
  CK_OBJECT_HANDLE_PTR phPublicKey,                 /* gets pub.
                                                     * key
                                                     * handle */
  CK_OBJECT_HANDLE_PTR phPrivateKey                 /* gets
                                                     * priv. key
                                                     * handle */
);
#endif


/* C_WrapKey wraps (i.e., encrypts) a key. */
CK_PKCS11_FUNCTION_INFO(C_WrapKey)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,        /* the session's handle */
  CK_MECHANISM_PTR  pMechanism,      /* the wrapping mechanism */
  CK_OBJECT_HANDLE  hWrappingKey,    /* wrapping key */
  CK_OBJECT_HANDLE  hKey,            /* key to be wrapped */
  CK_BYTE_PTR       pWrappedKey,     /* gets wrapped key */
  CK_ULONG_PTR      pulWrappedKeyLen /* gets wrapped key size */
);
#endif


/* C_UnwrapKey unwraps (decrypts) a wrapped key, creating a new
 * key object. */
CK_PKCS11_FUNCTION_INFO(C_UnwrapKey)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,          /* session's handle */
  CK_MECHANISM_PTR     pMechanism,        /* unwrapping mech. */
  CK_OBJECT_HANDLE     hUnwrappingKey,    /* unwrapping key */
  CK_BYTE_PTR          pWrappedKey,       /* the wrapped key */
  CK_ULONG             ulWrappedKeyLen,   /* wrapped key len */
  CK_ATTRIBUTE_PTR     pTemplate,         /* new key template */
  CK_ULONG             ulAttributeCount,  /* template nResultLength */
  CK_OBJECT_HANDLE_PTR phKey              /* gets new handle */
);
#endif


/* C_DeriveKey derives a key from a base key, creating a new key
 * object. */
CK_PKCS11_FUNCTION_INFO(C_DeriveKey)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE    hSession,          /* session's handle */
  CK_MECHANISM_PTR     pMechanism,        /* key deriv. mech. */
  CK_OBJECT_HANDLE     hBaseKey,          /* base key */
  CK_ATTRIBUTE_PTR     pTemplate,         /* new key template */
  CK_ULONG             ulAttributeCount,  /* template nResultLength */
  CK_OBJECT_HANDLE_PTR phKey              /* gets new handle */
);
#endif



/* Random number generation */

/* C_SeedRandom mixes additional seed material into the token's
 * random number generator. */
CK_PKCS11_FUNCTION_INFO(C_SeedRandom)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,  /* the session's handle */
  CK_BYTE_PTR       pSeed,     /* the seed material */
  CK_ULONG          ulSeedLen  /* nResultLength of seed material */
);
#endif


/* C_GenerateRandom generates random data. */
CK_PKCS11_FUNCTION_INFO(C_GenerateRandom)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession,    /* the session's handle */
  CK_BYTE_PTR       RandomData,  /* receives the random data */
  CK_ULONG          ulRandomLen  /* # of bytes to generate */
);
#endif



/* Parallel function management */

/* C_GetFunctionStatus is a legacy function; it obtains an
 * updated status of a function running in parallel with an
 * application. */
CK_PKCS11_FUNCTION_INFO(C_GetFunctionStatus)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession  /* the session's handle */
);
#endif


/* C_CancelFunction is a legacy function; it cancels a function
 * running in parallel. */
CK_PKCS11_FUNCTION_INFO(C_CancelFunction)
#ifdef CK_NEED_ARG_LIST
(
  CK_SESSION_HANDLE hSession  /* the session's handle */
);
#endif

/* Functions added in for Cryptoki Version 2.01 or later */

/* C_WaitForSlotEvent waits for a slot event (token insertion,
 * removal, etc.) to occur. */
CK_PKCS11_FUNCTION_INFO(C_WaitForSlotEvent)
#ifdef CK_NEED_ARG_LIST
(
  CK_FLAGS flags,        /* blocking/nonblocking flag */
  CK_SLOT_ID_PTR pSlot,  /* location that receives the slot ID */
  CK_VOID_PTR pRserved   /* reserved.  Should be NULL_PTR */
);
#endif

/* Functions added only for 30RayCom*/
/* C_CryptoExtend is a 30RayCom defined for its private usage.
 * 支持几种业务类型（在 pExtendIn->extendType 中标明）：
 * 1.CK_EXTEND_VERIFYPIN：验证PIN码
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_VERIFYPIN
 *   pExtendIn->pParameter：存储PIN码的缓冲区首地址
 *   pExtendIn->ulParameterLen：上述PIN码缓冲区中PIN码的有效长度
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_VERIFYPIN
 *   pExtendOut->pParameter：任意值，推荐为NULL
 *   pExtendOut->ulParameterLen：任意值，推荐为0
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：无意义
 *   pExtendOut->ulParameterLen：无意义
 *   返回值：
 *   若返回CKR_OK，则验证成功；其它结果均为验证失败
 *
 * 2.CK_EXTEND_GETSN：获取SN号
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_GETSN
 *   pExtendIn->pParameter：任意值，推荐为NULL
 *   pExtendIn->ulParameterLen：任意值，推荐为0
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_GETSN
 *   pExtendOut->pParameter：NULL或者存放输出SN号的缓冲区首地址
 *   pExtendOut->ulParameterLen：若pExtendOut->pParameter为NULL，则ulParameterLen可为任意值
 *                               若pExtendOut->pParameter为缓冲区首地址，则ulParameterLen为缓冲区的有效长度
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：若输入为NULL，则输出也为NULL。
 *   pExtendOut->ulParameterLen：若pExtendOut->pParameter为NULL，则ulParameterLen输出为输出SN号所需要的最小缓冲区长度
 *                               若pExtendOut->pParameter为缓冲区首地址，则ulParameterLen为缓冲区中有效的SN长度
 *   返回值：
 *   若返回CKR_OK，则获取成功；其它结果均为获取失败
 *
 * 3.CK_EXTEND_GETPINTIME：获取PIN码的重试次数
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_GETPINTIME
 *   pExtendIn->pParameter：任意值，推荐为NULL
 *   pExtendIn->ulParameterLen：任意值，推荐为0
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_GETPINTIME
 *   pExtendOut->pParameter：一个指向int类型变量的地址值
 *   pExtendOut->ulParameterLen：sizeof(int)
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：存储了PIN码重试次数的int变量的地址值
 *   pExtendOut->ulParameterLen：sizeof(int)
 *   返回值：
 *   若返回CKR_OK，则获取成功；其它结果均为获取失败
 *
 * 4.CK_EXTEND_GETSDSTATUS：获取SD卡状态
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_GETSDSTATUS
 *   pExtendIn->pParameter：任意值，推荐为NULL
 *   pExtendIn->ulParameterLen：任意值，推荐为0
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_GETSDSTATUS
 *   pExtendOut->pParameter：任意值，推荐为NULL
 *   pExtendOut->ulParameterLen：任意值，推荐为0
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：无意义
 *   pExtendOut->ulParameterLen：无意义
 *   返回值：
 *   若返回CKR_DEVICE_REMOVED，则SD卡无效；若返回CKR_PIN_LOCKED，则PIN死锁；若返回CKR_OK，则SD卡正常；其它结果均失败
 *
 * 5.CK_EXTEND_GETEXCHANGEPUBKEY：获取用于密钥交换的SM2公钥值
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_GETEXCHANGEPUBKEY
 *   pExtendIn->pParameter：任意值，推荐为NULL
 *   pExtendIn->ulParameterLen：任意值，推荐为0
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_GETEXCHANGEPUBKEY
 *   pExtendOut->pParameter：接收公钥值的缓冲区首地址，长度至少为64字节
 *   pExtendOut->ulParameterLen：上述公钥值缓冲区的长度
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：存储了公钥值的缓冲区首地址
 *   pExtendOut->ulParameterLen：上述公钥值缓冲区中公钥的有效长度
 *   返回值：
 *   若返回CKR_OK，则获取成功；其它结果均为失败
 *
 * 6.CK_EXTEND_GETEXCHANGESESSKEY：使用监听公钥，导出之前协商出的密钥
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_GETEXCHANGESESSKEY
 *   pExtendIn->pParameter：指向一个类型为CK_BYTE的变量，变量记录了需要导出的协商出的密钥的ID号
 *   pExtendIn->ulParameterLen：CK_BYTE的长度
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_GETEXCHANGESESSKEY
 *   pExtendOut->pParameter：接收密钥密文的缓冲区首地址，长度至少为128字节
 *   pExtendOut->ulParameterLen：上述密钥密文缓冲区的长度
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：存储了密钥密文的缓冲区首地址
 *   pExtendOut->ulParameterLen：上述密钥密文缓冲区中密钥密文的有效长度
 *   返回值：
 *   若返回CKR_OK，则导出成功；其它结果均为失败
 *
 * 7.CK_EXTEND_SETMONITORSM2PUBKEY：设置用于监听的SM2公钥值
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_SETMONITORSM2PUBKEY
 *   pExtendIn->pParameter：存储了公钥值、版本号和管理员PIN码的缓冲区首地址，前64字节为公钥值，后面的4字节为版本号，之后的字节存储PIN码
 *   pExtendIn->ulParameterLen：上述公钥值和版本号及管理员PIN码缓冲区的长度，为（64+4+PIN码长度）
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_SETMONITORSM2PUBKEY
 *   pExtendOut->pParameter：任意值，推荐为NULL
 *   pExtendOut->ulParameterLen：任意值，推荐为0
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：无意义
 *   pExtendOut->ulParameterLen：无意义
 *   返回值：
 *   若返回CKR_OK，则设置成功；其它结果均为失败
 *
 * 8.CK_EXTEND_SETBASEKEY：设置用于短信加密的基础密钥
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_SETBASEKEY
 *   pExtendIn->pParameter：存储了基础密钥值的缓冲区首地址，长度为16字节，要求Login后才可以导入
 *   pExtendIn->ulParameterLen：上述基础密钥值的长度，为16
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_SETBASEKEY
 *   pExtendOut->pParameter：任意值，推荐为NULL
 *   pExtendOut->ulParameterLen：任意值，推荐为0
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：无意义
 *   pExtendOut->ulParameterLen：无意义
 *   返回值：
 *   若返回CKR_OK，则设置成功；其它结果均为失败
 *
 * 9.CK_EXTEND_ENCRYPTBYSK：使用基础密钥进行一次一密的短信加密操作
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_ENCRYPTBYSK
 *   pExtendIn->pParameter：存储了明文值和IV的缓冲区首地址，前面的字节为明文值，最后的16字节存储IV
 *   pExtendIn->ulParameterLen：上述明文值和IV缓冲区的长度，为（明文长度+16），其中明文长度必须为16字节的整数倍
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_ENCRYPTBYSK
 *   pExtendOut->pParameter：接收密文和会话密钥密文的缓冲区首地址，长度至少为（明文长度+16）字节
 *   pExtendOut->ulParameterLen：接收密文和会话密钥密文的缓冲区长度，至少为（明文长度+16）
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：存储了密文和会话密钥密文的缓冲区首地址，前面的字节为密文值，最后16字节为加密后的会话密钥
 *   pExtendOut->ulParameterLen：存储了密文和会话密钥密文的缓冲区的有效长度
 *   返回值：
 *   若返回CKR_OK，则加密成功；其它结果均为失败
 *
 *10.CK_EXTEND_DECRYPTBYSK：使用基础密钥进行一次一密的短信加密操作
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_DECRYPTBYSK
 *   pExtendIn->pParameter：存储了密文值、会话密钥密文和IV的缓冲区首地址，前面的字节为明文值，之后的16字节为会话密钥的密文，最后的16字节存储IV
 *   pExtendIn->ulParameterLen：上述密文值、会话密钥密文和IV的缓冲区的长度，为（密文长度+16+16），其中密文长度必须为16字节的整数倍
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_DECRYPTBYSK
 *   pExtendOut->pParameter：接收明文的缓冲区首地址，长度至少和密文长度相等
 *   pExtendOut->ulParameterLen：接收明文的缓冲区长度，至少为密文长度
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：存储了明文的缓冲区首地址
 *   pExtendOut->ulParameterLen：存储了明文的缓冲区的有效长度
 *   返回值：
 *   若返回CKR_OK，则解密成功；其它结果均为失败
 *
 *11.CK_EXTEND_GETSK：使用监听公钥，导出之前加密短信的一次一密密钥SK
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_GETSK
 *   pExtendIn->pParameter：任意值，推荐为NULL
 *   pExtendIn->ulParameterLen：任意值，推荐为0
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_GETSK
 *   pExtendOut->pParameter：接收SK密文CSK的缓冲区首地址，长度至少为112字节
 *   pExtendOut->ulParameterLen：上述CSK缓冲区的长度，至少为112
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：存储了CSK的缓冲区首地址
 *   pExtendOut->ulParameterLen：上述CSK缓冲区的有效长度
 *   返回值：
 *   若返回CKR_OK，则导出成功；其它结果均为失败
 *
 *12.CK_EXTEND_SETSM3KDFBASEKEY：设置衍生算法基础密钥
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_SETSM3KDFBASEKEY
 *   pExtendIn->pParameter：存储了基础密钥值的缓冲区首地址，长度为16字节，必须在Login之后才可以导入
 *   pExtendIn->ulParameterLen：上述基础密钥值缓冲区的长度，为16
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_SETSM3KDFBASEKEY
 *   pExtendOut->pParameter：任意值，推荐为NULL
 *   pExtendOut->ulParameterLen：任意值，推荐为0
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：无意义
 *   pExtendOut->ulParameterLen：无意义
 *   返回值：
 *   若返回CKR_OK，则设置成功；其它结果均为失败
 *
 *13.CK_EXTEND_GETLOGINSTATE：获取登录状态
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_GETLOGINSTATE
 *   pExtendIn->pParameter：任意值，推荐为NULL
 *   pExtendIn->ulParameterLen：任意值，推荐为0
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_GETLOGINSTATE
 *   pExtendOut->pParameter：将要存储登录状态的int变量的地址值
 *   pExtendOut->ulParameterLen：sizeof(int)
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：存储了登录状态的int变量的地址值，0为未登录，1为已登录
 *   pExtendOut->ulParameterLen：sizeof(int)
 *   返回值：
 *   若返回CKR_OK，则获取成功；其它结果均为失败
 *
 *14.CK_EXTEND_SETDESTROYKEY：注入毁卡随机数
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_SETDESTROYKEY
 *   pExtendIn->pParameter：存储了毁卡随机数和管理员PIN码的缓冲区首地址，长度为64字节，前32字节是上行随机数，接下来32字节是下行随机数，最后的不定长字节（至少为PIN码的最小长度）为管理员PIN码。
 *   pExtendIn->ulParameterLen：上述毁卡随机数和管理员PIN码缓冲区的长度，为64+管理员PIN码长度
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_SETDESTROYKEY
 *   pExtendOut->pParameter：任意值，推荐为NULL
 *   pExtendOut->ulParameterLen：任意值，推荐为0
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：无意义
 *   pExtendOut->ulParameterLen：无意义
 *   返回值：
 *   若返回CKR_OK，则设置成功；其它结果均为失败
 *
 *15.CK_EXTEND_DODESTROY：毁卡
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_DODESTROY
 *   pExtendIn->pParameter：存储了上行毁卡随机数的缓冲区首地址，长度为32字节
 *   pExtendIn->ulParameterLen：上行毁卡随机数缓冲区的长度，为32
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_DODESTROY
 *   pExtendOut->pParameter：空白缓冲区，用于接收毁卡成功后的下行毁卡随机数，长度至少为32字节
 *   pExtendOut->ulParameterLen：用于接收下行毁卡随机数的缓冲区长度，至少为32
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：毁卡成功后，此处存储的是下行毁卡随机数，有效长度为32字节
 *   pExtendOut->ulParameterLen：毁卡成功后，此处存储的是下行毁卡随机数的长度，为32
 *   返回值：
 *   若返回CKR_OK，则毁卡成功；其它结果均为失败
 *
 *16.CK_EXTEND_RESET_USERPIN：重设用户口令
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_RESET_USERPIN
 *   pExtendIn->pParameter：存储了OTPPIN和新的用户PIN的缓冲区首地址，第一个字节为OTPPIN的长度，后面为该长度的OTPPIN内容，之后一个字节为用户PIN的长度，后面为该长度的用户PIN的内容
 *   pExtendIn->ulParameterLen：1 + OTPPIN的长度 + 1 + 用户PIN的长度
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_RESET_USERPIN
 *   pExtendOut->pParameter：任意值，推荐为NULL
 *   pExtendOut->ulParameterLen：任意值，推荐为0
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：无意义
 *   pExtendOut->ulParameterLen：无意义
 *   返回值：
 *   若返回CKR_OK，则重设成功；其它结果均为失败
 *
 *17.CK_EXTEND_RESET_OTPPIN：重设OTPPIN
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_RESET_OTPPIN
 *   pExtendIn->pParameter：存储了管理员PIN码和新的OTPPIN的缓冲区首地址，第一个字节为管理员PIN的长度，后面为该长度的管理员PIN的内容，之后一个字节为新的OTPPIN的长度，后面为该长度的OTPPIN的内容
 *   pExtendIn->ulParameterLen：1 + 管理员PIN码的长度 + 1 + OTPPIN的长度
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_RESET_OTPPIN
 *   pExtendOut->pParameter：任意值，推荐为NULL
 *   pExtendOut->ulParameterLen：任意值，推荐为0
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：无意义
 *   pExtendOut->ulParameterLen：无意义
 *   返回值：
 *   若返回CKR_OK，则重设成功；其它结果均为失败
 *
 *18.CK_EXTEND_GETOTPTIME_USABLE：获取OTPPIN的剩余可用次数
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_GETOTPTIME_USABLE
 *   pExtendIn->pParameter：任意值，推荐为NULL
 *   pExtendIn->ulParameterLen：任意值，推荐为0
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_GETOTPTIME_USABLE
 *   pExtendOut->pParameter：空白缓冲区，用于接收OTPPIN的剩余可用次数，长度为1字节
 *   pExtendOut->ulParameterLen：值为1
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：获取成功后，此处存储的是OTPPIN的剩余可用次数，有效长度为1字节
 *   pExtendOut->ulParameterLen：值为1
 *   返回值：
 *   若返回CKR_OK，则获取成功；其它结果均为失败
 *
 *19.CK_EXTEND_GETOTPTIME_TRY：获取OTPPIN的剩余可出错次数
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_GETOTPTIME_TRY
 *   pExtendIn->pParameter：任意值，推荐为NULL
 *   pExtendIn->ulParameterLen：任意值，推荐为0
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_GETOTPTIME_TRY
 *   pExtendOut->pParameter：空白缓冲区，用于接收OTPPIN的剩余可出错次数，长度为1字节
 *   pExtendOut->ulParameterLen：值为1
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：获取成功后，此处存储的是OTPPIN的剩余可出错次数，有效长度为1字节
 *   pExtendOut->ulParameterLen：值为1
 *   返回值：
 *   若返回CKR_OK，则获取成功；其它结果均为失败
 *
 *20.CK_EXTEND_REMOTE_SET_DATA：远程设置SO保护的对象
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_REMOTE_SET_DATA
 *   pExtendIn->pParameter：存储了保护对象类型（4字节），IV值（16字节）和保护对象密文的缓冲区首地址，前4字节为类型，中间16字节存储IV，剩下的所有（N）字节为密文
 *							保护对象类型定义如下：
 *							CKO_REMOTE_TT						0x80000001
 *							CKO_REMOTE_OTP					0x80000002
 *							CKO_REMOTE_SECRET_KEY		0x80000003
 *   pExtendIn->ulParameterLen：上述保护对象类型和IV值和保护对象密文的长度，为（20+N）
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_REMOTE_SET_DATA
 *   pExtendOut->pParameter：任意值，推荐为NULL
 *   pExtendOut->ulParameterLen：任意值，推荐为0
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：无意义
 *   pExtendOut->ulParameterLen：无意义
 *   返回值：
 *   若返回CKR_OK，则设置成功；其它结果均为失败
 *
 *21.CK_EXTEND_REMOTE_GET_DATAVER：获取SO保护的对象的版本号
 *   输入：
 *   pExtendIn->extendType：必须为CK_EXTEND_REMOTE_GET_DATAVER
 *   pExtendIn->pParameter：存储了保护对象类型（4字节）
  *							保护对象类型定义如下：
 *							CKO_REMOTE_TT						0x80000001
 *							CKO_REMOTE_OTP					0x80000002
 *							CKO_REMOTE_SECRET_KEY		0x80000003
 *   pExtendIn->ulParameterLen：上述保护对象类型的长度，为4
 *   pExtendOut->extendType：任意值，推荐为CK_EXTEND_REMOTE_GET_DATAVER
 *   pExtendOut->pParameter：存储输出的版本号的缓冲区首地址
 *   pExtendOut->ulParameterLen：缓冲区的大小（字节数），至少为4
 *   输出：
 *   pExtendOut->extendType：无意义
 *   pExtendOut->pParameter：存储了版本号的缓冲区首地址
 *   pExtendOut->ulParameterLen：版本号长度，值为4
 *   返回值：
 *   若返回CKR_OK，则获取成功；其它结果均为失败
 *
 */
CK_PKCS11_FUNCTION_INFO(C_CryptoExtend)
#ifdef CK_NEED_ARG_LIST
(
	CK_SESSION_HANDLE hSession, //P11会话句柄，由C_OpenSession取得
	CK_EXTEND_IN_PTR pExtendIn, //扩展业务接口传入参数
	CK_EXTEND_OUT_PTR pExtendOut, //扩展业务接口传出参数
	CK_VOID_PTR pReserved //为未来使用保留。此版本暂为NULL_PTR
);
#endif

/* C_CleanFlags is a 30RayCom defined for its private usage.
 * 该函数清理所有slot的状态
 * 1. flagType为1时，清理所有Slot的Hash/Encrypt/Decrypt/Find锁
 */
CK_PKCS11_FUNCTION_INFO(C_CleanFlags)
#ifdef CK_NEED_ARG_LIST
(
	CK_BYTE flagType //要清除的标志类型，从1开始
);
#endif