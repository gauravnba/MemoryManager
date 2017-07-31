#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl/glut.h>
#include "baseTypes.h"
#include "macros.h"
#include "list.h"
#include "memory.h"
#include "ShapeDraw.h"
#include "collInfo.h"
#include "object.h"
#include "ball.h"
#include "field.h"
#include "fieldmanager.h"
#include "random.h"
#include "stateManager.h"
#include "heapManager.h"
#include "heapSizes.h"
#include "memory.h"

//memory buffer control parameters
#define RANDOM_MEMORY_EVENT_CHANCE (10)
#define MIN_MEMORY_SIZE (1024)
#define MAX_MEMORY_SIZE (20*1024)


BallC::BallC()
{
    mEnabled = true;
    mCollInfo.shape = CollInfoC::SHAPE_CIRCLE;
	mDataBufferSize = 0;
	mDataBuffer = NULL;
};
BallC::BallC(float_t initPosX, float_t initPosY, float_t initVelX, float_t initVelY, int id) : ObjectC(initPosX,initPosY,initVelX,initVelY)
{
     mFramesSinceCreation = 0;
     mDataBufferSize = 0;
     mDataBuffer = NULL;
     setID(id);
 };
BallC::~BallC()
{
	if(mDataBuffer != NULL)
	{
		FIEA_FREE(mDataBuffer);
		mDataBuffer = NULL;
	}	
};

void BallC::update(DWORD milliseconds)
{
    mPosition.x += mVelocity.x*milliseconds/10;
    mPosition.y += mVelocity.y*milliseconds/10;
	this->addFrameSinceCreation();
	DoCollisions();
	unsigned char blue=0;
	int frames = this->mFramesSinceCreation;

	if(frames>510)frames=510;
	unsigned char red = (unsigned char)(frames/2);
	unsigned char green = 255-(unsigned char)(frames/2);

	if(red>255)red=(char)255;
	if(green<0)green=0;

	setBallColor(red,green,blue);


	if(getRangedRandom(0,100) < RANDOM_MEMORY_EVENT_CHANCE)
	{
		if(mDataBuffer != NULL)
		{
			FIEA_FREE(mDataBuffer);
			mDataBuffer = NULL;
			mDataBufferSize = 0;
		}
		else
		{
			mDataBufferSize = getRangedRandom(MIN_MEMORY_SIZE,MAX_MEMORY_SIZE);
			mDataBuffer = FIEA_ALLOC_ALIGNED(mDataBufferSize,BALL_HEAP,"Ball Data Buffer",1024);
		}
	}
}
void BallC::DoCollisions()
{
	CollideField();
}
void BallC::CollideField(void)
{
	FieldC *field = FieldManagerC::GetInstance()->getFieldPtr();

	float_t rightSide = field->getPosition()->x+((float_t)field->getWidth()/2.0f);
	float_t leftSide = field->getPosition()->x-((float_t)field->getWidth()/2.0f);
	float_t topSide = field->getPosition()->y+((float_t)field->getHeight()/2.0f);
	float_t bottomSide = field->getPosition()->y-((float_t)field->getHeight()/2.0f);
	
	if(mPosition.x- mRadius <= leftSide  )
	{
		mVelocity.x = -mVelocity.x;
		mPosition.x = leftSide + mRadius;
	}
	if(mPosition.x+ mRadius >= rightSide  )
	{
		mVelocity.x = -mVelocity.x;
		mPosition.x = rightSide - mRadius;
	}
	if(mPosition.y- mRadius <= bottomSide  )
	{
		mVelocity.y = -mVelocity.y;
		mPosition.y = bottomSide + mRadius;
	}
	if(mPosition.y+ mRadius >= topSide  )
	{
		mVelocity.y = -mVelocity.y;
		mPosition.y = topSide - mRadius;
	}
};
void BallC::render()
{
	char8_t red = (char8_t)((mBallColor>>16)& 0xFF);
	char8_t green = (char8_t)((mBallColor>>8) & 0xFF);
	char8_t blue = (char8_t)((mBallColor>>0) & 0xFF);
	bool filledVal;

	switch(StateManagerC::GetInstance()->getState())
	{
		case StateManagerC::ALL_BALLS_FILLED:
			filledVal = true;
			break;
		case StateManagerC::HALF_BALLS_FILLED:
			filledVal = (mID%2)?true:false;
			break;
		case StateManagerC::NO_BALLS_FILLED:
			filledVal = false;
			break;
	}
	DrawCircle(mRadius,mPosition.x,mPosition.y,red,green,blue,filledVal);

}
void BallC::setRandomColor()
{
	mBallColor = getRangedRandom(0,256);
	mBallColor += getRangedRandom(0,256) << 8;
	mBallColor += getRangedRandom(0,256) << 16;
}
void BallC::setBallColor(unsigned char r, unsigned char g, unsigned char b)
{
	mBallColor = b;
	mBallColor += g << 8;
	mBallColor += r << 16;
}

