#ifndef TIMAGEHANDLER_H
#define TIMAGEHANDLER_H

#include "Basic.h"

//#include <png.h>

typedef struct tagBITMAPFILEHEADER 
	{
    unsigned short  bfType;
    unsigned long   bfSize;
    unsigned short  bfReserved1;
    unsigned short  bfReserved2;
    unsigned long   bfOffBits;
	} WBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
	{
    unsigned long   biSize;
    long			biWidth;
    long			biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned long   biCompression;
    unsigned long   biSizeImage;
    long			biXPelsPerMeter;
    long			biYPelsPerMeter;
    unsigned long   biClrUsed;
    unsigned long   biClrImportant;
	} WBITMAPINFOHEADER;

typedef struct tagRGBTRIPLE 
	{
    unsigned char    rgbtBlue;
    unsigned char    rgbtGreen;
    unsigned char    rgbtRed;
	} WRGBTRIPLE;

class TImageHandler
	{
	public:
		TImageHandler();
		~TImageHandler();
		bool LoadPNG( char* aFileName );
		bool SavePNG( char* aFileName );
		bool CreatePNG( int aWidth, int aHeight);
		void ModifyPNG();
		int LoadBMP( char* aFileName, int aCurrentTextureNum );
	private:
		int iX, iY;
		int iWidth, iHeight;

//		png_byte iColorType;
//		png_byte iBitDepth;
		
//		png_structp iPngData;
//		png_infop	iPngInfo;
//		png_infop	iPngEndInfo;

		int iNumberOfPasses;
//		png_bytepp iPngRows;
		//unsigned char* iPngRows;

	};
//CALLBACKS
//extern void readChunkCallback( png_ptr ptr, png_unknown_chunkp chunk );

#endif
