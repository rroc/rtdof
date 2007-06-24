//
//
//
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE

#include "Basic.h"

//#include <gl/glut.h>
//#include <stdio.h>

//#include "glATI.h"
//#include "ATIExtensions.h"

//image libraries
//#include <tiffio.h>
//#include <png.h>

#include "ImageIO.h"


/*
	////LOAD GAUSS IMAGE
	Image img = imgCreateImage();
	if (imgLoadImage(img, "textures/gauss.tga"))
		{
		imgDestroyImage(img);
		cout << "Gauss texture not found.";
		exit(-1);
		} 
	else 
		{
		cout << "Gauss texture load ok.";
		glBindTexture( GL_TEXTURE_2D, iTextureID[2]);
		imgLoadImageAsTextureFormat(img, GL_RGB);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		imgDestroyImage(img);
		}
*/
//
//
//
////////////////////////////////////////////////////////////////////////////////
struct ImageObj
{
   int width;
   int height;
   int depth;
   int components;

   float *fdata;
   GLushort *sdata;
   GLubyte *bdata;
};

//
//
//
////////////////////////////////////////////////////////////////////////////////
static void imgFlipRows( Image img)
{
   ImageObj *obj = (ImageObj*)img;
   if (obj->depth >1)
   {
      return;
   }
   GLubyte *tmp = new GLubyte[obj->width*obj->components*4];
   int offset = obj->width*obj->components;
   GLubyte *src, *dst;

   if (obj->bdata)
   {
      offset *= 1;
      src = (GLubyte*)obj->bdata;
      dst = src + offset*(obj->height-1);
   }
   else if (obj->sdata)
   {
      offset *= 2;
      src = (GLubyte*)obj->sdata;
      dst = src + offset*(obj->height-1);
   }
   else if (obj->fdata)
   {
      offset *= 4;
      src = (GLubyte*)obj->fdata;
      dst = src + offset*(obj->height-1);
   }

   for (int ii=0; ii<(obj->height*obj->depth)/2; ii++)
   {
      memcpy( tmp, src, offset);
      memcpy( src, dst, offset);
      memcpy( dst, tmp, offset);
      src += offset;
      dst -= offset;
   }

   delete []tmp;
}

//
//
////////////////////////////////////////////////////////////////////////////////
static void imgRGBtoBGR( Image img)
{
   ImageObj *obj = (ImageObj*)img; 

   if (obj->bdata)
   {
      GLubyte *pos = obj->bdata;
      GLubyte temp;
      for (int ii=0; ii<obj->width*obj->height; ii++)
      {
         temp = *pos;
         *pos = pos[2];
         pos[2] = temp;
         pos += obj->components;
      }
   }
   else if (obj->sdata)
   {
      GLushort *pos = obj->sdata;
      GLushort temp;
      for (int ii=0; ii<obj->width*obj->height; ii++)
      {
         temp = *pos;
         *pos = pos[2];
         pos[2] = temp;
         pos += obj->components;
      }
   }
   else if (obj->fdata)
   {
      float *pos = obj->fdata;
      float temp;
      for (int ii=0; ii<obj->width*obj->height; ii++)
      {
         temp = *pos;
         *pos = pos[2];
         pos[2] = temp;
         pos += obj->components;
      }
   }

}

//
//
////////////////////////////////////////////////////////////////////////////////
Image imgCreateImage()
{
   ImageObj *ret = new ImageObj;

   ret->width = 0;
   ret->height = 0;
   ret->depth = 0;
   ret->components = 0;

   ret->fdata = 0;
   ret->sdata = 0;
   ret->bdata = 0;

   return ret;
}

//
//
////////////////////////////////////////////////////////////////////////////////
void imgDestroyImage( Image img)
{
   ImageObj *obj = (ImageObj*)img;

   delete []obj->bdata;
   delete []obj->fdata;
   delete []obj->sdata;

   delete obj;
}

//
//
////////////////////////////////////////////////////////////////////////////////
int imgLoadImage( Image img, char *file)
{
   char *ext;
   FILE *fp;
   ImageObj *obj = (ImageObj*)img;

   //reset image?
   ext = strrchr( file, '.');
   if ( !(fp = fopen( file, "rb")))
   {
      return 1;
   }

   if (ext != NULL)
   {
      ext++;
      if (!_stricmp( ext, "tga"))
      {
         //read targa
         delete []obj->bdata;
         delete []obj->sdata;
         delete []obj->fdata;
         obj->bdata = 0;
         obj->sdata = 0;
         obj->fdata = 0;

         //this obfusated read code is designed to avoid endian issues
         unsigned char header [18];

         fread( header, 18, 1, fp);
         if (header[2] !=2)
         {
            //not RGB data, can't translate
            fclose(fp);
            return 1;
         }

         if (header[16] < 24)
         {
            //only support 8bits per component images
            fclose(fp);
            return 1;
         }

         obj->depth = 1; //targa only supports 2D
         obj->width = header[12] + (header[13] <<8);
         obj->height = header[14] + (header[15] <<8);
         obj->components = header[16]/8;
         obj->bdata = new GLubyte[obj->width*obj->height*obj->components];

         fseek( fp, header[0], SEEK_CUR);
         fread( obj->bdata, obj->width*obj->height*obj->components, 1, fp);
         fclose(fp);
         imgRGBtoBGR( (Image)obj);
         
      }
/*      else if (!_stricmp( ext, "tif"))
      {
         //read tiff
         delete []obj->bdata;
         delete []obj->sdata;
         delete []obj->fdata;
         obj->bdata = 0;
         obj->sdata = 0;
         obj->fdata = 0;
         fclose(fp);
         TIFF *tif = TIFFOpen( file, "r");

         if (!tif)
         {
            return NULL;
         }

         GLushort config, photo, bitdepth, datatype, components;

         //figure out the parameters
         TIFFGetField( tif, TIFFTAG_IMAGEWIDTH, &obj->width);
         TIFFGetField( tif, TIFFTAG_IMAGELENGTH, &obj->height);
         if (!TIFFGetField( tif, TIFFTAG_IMAGEDEPTH, &obj->depth))
         {
            obj->depth = 1;
         }
         if (obj->depth < 1)
         {
            obj->depth = 1;
         }
         TIFFGetField( tif, TIFFTAG_PLANARCONFIG, &config);
         if (config != PLANARCONFIG_CONTIG)
         {
            TIFFClose( tif);
            return 1;
         }
         TIFFGetField( tif, TIFFTAG_PHOTOMETRIC, &photo); //only rgb and greyscale
         if ((config != PHOTOMETRIC_MINISBLACK) && (photo != PHOTOMETRIC_RGB))
         {
            TIFFClose( tif);
            return 1;
         }

         TIFFGetField( tif, TIFFTAG_BITSPERSAMPLE, &bitdepth);
         TIFFGetField( tif, TIFFTAG_SAMPLESPERPIXEL, &components);
         obj->components = components;

         GLushort bytes = components*bitdepth/8;

         if ( !TIFFGetField( tif, TIFFTAG_SAMPLEFORMAT, &datatype))
         {
            // TIFFGetField( tif, TIFFTAG_DATATYPE, datatype); //necessary? tag is SGI proprietary
            datatype = SAMPLEFORMAT_UINT;
         }

         if ( datatype >= SAMPLEFORMAT_VOID)
         {
            //bad datatype, can't interpret
            TIFFClose( tif);
            return 1;
         }

         //alloc buffer
         GLubyte * buffer = new GLubyte[obj->width*obj->height*obj->depth*bytes];

         //strip versus tile stuff
         if (TIFFIsTiled( tif))
         {
            GLuint tWidth, tHeight, tDepth;
            //tiled
            TIFFGetField( tif, TIFFTAG_TILEWIDTH, &tWidth);
            TIFFGetField( tif, TIFFTAG_TILELENGTH, &tHeight);
            if (!TIFFGetField( tif, TIFFTAG_TILEDEPTH, &tDepth))
               tDepth = 1;

            GLubyte *buf = new GLubyte[TIFFTileSize( tif)];

            for (int ii=0; ii<obj->depth; ii+=tDepth)
            {
               for (int gg=0; gg<obj->height; gg+=tHeight)
               {
                  for ( int hh=0; hh<obj->width; hh+=tWidth)
                  {
                     TIFFReadTile( tif, buf, ii, gg, hh, 0);
                     //copy it
                     for ( int jj=0; jj<tDepth; jj++)
                     {
                        for ( int kk=0; kk<tHeight; kk++)
                        {
                           memcpy( &buffer[(((ii+jj)*obj->height+gg+kk)*obj->width+hh)*bytes], &buf[(jj*obj->height+kk)*obj->width*bytes], tWidth*bytes);
                        }
                     }
                  }
               }
            }

            delete []buf;
         }
         else
         {
            //stripped
            GLushort rowCount;
            TIFFGetField( tif, TIFFTAG_ROWSPERSTRIP, &rowCount);
            GLuint stripSize = TIFFStripSize(tif);

            GLubyte *buf = new GLubyte[TIFFStripSize( tif)];
            
            for (int ii=0; ii< obj->depth*obj->height; ii+=rowCount)
            {
               tstrip_t strip = TIFFComputeStrip(tif, ii, 0);
               tsize_t size = TIFFReadEncodedStrip( tif, strip, buf, stripSize);
               memcpy( &buffer[ii*obj->width*bytes], buf, size);
            }
            delete []buf;
         }

         TIFFClose( tif);

         switch (datatype)
         {
            case SAMPLEFORMAT_UINT:
            case SAMPLEFORMAT_INT:
               //need to add sign fix
               if (bitdepth == 8)
               {
                  obj->bdata = buffer;
               }
               else if (bitdepth == 16)
               {
                  obj->sdata = (GLushort*)buffer;
               }
               break;
            case SAMPLEFORMAT_IEEEFP:
               obj->fdata = (float*)buffer;
               break;
         };
         imgFlipRows( (Image) obj);
      }
      else if (!_stricmp( ext, "png"))
      {
         //read PNG
         delete []obj->bdata;
         delete []obj->sdata;
         delete []obj->fdata;
         obj->bdata = 0;
         obj->sdata = 0;
         obj->fdata = 0;

         GLubyte sig[8];
         png_structp png_ptr = NULL;
         png_infop info_ptr = NULL;
         int colorType, bitDepth;
         int rowBytes;

         fread(sig, 1, 8, fp);
         if (!png_check_sig(sig, 8))
         {
            return 1;   // bad signature 
         }


         //could pass pointers to user-defined error handlers instead of NULLs:

         png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
         if (!png_ptr)
         {
           return 1;   // out of memory 
         }

         info_ptr = png_create_info_struct(png_ptr);
         if (!info_ptr) {
           png_destroy_read_struct(&png_ptr, NULL, NULL);
           return 1;   // out of memory 
         }


         // we could create a second info struct here (end_info), but it's only
         // useful if we want to keep pre- and post-IDAT chunk info separated
         // (mainly for PNG-aware image editors and converters) *


         // setjmp() must be called in every function that calls a PNG-reading
         // libpng function 

         if (setjmp(png_jmpbuf(png_ptr)))
         {
           png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
           return 1;
         }


         png_init_io(png_ptr, fp);
         png_set_sig_bytes(png_ptr, 8);  //we already read the 8 signature bytes 

         png_read_info(png_ptr, info_ptr);  // read all PNG info up to image data


         obj->width = png_get_image_width(png_ptr, info_ptr);
         obj->height = png_get_image_height(png_ptr, info_ptr);
         obj->depth = 1;
         colorType = png_get_color_type( png_ptr, info_ptr);
         bitDepth = png_get_bit_depth( png_ptr, info_ptr);
         

         if (setjmp(png_jmpbuf(png_ptr)))
         {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return 1;
         }


         // expand palette images to RGB, low-bit-depth grayscale images to 8 bits,
         // transparency chunks to full alpha channel; strip 16-bit-per-sample
         // images to 8 bits per sample; and convert grayscale to RGB[A] 

         if (colorType == PNG_COLOR_TYPE_PALETTE)
         {
            png_set_palette_to_rgb(png_ptr);
         }
         if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
         {
            png_set_gray_1_2_4_to_8(png_ptr);
         }
         if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
         {
            png_set_tRNS_to_alpha(png_ptr);
         }



         // all transformations have been registered; now update info_ptr data,
         // get rowbytes and channels, and allocate image memory 

         png_read_update_info(png_ptr, info_ptr);

         rowBytes = png_get_rowbytes(png_ptr, info_ptr);
         obj->components = (int)png_get_channels(png_ptr, info_ptr);

         GLubyte *buffer = new GLubyte[rowBytes * obj->height];

         GLubyte **rowPointers = new GLubyte*[obj->height]; 
         
         //set the individual row_pointers to point at the correct offsets

         for ( int ii = 0;  ii < obj->height;  ++ii)
         {
           rowPointers[ii] = buffer + ii*rowBytes;
         }


        //now we can go ahead and just read the whole image 

         png_read_image(png_ptr, rowPointers);


         //and we're done!  (png_read_end() can be omitted if no processing of
         // post-IDAT text/time/etc. is desired) 

         delete []rowPointers;

         if (bitDepth<=8)
         {
            bitDepth = 8;
            obj->bdata = buffer;
         }
         else
         {
            obj->sdata = (GLushort *)buffer;
         }


         png_read_end(png_ptr, NULL);
         png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
         fclose(fp);

         imgFlipRows( (Image) obj);

      }
*/
      else if (!_stricmp( ext, "raw"))
      {
         //read raw
         delete []obj->bdata;
         delete []obj->sdata;
         delete []obj->fdata;
         obj->bdata = 0;
         obj->sdata = 0;
         obj->fdata = 0;
      }
      else
      {
         //unknown
         return 1;
      }
   }
   else
   {
      //no extension
      return 1;
   }

   return 0;
}

//
//
////////////////////////////////////////////////////////////////////////////////
int imgSaveImage( Image img, char *file)
{
   char *ext;
   FILE *fp;
   ImageObj *obj = (ImageObj*)img;

   ext = strrchr( file, '.');
   if ( !(fp = fopen( file, "wb")))
   {
      return 1;
   }

   if (ext != NULL)
   {
      ext++;
/*
      if (!_stricmp( ext, "tif"))
      {
         //save tiff
         fclose(fp);
         TIFF *tif = TIFFOpen( file, "w");

         if (!tif)
         {
            return 0;
         }

         GLushort photo, bitdepth, datatype, components;

         //figure out the parameters
         TIFFSetField( tif, TIFFTAG_IMAGEWIDTH, obj->width);
         TIFFSetField( tif, TIFFTAG_IMAGELENGTH, obj->height);
         if (obj->depth>1)
         {
            TIFFSetField( tif, TIFFTAG_IMAGEDEPTH, obj->depth);
         }
         
         TIFFSetField( tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
         
         if (obj->components<3)
         {
            photo = PHOTOMETRIC_MINISBLACK;
         }
         else
         {
            photo = PHOTOMETRIC_RGB;
         }
         TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, photo); //only rgb and greyscale

         if (obj->bdata)
         {
            bitdepth = 8;
         }
         else if (obj->sdata)
         {
            bitdepth = 16;
         }
         else if (obj->fdata)
         {
            bitdepth = 32;
         }
         components = obj->components;

         TIFFSetField( tif, TIFFTAG_BITSPERSAMPLE, bitdepth);
         TIFFSetField( tif, TIFFTAG_SAMPLESPERPIXEL, components);

         if (obj->bdata)
         {
            datatype = SAMPLEFORMAT_UINT;
         }
         else if (obj->sdata)
         {
            datatype = SAMPLEFORMAT_UINT;
         }
         else if (obj->fdata)
         {
            datatype = SAMPLEFORMAT_IEEEFP;
         }
         
         TIFFSetField( tif, TIFFTAG_SAMPLEFORMAT, datatype);
         
         imgFlipRows( (Image) obj);

         
         GLushort tWidth = obj->width, tHeight = obj->height, tDepth = obj->depth;
         //tiled
         TIFFSetField( tif, TIFFTAG_TILEWIDTH, tWidth);
         TIFFSetField( tif, TIFFTAG_TILELENGTH, tHeight);
         TIFFSetField( tif, TIFFTAG_TILEDEPTH, tDepth);

         if (obj->bdata)
         {
            TIFFWriteTile( tif, obj->bdata, 0, 0, 0, 0);
         }
         else if (obj->sdata)
         {
            TIFFWriteTile( tif, obj->sdata, 0, 0, 0, 0);
         }
         else if (obj->fdata)
         {
            TIFFWriteTile( tif, obj->fdata, 0, 0, 0, 0);
         }

         imgFlipRows( (Image) obj);
         

         TIFFClose( tif);
      }
      else */
	  if (!_stricmp( ext, "tga"))
      {
         //save targa
         if (obj->bdata) // only supports ubytes
         {
            //this obfusated write code is designed to avoid endian issues
            unsigned char header [18];

            memset( header, 0, 18);
            header[2] =2;
            
            header[12] = obj->width & 0xff; 
            header[13] = obj->width >> 8;
            header[14] = obj->height & 0xff;
            header[15] = obj->height >> 8;
            header[16] = obj->components*8;

            fwrite( header, 18, 1, fp);

            imgRGBtoBGR( (Image)obj);

            fwrite( obj->bdata, obj->width*obj->height*obj->components, 1, fp);
            fclose(fp);
            imgRGBtoBGR( (Image)obj);
         }

      }
/*
      else if (!_stricmp( ext, "png"))
      {
         //save PNG
      }
*/
      else if (!_stricmp( ext, "raw"))
      {
         if (obj->bdata)
         {
            fwrite( obj->bdata, sizeof(GLubyte), obj->width*obj->height*obj->depth*obj->components, fp);
         }
         else if (obj->sdata)
         {
            fwrite( obj->sdata, sizeof(GLushort), obj->width*obj->height*obj->depth*obj->components, fp);
         }
         else if (obj->fdata)
         {
            fwrite( obj->fdata, sizeof(GLfloat), obj->width*obj->height*obj->depth*obj->components, fp);
         }
         fclose(fp);
      }
      else
      {
         return 1;
      }
   }
   else
   {
      return 1;
   }
   return 0;
}

//
//
////////////////////////////////////////////////////////////////////////////////
int imgGetWidth( Image img)
{
   ImageObj *obj = (ImageObj*)img;

   return obj->width;
}

//
//
////////////////////////////////////////////////////////////////////////////////
int imgGetHeight( Image img)
{
   ImageObj *obj = (ImageObj*)img;

   return obj->height;
}

//
//
////////////////////////////////////////////////////////////////////////////////
int imgGetDepth( Image img)
{
   ImageObj *obj = (ImageObj*)img;

   return obj->depth;
}

//
//
////////////////////////////////////////////////////////////////////////////////
int imgIsFloat( Image img)
{
   ImageObj *obj  = (ImageObj*)img;
   
   return obj->fdata != 0;
}

//
//
////////////////////////////////////////////////////////////////////////////////
void imgGrabImage( Image img, int x, int y, int width, int height, GLenum format, GLenum type)
{
   ImageObj *obj = (ImageObj*)img;

   delete []obj->bdata;
   delete []obj->sdata;
   delete []obj->fdata;
   obj->bdata = 0;
   obj->sdata = 0;
   obj->fdata = 0;

   obj->width = width;
   obj->height = height;
   obj->depth = 1;

   switch (format)
   {
      case GL_RED:
      case GL_GREEN:
      case GL_BLUE:
      case GL_ALPHA:
      case GL_DEPTH_COMPONENT:
      case GL_STENCIL_INDEX:
      case GL_LUMINANCE:
         obj->components = 1;
         break;
      case GL_LUMINANCE_ALPHA:
         obj->components = 2;
         break;
      case GL_RGB:
      case GL_BGR_EXT:
         obj->components = 3;
         break;
      case GL_RGBA:
      case GL_BGRA_EXT:
         obj->components = 4;
         break;
   };

   switch (type)
   {
      case GL_UNSIGNED_BYTE:
         obj->bdata = new GLubyte[obj->width*obj->height*obj->components];
         glReadPixels( x, y, width, height, format, type, obj->bdata);
         break;
      case GL_UNSIGNED_SHORT:
         obj->sdata = new GLushort[obj->width*obj->height*obj->components];
         glReadPixels( x, y, width, height, format, type, obj->sdata);
         break;
      case GL_FLOAT:
         obj->fdata = new GLfloat[obj->width*obj->height*obj->components];
         glReadPixels( x, y, width, height, format, type, obj->fdata);
         break;
   };
}

//
//
////////////////////////////////////////////////////////////////////////////////
int Pow2( int num)
{
   int ret = 1;

   while (num >ret)
      ret <<= 1;

   return ret;
}

//
//
////////////////////////////////////////////////////////////////////////////////
/*
void imgLoadImageAsTexture( Image img)
{
   ImageObj *obj = (ImageObj*)img;
   GLenum fmt, ifmt;
   int stride;
   int p2w, p2h, p2d;

   switch (obj->components)
   {
      case 1:
         fmt = GL_LUMINANCE;
         if (obj->bdata)
         {
            //8bit
            ifmt = GL_LUMINANCE8;
         }
         else if (obj->sdata)
         {
            //16bit
            ifmt = GL_LUMINANCE16;
         }
         else
         {
            //float
            if (GL_ATI_texture_float_Flag)
            {
               ifmt = GL_LUMINANCE_FLOAT32_ATI;;
            }
            else
            {
               ifmt = GL_LUMINANCE16;
            }
         }
         break;
      case 2:
         fmt = GL_LUMINANCE_ALPHA;
         if (obj->bdata)
         {
            //8bit
            ifmt = GL_LUMINANCE8_ALPHA8;
         }
         else if (obj->sdata)
         {
            //16bit
            ifmt = GL_LUMINANCE16_ALPHA16;
         }
         else
         {
            //float
            if (GL_ATI_texture_float_Flag)
            {
               ifmt = GL_LUMINANCE_ALPHA_FLOAT32_ATI;
            }
            else
            {
               ifmt = GL_LUMINANCE16_ALPHA16;
            }
         }
         break;
      case 3:
         fmt = GL_RGB;
         if (obj->bdata)
         {
            //8bit
            ifmt = GL_RGB8;
         }
         else if (obj->sdata)
         {
            //16bit
            ifmt = GL_RGB16;
         }
         else
         {
            //float
            if (GL_ATI_texture_float_Flag)
            {
               ifmt = GL_RGB_FLOAT32_ATI;
            }
            else
            {
               ifmt = GL_RGB16;
            }
         }
         break;
      case 4:
         fmt = GL_RGBA;
         if (obj->bdata)
         {
            //8bit
            ifmt = GL_RGBA8;
         }
         else if (obj->sdata)
         {
            //16bit
            ifmt = GL_RGBA16;
         }
         else
         {
            //float
            //if (GL_ATI_texture_float_Flag)
            //{
            //   ifmt = GL_RGBA_FLOAT32_ATI;
            //}
            //else
            //{
               ifmt = GL_RGBA16;
  //          }
         }
         break;
   };

   
   if ((obj->depth >1) && (glTexImage3D != NULL))
   {
      //3D texture

      //fix for non-pow2
      p2w = Pow2(obj->width);
      p2h = Pow2(obj->height);
      p2d = Pow2(obj->depth);
      glTexImage3D( GL_TEXTURE_3D_EXT, 0, ifmt, p2w, p2h, p2d, 0, fmt, GL_UNSIGNED_BYTE, NULL);
      if (obj->bdata)
      {
         glTexSubImage3D( GL_TEXTURE_3D_EXT, 0, 0, 0, 0, obj->width, obj->height, obj->depth,  fmt, GL_UNSIGNED_BYTE, obj->bdata);
      }
      else if (obj->sdata)
      {
         glTexSubImage3D( GL_TEXTURE_3D_EXT, 0, 0, 0, 0, obj->width, obj->height, obj->depth,  fmt, GL_UNSIGNED_SHORT, obj->sdata);
      }
      else if (obj->fdata)
      {
         glTexSubImage3D( GL_TEXTURE_3D_EXT, 0, 0, 0, 0, obj->width, obj->height, obj->depth,  fmt, GL_FLOAT, obj->fdata);
      }
   }
   else
   {
      //2D texture
      if (obj->bdata)
      {
         glTexImage2D( GL_TEXTURE_2D, 0, ifmt, obj->width, obj->height, 0, fmt, GL_UNSIGNED_BYTE, obj->bdata);
      }
      else if (obj->sdata)
      {
         glTexImage2D( GL_TEXTURE_2D, 0, ifmt, obj->width, obj->height, 0, fmt, GL_UNSIGNED_SHORT, obj->sdata);
      }
      else if (obj->fdata)
      {
         glTexImage2D( GL_TEXTURE_2D, 0, ifmt, obj->width, obj->height, 0, fmt, GL_FLOAT, obj->fdata);
      }
   }
}
*/
//
//
////////////////////////////////////////////////////////////////////////////////
void imgLoadImageAsTextureFormat( Image img, GLenum format)
{
   ImageObj *obj = (ImageObj*)img;
   GLenum fmt, ifmt = format;
//   int stride;
   int p2w, p2h, p2d;

   switch (obj->components)
   {
      case 1:
         fmt = GL_LUMINANCE;
         
         break;
      case 2:
         fmt = GL_LUMINANCE_ALPHA;
         
         break;
      case 3:
         fmt = GL_RGB;
         
         break;
      case 4:
         fmt = GL_RGBA;
         
         break;
   };

   
   if ((obj->depth >1) && (glTexImage3D != NULL))
   {
      //3D texture

      //fix for non-pow2
      p2w = Pow2(obj->width);
      p2h = Pow2(obj->height);
      p2d = Pow2(obj->depth);
      glTexImage3D( GL_TEXTURE_3D_EXT, 0, ifmt, p2w, p2h, p2d, 0, fmt, GL_UNSIGNED_BYTE, NULL);
      if (obj->bdata)
      {
         glTexSubImage3D( GL_TEXTURE_3D_EXT, 0, 0, 0, 0, obj->width, obj->height, obj->depth,  fmt, GL_UNSIGNED_BYTE, obj->bdata);
      }
      else if (obj->sdata)
      {
         glTexSubImage3D( GL_TEXTURE_3D_EXT, 0, 0, 0, 0, obj->width, obj->height, obj->depth,  fmt, GL_UNSIGNED_SHORT, obj->sdata);
      }
      else if (obj->fdata)
      {
         glTexSubImage3D( GL_TEXTURE_3D_EXT, 0, 0, 0, 0, obj->width, obj->height, obj->depth,  fmt, GL_FLOAT, obj->fdata);
      }
   }
   else
   {
      //2D texture
      if (obj->bdata)
      {
         glTexImage2D( GL_TEXTURE_2D, 0, ifmt, obj->width, obj->height, 0, fmt, GL_UNSIGNED_BYTE, obj->bdata);
      }
      else if (obj->sdata)
      {
         glTexImage2D( GL_TEXTURE_2D, 0, ifmt, obj->width, obj->height, 0, fmt, GL_UNSIGNED_SHORT, obj->sdata);
      }
      else if (obj->fdata)
      {
         glTexImage2D( GL_TEXTURE_2D, 0, ifmt, obj->width, obj->height, 0, fmt, GL_FLOAT, obj->fdata);
      }
   }
}