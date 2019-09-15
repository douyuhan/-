#include "gray_sobel.h"
#include "loadbmp.h"
#include <fstream>
#include <sys/time.h>
#include <string.h>
#include <iostream>

#define CLOCK_FREQ 800 //in MHz

char inputfilename[100];
char outputfilename[100];

using namespace std;
int main(int argc, char const *argv[])
{	
	ofstream outputfile("time_data.txt");

	unsigned char *pixels = NULL;
	unsigned int width, height; 
	struct timeval start,end;

	char c[2] = "0";
	for (int i = 0; i < 7; ++i)
	{
		strcpy(inputfilename,"images/");
		strcat(inputfilename,c);
		strcat(inputfilename,".bmp");

///////////////////BMP -> CHAR[]//////////////////
		unsigned int err = loadbmp_decode_file(inputfilename, &pixels, &width, &height, LOADBMP_RGBA);
		if (err){
			std::cout << "LoadBMP Load Error: " << err << std::endl;
			return err;
		}

//////////////////GRAY /////////////////////////
		grayscale(pixels,height,width,LOADBMP_RGBA);
		std::cout << "->Grayscale done"<< std::endl;
//////////////////SOBEL//////////////////////////////
		unsigned char * sobel = new unsigned char [width * height * LOADBMP_RGBA];

		gettimeofday(&start,NULL);
		sobel_filter(pixels,height,width,LOADBMP_RGBA,sobel);
		gettimeofday(&end,NULL);

		std::cout <<"->Sobel done"<<std::endl;

//////////////////CHAR[] -> BMP//////	////////////////
		strcpy(outputfilename,"images/");
		strcat(outputfilename,c);
		strcat(outputfilename,"_reencoded.bmp");
		puts(outputfilename);
		err = loadbmp_encode_file(outputfilename, sobel, width, height, LOADBMP_RGBA);
		if (err)	
		{			
			std::cout << "LoadBMP Save Error: " << err << std::endl;
			return err;
		}
		delete []pixels;
		delete []sobel;
//////////////////TIME///////////////////////////////
		long double time_exe = (long double)(((end.tv_sec - start.tv_sec)*1000000 + end.tv_usec) - start.tv_usec);
		std::cout << "width : " << width << " , height : " << height << std::endl;
		std::cout << "Sobel time : " << std::fixed <<time_exe << "  micro-seconds" << std::endl;
		std::cout << "ticks : " << std::fixed << time_exe  * CLOCK_FREQ<< std::endl;
		std::cout << "ticks per pixel : " << std::fixed << time_exe * CLOCK_FREQ / (height*width) <<std::endl;
		std::cout << std::endl << std::endl;

		outputfile << width <<" "<< height <<" "<<std::fixed << time_exe << " " <<std::fixed << time_exe*CLOCK_FREQ/(height*width) << endl;
		c[0] +=1 ;	
	}
	return 0;
}