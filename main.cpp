#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include "cipher.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList ArgsList = a.arguments();
    qDebug() << ArgsList;
    if (ArgsList.length()<4){
        qDebug() << "dir path or file path as first parameter !";
        qDebug() << "password as second parameter !";
        qDebug() << "iv=B64[16] as third parameter !";
        qDebug() << "contain string name as fourth parameter [*]";
        a.exit();
    }else{
        QString selector="*";
        if (ArgsList.length()>4) selector=ArgsList[4];
        QDir::setCurrent(ArgsList[1]);//qApp->applicationDirPath());
        QDir directory(ArgsList[1]);
        QStringList images = directory.entryList(QStringList() << ("*"+selector+"*.*"),QDir::Files);
        foreach(QString filename, images) {
            qDebug() << "-----------------" << filename;
            QFile file(filename);
            QByteArray raw;
            if (file.open(QIODevice::ReadOnly)){
                raw=file.readAll();
            }else{
                qDebug() << filename << "notFound";
            }
            file.close();

            QByteArray B64=raw.toBase64();
            Cipher cipher;
            QByteArray res=cipher.encryptAES(B64,ArgsList[2],ArgsList[3]);
            B64=res.toBase64();
            if (file.open(QIODevice::WriteOnly)){
                QTextStream stream( &file );
                stream << B64;
                stream.flush();
            }else{
                qDebug() << filename << "notWrite";
            }
            file.close();
        }
        a.exit();
    }

    return a.exec();
}
