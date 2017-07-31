#define GAME_CPP
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
#include "collInfo.h"
#include "object.h"
#include "ball.h"
#include "field.h"
#include "random.h"
#include "openGLStuff.h"
#include "game.h"
#include "StateManager.h"
#include "BallManager.h"
#include "FieldManager.h"
#include "UserInterface.h"
#include "memory.h"
#include "HeapManager.h"
#include "HeapSizes.h"

// Declarations
const char8_t CGame::mGameTitle[]="Memory Manager";
CGame* CGame::sInstance=NULL;
BOOL Initialize (GL_Window* window, Keys* keys)					// Any OpenGL Initialization Goes Here
{
    HeapManagerC::GetHeapManager()->init(heaps,NUM_HEAPS,DEFAULT_HEAP);
    initOpenGLDrawing(window,keys,0.0f, 0.0f, 0.0f);
	CGame::CreateInstance();
	CGame::GetInstance()->init();
	return TRUE;						
}

void CGame::init()
{
	BallManagerC::CreateInstance();
	StateManagerC::CreateInstance();
	FieldManagerC::CreateInstance();
	UserInterfaceC::CreateInstance();

	BallManagerC::GetInstance()->init();
	StateManagerC::GetInstance()->setState(StateManagerC::HALF_BALLS_FILLED);
	FieldManagerC::GetInstance()->init();
}
void CGame::UpdateFrame(DWORD milliseconds)			
{
	keyProcess();
	BallManagerC::GetInstance()->updateBalls(milliseconds);
}

void CGame::DrawScene(void)											
{
	startOpenGLDrawing();
	BallManagerC::GetInstance()->renderBalls();
	FieldManagerC::GetInstance()->renderField();
	UserInterfaceC::GetInstance()->render();
}


CGame *CGame::CreateInstance()
{

	sInstance = FIEA_NEW(SYSTEM_HEAP,"Game Instance") CGame();
	return sInstance;
}
void CGame::shutdown()
{
	BallManagerC::GetInstance()->shutdown();
	StateManagerC::GetInstance()->shutdown();
	FieldManagerC::GetInstance()->shutdown();
}
void CGame::DestroyGame(void)
{
	delete BallManagerC::GetInstance();	
	delete StateManagerC::GetInstance();	
	delete FieldManagerC::GetInstance();	
}