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
#include "BallManager.h"
#include "random.h"
#include "heapSizes.h"
#include "HeapManager.h"
#include "memory.h"


BallManagerC* BallManagerC::sInstance=NULL;

BallManagerC *BallManagerC::CreateInstance()
{
	assert(sInstance==NULL);
	sInstance = FIEA_NEW(BALL_HEAP,"Ball Manager") BallManagerC();
	return sInstance;
}

void BallManagerC::init()
{
    mCreateID = 0;
}

float_t BallManagerC::getCreateProbability()
{
	return 100.0f - (float_t)((getNumBalls()*100)/MAX_BALLS);
}

BallC *BallManagerC::getRandomBall()
{
	if(getNumBalls()==0) return NULL;
	if(getNumBalls()==1) return mBallList.Head()->GetItem();

	int sizeOfList = mBallList.Size();
	int whichBall = getRangedRandom(0,getNumBalls()-1);
	
	LinkNode<BallC *> *theBall;
	int ballCount = 0;
	for(theBall = mBallList.Head(); theBall != NULL; theBall = theBall->GetLink())
	{
		if(ballCount == whichBall)return theBall->GetItem();
		ballCount++;
	}
	ASSERT(0);				//problem finding the ball...
	return NULL;
}

void BallManagerC::createBall()
{
	BallC *ballPtr;

	ballPtr = FIEA_NEW(OTHER_HEAP,"ball") BallC(0.0f, 0.0f,getRangedRandom(-30.0f,30.0f),getRangedRandom(-30.0f,30.0f),mCreateID);
	ballPtr->setRandomColor();
    ballPtr->setRadius(getRangedRandom(10.0f,150.0f));
    ballPtr->enable();

    mCreateID++;

	mBallList.PushFront(ballPtr);

}
void BallManagerC::deleteBall()
{
	BallC *ballToDelete = getRandomBall();
	mBallList.Remove(ballToDelete);
    ballToDelete->setBallColor(0,0,0);
	FIEA_DELETE(ballToDelete);
}

void BallManagerC::handleCreationDeletion()
{
	bool createBallFlag = FALSE;
	bool deleteBallFlag = FALSE;

	if(getNumBalls() <= 0)
	{
		createBallFlag = TRUE;
	}
	else if(getNumBalls() >= MAX_BALLS)
	{
		createBallFlag = FALSE;
		deleteBallFlag = TRUE;
	}
	else
	{
		if(getRangedRandom(0,100) < getCreateProbability())
		{
			createBallFlag = TRUE;
		}
		else
		{
			deleteBallFlag = TRUE;
		}
	}
	if(createBallFlag)
	{
		createBall();
	}
	if(deleteBallFlag)
	{
		deleteBall();
	}
}

void BallManagerC::updateBalls(DWORD milliseconds)
{
	handleCreationDeletion();

	LinkNode<BallC *> *theBall;
	for(theBall = mBallList.Head(); theBall != NULL; theBall = theBall->GetLink())
	{
		theBall->GetItem()->update(milliseconds);
    }
}
void BallManagerC::renderBalls()
{
	LinkNode<BallC *> *theBall;
	for(theBall = mBallList.Head(); theBall != NULL; theBall = theBall->GetLink())
	{
		theBall->GetItem()->render();
    }
}

uint32_t BallManagerC::getOldestBallsAge(int *id)
{
	uint32_t oldestAge = 0;
	LinkNode<BallC *> *theBall;

	for(theBall = mBallList.Head(); theBall != NULL; theBall = theBall->GetLink())
	{
		if(theBall->GetItem()->getAge() > oldestAge)
        {
			oldestAge = theBall->GetItem()->getAge();
            *id = theBall->GetItem()->getID();
        }
	}
	return oldestAge;
}
uint32_t BallManagerC::getTotalDataBufferSize()
{
	uint32_t totalBytes = 0;
	LinkNode<BallC *> *theBall;

	for(theBall = mBallList.Head(); theBall != NULL; theBall = theBall->GetLink())
	{
		totalBytes += theBall->GetItem()->getDataBufferSize();
	}
	return totalBytes;
}
void BallManagerC::shutdown()
{
	mBallList.Obliterate();
}