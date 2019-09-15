#include <iostream>
#include <CL/cl.h>
#include <stdlib.h>

using namespace std;
void afficherPlatformsInfo(cl_platform_id id,cl_platform_info name,std::string str){
    cl_int errNum;
    size_t paramValueSize;
    errNum=clGetPlatformInfo(id,name,0,NULL,&paramValueSize);
    if(errNum!=CL_SUCCESS){
        cerr<<"Step1:Failed to find Info of OpenCL platforms."<<endl;
        return;
    }
    char *info=(char*)alloca(sizeof(char)*paramValueSize);
    errNum=clGetPlatformInfo(id,name,paramValueSize,info,NULL);
    if(errNum!=CL_SUCCESS){
        cerr<<"Step2:Failed to find Info of OpenCL platforms."<<endl;
        return;
    }
    cout<<"\t"<<str<<":\t"<<info<<endl;

}
void trouverPlatforms(void){
    cl_int errNum;
    cl_uint numPlatforms;
    cl_platform_id *platformIds;
    errNum=clGetPlatformIDs(0,NULL,&numPlatforms);
    if(errNum!=CL_SUCCESS||numPlatforms<=0)
    {
        cerr<<"Step1:Failed to find any OpenCL platform!"<<endl;
        return;
    }
    platformIds=(cl_platform_id *)alloca(sizeof(cl_platform_id)*numPlatforms);
    errNum=clGetPlatformIDs(numPlatforms,platformIds,NULL);
    if(errNum!=CL_SUCCESS||numPlatforms<=0)
    {
        cerr<<"Step2:Failed to find any OpenCL platform!"<<endl;
        return;
    }
    cout<<"Number of platforms:\t"<<numPlatforms<<endl;
    for(cl_uint i=0;i<numPlatforms;i++){
        cout<<i+1<<" platform :"<<endl;
        afficherPlatformsInfo(platformIds[i],CL_PLATFORM_PROFILE,"CL_PLATFORM_PROFILE");
        afficherPlatformsInfo(platformIds[i],CL_PLATFORM_VERSION,"CL_PLATFORM_VERSION");
        afficherPlatformsInfo(platformIds[i],CL_PLATFORM_VENDOR,"CL_PLATFORM_VENDOR");
    }
}
int main()
{
    trouverPlatforms();
    return 0;
}
