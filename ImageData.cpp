#include "ImageData.h"

ImageData::ImageData(int x, int y):imgWidth(x),imgHeight(y)
	{
		StoredImage.resize(3);
		for (int z=0;z<3;z++)
		{
			StoredImage[z].resize(imgHeight);
			for (int y=0;y<imgHeight;y++)
				StoredImage[z][y].resize(imgWidth,0);

		}
		displayImageString.resize(imgHeight*imgWidth*3,0);
	}

ImageData::ImageData(const ImageData & other)
	{
		imgHeight=other.imgHeight;
		imgWidth=other.imgWidth;
		displayImageString.resize(imgHeight*imgWidth*3,0);
		StoredImage.resize(3);
		for (int z=0;z<3;z++)
		{
			StoredImage[z].resize(0);
			StoredImage[z].insert(StoredImage[z].begin(),other.StoredImage[z].begin(),other.StoredImage[z].end());
			for (int y=0;y<imgHeight;y++)
			{
				StoredImage[z][y].resize(0);
				StoredImage[z][y].insert(StoredImage[z][y].begin(),other.StoredImage[z][y].begin(),other.StoredImage[z][y].end());
			}
		}

	}

bool ImageData::operator = (const ImageData & other)
	{
		imgHeight=other.imgHeight;
		imgWidth=other.imgWidth;
		displayImageString.resize(imgHeight*imgWidth*3,0);
		StoredImage.resize(3);
		for (int z=0;z<3;z++)
		{
			StoredImage[z].resize(0);
			StoredImage[z].insert(StoredImage[z].begin(),other.StoredImage[z].begin(),other.StoredImage[z].end());
			for (int y=0;y<imgHeight;y++)
			{
				StoredImage[z][y].resize(0);
				StoredImage[z][y].insert(StoredImage[z][y].begin(),other.StoredImage[z][y].begin(),other.StoredImage[z][y].end());
			}
		}
		return true;
	}

unsigned char & ImageData::operator ()(int x ,int y, int z)
	{
		return StoredImage[z][y][x];
	}

const unsigned char & ImageData::operator ()(int x ,int y, int z) const
	{
		return StoredImage[z][y][x];
	}

bool ImageData::ImageDataResize (int x, int y)
	{
		imgHeight=y;
		imgWidth=x;
		displayImageString.resize(imgHeight*imgWidth*3,0);
		StoredImage.resize(3);
		for (int z=0;z<3;z++)
		{
			StoredImage[z].resize(imgHeight);
			for (int y=0;y<imgHeight;y++)
			{
				StoredImage[z][y].resize(imgWidth,0);
			}
		}

		return true;
	}

char * ImageData::giveCharStringForImage()
	{
		for (int x=0;x<imgHeight*imgWidth;x++)
		{
			int i,j;
			i=x%imgWidth;
			j=x/imgWidth;
			displayImageString[3*x]  =StoredImage[BLUE][j][i];
			displayImageString[3*x+1]=StoredImage[GREEN][j][i];
			displayImageString[3*x+2]=StoredImage[RED][j][i];
		}
	return displayImageString.data();
	}