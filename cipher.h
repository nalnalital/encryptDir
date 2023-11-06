#ifndef CIPHER_H
#define CIPHER_H

#include <QString>
#include <QFile>
#include <QDebug>


#include <openssl/conf.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/err.h>

#define KEY_LEN 32
#define IV_LEN 32


//#define SCRYPT_N 1048576
//#define SCRYPT_r 8
//#define SCRYPT_p 1

class Cipher
{
public:
    Cipher();

    QByteArray encryptAES(QByteArray &data, QString _key, QString _iv);
    QByteArray decryptAES(QByteArray &data, QString _key, QString _iv);
};

#endif // CIPHER_H
