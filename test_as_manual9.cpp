#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;
#include <GL/gl.h> 
#include <unistd.h>
#include "SiftGPU/GlobalUtil.h"
#include "SiftGPU/SiftGPU.h"
#include "SiftGPU/GLTexImage.h"


/****Example #9, get the coordinates of the matched features***/
/*
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
*/
class SiftGPUimage:public SiftGPU
{
public:
	int getsize(char *_imgpath,int &w,int &h)
	{
		if(!_texImage->LoadImageFile(_imgpath, w,h)) return 0;
		return 1;
	}
};


vector<float> descriptors1, descriptors2;
vector<SiftGPU::SiftKeypoint> keys1, keys2;
int num1,num2;

int main(int argc, char *argv[])
{
	SiftGPUimage sift;
	
	//specify the naximum number of features to match
	SiftMatchGPU matcher(4096);
	//You can call matcher.SetMaxSift anytime to change this limit

	//char *argv[]={"-fo","-l","-v","l"};
	//-fo -l,starting from -l octave
	//-v l,  only print out # feature and overall time
	//sift.ParseParam(argc,argv);

	int support = sift.CreateContextGL();
	
	if(support!=SiftGPU::SIFTGPU_FULL_SUPPORTED) return 0;

    	if(matcher.VerifyContextGL() == 0) return 0;//must call once:matcher->VerifyContextGL();


	char *data;
	data="640-1.jpg";
	int w,h;
	sift.getsize(data,w,h);
	int half_w,half_h;
	half_w=(int)(0.5*w);
	half_h=(int)(0.5*h);
	if(sift.RunSIFT(data))
	{
		//get feature count
		num1 = sift.GetFeatureNum();

		//allocate memory
		keys1.resize(num1);	
		descriptors1.resize(128*num1);

		//read back feature vectors is faster than writing files
		sift.GetFeatureVector(&keys1[0], &descriptors1[0]);
		
 		for(int i=0;i<num1;i++)
		{
			keys1[i].x-=half_w;keys1[i].y-=half_h;
			//cout<<i<<": "<<keys1[i].x<<" "<<keys1[i].y<<endl;
		}

	}else{
		cerr<<" did not find first image!"<<endl;
	}

	data="640-5.jpg";
	sift.getsize(data,w,h);
	half_w=(int)(0.5*w);
	half_h=(int)(0.5*h);
	if(sift.RunSIFT(data))
	{
		num2=sift.GetFeatureNum();
		keys2.resize(num2); 	
                descriptors2.resize(128*num2);

		sift.GetFeatureVector(&keys2[0], &descriptors2[0]);

		for(int i=0;i<num2;i++)
		{
			keys2[i].x-=half_w;keys2[i].y-=half_h;
			//cout<<i<<": "<<keys2[i].x<<" "<<keys2[i].y<<endl;
		}

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
	for(int i=0;i<num_match;i++)
	{
		//the indices of the matched features:
		//cout<<i<<": "<<match_buf[i][0]<<" "<<match_buf[i][1]<<endl;

		//the x,y of the matched features:
		cout<<i<<": x "<<keys1[match_buf[i][0]].x<<", y "<<keys1[match_buf[i][0]].y<<";  x "<<keys2[match_buf[i][1]].x<<",y "<<keys2[match_buf[i][1]].y<<endl;
	}
	cout<<num_match<<" sift match were found"<<endl;


	return 0;
}


