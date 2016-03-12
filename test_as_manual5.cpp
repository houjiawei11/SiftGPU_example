#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;
#include "SiftGPU/SiftGPU.h"

#include <GL/gl.h> 

/****Example #5, runtime library loading***/
#if !defined(SIFTGPU_STATIC) && !defined(SIFTGPU_DLL_RUNTIME) 
// SIFTGPU_STATIC comes from compiler
#define SIFTGPU_DLL_RUNTIME
// Load at runtime if the above macro defined
// comment the macro above to use static linking
#endif

    #ifdef SIFTGPU_DLL_RUNTIME
        #include <dlfcn.h>
        #define FREE_MYLIB dlclose
        #define GET_MYPROC dlsym
    #endif
int main(void)
{
SiftGPU sift;
char *argv[]={"-fo","-l","-v","l"};
//-fo -l,starting from -l octave
//-v l,  only print out # feature and overall time
sift.ParseParam(4,argv);

int support = sift.CreateContextGL();
//call VerfifyContexGL instead if using your own GL context
//int support = sift.VerifyContextGL();. I don't know why I have both packages available, but the following one
if(support!=SiftGPU::SIFTGPU_FULL_SUPPORTED) return 0;

SiftGPU* (*pCreateNewSiftGPU)(int)=NULL;

//HMODULE hsiftgpu = LoadLibrary("siftgpu.dll");
//Load siftgpu dll... use dlopen in linux/mac
void * hsiftgpu = dlopen("libsiftgpu.so", RTLD_LAZY);
if(hsiftgpu == NULL) return 0;

// pCreateNewSiftGPU =(SiftGPU*(*)(int)) GetProcAddress(hsiftgpu,"CreateNewSiftGPU");
 pCreateNewSiftGPU = (SiftGPU* (*) (int)) GET_MYPROC(hsiftgpu, "CreateNewSiftGPU");
SiftGPU * psift = pCreateNewSiftGPU(1);
return 0;
}


