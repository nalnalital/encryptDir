#include "cipher.h"

Cipher::Cipher(){}

QByteArray Cipher::encryptAES(QByteArray &data, QString _key, QString _iv){
    bool success = true;

    unsigned char key[KEY_LEN]="";
    memcpy( key, _key.toStdString().c_str() ,_key.size());
    unsigned char iv[IV_LEN]="";
    memcpy( iv, _iv.toStdString().c_str() ,_iv.size());

    // init ciphering context and set ciphering algorithm,
    // key and initialization vector
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        //qDebug() << ("EVP_EncryptInit_ex");
        return QByteArray();
    }

    // openssl will start with char pointer and data length
    char *input = data.data();
    int len = data.size();

    int c_len = len + AES_BLOCK_SIZE, f_len = 0;

    // alloc memory for the cipher
    unsigned char *cipher = (unsigned char *)malloc(c_len);

    // this just verifies that the crypto engine is ready
    if (!EVP_EncryptInit_ex(ctx, NULL, NULL, NULL, NULL)) {
        //qDebug() << ("EVP_EncryptInit_ex");
        success = false;
    }

    // this actually ciphers the whole input (see input and len parameters)
    // to the cipher buffer and updates c_len (cipher len).
    // beware of large byte arrays ..
    // in such cases update the cipher in a loop !
    if (!EVP_EncryptUpdate(ctx, cipher, &c_len, (unsigned char *)input, len)) {
        //qDebug() << ("EVP_EncryptUpdate");
        success = false;
    }

    // completes the encryption at the given pointer (cipher+c_len)
    // and update f_len (final length)
    if (!EVP_EncryptFinal(ctx, cipher+c_len, &f_len)) {
        //qDebug() << ("EVP_EncryptFinal");
        success = false;
    }

    // the total cipher length is
    len = c_len + f_len;

    // we return the cipher data in a byte array where we prepend the salt
    // so when deciphering the data we can start by extracting the salt
    QByteArray finished;
    if (success) {
        finished = QByteArray(reinterpret_cast<char *>(cipher), len);
    }

    // free context and cipher buffer
    EVP_CIPHER_CTX_free(ctx);
    free(cipher);

    return finished;
}
QByteArray Cipher::decryptAES(QByteArray &data, QString _key, QString _iv){
    bool success = true;

    // extract salt from input data

    unsigned char key[KEY_LEN]="";
    memcpy( key, _key.toStdString().c_str() ,_key.size());
    unsigned char iv[IV_LEN]="";
    memcpy( iv, _iv.toStdString().c_str() ,_iv.size());

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        //qDebug() << ("EVP_DecryptInit_ex");
        return QByteArray();
    }

    // same as encrypt we need a pointer to the input data
    // and an output buffer for deciphered data
    int len = data.size();
    char *input = data.data();
    char *out;


    // if you got the encrypt part then this works the same
    // we malloc plain length + block size which would be the minimum possible size a cipher would be ..
    int f_len = 0;
    int p_len = len;
    unsigned char *decipher = (unsigned char *) malloc(p_len + AES_BLOCK_SIZE);

    // also beware of large files ..
    // in such cases update in a loop !
    if (!EVP_DecryptUpdate(ctx, decipher, &p_len, (unsigned char *)input, len)) {
        //qDebug() << ("EVP_DecryptUpdate");
        success = false;
    }

    // complete and update sizes .. etc
    if (!EVP_DecryptFinal_ex(ctx, decipher+p_len, &f_len)) {
        //qDebug() << ("EVP_DecryptFinal_ex");
        success = false;
    }

    len = p_len + f_len;
    out = (char *)decipher;
    QByteArray result;

    // and return that as a QByteArray
    if (success) {
        result = QByteArray(reinterpret_cast<char *>(out), len);
    }

    // of course free memory !
    EVP_CIPHER_CTX_free(ctx);
    free(decipher);

    return result;
}
