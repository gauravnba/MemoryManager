/* macros.h: useful macros
//
//
// AUTHOR
//   Written and Copyright (C) 1999-2006 by Michael J. Gourlay
*/
#ifndef MACROS_H
#define MACROS_H

#include <cassert>


#if ( ! defined( UNIT_TEST ) ) && defined( _DEBUG )
#define UNIT_TEST
#endif

#define ABS(x)             (((x)<0)?(-(x)):(x))
#define SIGN(x)            (((x)<0)? (-1) :(1))

#define MIN2(x,y)          (((x)<(y))?(x):(y))
#define MIN3(x,y,z)        MIN2(MIN2((x),(y)),(z))

#define MAX2(x,y)          (((x)>(y))?(x):(y))
#define MAX3(x,y,z)        MAX2(MAX2((x),(y)),(z))

#define CLAMP( x , min , max )   MIN2( MAX2( x , min ) , max )

#define LITTLE_ENDIAN_ARCH (*((short*)"A") == 'A')

#ifdef __cplusplus
static const float PI       = 3.1415926535897932384626433832795f ;
static const float TWO_PI   = 2.0f * PI ;
#else
#define PI      3.1415926535897932384626433832795f
#define TWO_PI  ( 2.0f * PI )
#endif

static const float RAD2DEG  = (180.0f / PI) ;
static const float DEG2RAD  = (PI / 180.0f) ;

/* Token macros: for converting enums to strings and vice versa */
#define TOKEN_TO_STRING(tok,var)  if((var) == (tok)) return #tok
#define STRING_TO_TOKEN(tok,var)  if(!strcmp(#tok,(var))) return tok

#if defined( __GNUC__ )
    // MinGW ASSERT exits from the program a little too gracefully -- no call stack from the debugger.
    // Crashing actually provides more info.
    #if defined( _DEBUG )
        #define ASSERT( condition ) { if( ! ( condition ) ) { int * pNull = 0 ; * pNull = 0 ; } }
    #else
        #define ASSERT( condition )
    #endif
#else
    #define ASSERT( condition ) assert( condition )
#endif

#if defined( __GNUC__ ) // GNU C++ compiler requires a different syntax for defining static member variables of template classes.
    #define TEMPLATE_DECL template <class T> 
#else
    #define TEMPLATE_DECL
#endif

#if ! defined( _XBOX )
    typedef unsigned char   BYTE ;
    typedef short           SHORT ;
    typedef unsigned short  WORD ;
    typedef unsigned long   DWORD ;
    typedef int             BOOL ;
    typedef float           FLOAT ;
#endif

#if ! defined( _XBOX )
    #define OPENGL
#endif

#endif
