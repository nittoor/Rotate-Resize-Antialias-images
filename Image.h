//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Prashant Rao Nittoor
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include "ImageData.h"
#include <chrono>

#define _USE_MATH_DEFINES
#include <math.h>


using namespace std;

class MyImage 
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	ImageData	Data;					// RGB data of the image
	ImageData	ModifiedData;
	int		ModifiedWidth;					// Width of Image
	int		ModifiedHeight;					// Height of Image
	bool replotXY(const int& x,const int& y,int & newX,int & newY,const int & rotImageWidth,const int &  rotImageHeight, const vector<vector<double>> & RotationalMatrix);
	double inputAngle;
	char ** framesForMovie;
	int numberOfFrames;
	std::chrono::milliseconds timeInterval;
	std::chrono::high_resolution_clock::time_point startTime;
	int animationTime;
	bool start;
	int frameNo;

public:
	// Constructor
	MyImage();
	// Copy Constructor
	MyImage::MyImage( MyImage *otherImage);
	// Destructor
	~MyImage();

	// operator overload
	MyImage & operator= (const MyImage & otherImage);

	// Reader & Writer functions
	void	setWidth( const int w)  { Width = w; }; 
	void	setHeight(const int h) { Height = h; }; 
	void	setImageData( const ImageData &img ) { Data = img; };
	void	setImagePath( const char *path) { strcpy(ImagePath, path); }
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	char*	getImageData() { return Data.giveCharStringForImage(); };
	int		getModifiedWidth() { return ModifiedWidth; };
	int		getModifiedHeight() { return ModifiedHeight; };
	char*	getModifiedImageData();
	char*	getImagePath() { return ImagePath; }

	// Input Output operations
	bool	ReadImage();
	bool	WriteImage();

	// Modifications
	bool	Modify(const int & angle,const float & size=1.0,const int & antiAlias=0,const int &fps =0,const int & animeTime=0);
	bool    RotateImage(const double & angle,const float & finalSize,const float & size=1);
	bool    antiAliasImage();
	bool	applyConvolutionOverMatrix();
	bool	Convolute(const int & row,const int &column,vector<vector<vector<unsigned char>>>& tempInputMatrix);
	bool	createFramesForMovie(char** framesForMovie,const int &numberOfFrames,const int & angle,const float & size);
};

#endif //IMAGE_DISPLAY
