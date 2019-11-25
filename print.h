#ifndef PRINT_H
#define PRINT_H

#include <QString>

class Print
{
public:
    Print();
    bool beginPrint();
    void TextData(unsigned char * p);
    bool ImageData(unsigned char * p);
    int print_w; //<800 1440
    int print_h; //<203 dot 784
    const int max_print_w = 1440;
    const int max_print_h = 784;
    bool turn;
    QString fileName;
    bool isDebug;
    bool resizeWH(int &w,int &h);
};

#endif // PRINT_H
