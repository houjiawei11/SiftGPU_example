#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;
#include "SiftGPU/SiftGPU.h"

#include <GL/gl.h> 

#include <unistd.h>

/****Example #7, Compute (guided) putative sift matches.***/

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

#define CUDA_SIFTGPU_ENABLED 1


int main(int argc, char *argv[])
{
	SiftGPUEX sift;
	//specify the naximum number of features to match
	SiftMatchGPU matcher(4096);
	//You can call matcher.SetMaxSift anytime to change this limit

//	char *argv[]={"-fo","-l","-v","l"};
	//-fo -l,starting from -l octave
	//-v l,  only print out # feature and overall time
	sift.ParseParam(argc,argv);

	int support = sift.CreateContextGL();
	//call VerfifyContexGL instead if using your own GL context
	//int support = sift.VerifyContextGL();. I don't know why I have both packages available, but the following one
	if(support!=SiftGPU::SIFTGPU_FULL_SUPPORTED) return 0;


	//Before initialization, you can choose between glsl, and CUDA(if compiled). 
    	//matcher.SetLanguage(SiftMatchGPU::SIFTMATCH_CUDA); // my computer doesn't support CUDA

	//Verify current OpenGL Context and initialize the Matcher;
	//If you don't have an OpenGL Context, call matcher->CreateContextGL instead,but we have anOpenGL Context
    	if(matcher.VerifyContextGL() == 0) return 0;//must call once:matcher->VerifyContextGL();

	vector<float> descriptors1, descriptors2;
	vector<SiftGPU::SiftKeypoint> keys1, keys2;
	int num1,num2;

	if(sift.RunSIFT("640-1.jpg"))
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

	if(sift.RunSIFT("640-5.jpg"))
	{
		num2=sift.GetFeatureNum();
		keys2.resize(num2); 	
                descriptors2.resize(128*num2);

		sift.GetFeatureVector(&keys2[0], &descriptors2[0]);

	}else{
		cerr<<" did not find second image!"<<endl;
	}
	
	//Set two sets of descriptor data to the matcher
	matcher.SetDescriptors(0, num1, &descriptors1[0]);
	matcher.SetDescriptors(1, num2, &descriptors2[0]);

	//match and get result.    
	int match_buf[num1][2];
	//use the default thresholds. Check the declaration in SiftGPU.h
	int num_match = matcher.GetSiftMatch(num1,match_buf);
	cout<<num_match<<" sift match were found"<<endl;


	return 0;
}


