#include "TImageHandler.h"

#define _CRT_SECURE_NO_DEPRECATE
//#include <fstream>
#include <stdio.h>

#include <iostream>
//#include <iomanip>
using namespace std;

//CONSTANTS
const int KMaxHeaderSize = 8;

/*
int iX, iY;
int iWidth, iHeight;

png_byte iColor_type;
png_byte iBit_depth;

png_structp iPngData;
png_infop	iPngInfo;

int iNumberOfPasses;
png_bytep* iPngRow;
*/

TImageHandler::TImageHandler()
	:iX(0),iY(0)
	{
//	iPngRows=NULL;
	}

TImageHandler::~TImageHandler()
	{
	//if(0!= iPngRows)
	//	{
	//	for ( iY=0; iY < iHeight; iY++ )
	//		free( iPngRows[iY] );
	//	free(iPngRows);
	//	}

	}





/** \brief PNG LOAD
 *
 *
 * \param aFileName 
 */
bool TImageHandler::LoadPNG( char* aFileName )
	{
/*
	//fstream dataFile( aFileName, ios::in | ios::binary );
	
	// open file in C-style (as the default is in libpng)
	//FILE *dataFile = fopen( aFileName, "rb"); //read binary
	FILE *dataFile;
	fopen_s( &dataFile, aFileName, "rb"); //read binary

	// exit program if fstream cannot open file
   if ( !dataFile ) 
		{
		//cerr << "File: \"" << aFileName << "\" could not be opened." << endl;
		std::cout << "Reading image file: \"" << aFileName << "\" failed, the file could not be opened." << endl;
		return( false );
		} 

	//VALIDATE PNG-FILE BY ITS HEADER
 //   unsigned char header[KMaxHeaderSize];	// 8 is the maximum size that can be checked
	////dataFile.read( reinterpret_cast<char *>(&header), KMaxHeaderSize );
	//fread(header, 1, 8, dataFile);

	//bool validPng = !png_sig_cmp( header, 0, KMaxHeaderSize);
	//if( !validPng )
	//	{
	//	fclose(dataFile);
	//	return false;
	//	}

	//INITIALIZE...

	//struct for the data
	iPngData = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0); //user_error_ptr, user_error_fn, user_warning_fn;
	if (!iPngData)
		{
		cout << "TImageHandler::LoadPNG - png_create_read_struct - error.";
		fclose(dataFile);
		return false;
		}
	//struct for the file info
	iPngInfo = png_create_info_struct(iPngData);
	if (!iPngInfo)
		{
		png_destroy_read_struct(&iPngData,(png_infopp)NULL, (png_infopp)NULL);
		cout << "TImageHandler::LoadPNG - png_create_info_struct - error.";
		fclose(dataFile);
		return false;
		}
	//struct for the file end(?) info
 //   iPngEndInfo = png_create_info_struct(iPngData);
	//if (!iPngEndInfo)
	//	{
	//	png_destroy_read_struct(&iPngData, &iPngInfo, (png_infopp)NULL);
	//	cout << "TImageHandler::LoadPNG - png_create_info_struct - error.";
	//	fclose(dataFile);
	//	return false;
	//	}

	//setting up I/O 
	if ( setjmp(iPngData->jmpbuf) )
		{
		png_destroy_read_struct(&iPngData, &iPngInfo, &iPngEndInfo);
		cout << "TImageHandler::LoadPNG - general load error.";
		//dataFile.close();
		fclose(dataFile);
		return false;
		}
	png_init_io(iPngData, dataFile);
	//these bytes were already read for the header...
//	png_set_sig_bytes(iPngData, KMaxHeaderSize);

	//READING INFO
	png_read_info(iPngData, iPngInfo);

	iWidth		= iPngInfo->width;  //up to 2^31
	iHeight		= iPngInfo->height; //up to 2^31

	//COLOR TYPES:
	// PNG_COLOR_TYPE_GRAY			(bit depths 1, 2, 4, 8, 16)
	// PNG_COLOR_TYPE_GRAY_ALPHA	(bit depths 8, 16)
	// PNG_COLOR_TYPE_PALETTE		(bit depths 1, 2, 4, 8)
	// PNG_COLOR_TYPE_RGB			(bit_depths 8, 16)
	// PNG_COLOR_TYPE_RGB_ALPHA	(bit_depths 8, 16)
	// PNG_COLOR_MASK_PALETTE
	// PNG_COLOR_MASK_COLOR
	// PNG_COLOR_MASK_ALPHA
	iColorType	= iPngInfo->color_type; 
	iBitDepth	= iPngInfo->bit_depth; //valid values: 1, 2, 4, 8, 16
	//OTHER POSSIBLE INFO
	//filter_method - (must be PNG_FILTER_TYPE_BASE for PNG 1.0) | PNG_INTRAPIXEL_DIFFERENCING if MNG-1.0 datastream)
	//compression_type - (must be PNG_COMPRESSION_TYPE_BASE for PNG 1.0)
	//interlace_type - (PNG_INTERLACE_NONE or PNG_INTERLACE_ADAM7)
	//channels - (valid values are 1 (GRAY,PALETTE), 2 (GRAY_ALPHA), 3 (RGB),4 (RGB_ALPHA or RGB + filler byte) )
	//rowbytes - number of bytes needed to hold a row
	//signature - holds the signature read from the file (if any).


	//DO IMAGE FORMAT TRANSLATION HERE
	//...

	//HANDLE INTERLACED IMAGES
	iNumberOfPasses = png_set_interlace_handling(iPngData);
	//UPDATE ALL THE TRANSLATIONS TO IMAGE
	png_read_update_info(iPngData, iPngInfo);

	//READ DATA
	//...go to the begining of data
	if ( setjmp(iPngData->jmpbuf) )
		{
		png_destroy_read_struct(&iPngData, &iPngInfo, &iPngEndInfo);
		cout << "TImageHandler::LoadPNG - general load error.";
		fclose(dataFile);
		return false;
		}

	//RESERVE MEMORY FOR THE IMAGE
//	iPngRows = (png_bytep*) malloc(sizeof(png_bytep) * iHeight );
//	for ( iY=0; iY < iHeight; iY++ )
//		iPngRows[iY] = (png_byte*) malloc( iPngInfo->rowbytes );
	iPngRows = new png_bytep[iHeight];
	for(iY=0; iY<iHeight; iY++)
		{
		(iPngRows)[iY] = new png_byte[ iPngInfo->rowbytes ];
		}

	//READ THE IMAGE DATA TO MEMORY
	png_read_image(iPngData, iPngRows);

	fclose(dataFile);

	//FREE THE MEMORY
	png_destroy_read_struct(&iPngData, &iPngInfo, &iPngEndInfo);

	return true;
*/
	return false;
	}











/** \brief PNG SAVE
 *
 *
 * \param aFileName 
 */
bool TImageHandler::SavePNG( char* aFileName )
	{
/*
	//fstream dataFile( aFileName, ios::in | ios::binary );
	
	// open file in C-style (as the default is in libpng)
	//FILE *dataFile = fopen( aFileName, "wb"); //write binary
	FILE *dataFile;
	fopen_s( &dataFile, aFileName, "w+b"); //write binary

	// exit program if fstream cannot open file
   if ( !dataFile ) 
		{
		//cerr << "File: \"" << aFileName << "\" could not be opened." << endl;
		std::cout << "Saving image file: \"" << aFileName << "\" failed, file could not be opened." << endl;
		return( false );
		} 

	//INITIALIZE FOR WRITING
	//struct for the data
	iPngData = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!iPngData)
		{
		cout << "TImageHandler::SavePNG - png_create_write_struct - error.";
		fclose(dataFile);
		return false;
		}
	//struct for the file info
	iPngInfo = png_create_info_struct(iPngData);
	if (!iPngInfo)
		{
		png_destroy_write_struct(&iPngData,(png_infopp)NULL);
		cout << "TImageHandler::SavePNG - png_create_info_struct - error.";
		fclose(dataFile);
		return false;
		}

	//setting up I/O 
	if ( setjmp(png_jmpbuf(iPngData)) )
		{
		png_destroy_write_struct(&iPngData, &iPngInfo);
		cout << "TImageHandler::SavePNG - general save error.";
		//dataFile.close();
		fclose(dataFile);
		return false;
		}
	png_init_io(iPngData, dataFile);

	//SET CALLBACKS
//	png_voidp userChunkP = png_get_user_chunk_ptr(png_ptr);
//	png_set_read_user_chunk_fn( iPngData, userChunkP, readChunkCallback );



	//WRITE THE HEADER
	//init zlib compression level
    //png_set_compression_level(iPngData, Z_BEST_COMPRESSION);

	png_set_IHDR(iPngData, iPngInfo, iWidth, iHeight,
		       iBitDepth, iColorType
			 , PNG_INTERLACE_NONE
		     , PNG_COMPRESSION_TYPE_BASE 
			 , PNG_FILTER_TYPE_BASE
			 );
	png_write_info(iPngData, iPngInfo);
	
	//DO IMAGE TRANSFORMATIONS HERE...


	//WRITE DATA
	if ( setjmp(png_jmpbuf(iPngData)) )
		{
		png_destroy_write_struct(&iPngData,(png_infopp)NULL);
		cout << "TImageHandler::SavePNG - general save error.";
		//dataFile.close();
		fclose(dataFile);
		return false;
		}

	png_write_image( iPngData, iPngRows );

	if ( setjmp(png_jmpbuf(iPngData)) )
		{
		png_destroy_write_struct(&iPngData, &iPngInfo);
		cout << "TImageHandler::SavePNG - error during the ending.";
		//dataFile.close();
		fclose(dataFile);
		return false;
		}
	//ENDING
	png_write_end(iPngData, NULL); //,iPngInfo); for timestamp & comments
	fclose(dataFile);	
	png_destroy_write_struct(&iPngData, &iPngInfo);
	return true;
	*/
	return false;
	}









bool TImageHandler::CreatePNG( int aWidth, int aHeight)
	{
	//DELETE PREVIOUS IMAGE IN MEMORY
/*	if(0!= iPngRows)
		{
		for ( iY=0; iY < iHeight; iY++ )
			free( iPngRows[iY] );
		free(iPngRows);
		}
*/	

/*
	iWidth		= aWidth;  //up to 2^31
	iHeight		= aHeight; //up to 2^31
	iColorType	= PNG_COLOR_TYPE_RGB_ALPHA; //PNG_COLOR_TYPE_GRAY(bit depths 1, 2, 4, 8, 16) | PNG_COLOR_TYPE_GRAY_ALPHA(bit depths 8, 16) | PNG_COLOR_TYPE_PALETTE(bit depths 1, 2, 4, 8) | PNG_COLOR_TYPE_RGB(bit_depths 8, 16) | PNG_COLOR_TYPE_RGB_ALPHA(bit_depths 8, 16) | PNG_COLOR_TYPE_RGBA | PNG_COLOR_MASK_PALETTE | PNG_COLOR_MASK_COLOR | PNG_COLOR_MASK_ALPHA
	iBitDepth	= 8; //valid values: 1, 2, 4, 8, 16

	//RESERVE MEMORY FOR THE IMAGE
//	iPngRows = (png_bytep*) malloc(sizeof(png_bytep) * iHeight );
//	for ( iY=0; iY < iHeight; iY++ )
//		iPngRows[iY] = (png_byte*) malloc( iPngInfo->rowbytes );
	//iPngRows = (png_bytep*)new int( 10 );
	//int* joo = new int( 10 );
	//unsigned int* joo = new unsigned int( 10 );
	//iPngRows = (png_bytep*)new unsigned char( sizeof(unsigned char)*iHeight );
	//for ( iY=0; iY < iHeight; iY++ )
	//	iPngRows[iY] = (png_bytep*)new unsigned char( iPngInfo->rowbytes );

	iX=aWidth;
	iPngRows = new png_bytep[iHeight];
	for(iY=0; iY<iHeight; iY++)
		{
		(iPngRows)[iY] = new png_byte[iX*4];
		}

	//(iPngRows)[0] = new png_byte;
	//(iPngRows)[1] = new png_byte;
	//(iPngRows)[2] = new png_byte;
	//(iPngRows)[3] = new png_byte;
	//(iPngRows)[4] = new png_byte;

//	iPngRows = new png_bytep(iHeight);
//	int rowBufSize = 10; //png_get_rowbytes(read_ptr, read_info_ptr);
//	iX = rowBufSize;
//	for(iY=0; iY<(int)iHeight; iY++) 
//		(iPngRows)[iY] = new png_byte[rowBufSize];
	
	
	//iPngRows = static_cast<png_bytep>(joo);
	//delete joo;
	//for ( iY=0; iY < iHeight; iY++ )
		//iPngRows = new png_bytep ( sizeof(png_bytep)*iHeight );
		//iPngRows[iY] = (png_byte*) malloc( iPngInfo->rowbytes );

	//iPngRows = new (unsigned char)(10);
	//iPngRows = (png_bytep*) malloc(sizeof(png_bytep) * iHeight );
	//for ( iY=0; iY < iHeight; iY++ )
	//	iPngRows[iY] = (png_byte*) malloc( aWidth );

	return true;
*/
	return false;
	}





void TImageHandler::ModifyPNG()
	{
/*
	//IF THERE IS NO IMAGE
	if(0== iPngRows)
		return;
	
	//CHEACK THE COLORSPACE
	if (iColorType != PNG_COLOR_TYPE_RGBA)
		{
		cout << "TImageHandler::ModifyPNG - color type should be PNG_COLOR_TYPE_RGBA (is "<< iColorType << ")";
		return;
		}

	//PROCESS...
	png_byte* row, *point;
	//unsigned int dataSize = sizeof( png_byte );
	for(iY=0; iY<iHeight; iY++) 
		{
		 row = iPngRows[iY];
		for (iX=0; iX<iWidth; iX++) 
			{
			point = &(row[ iX*4 ]);
			//MODIFY
			point[0] = 1;// R
			point[1] = 1;// G
			point[2] = 1;// B
			point[3] = 1;// A
			cout << "["<<iX<<","<<iY<<"] = "<< static_cast<short>(point[0])<<static_cast<short>(point[0])<<static_cast<short>(point[0])<<static_cast<short>(point[0]) <<"(RGBA)\n";
			}
		}
*/
	}



int TImageHandler::LoadBMP( char* aFileName, int aCurrentTextureNum )
	{
    int i, j=0; //Index variables
    FILE *bmpFile; //File pointer
    unsigned char *l_texture; //The pointer to the memory zone in which we will load the texture
     
    // windows.h gives us these types to work with the Bitmap files
    BITMAPFILEHEADER fileheader; 
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;

	if (NULL == aFileName ) return (-1);


#ifdef _WIN32
	if(( NULL == fopen_s( &bmpFile, aFileName, "rb"))) 
#else
	if((bmpFile = fopen(aFileName, "rb"))==NULL) 
#endif
		{
        //MessageBox(NULL,"Texture not found","Spacesim",MB_OK | MB_ICONERROR);
		cout << "Texture not found.\n";
		return (-1);
		}
    fread(&fileheader, sizeof(fileheader), 1, bmpFile); // Read the fileheader
    
    fseek(bmpFile, sizeof(fileheader), SEEK_SET); // Jump the fileheader
    fread(&infoheader, sizeof(infoheader), 1, bmpFile); // and read the infoheader

    // Now we need to allocate the memory for our image (width * height * color deep)
    l_texture = (BYTE  *) malloc(infoheader.biWidth * infoheader.biHeight * 4);
    // And fill it with zeros
    memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);
 
    // At this point we can read every pixel of the image
    for (i=0; i < infoheader.biWidth*infoheader.biHeight; i++)
    {            
            // We load an RGB value from the file
            fread(&rgb, sizeof(rgb), 1, bmpFile); 

            // And store it
            l_texture[j+0] = rgb.rgbtRed; // Red component
            l_texture[j+1] = rgb.rgbtGreen; // Green component
            l_texture[j+2] = rgb.rgbtBlue; // Blue component
            l_texture[j+3] = 255; // Alpha value
            j += 4; // Go to the next position
    }
    fclose(bmpFile); // Closes the file stream
     
	aCurrentTextureNum++; // The counter of the current texture is increased
    glBindTexture(GL_TEXTURE_2D, aCurrentTextureNum); // Bind the ID texture specified by the 2nd parameter

    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // We don't combine the color with the original surface color, use only the texture map.

    // Finally we define the 2d texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

    // And create 2d mipmaps for the minifying function
//    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

    free(l_texture); // Free the memory we used to load the texture
    return (aCurrentTextureNum); // Returns the current texture OpenGL ID
	}

/*
//CPP:

int TImageHandler::LoadBMP( char* aFileName, int aCurrentTextureNum )
	{
	cout << "Loading BMP..." << endl;
	fstream dataFile( aFileName, ios::in | ios::binary );
	if ( !dataFile )
		{
		//cerr << "File: \"" << aFileName << "\" could not be opened." << endl;
		cout << "File: \"" << aFileName << "\" could not be opened." << endl;
		return( false );
		}
	unsigned char* texture; //The pointer to the memory zone in which we will load the texture
     
    // windows.h gives us these types to work with the Bitmap files
    WBITMAPFILEHEADER fileheader; 
    WBITMAPINFOHEADER infoheader;
    WRGBTRIPLE rgb;
	int a =	sizeof(fileheader);
	a =	sizeof(infoheader);
	// Read the fileheader
	dataFile.read( reinterpret_cast<char *>(&fileheader), sizeof(fileheader) );
	dataFile.seekg( sizeof(fileheader), ios::beg ); // Jump the fileheader
	dataFile.read( reinterpret_cast<char *>(&infoheader), sizeof(infoheader) );// and read the infoheader

    // Now we need to allocate the memory for our image (width * height * color deep)
    texture = new unsigned char(infoheader.biWidth*infoheader.biHeight*4);
    
	//And fill it with zeros
    //memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);
 
	cout << "Reading: " << aFileName << "..." << endl;
	
	// At this point we can read every pixel of the image
	int i(0), j(0);
	while( !dataFile.eof() && i<infoheader.biWidth*infoheader.biHeight)
		{
		dataFile.read( reinterpret_cast<char *>(&rgb), sizeof(rgb) );
		
		// And store it
        texture[j+0] = rgb.rgbtRed; // Red component
        texture[j+1] = rgb.rgbtGreen; // Green component
        texture[j+2] = rgb.rgbtBlue; // Blue component
        texture[j+3] = 255; // Alpha value
        j += 4; // Go to the next position
		i++;
		}
	dataFile.close();
	cout << "OK." << endl;     
	aCurrentTextureNum++; // The counter of the current texture is increased

	glBindTexture(GL_TEXTURE_2D, aCurrentTextureNum); // Bind the ID texture specified by the 2nd parameter

    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // We don't combine the color with the original surface color, use only the texture map.

    // Finally we define the 2d texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

    // And create 2d mipmaps for the minifying function
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	
	cout << "Assigned to texture." << endl;     

    delete(texture); // Free the memory we used to load the texture
    return (aCurrentTextureNum); // Returns the current texture OpenGL ID
	}
*/





//void readChunkCallback(png_ptr ptr, png_unknown_chunkp chunk)
//    {
//	// The unknown chunk structure contains your chunk data:
//	//png_byte name[5];
//	//png_byte *data;
//	//png_size_t size;
//
//	return chunk->size;
//	// libpng has already taken care of the CRC handling
//
//	//my code here.  Return one of the following: 
//	//return (-n);// chunk had an error
//	//return (0); // did not recognize
//	//return (n); // success
//	}
//
//void readRowCallback(png_ptr ptr, png_uint_32 row, int pass);
//	{
//	//my code
//	}
//read_row_callback(png_structp png_ptr, png_uint_32 row_number, int pass)
//	{
//	if(png_ptr == NULL || row_number > PNG_MAX_UINT) return;
//	cout << "r";
///*	if(status_pass != pass)
//		{
//		fprintf(stdout,"\n Pass %d: ",pass);
//		status_pass = pass;
//		status_dots = 31;
//		}
//	status_dots--;
//	if(status_dots == 0)
//		{
//		fprintf(stdout, "\n         ");
//		status_dots=30;
//		}
//	fprintf(stdout, "r");
//*/
//	}
//
//write_row_callback(png_structp png_ptr, png_uint_32 row_number, int pass)
//	{
//    if(png_ptr == NULL || row_number > PNG_MAX_UINT || pass > 7) return;
//    cout << "w";
//	}

