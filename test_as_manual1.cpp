#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;
#include "SiftGPU/SiftGPU.h"

int main(void)
{
SiftGPU sift;
char *argv[]={"-fo","-l","-v","l"};
//-fo -l,starting from -l octave
//-v l,  only print out # feature and overall time
sift.ParseParam(4,argv);

int support = sift.CreateContextGL();
//call VerfifyContexGL instead if using your own GL context
//int support = sift.VerifyContextGL();
if(support!=SiftGPU::SIFTGPU_FULL_SUPPORTED) return 0;

if(sift.RunSIFT("640-1.jpg")) sift.SaveSIFT("640-1.sift");

sift.RunSIFT("640-2.jpg");
int num=sift.GetFeatureNum();

//allocate memory for readback
//read back keypoints and normalized descritpros
vector<float> descriptors(128*num);
//descriptors: a K-by-128 matrix, where each row gives an invariant descriptor for one of the K keypoints. 
vector<SiftGPU::SiftKeypoint> keys(num);

sift.GetFeatureVector(&keys[0],&descriptors[0]);
return 0;
}
