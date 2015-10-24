/* ====================================================================
 * Copyright (c) 2001-2008 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

#ifndef HEADER_CAPI_ERR_H
# define HEADER_CAPI_ERR_H

#ifdef  __cplusplus
extern "C" {
#endif

/* BEGIN ERROR CODES */
/*
 * The following lines are auto generated by the script mkerr.pl. Any changes
 * made after this point may be overwritten when the script is next run.
 */
static void ERR_load_CAPI_strings(void);
static void ERR_unload_CAPI_strings(void);
static void ERR_CAPI_error(int function, int reason, char *file, int line);
# define CAPIerr(f,r) ERR_CAPI_error((f),(r),__FILE__,__LINE__)

/* Error codes for the CAPI functions. */

/* Function codes. */
# define CAPI_F_CAPI_CERT_GET_FNAME                       99
# define CAPI_F_CAPI_CTRL                                 100
# define CAPI_F_CAPI_CTX_NEW                              101
# define CAPI_F_CAPI_CTX_SET_PROVNAME                     102
# define CAPI_F_CAPI_DSA_DO_SIGN                          114
# define CAPI_F_CAPI_GET_KEY                              103
# define CAPI_F_CAPI_GET_PKEY                             115
# define CAPI_F_CAPI_GET_PROVNAME                         104
# define CAPI_F_CAPI_GET_PROV_INFO                        105
# define CAPI_F_CAPI_INIT                                 106
# define CAPI_F_CAPI_LIST_CONTAINERS                      107
# define CAPI_F_CAPI_LOAD_PRIVKEY                         108
# define CAPI_F_CAPI_OPEN_STORE                           109
# define CAPI_F_CAPI_RSA_PRIV_DEC                         110
# define CAPI_F_CAPI_RSA_PRIV_ENC                         111
# define CAPI_F_CAPI_RSA_SIGN                             112
# define CAPI_F_CERT_SELECT_DIALOG                        117
# define CAPI_F_CLIENT_CERT_SELECT                        116
# define CAPI_F_WIDE_TO_ASC                               113

/* Reason codes. */
# define CAPI_R_CANT_CREATE_HASH_OBJECT                   99
# define CAPI_R_CANT_FIND_CAPI_CONTEXT                    100
# define CAPI_R_CANT_GET_KEY                              101
# define CAPI_R_CANT_SET_HASH_VALUE                       102
# define CAPI_R_CRYPTACQUIRECONTEXT_ERROR                 103
# define CAPI_R_CRYPTENUMPROVIDERS_ERROR                  104
# define CAPI_R_DECRYPT_ERROR                             105
# define CAPI_R_ENGINE_NOT_INITIALIZED                    106
# define CAPI_R_ENUMCONTAINERS_ERROR                      107
# define CAPI_R_ERROR_ADDING_CERT                         125
# define CAPI_R_ERROR_CREATING_STORE                      126
# define CAPI_R_ERROR_GETTING_FRIENDLY_NAME               108
# define CAPI_R_ERROR_GETTING_KEY_PROVIDER_INFO           109
# define CAPI_R_ERROR_OPENING_STORE                       110
# define CAPI_R_ERROR_SIGNING_HASH                        111
# define CAPI_R_FUNCTION_NOT_SUPPORTED                    112
# define CAPI_R_GETUSERKEY_ERROR                          113
# define CAPI_R_INVALID_DIGEST_LENGTH                     124
# define CAPI_R_INVALID_DSA_PUBLIC_KEY_BLOB_MAGIC_NUMBER  122
# define CAPI_R_INVALID_LOOKUP_METHOD                     114
# define CAPI_R_INVALID_PUBLIC_KEY_BLOB                   115
# define CAPI_R_INVALID_RSA_PUBLIC_KEY_BLOB_MAGIC_NUMBER  123
# define CAPI_R_PUBKEY_EXPORT_ERROR                       116
# define CAPI_R_PUBKEY_EXPORT_LENGTH_ERROR                117
# define CAPI_R_UNKNOWN_COMMAND                           118
# define CAPI_R_UNSUPPORTED_ALGORITHM_NID                 119
# define CAPI_R_UNSUPPORTED_PADDING                       120
# define CAPI_R_UNSUPPORTED_PUBLIC_KEY_ALGORITHM          121
# define CAPI_R_WIN32_ERROR                               127

#ifdef  __cplusplus
}
#endif
#endif
