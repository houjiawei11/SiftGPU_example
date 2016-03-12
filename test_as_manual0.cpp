#include <stdlib.h>
#include <iostream>
using namespace std;
#include "SiftGPU/SiftGPU.h"
int main(void)
{
SiftGPU sift;
char *argv[]={"-fo","-l","-v","l"};
sift.ParseParam(4,argv);
int support = sift.CreateContextGL();
if(support!=SiftGPU::SIFTGPU_FULL_SUPPORTED) return 0;
return 0;
}
