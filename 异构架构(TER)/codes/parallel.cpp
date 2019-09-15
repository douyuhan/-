#include <iostream>
#include "defines.h"
#include <sys/time.h>

#include "AOCLUtils/aocl_utils.h"

#include "opencv/cv.hpp"
#include <opencv/highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>

#include <thread>

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
void filter_ARM_1()
{
	struct timeval begin_l,end_l;
	long elapsed_secs_l;
	gettimeofday(&begin_l, 0);
	// Filter coefficients
    int Gx[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
    int Gy[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    
    for(int i=0; i<IMG_ROWS; i++)
    {
		for(int j=0; j<IMG_COLS/2; j++)
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
	std::cout <<"parallel thread 1 time : " << elapsed_secs_l<< " micro-seconds\n";
}



void filter_ARM_2()
{
	struct timeval begin_l,end_l;
	long elapsed_secs_l,  ;
	gettimeofday(&begin_l, 0);
	// Filter coefficients
    int Gx[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
    int Gy[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    
    for(int i=0; i<IMG_ROWS; i++)
    {
		for(int j=IMG_COLS/2; j<IMG_COLS; j++)
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
	std::cout <<"parallel thread 2 time : " << elapsed_secs_l<< " micro-seconds\n";
}


void filter_ARM()
{
	struct timeval begin_l,end_l;
	long elapsed_secs_l,  ;
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
	std::cout <<"single thread time : " << elapsed_secs_l<< " micro-seconds\n";
}
//*neccessary*//
int main(int argc, const  char argv [])
{
	if (argc < 2){
		std::cout <<"Usage : parallel_sobel image.jpeg"<<"\n";
		return -1;
	}
	
	Mat image_input ;
	image_input = imread(argv[1],CV_LOAD_IMAGE_COLOR);

	unsigned char *raw_ptr = (unsigned char *)image_input.data;
    unsigned char *data_ptr = (unsigned char *)input;
    
	for (int i = 0, e = IMG_COLS * IMG_ROWS; i != e; ++i) {
    // Read bgr and pad
    *data_ptr++ = *raw_ptr++;//bleu
    *data_ptr++ = *raw_ptr++;//vert
    *data_ptr++ = *raw_ptr++;//rouge
    *data_ptr++ = 0;//reste à 0
	}
	//filter on a single thread
	filter_ARM();

	Mat image_out_arm = Mat(IMG_ROWS,IMG_COLS,CV_8UC1);
	unsigned char *out_ptr = (unsigned char *)image_out_CL.data;
	
	for(int i=0;i<IMG_ROWS;i++)
	{
		for(int j=0;j<IMG_COLS;j++)
		{
			if(output_ARM[i*IMG_COLS+j] == 0) { out_ptr[i*IMG_COLS+j] = 0;}
			else {out_ptr[i*IMG_COLS+j] = 255;}
		}
	}
	
	imwrite("ARm_single_thread.jpeg",image_out_arm);
	
	// thread creation	
	std::thread thread1(filter_ARM_1),thread2(filter_ARM_2);
	//synchronization
	thread1.join();
	thread2.join();

	Mat image_out_arm_parallel = Mat(IMG_ROWS,IMG_COLS,CV_8UC1);
	unsigned char *out_ptr = (unsigned char *)image_out_arm_parallel.data;
	
	for(int i=0;i<IMG_ROWS;i++)
	{
		for(int j=0;j<IMG_COLS;j++)
		{
			if(output_ARM[i*IMG_COLS+j] == 0) { out_ptr[i*IMG_COLS+j] = 0;}
			else {out_ptr[i*IMG_COLS+j] = 255;}
		}
	}
	
	imwrite("ARM_parallel.jpeg",image_out_arm_parallel);

	std::cout << "IMAGE IMG_ROWS  "<<IMG_ROWS<<  " , IMG_COLS: " <<IMG_COLS;
	teardown(0);
	return 0;
}


//*neccessary*//
void initCL()
{
	cl_int status;
	if (!setCwdToExeDir())
		teardown();
	
	platform = findPlatform("Altera");
	if (platform == NULL) 
		teardown();

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