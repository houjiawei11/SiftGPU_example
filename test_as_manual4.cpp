#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;
#include "SiftGPU/SiftGPU.h"

#include <GL/gl.h> 

/****example3:specify a set of image inputs using SetImageList***/
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

sift.AllocatePyramid(1024,1024);
sift.RunSIFT("800-4.jpg");

return 0;
}


