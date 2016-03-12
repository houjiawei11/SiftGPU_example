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

const char * files[4]={"640-1.jpg","640-2.jpg","640-3.jpg","640-4.jpg"};
sift.SetImageList(4,files);
sift.RunSIFT(1);
sift.RunSIFT(0);

return 0;
}


