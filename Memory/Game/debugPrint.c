// Includes --------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> 

#include "macros.h"

#if defined( _XBOX )
#include <xtl.h>
#endif

#include "debugPrint.h"

// Macros --------------------------------------------------------------
// Variables --------------------------------------------------------------

#define DEBUG_TEXT_NUM_LINES_MAX 80
#define DEBUG_TEXT_WIDTH_MAX     256
static char      sDebugText[DEBUG_TEXT_NUM_LINES_MAX][DEBUG_TEXT_WIDTH_MAX] ;
static int       sDebugTextLine = 0 ;

// Functions --------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name: DebugPrintf()
// Desc: For printing to the debugger with formatting.
//-----------------------------------------------------------------------------
void DebugPrintf( const char * buf, ... )
{
#if defined( _DEBUG )
    char strBuffer[1024];

    va_list arglist;
    va_start( arglist, buf );
    _vsnprintf_s( strBuffer, 1024,sizeof(strBuffer), buf, arglist );
    va_end( arglist );

    strBuffer[sizeof(strBuffer)-1] = '\0';
    #if defined( _XBOX )
        OutputDebugStringA( strBuffer );
    #else
        fprintf( stderr , strBuffer ) ;
    #endif
#endif
}




void DebugTextReset(void)
{
    sDebugTextLine = 0 ;
}




int DebugTextGetNumLines(void)
{
    return sDebugTextLine ;
}




const char * DebugTextGetLine( int iLine )
{
    ASSERT( iLine < sDebugTextLine ) ;
    return sDebugText[ iLine ] ;
}



/*
*/
void DebugText( const char * buf, ... )
{
//    ASSERT( sDebugTextLine < DEBUG_TEXT_NUM_LINES_MAX ) ;

    va_list arglist ;
    va_start( arglist, buf );
    _vsnprintf_s( sDebugText[ sDebugTextLine ], 256,DEBUG_TEXT_WIDTH_MAX , buf , arglist ) ;
    va_end( arglist );

    sDebugText[ sDebugTextLine ][ DEBUG_TEXT_WIDTH_MAX - 1 ] = '\0' ; // paranoid string terminator

    ++ sDebugTextLine ;
}
