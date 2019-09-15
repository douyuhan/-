
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "parse_ppm.h"
#include "defines.h"
#include <sys/time.h>
#include "AOCLUtils/aocl_utils.h"

#include "opencv/cv.hpp"
#include <opencv/highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>

using namespace aocl_utils;
using namespace cv;

unsigned int thresh = 128;


cl_uchar *input = NULL;
cl_uchar *output = NULL;
cl_uchar *output_ARM = NULL;
cl_uchar *inputYUV=NULL;
cl_uchar *outputYUV=NULL;
cl_uchar *outputYUV_ARM = NULL;

cl_uint num_platforms;
cl_platform_id platform;
cl_uint num_devices;
cl_device_id device;
cl_context context;
cl_command_queue queue;
cl_program program;
cl_kernel kernel;

cl_mem in_buffer, out_buffer;


std::string aocxFilename;
std::string deviceInfo;

void teardown(int exit_status = 1);
void initCL();


//*neccessary*//
void filter_ARM()
{
	struct timeval begin_l,end_l, begin_tt, end_tt;
	long elapsed_secs_l, elapsed_secs_tt;
	gettimeofday(&begin_l, 0);
	// Filter coefficients
    int Gx[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
    int Gy[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    
    for(int i=0; i<IMG_ROWS; i++)
    {
		for(int j=0; j<IMG_COLS; j++)
		{
			if( (i==0)||(i==IMG_ROWS-1)||(j==0)||(j==IMG_COLS-1) )
				output_ARM[i*IMG_COLS+j] = 0;
			else
			{
				 int x_dir = 0;
				 int y_dir = 0;				 
				 for(int ii=0; ii<3; ii++)
				 {
					 for(int jj=0; jj<3; jj++)
					 {
						unsigned char pixel = input[(i+ii-1)*IMG_COLS + (j+jj-1)];
					
						unsigned int luma = (unsigned int)pixel;
					
						x_dir += luma * Gx[ii][jj];
						y_dir += luma * Gy[ii][jj];
					 }
				 }
				 x_dir = x_dir>>1;
				 y_dir = y_dir>>1;
				 int temp = abs(x_dir) + abs(y_dir);
				 if (temp > thresh) 
					output_ARM[i*IMG_COLS+j] = 0xff;
				 else 
					output_ARM[i*IMG_COLS+j] = 0;
			}
		}
	}
	
gettimeofday(&end_l, 0);
elapsed_secs_l = (end_l.tv_sec-begin_l.tv_sec)*1000000 + end_l.tv_usec-begin_l.tv_usec;
printf("%ld \n",elapsed_secs_l);

}


void filter()
{
	struct timeval begin_l,end_l;
	long elapsed_secs_l;

	size_t sobelSize = 1;
	cl_int status;

	gettimeofday(&begin_l, 0);

	status = clEnqueueWriteBuffer(queue, in_buffer, CL_FALSE, 0, sizeof(unsigned char) * IMG_ROWS * IMG_COLS, input, 0, NULL, NULL);
	checkError(status, "Error: could not copy data into device");

	status = clFinish(queue);
	checkError(status, "Error: could not finish successfully");

	gettimeofday(&end_l, 0);
	elapsed_secs_l = (end_l.tv_sec-begin_l.tv_sec)*1000000 + end_l.tv_usec-begin_l.tv_usec;
	std::cout <<"CPU to FPGA "<<elapsed_secs_l <<"\n";	

	status = clSetKernelArg(kernel, 3, sizeof(unsigned int), &thresh);
	checkError(status, "Error: could not set sobel threshold");

	cl_event event;

	gettimeofday(&begin_l, 0); 

	status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &sobelSize, &sobelSize, 0, NULL, &event);


	status  = clFinish(queue);
	checkError(status, "Error: could not finish successfully");

	gettimeofday(&end_l, 0);
	elapsed_secs_l = (end_l.tv_sec-begin_l.tv_sec)*1000000 + end_l.tv_usec-begin_l.tv_usec;
	std::cout <<"Execution time : "<<elapsed_secs_l <<"\n";	


	gettimeofday(&begin_l, 0); 

	status = clEnqueueReadBuffer(queue, out_buffer, CL_FALSE, 0, sizeof(unsigned char) * IMG_ROWS * IMG_COLS, output, 0, NULL, NULL);
	checkError(status, "Error: could not copy data from device");
	status = clFinish(queue);
	checkError(status, "Error: could not successfully finish copy");


	gettimeofday(&end_l, 0);
	elapsed_secs_l = (end_l.tv_sec-begin_l.tv_sec)*1000000 + end_l.tv_usec-begin_l.tv_usec;
	std::cout <<"FPGA to CPU "<<elapsed_secs_l <<"\n";	
}





//*neccessary*//
int main(int argc , const char * argv [])
{
	if (argc < 2 ){
		std::cout << "Usage : sobel_video file.yuv number_of_frames\n";
		return -1
	}

    char videoNameSobel[]="sobel_FPGA.yuv";
	char videoNameSobel_ARM [] = "sobel_ARM.yuv";
	
	FILE *f_video=fopen(argv[1],"rb");
	FILE *f_videoSobel=fopen(videoNameSobel,"wb");
	FILE *f_videoSobel_ARM = fopen(videoNameSobel_ARM,"wb");
	
	if(f_video==NULL)
		std::cout << "Failed to open video.yuv\n";
	if(f_videoSobel==NULL)
		std::cout << "Failed to open sobel_FPGA.yuv\n";
	if(f_videoSobel_ARM==NULL)
		std::cout << "Failed to open sobel_ARM.yuv\n";
	
    input = (cl_uchar*)alignedMalloc(sizeof(unsigned char) * IMG_ROWS * IMG_COLS);
	output = (cl_uchar*)alignedMalloc(sizeof(unsigned char) * IMG_ROWS * IMG_COLS);
	output_ARM = (cl_uchar*)alignedMalloc(sizeof(unsigned char) * IMG_ROWS * IMG_COLS);
	
	// format is 4:2:0 so we have resolution * 1.5
	inputYUV=(cl_uchar*)alignedMalloc(sizeof(unsigned char) * IMG_ROWS * IMG_COLS* 1.5);
	outputYUV=(cl_uchar*)alignedMalloc(sizeof(unsigned char) * IMG_ROWS * IMG_COLS* 1.5);
	outputYUV_ARM = (cl_uchar*)alignedMalloc(sizeof(unsigned char) * IMG_ROWS * IMG_COLS* 1.5);
	
	unsigned char yuv_y[IMG_COLS*IMG_ROWS];
	unsigned int pixels[IMG_COLS*IMG_ROWS];
	int numFrame = atoi(argv[2]); // convert your input to a number

	int ySize=IMG_COLS*IMG_ROWS;
	int uSize=(IMG_COLS*IMG_ROWS)/4;
	int vSize=(IMG_COLS*IMG_ROWS)/4;
	int frameSize=ySize+uSize+vSize;
	
	initCL();
		
	for(int frameIndex=0;frameIndex<numFrame;frameIndex++){
		fseek(f_video,frameIndex*frameSize,SEEK_SET);
		fread(inputYUV,1,frameSize,f_video);
		for(int i=0;i<IMG_COLS*IMG_ROWS;i++)
			input[i]=inputYUV[i];
		//filter on FPGA
		filter();		
		for(int i=0;i<frameSize;i++){
			if(i<IMG_COLS*IMG_ROWS){
				outputYUV[i]=output[i];
				outputYUV_ARM[i] = output_ARM[i];
			}
			else{
				outputYUV[i]=128;
				outputYUV_ARM[i] = 128;
			}
		}
		fwrite(outputYUV,1,frameSize,f_videoSobel);
	}
	
	rewind(f_video);
	for(int frameIndex=0;frameIndex<numFrame;frameIndex++){
		fseek(f_video,frameIndex*frameSize,SEEK_SET);
		fread(inputYUV,1,frameSize,f_video);
		for(int i=0;i<IMG_COLS*IMG_ROWS;i++)
			input[i]=inputYUV[i];
		//filter on ARM
		filter_ARM();
		
		for(int i=0;i<frameSize;i++){
			if(i<IMG_COLS*IMG_ROWS){
				outputYUV[i]=output[i];
				outputYUV_ARM[i] = output_ARM[i];
			}
			else{
				outputYUV[i]=128;
				outputYUV_ARM[i] = 128;
			}
		}
		fwrite(outputYUV_ARM,1,frameSize, f_videoSobel_ARM);
	}
	fclose(f_video);
	fclose(f_videoSobel);
	fclose(f_videoSobel_ARM);
	
	printf("IMAGE IMG_ROWS IMG_COLS:  %d   %d \n",IMG_ROWS,IMG_COLS);
	teardown(0);
	return 0;
}


//*neccessary*//
void initCL()
{
  cl_int status;

  if (!setCwdToExeDir()) {
    teardown();
  }
  
  printf("OK1 \n");

  platform = findPlatform("Altera");
  if (platform == NULL) {
    teardown();
  }
  
  printf("OK2 \n");

  status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);
  checkError (status, "Error: could not query devices");
  num_devices = 1; // always only using one device

  char info[256];
  clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(info), info, NULL);
  deviceInfo = info;

  context = clCreateContext(0, num_devices, &device, &oclContextCallback, NULL, &status);
  checkError(status, "Error: could not create OpenCL context");

  queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
  checkError(status, "Error: could not create command queue");

  std::string binary_file = getBoardBinaryFile("sobel", device);
  std::cout << "Using AOCX: " << binary_file << "\n";
  program = createProgramFromBinary(context, binary_file.c_str(), &device, 1);

  status = clBuildProgram(program, num_devices, &device, "", NULL, NULL);
  checkError(status, "Error: could not build program");

  kernel = clCreateKernel(program, "sobel", &status);
  checkError(status, "Error: could not create sobel kernel");


  in_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(unsigned char) * IMG_ROWS * IMG_COLS, NULL, &status);
  checkError(status, "Error: could not create device buffer");

  out_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(unsigned char) * IMG_ROWS * IMG_COLS, NULL, &status);
  checkError(status, "Error: could not create output buffer");

  int pixels = IMG_COLS * IMG_ROWS;

  status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &in_buffer);
  checkError(status, "Error: could not set sobel arg 0");
  status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &out_buffer);
  checkError(status, "Error: could not set sobel arg 1");

  status = clSetKernelArg(kernel, 2, sizeof(int), &pixels);
  checkError(status, "Error: could not set sobel arg 2");
  
  printf("OK3 \n");
  
}


//*neccessary*//
void cleanup()
{
  // Called from aocl_utils::check_error, so there's an error.
  teardown(-1);
}

//*neccessary*//
void teardown(int exit_status)
{

  if (input) alignedFree(input);
  if (output) alignedFree(output);
  if (in_buffer) clReleaseMemObject(in_buffer);
  if (out_buffer) clReleaseMemObject(out_buffer);

  if (kernel) clReleaseKernel(kernel);
  if (program) clReleaseProgram(program);
  if (queue) clReleaseCommandQueue(queue);
  if (context) clReleaseContext(context);

  //SDL_Quit();
  exit(exit_status);
}

