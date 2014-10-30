//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Prashant Rao Nittoor
// 
//
//*****************************************************************************

#include "Image.h"
#include <cmath>


// Constructor and Desctructors
MyImage::MyImage()
{
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
	start=false;
}

MyImage::~MyImage()
{
	for (int i=0;i<numberOfFrames;i++)
	{
		if (framesForMovie[i]!=NULL)
		delete framesForMovie[i];
	}
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	strcpy(otherImage->ImagePath, ImagePath );
	Data	= otherImage->Data;
	start=false;

}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	strcpy( (char *)otherImage.ImagePath, ImagePath );
	Data= otherImage.Data;
	start=false;
	return *this;
	
}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}
	
	Data.ImageDataResize(Width,Height);

	for (int z=2;z>=0;z--)
	{
		for (int y=0;y<Height;y++)
		{
			for (int x=0;x<Width;x++)
			{
				Data(x,y,z)=fgetc(IN_FILE);
			}
		}
	}

	
	// Clean up and return
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	for (int z=2;z>=0;z--)
	{
		for (int y=0;y<Height;y++)
		{
			for (int x=0;x<Width;x++)
			{
				fputc(Data(x,y,z), OUT_FILE);
			}
		}
	}

	fclose(OUT_FILE);

	return true;

}




// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify(const int & angle,const float & size,const int & antiAlias,const int &fps ,const int & animeTime)
{
	start=true;
	// TO DO by student
	if (antiAlias)
	{
		antiAliasImage();
		Data=ModifiedData;
	}
	animationTime=animeTime;
	numberOfFrames=fps*animeTime;

	if (!numberOfFrames)
	{
		RotateImage(angle%360,size,size);
		return true;
	}
	else 
	{
		framesForMovie= new char * [numberOfFrames];
		createFramesForMovie(framesForMovie,numberOfFrames,angle%360,size);
	}
	return true;
}

bool MyImage::createFramesForMovie(char ** framesForMovie,const int &numberOfFrames,const int & angle,const float & size)
{
	float newSize=size;
	if (size<1)
	{
		newSize=1;
	}
	for (int i=0;i<numberOfFrames;i++)
	{
		RotateImage((angle*(i))/(numberOfFrames-1),newSize,(1+((size-1)*i)/(numberOfFrames-1)));
		char * temp= new char[ModifiedWidth*ModifiedHeight*3];
		memcpy(temp,ModifiedData.giveCharStringForImage(),ModifiedWidth*ModifiedHeight*3);
		framesForMovie[i]=temp;
	}
	return true;
}

char*	MyImage::getModifiedImageData() 
{ 
	if (start)
	{
		startTime=std::chrono::high_resolution_clock::now();
		start=false;
		frameNo=0;
	}
	if (!numberOfFrames)
		return ModifiedData.giveCharStringForImage();
	else
	{
		
		std::chrono::high_resolution_clock::time_point timeNow=std::chrono::high_resolution_clock::now();
		int timeLapse= std::chrono::duration_cast<std::chrono::milliseconds> (timeNow-startTime).count();
		
		int frameNumber=(timeLapse*(numberOfFrames))/(animationTime*1000);
		
		if (frameNumber<numberOfFrames)
			return framesForMovie[frameNumber];
		else 
			{
			start=true;
			return framesForMovie[numberOfFrames-1];
			}
	}
}

bool MyImage::antiAliasImage()
{
	applyConvolutionOverMatrix();
	return true;
}

bool MyImage::applyConvolutionOverMatrix()
{
	ModifiedData.ImageDataResize(Width,Height);
	vector<vector<vector<unsigned char >>> tempInputMatrix;
	tempInputMatrix.resize(3);
	for (int z=0;z<3;z++)
	{
		tempInputMatrix[z].resize(3);
		for (int y=0;y<3;y++)
			{
				tempInputMatrix[z][y].resize(3,0);
			}
	}

	for (int i=0;i<Data.getImgDataHeight();i++)
	{
		for (int j=0;j<Data.getImgDataWidth();j++)
		{
			Convolute(i,j,tempInputMatrix);
		}
	}
	return true;

}

bool MyImage::Convolute(const int & row,const int &column,vector<vector<vector<unsigned char>>>& tempInputMatrix)
{ 

		int mid=3/2;     //ConvolutionKernel.size()/2;
		for (int i=0;i<tempInputMatrix[0].size();i++)
		{
			for (int j=0;j<tempInputMatrix[0][i].size();j++)
			{
				if ((row-mid+i)<Data.getImgDataHeight() && (column-mid+j)<Data.getImgDataWidth() && (row-mid+i)>=0 && (column-mid+j) >=0)
				{
					tempInputMatrix[0][i][j]= Data(column-mid+j,row-mid+i,BLUE);
					tempInputMatrix[1][i][j]= Data(column-mid+j,row-mid+i,GREEN);
					tempInputMatrix[2][i][j]= Data(column-mid+j,row-mid+i,RED);
				}
				else
				{
					if (row-mid+i<0 && column-mid+j>=0 && (column-mid+j)<Data.getImgDataWidth())
					{
						tempInputMatrix[0][i][j]=Data(column-mid+j,row-mid+i+1,BLUE);
						tempInputMatrix[1][i][j]=Data(column-mid+j,row-mid+i+1,GREEN);
						tempInputMatrix[2][i][j]=Data(column-mid+j,row-mid+i+1,RED);
					}
					else if (row-mid+i<0 && column-mid+j>=0 && (column-mid+j)>=Data.getImgDataWidth())
					{
						tempInputMatrix[0][i][j]=Data(column-mid+j-1,row-mid+i+1,BLUE);
						tempInputMatrix[1][i][j]=Data(column-mid+j-1,row-mid+i+1,GREEN);
						tempInputMatrix[2][i][j]=Data(column-mid+j-1,row-mid+i+1,RED);
					}
					else if (column-mid+j<0 && row-mid+i>=0 && (row-mid+i)<Data.getImgDataHeight() )
					{
						tempInputMatrix[0][i][j]=Data(column-mid+j+1,row-mid+i,BLUE);
						tempInputMatrix[1][i][j]=Data(column-mid+j+1,row-mid+i,GREEN);
						tempInputMatrix[2][i][j]=Data(column-mid+j+1,row-mid+i,RED);
					}
					else if (column-mid+j<0 && row-mid+i>=0 && (row-mid+i)>=Data.getImgDataHeight())
					{
						tempInputMatrix[0][i][j]=Data(column-mid+j+1,row-mid+i-1,BLUE);
						tempInputMatrix[1][i][j]=Data(column-mid+j+1,row-mid+i-1,GREEN);
						tempInputMatrix[2][i][j]=Data(column-mid+j+1,row-mid+i-1,RED);
					}
					else if (column-mid+j<0 && row-mid+i<0)
					{
						tempInputMatrix[0][i][j]=Data(column-mid+j+1,row-mid+i+1,BLUE);
						tempInputMatrix[1][i][j]=Data(column-mid+j+1,row-mid+i+1,GREEN);
						tempInputMatrix[2][i][j]=Data(column-mid+j+1,row-mid+i+1,RED);
					}
					else if ((row-mid+i)>=Data.getImgDataHeight() && (column-mid+j)<Data.getImgDataWidth())
					{
						tempInputMatrix[0][i][j]=Data(column-mid+j,row-mid+i-1,BLUE);
						tempInputMatrix[1][i][j]=Data(column-mid+j,row-mid+i-1,GREEN);
						tempInputMatrix[2][i][j]=Data(column-mid+j,row-mid+i-1,RED);
					}
					else if ((row-mid+i)<Data.getImgDataHeight() && (column-mid+j)>=Data.getImgDataWidth())
					{
						tempInputMatrix[0][i][j]=Data(column-mid+j-1,row-mid+i,BLUE);
						tempInputMatrix[1][i][j]=Data(column-mid+j-1,row-mid+i,GREEN);
						tempInputMatrix[2][i][j]=Data(column-mid+j-1,row-mid+i,RED);
					}
					else 
					{
						tempInputMatrix[0][i][j]=Data(column-mid+j-1,row-mid+i-1,BLUE);
						tempInputMatrix[1][i][j]=Data(column-mid+j-1,row-mid+i-1,GREEN);
						tempInputMatrix[2][i][j]=Data(column-mid+j-1,row-mid+i-1,RED);
					}
				}
			}
		}
	

	//Convolute
	float sumB=0.0,sumG=0.0,sumR=0.0;
	for (int m=0;m<tempInputMatrix[0].size();m++)
	{
		for (int n=0;n<tempInputMatrix[m].size();n++)
		{
			sumB+=tempInputMatrix[BLUE][m][n];
			sumG+=tempInputMatrix[GREEN][m][n];
			sumR+=tempInputMatrix[RED][m][n];

		}
	}


	ModifiedData(column,row,BLUE) = (sumB/9);
	ModifiedData(column,row,GREEN)= (sumG/9);
	ModifiedData(column,row,RED)  = (sumR/9);

	return true;
}

bool   MyImage::RotateImage(const double & angle,const float & finalSize,const float & size)
{
	cout<<"MyImage::RotateImage";
	// we will set the dimensions of the layout to maximum possible size wrt to the original iamge
	int rotImageWidth,rotImageHeight;

	rotImageWidth= (finalSize*(sqrt((Width*Width)+(Height*Height))));

	//rotImageWidth=Width;
	rotImageWidth+=(sizeof(DWORD)- rotImageWidth%sizeof(DWORD));
	//rotImageWidth*=size;
	rotImageHeight = rotImageWidth;
	ImageData	NewData(rotImageWidth,rotImageHeight);
	int newX,newY;

	vector<vector<double>> InverseRotationalMatrix;
	InverseRotationalMatrix.resize(2);
	for (int i=0;i<2;i++)
	{
		InverseRotationalMatrix[i].resize(2);
	}
	InverseRotationalMatrix[0][0]=cos((angle/180)*M_PI)/size;
	InverseRotationalMatrix[0][1]=sin((angle/180)*M_PI)/size;
	InverseRotationalMatrix[1][0]=(-sin((angle/180)*M_PI))/size;
	InverseRotationalMatrix[1][1]=cos((angle/180)*M_PI)/size;


	for (int y=0 ; y< rotImageHeight;y++ )
	{
		for (int x=0; x<rotImageWidth;x++ )
		{
			replotXY(x,-y,newX,newY,rotImageWidth,rotImageHeight,InverseRotationalMatrix);
			if (newX<Width && newY <Height && newX>=0 && newY>=0)
			{
				NewData(x,y,BLUE)  =Data(newX,newY,BLUE)  ;
				NewData(x,y,GREEN) =Data(newX,newY,GREEN)  ;
				NewData(x,y,RED)   =Data(newX,newY,RED)  ;
			}
		}
	}

	ModifiedWidth=NewData.getImgDataHeight();
	ModifiedHeight=NewData.getImgDataWidth();
	ModifiedData=NewData;
	
	return true;
}

bool MyImage::replotXY(const int& x,const int& y,int & newX,int & newY,const int & rotImageWidth,const int &  rotImageHeight, const vector<vector<double>> & RotationalMatrix)
{
	vector<int> originPlot;
	vector<double> transformedPlot;

	originPlot.resize(2);
	transformedPlot.resize(2,0);

	originPlot[0]=x-rotImageWidth/2;
	originPlot[1]=y-(-rotImageHeight)/2;

	for (int m=0;m<2;m++)
	{
		for (int n=0;n<1;n++)
		{
			float sum=0.0;
			for (int k=0;k<2;k++)
			{
				sum+=RotationalMatrix[m][k]*originPlot[k];
			}
			transformedPlot[m]=sum;
		}
	}
	
	newX=transformedPlot[0]+(rotImageWidth/2);
	newY=-(transformedPlot[1]+(-rotImageHeight)/2);
	newX+=(Width-rotImageWidth)/2;
	newY+=(Height-(rotImageHeight))/2;
	
	return true;
}