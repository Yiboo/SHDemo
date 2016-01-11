//////////////////////////////////////////////////////////////////////////////////////////
//	IMAGE_Load24BitBMP.cpp
//	Load a 24 bit .bmp file
//	Downloaded from: www.paulsprojects.net
//	Created:	9th November 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../GL files/glee.h"
#include "../Log/LOG.h"
#include "IMAGE.h"

//Load a 24 bit .bmp file
bool IMAGE::Load24BitBMP(char * filename)
{
	LOG::Instance()->OutputSuccess("Loading %s in Load24BitBMP()", filename);

	FILE * file;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	//Open file for reading
	file=fopen(filename, "rb");
	if(!file)
	{
		LOG::Instance()->OutputError("Unable to open %s", filename);
		return false;
	}

	//read the file header
	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);

	//Check this is a bitmap
	if(fileHeader.bfType != bitmapID)
	{
		fclose(file);
		LOG::Instance()->OutputError("%s is not a legal .bmp", filename);
		return false;
	}

	//Read in the information header
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

	if(infoHeader.biBitCount!=24)
	{
		fclose(file);
		LOG::Instance()->OutputError("%s is not a 24 bit .bmp", filename);
		return false;
	}

	//Set class variables
	bpp=24;
	format=GL_RGB;
	width=infoHeader.biWidth;
	height=infoHeader.biHeight;

	//Calculate the stride in bytes for each row (allow for 4-byte padding)
	stride=CalculateStride();

	//Point "file" to the beginning of the data
	fseek(file, fileHeader.bfOffBits, SEEK_SET);

	//Allocate space for the image data
	data=new GLubyte[stride*height];
	if(!data)
	{
		fclose(file);
		LOG::Instance()->OutputError(	"Unable to allocate data for %s of size %d x %d", filename,
								stride, height);
		return false;
	}

	//read in the data
	fread(data, 1, stride*height, file);

	fclose(file);

	//Data is in BGR format
	//swap b and r
	for(unsigned int row=0; row<height; ++row)
	{
		for(unsigned int i=0; i<width; ++i)
		{
			//Repeated XOR to swap bytes 0 and 2
			data[(row*stride)+i*3] ^= data[(row*stride)+i*3+2] ^=
			data[(row*stride)+i*3] ^= data[(row*stride)+i*3+2];
		}
	}

	LOG::Instance()->OutputSuccess("Loaded %s Correctly!", filename);
	return true;
}