#define BALL_MANAGER_CPP
#include <assert.h>
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glut.h>
#include "macros.h"
#include "list.h"
#include "baseTypes.h"
#include "gameFramework.h"	
#include "collinfo.h"
#include "object.h"
#include "Ball.h"
#include "UserInterface.h"
#include "BallManager.h"
#include "random.h"
#include "memory.h"
#include "heapSizes.h"
#include "heapmanager.h"

UserInterfaceC* UserInterfaceC::sInstance=NULL;

UserInterfaceC *UserInterfaceC::CreateInstance()
{
	assert(sInstance==NULL);
	sInstance = FIEA_NEW(OTHER_HEAP,"User Interface Class") UserInterfaceC();
	return sInstance;
}

void UserInterfaceC::render()
{
	drawString(-1500,-1850,"Num Balls = %d",BallManagerC::GetInstance()->getNumBalls());
    int id;
    int age = BallManagerC::GetInstance()->getOldestBallsAge(&id);
	drawStringNoRaster("   Oldest Ball(%d) - %d frames",id,age);
	drawStringNoRaster("   Total Data Buffer Size- %d bytes",BallManagerC::GetInstance()->getTotalDataBufferSize());
}

void UserInterfaceC::drawString(uint32_t x, uint32_t y, char* format, ...)
{
    va_list args;
    char buffer[255], *s;
    va_start(args, format);
    vsprintf_s(buffer,255, format, args);
    va_end(args);
    
	glColor3ub(255, 255, 255);
    glRasterPos2i(x, y);
    for (s = buffer; *s; s++)
	{
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, *s);
	}
}
void UserInterfaceC::drawStringNoRaster(char* format, ...)
{
    va_list args;
    char buffer[255], *s;
    va_start(args, format);
    vsprintf_s(buffer,255, format, args);
    va_end(args);
    
    for (s = buffer; *s; s++)
	{
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, *s);
	}
}

void UserInterfaceC::rightMousePressed()
{
    HeapManagerC::GetHeapManager()->dumpAllHeaps();
}

