#include "tool.h"

const char * QStringToCharP(QString text){
    //QByteArray ba = text.toLatin1();
    QByteArray ba2 = text.toUtf8();
    //char *ch=ba.data();
    const char *ch2 = ba2.data();
    return ch2;
}

char ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}


/**
  16 hex string to QByteArray
 * @brief StringToHex
 * @param str
 * @param senddata
 */
void StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}


/**
  picture data to 16 hex string
 * @brief getImgToData
 * @param img
 * @param isAnti
 * @return
 */
QString getImgToData(QImage img, QByteArray &imgData,bool isAnti)
{
    QString ret = "";

    int mH = img.height();
    int mW = img.width();

    int mh = mH / 8;
    int mw = mW / 8;

    imgData.resize(mw * mH); //40 len

    QRgb srcColor;
    int average;
    Byte d = 0;


    for (int i = 0; i < mH; i++)
    {
        for (int j = 0; j < mw; j++)
        {
            d = 0;
            for (int k = 0; k < 8; k++)
            {
                if ((j * 8 + k) < mW)
                {
                    srcColor = img.pixel(j * 8 + k,i);
                    int r = qRed(srcColor);
                    int g = qGreen(srcColor);
                    int b = qBlue(srcColor);

                    average = (r + g + b) / 3;
                    if (average > 128)
                    {
                        d |= (Byte)(0x80 >> k);
                    }
                }
            }
            imgData[i * mw + j] = isAnti ? (Byte)~d : d;
        }
    }

    /*for (int i = 0; i < len; i++)
    {
        //QString m_tmp;

        Byte sigleData[1];
        memcpy(sigleData,imgData.data[i],1);
        //sigleData = imgData[i];

        sprintf((char *)QStringToCharP(m_tmp), "%2X",sigleData);

    }*/
    ret = imgData.toHex().toUpper();  //ToString("X02")

    return ret;
}

/**
 * @brief isFileExist
 * @param fullFileName
 * @return
 */
bool isFilesExist(QString fullFileName)
{
    QFileInfo fileInfo(fullFileName);
    if(fileInfo.isFile())
    {
        return true;
    }
    return false;
}
