#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

#ifdef __cplusplus
extern "C" {
#endif


#if defined( _DEBUG )
#define DEBUG_ONLY( expr ) expr
#else
#define DEBUG_ONLY( expr )
#endif

#if ( ! defined( VERBOSE ) ) && defined( _DEBUG ) && 0
#define VERBOSE
#endif

#if defined( VERBOSE )
#define VERBOSE_ONLY( expr ) expr
#else
#define VERBOSE_ONLY( expr )
#endif


#if defined( _DEBUG )
void DebugPrintf( const char *buf, ... );
#endif

void DebugTextReset(void) ;
int  DebugTextGetNumLines(void) ;
const char * DebugTextGetLine( int iLine ) ;
void DebugText( const char * buf, ... ) ;

#ifdef __cplusplus
}
#endif

#endif
