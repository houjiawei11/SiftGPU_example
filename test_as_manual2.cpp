#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;
#include "SiftGPU/SiftGPU.h"

#include <GL/gl.h> 

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

//myimg-0.jpg:580*304
int width=580,height=304;
const char *data = "myimg-0.jpg";
//如果给char*传递一个常量字符串，则不可用 unsigned char 或 char 定义，而要用const char定义
  sift.RunSIFT(width, height, data, GL_LUMINANCE, GL_UNSIGNED_BYTE);
//sift.RunSIFT(width,height,data,GL_RGBA,GL_UNSIGNED_BYTE);
//使用GL_LUMINANCE, GL_UNSIGNED_BYTE等GL_前缀的参数需要包含头文件<GL/gl.h>
//Using GL_LUMINANCE data saves transfer time

return 0;
}


