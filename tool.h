#ifndef TOOL_H
#define TOOL_H

#include <QString>
#include <QTextCodec>
#include <QImage>
#include <QFileInfo>

typedef unsigned char Byte;

const char * QStringToCharP(QString text);
char ConvertHexChar(char ch);
void StringToHex(QString str, QByteArray &senddata);
QString getImgToData(QImage img, QByteArray &imgData,bool isAnti=false);
bool isFilesExist(QString fullFileName);


#endif // TOOL_H
