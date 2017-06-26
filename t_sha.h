#ifndef T_SHA_H
#define T_SHA_H

#if     !defined(P)
#ifdef  __STDC__
#define P(x)    x
#else
#define P(x)    ()
#endif
#endif

#define SHA_DIGESTSIZE (256 / 8)

#ifdef OPENSSL
#define OPENSSL_SHA 256
#endif

#ifdef TOMCRYPT
# include <tomcrypt.h>
# ifdef SHA1
#  define TOMCRYPT_SHA 256
# endif
#endif

#ifdef CRYPTOLIB
/* The SHA (shs) implementation in CryptoLib 1.x breaks when Update
 * is called multiple times, so we still use our own code.
 * Uncomment below if you think your copy of CryptoLib is fixed. */
/*#define CRYPTOLIB_SHA 1*/
#endif

#ifdef GCRYPT
# define GCRYPT_SHA 256
#endif

#ifdef OPENSSL_SHA
#include <openssl/sha.h>

typedef SHA256_CTX SHA1_CTX;
#define SHA1Init SHA256_Init
#define SHA1Update SHA256_Update
#define SHA1Final SHA256_Final
#elif defined(TOMCRYPT_SHA)
/* mycrypt.h already included above */

typedef hash_state SHA1_CTX;
#define SHA1Init sha1_init
#define SHA1Update sha1_process
#define SHA1Final(D,C) sha1_done(C,D)
#elif defined(GCRYPT_SHA)
#include "gcrypt.h"
typedef gcry_md_hd_t SHA1_CTX;
#define SHA1Init SHA1Init_gcry
#define SHA1Update SHA1Update_gcry
#define SHA1Final SHA1Final_gcry
#elif defined(CRYPTOLIB_SHA)
#include "libcrypt.h"

typedef SHS_CTX SHA1_CTX;
#define SHA1Init shsInit
#define SHA1Update shsUpdate
#define SHA1Final shsFinalBytes

void shsFinalBytes P((unsigned char digest[SHA_DIGESTSIZE], SHS_CTX* context));
#else
typedef unsigned int uint32;

typedef struct {
    uint32 state[5];
    uint32 count[2];
    unsigned char buffer[64];
} SHA1_CTX;

void SHA1Init P((SHA1_CTX* context));
void SHA1Update P((SHA1_CTX* context, const unsigned char* data, unsigned int len));
void SHA1Final P((unsigned char digest[SHA_DIGESTSIZE], SHA1_CTX* context));
#endif /* !OPENSSL && !CRYPTOLIB */

#endif /* T_SHA_H */