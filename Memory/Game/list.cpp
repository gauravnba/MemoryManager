/*
    \file list.cpp

    \description singly linked list

    \author Copyright 2005-2006 UCF/FIEA/MJG; All rights reserved.
*/

#include <cassert>

#include "macros.h"
#include "DebugPrint.h"
#include "list.h"


/*  Specialization to print list of integer items.
    Intended for diagnostics and unit tests.
*/
void SList<int>::Print(void) const
{
    #if defined( _DEBUG )
    DebugPrintf( "SList<ItemT>::Print: " ) ;
    for( LinkNode<int> * pNode = Head() ; pNode != NULL ; pNode = pNode->GetLink() )
    {
        int rItemInt = pNode->GetItem() ;
        DebugPrintf( "%i-->", rItemInt ) ;
        ASSERT( ( pNode->GetLink() != NULL ) || ( mTail == pNode ) ) ; // make sure tail points to last node
    }
    DebugPrintf("NULL\n" ) ;
    #endif
}




#if defined( UNIT_TEST )

void SList<int>::UnitTest(void)
{
    DEBUG_ONLY( DebugPrintf( "SList::UnitTest: ----------------------------------------------\n" ); )

    SList<int> linkedList ;

    ASSERT( linkedList.Size() == 0 ) ;
    ASSERT( linkedList.Find( 0 ) == NULL ) ;

    // Pass 1: insert nodes again and remove in reverse order
    DEBUG_ONLY( DebugPrintf( "SList::UnitTest: 1\n" ); )

    linkedList.PushFront( 0 ) ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.PushFront( 1 ) ; linkedList.Print() ; ASSERT( linkedList.Size() == 2 ) ;
    linkedList.Remove( 1 )    ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.Remove( 0 )    ; linkedList.Print() ; ASSERT( linkedList.Size() == 0 ) ;

    // Pass 2a: append nodes and remove in same order
    DEBUG_ONLY( DebugPrintf( "SList::UnitTest: 2a\n" ); )

    linkedList.PushBack( 0 ) ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.PushBack( 1 ) ; linkedList.Print() ; ASSERT( linkedList.Size() == 2 ) ;
    linkedList.Remove( 0 )   ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.Remove( 1 )   ; linkedList.Print() ; ASSERT( linkedList.Size() == 0 ) ;

    // Pass 2b: append nodes and remove in reverse order
    DEBUG_ONLY( DebugPrintf( "SList::UnitTest: 2b\n" ); )

    linkedList.PushBack( 0 ) ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.PushBack( 1 ) ; linkedList.Print() ; ASSERT( linkedList.Size() == 2 ) ;
    linkedList.Remove( 1 )   ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.Remove( 0 )   ; linkedList.Print() ; ASSERT( linkedList.Size() == 0 ) ;
    ASSERT( linkedList.Size() == 0 ) ;

    // Pass 3a: insert & append nodes, remove in same order
    DEBUG_ONLY( DebugPrintf( "SList::UnitTest: 3a\n" ); )

    linkedList.PushFront( 0 ) ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.PushBack( 1 )  ; linkedList.Print() ; ASSERT( linkedList.Size() == 2 ) ;
    linkedList.Remove( 0 )    ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.Remove( 1 )    ; linkedList.Print() ; ASSERT( linkedList.Size() == 0 ) ;
    ASSERT( linkedList.Size() == 0 ) ;

    // Pass 3b: append & insert nodes, remove in same order
    DEBUG_ONLY( DebugPrintf( "SList::UnitTest: 3b\n" ); )

    linkedList.PushBack( 0 )  ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.PushFront( 1 ) ; linkedList.Print() ; ASSERT( linkedList.Size() == 2 ) ;
    linkedList.Remove( 0 )    ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.Remove( 1 )    ; linkedList.Print() ; ASSERT( linkedList.Size() == 0 ) ;
    ASSERT( linkedList.Size() == 0 ) ;

    // Pass 4: append & insert nodes, remove in same order
    DEBUG_ONLY( DebugPrintf( "SList::UnitTest: 4\n" ); )

    linkedList.PushBack( 0 )  ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.PushFront( 1 ) ; linkedList.Print() ; ASSERT( linkedList.Size() == 2 ) ;
    linkedList.Remove( 0 )    ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.Remove( 1 )    ; linkedList.Print() ; ASSERT( linkedList.Size() == 0 ) ;
    ASSERT( linkedList.Size() == 0 ) ;

    // Pass 5: append & insert nodes, then clear list
    DEBUG_ONLY( DebugPrintf( "SList::UnitTest: 5\n" ); )

    linkedList.PushBack( 0 )  ; linkedList.Print() ; ASSERT( linkedList.Size() == 1 ) ;
    linkedList.PushFront( 1 ) ; linkedList.Print() ; ASSERT( linkedList.Size() == 2 ) ;
    linkedList.Clear()        ; linkedList.Print() ; ASSERT( linkedList.Size() == 0 ) ;

    return ;
}

#endif
