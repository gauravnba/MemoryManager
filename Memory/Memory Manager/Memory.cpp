
#include <assert.h>
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#define MEMORY_CPP
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glut.h>
#include "macros.h"
#include "list.h"
#include "HeapManager.h"
#include "memory.h"
#include "heapsizes.h"

#define MEMORY_MANAGEMENT_ENABLED 1

void *operator new(size_t uiSize)
{
#if !MEMORY_MANAGEMENT_ENABLED
    return malloc(uiSize);
#else
	return HeapManagerC::GetHeapManager()->allocateBlock(DEFAULT_HEAP,uiSize,__FILE__,__LINE__,"Default New",0);
#endif
}
void *operator new(size_t uiSize, int heapID, char *file, int lineNumber, char *tag, int time, int alignment)
{
#if !MEMORY_MANAGEMENT_ENABLED
    return malloc(uiSize);
#else
	return HeapManagerC::GetHeapManager()->allocateBlock(heapID,uiSize,file,lineNumber,tag,time,alignment);
#endif
}


void operator delete(void *pvAddr,int heapID,char *file, int lineNumber,char *tag, int time,int alignment)
{
#if !MEMORY_MANAGEMENT_ENABLED
    free(pvAddr);
#else
     HeapManagerC::GetHeapManager()->freeBlock(pvAddr);
#endif
}
void operator delete(void *pvAddr)
{
#if !MEMORY_MANAGEMENT_ENABLED
    free(pvAddr);
#else
    HeapManagerC::GetHeapManager()->freeBlock(pvAddr);
#endif
}
