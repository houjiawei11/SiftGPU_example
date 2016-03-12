#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;
#include "SiftGPU/SiftGPU.h"

#include <GL/gl.h> 

#include <unistd.h>

/****Example #6, Compute descriptor for user-specified keypoints***/
#if !defined(SIFTGPU_STATIC) && !defined(SIFTGPU_DLL_RUNTIME) 
// SIFTGPU_STATIC comes from compiler
#define SIFTGPU_DLL_RUNTIME
// Load at runtime if the above macro defined
// comment the macro above to use static linking
#endif

    #ifdef SIFTGPU_DLL_RUNTIME
//        #include <dlfcn.h>
        #define FREE_MYLIB dlclose
//        #define GET_MYPROC dlsym
    #endif

/***Method 1, set new keypoints for the image you've just processed with siftgpu*/
void LoadMyKeyPoints1(SiftGPU& sift, vector<SiftGPU::SiftKeypoint>& keys2, int num2, vector<SiftGPU::SiftKeypoint>& mykeys)
{
	sift.RunSIFT(mykeys.size(),&mykeys[0]);
	sift.RunSIFT(num2,&keys2[0],1);		
        sift.SaveSIFT("../640-1.sift.2");
	sift.RunSIFT(num2,&keys2[0],0);		
        sift.SaveSIFT("../640-1.sift.3");
}

int main(int argc, char *argv[])
{
	SiftGPUEX sift;

        //sift.SetView(0,0,"Original Image");

	//SiftGPU* (*pCreateNewSiftGPU)(int)=NULL;

	//HMODULE hsiftgpu = LoadLibrary("siftgpu.dll");
	//Load siftgpu dll... use dlopen in linux/mac
	//void * hsiftgpu = dlopen("libsiftgpu.so", RTLD_LAZY);
	//if(hsiftgpu == NULL) return 0;

	// pCreateNewSiftGPU =(SiftGPU*(*)(int)) GetProcAddress(hsiftgpu,"CreateNewSiftGPU");
	// pCreateNewSiftGPU = (SiftGPU* (*) (int)) GET_MYPROC(hsiftgpu, "CreateNewSiftGPU");
	//SiftGPU * psift = pCreateNewSiftGPU(1);

//	char *argv[]={"-fo","-l","-v","l"};
	//-fo -l,starting from -l octave
	//-v l,  only print out # feature and overall time
	sift.ParseParam(argc,argv);

	int support = sift.CreateContextGL();
	//call VerfifyContexGL instead if using your own GL context
	//int support = sift.VerifyContextGL();. I don't know why I have both packages available, but the following one
	if(support!=SiftGPU::SIFTGPU_FULL_SUPPORTED) return 0;

	vector<float> descriptors1, descriptors2;
	vector<SiftGPU::SiftKeypoint> keys1, keys2;
	int num1,num2;

	if(sift.RunSIFT("640-5.jpg"))
	{
		//get feature count
		num1 = sift.GetFeatureNum();

		//allocate memory
		keys1.resize(num1);	
		descriptors1.resize(128*num1);

		//read back feature vectors is faster than writing files
		sift.GetFeatureVector(&keys1[0], &descriptors1[0]);
 

	}else{
		cerr<<" did not find first image!"<<endl;
	}

	if(sift.RunSIFT("640-1.jpg"))
	{
		num2=sift.GetFeatureNum();
		keys2.resize(num2); 	
                descriptors2.resize(128*num2);

		sift.GetFeatureVector(&keys2[0], &descriptors2[0]);

	}

	//compute descriptors for user-specified keypoints (with or without orientations)
	vector<SiftGPU::SiftKeypoint> mykeys;
	//LoadMyKeyPoints1(sift, keys2, num2, mykeys);

	sift.RunSIFT(mykeys.size(),&mykeys[0]);

	sift.RunSIFT(num2,&keys2[0],1);		
	sift.SaveSIFT("../640-1.sift.2");

	sift.RunSIFT(num2,&keys2[0],0);		
	sift.SaveSIFT("../640-1.sift.3");
	
	return 0;
}


