#include "print.h"
#include "tool.h"
#include "libusbt.h"
#include <QImage>
#include <QBuffer>

Print::Print()
{

}

/**
 * @brief Print::resizeWH
 * @param w
 * @param h
 * @return
 */
bool Print::resizeWH(int &w,int &h){

    bool isturn = false;
    if(w > this->max_print_w){
        w = this->max_print_w;
        isturn = true;
    }
    if(h > this->max_print_h){
        h = this->max_print_h;
        isturn = true;
    }
    return isturn;
}

/**
 * @brief Print::TextData
 * @param p
 */
void Print::TextData(unsigned char * p)
{

QString print_sdata_text ="SIZE 100 mm,155 mm\r\nREFERENCE 0,0\r\nDIRECTION 0,0\r\nGAP 3 mm,0 mm\r\nSET CUTTER OFF\r\nOFFSET 0 mm\r\nDENSITY 8\r\nSPEED 8\r\nSETC AUTODOTTED OFF\r\nSETC PAUSEKEY ON\r\nSETC WATERMARK OFF\r\nCLS\r\nTEXT 0,0,\"2\",0,1,1,\"Hello World\"";
strcpy((char*)p,QStringToCharP(print_sdata_text));
}

/**
 * @brief Print::ImageData
 * @param p
 */
bool Print::ImageData(unsigned char * p){

    QImage img;

    //load image data
    if(img.load(this->fileName)){

        QString print_base_data = "SIZE 180 mm,98 mm\r\nREFERENCE 0,0\r\nDIRECTION 2,0\r\nGAP 3 mm,0 mm\r\nSET CUTTER OFF\r\nOFFSET 0 mm\r\nDENSITY 8\r\nSPEED 8\r\nSETC AUTODOTTED OFF\r\nSETC PAUSEKEY ON\r\nSETC WATERMARK OFF\r\nCLS\r\nBITMAP 0,0,%d,%d,1,";

        QByteArray Image_data;
        if(this->turn == false){ //no turn 90

            this->print_w = img.width();
            this->print_h = img.height();
            if(this->resizeWH(this->print_w,this->print_h)){ // turn size
                img.convertToFormat(QImage::Format_Grayscale8);
                img = img.scaled(this->print_w,this->print_h);
            }

            sprintf((char *)p,QStringToCharP(print_base_data),this->print_w/8,this->print_h); //w <800 dot < 203

            if(this->isDebug){
                QByteArray ba;
                QBuffer bf(&ba);
                bf.open(QIODevice::ReadWrite);
                img.save("/home/pi/project/debug.bmp","BMP",-1);
                img.save(&bf,"BMP",-1);
            }
            QString ret = getImgToData(img,Image_data);


        }else{

            this->print_h = img.width(); //change
            this->print_w = img.height();

            if(this->resizeWH(this->print_w,this->print_h)){ // turn size
                img.convertToFormat(QImage::Format_Grayscale8);
                img = img.scaled(this->print_h,this->print_w);
            }

            sprintf((char *)p,QStringToCharP(print_base_data),this->print_w/8,this->print_h); //w <800 dot < 203



              QImage img2(this->print_w,this->print_h,QImage::Format_Grayscale8);
              //to sigle color black or white
              for(int x = 0; x<this->print_h; x++) {
                  for(int y = 0;y<this->print_w;y++){
                      QRgb pixel = img.pixel(x,y);
                      int r = qRed(pixel);
                      int g = qGreen(pixel);
                      int b = qBlue(pixel);

                      if(this->print_h-x != this->print_h){ // out of range
                          if(r+g+b>3*127){ //white
                              img2.setPixelColor(y,this->print_h-x,Qt::white);
                          }else{//black
                              img2.setPixelColor(y,this->print_h-x,Qt::black);
                          }
                      }

                  }
              }

              if(this->isDebug){
                  QByteArray ba;
                  QBuffer bf(&ba);
                  bf.open(QIODevice::ReadWrite);
                  img2.save("/home/pi/project/debug.bmp","BMP",-1);
                  img2.save(&bf,"BMP",-1);
              }

              QString ret = getImgToData(img2,Image_data);
              //printf(QStringToCharP(ret));

        }


        QString len = QString((char*)p);
        int index  = len.length();
        memcpy(p+index,Image_data.data(),Image_data.size());
        Image_data.clear();


        return true;
    }else{
        printf("image load fail");
        return false;
    }

}

/**
 * 开始连接打印机打印
 * @brief Print::beginPrint
 */
bool Print::beginPrint(){


        int rv,rv2;

        int length;

        const int print_data_len = 500000; //500k
        const int print_command_len = 100;

        unsigned char print_data[print_data_len];

        //print Text
        //TextData(print_data);

        //print Image
        if(!ImageData(print_data)){
           return false;
        }


        unsigned char print_command[print_command_len] = {'\r','\n','P','R','I','N','T',0x20,'1',',','1','\r','\n'};

        libusb_device_handle* g_usb_handle;
        libusb_context *ctx=NULL;


        struct userDevice user_device;
        struct libusb_device_descriptor dev_desc;

        user_device.idProduct = USB_PRODUCT_ID;
        user_device.idVendor =  USB_VENDOR_ID ;
        user_device.bInterfaceClass = LIBUSB_CLASS_PRINTER ;
        user_device.bInterfaceSubClass = LIBUSB_CLASS_PRINTER ;
        user_device.bmAttributes = LIBUSB_TRANSFER_TYPE_BULK ;
        user_device.dev = NULL;

        rv =libusb_init(&ctx);
        if(rv<0)
        {
                printf("init libusb error");
                return false;
        }

        libusb_set_debug(ctx,3);


        rv = get_device_descriptor(&dev_desc,&user_device);
        if(rv < 0) {
                printf("*** get_device_descriptor failed! \n");
                return false;
            }

        rv = get_device_endpoint(&dev_desc,&user_device);
        if(rv < 0) {
                printf("*** get_device_endpoint failed! rv:%d \n",rv);
                return false;
        }

        /*4.open device and start communication by usb*/
        //open the usb device
        g_usb_handle = libusb_open_device_with_vid_pid(ctx, user_device.idVendor, user_device.idProduct);
        if(g_usb_handle == NULL) {
            printf("*** Permission denied or Can not find the USB board (Maybe the USB driver has not been installed correctly), quit!\n");
            return false;
        }

        rv = libusb_claim_interface(g_usb_handle,user_device.bInterfaceNumber);
        if(rv < 0) {
            rv = libusb_detach_kernel_driver(g_usb_handle,user_device.bInterfaceNumber);
            if(rv < 0) {
                printf("*** libusb_detach_kernel_driver failed! rv%d\n",rv);
                return false;
            }
            rv = libusb_claim_interface(g_usb_handle,user_device.bInterfaceNumber);
            if(rv < 0)
            {
                printf("*** libusb_claim_interface failed! rv%d\n",rv);
                return false;
            }

        }



        rv2 = libusb_bulk_transfer(g_usb_handle,BULK_ENDPOINT_OUT,print_data,print_data_len,&length,10000); //send print_data
        if(rv2 <0){
            printf("*** bulk_transfer print_data failed! rv:%d \n",rv);
            close_device(g_usb_handle,user_device); //关闭设备
            return false;
        }
        rv = libusb_bulk_transfer(g_usb_handle,BULK_ENDPOINT_OUT,print_command,print_command_len,&length,1000); //send print_command
        if(rv < 0) {
            printf("*** bulk_transfer print_command failed! rv:%d \n",rv);
            close_device(g_usb_handle,user_device); //关闭设备
            return false;
        }

        close_device(g_usb_handle,user_device); //关闭设备

        memset(print_data,0,print_data_len);
        memset(print_command,0,print_command_len);

        return true;

}
