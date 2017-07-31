/*
    \file list.h

    \description singly linked list templated class

    \author Copyright 2005-2006 UCF/FIEA/MJG; All rights reserved.

    \notes
        This class performs allocation and deallocation.  The analogous
        Standard Template Library (STL) templated class facilitates
        the caller optionally providing an allocator.  We could
        extend this template to support such a feature,
        but in practice we tend to want to put link node allocations
        into the same pool as the data to which the links refer,
        so instead we opt to use a stateful allocator which gets
        informed of its pool by means external to this structure.

*/
#ifndef LIST_H
#define LIST_H
// Macros -----------------------------------------------------------------------

#if ! defined( NULL )
#define NULL 0
#endif


//-----------------------------------------------------------------------------

template <class ItemT> class LinkNode
{
    public:
        LinkNode( void ) : mNext( NULL ) {}
        LinkNode( const ItemT & rItem ) : mItem( rItem ) , mNext( NULL ) {}
        ~LinkNode() { ASSERT( NULL == mNext ) ; mNext = NULL ; }
        inline void        SetItem( const ItemT & rItem )       { mItem = rItem ; }
        inline ItemT &     GetItem( void )                      { return mItem  ; }
        inline void        SetLink( LinkNode<ItemT> * pNode )   { mNext = pNode ; }
        inline LinkNode *  GetLink( void ) const                { return mNext  ; }
    protected:
        ItemT             mItem ;
        LinkNode<ItemT> * mNext ;
} ;




template <class ItemT> class SList
{
    public:
        SList(void) : mHead( NULL ) , mTail( NULL ) , mNumNodes( 0 ) {}
        virtual ~SList() ;
        LinkNode<ItemT> *   Head( void ) const { return mHead ; }
        LinkNode<ItemT> *   Find( const ItemT & rItem ) const ;

        void                PushFront( const ItemT & rItem ) ;
        void                PushBack( const ItemT & rItem ) ;
        void                Remove( const ItemT & rItem ) ;
        void                Clear( void ) ;
        void                Obliterate( void ) ;
        int                 Size( void ) const { return mNumNodes ; }
        void                Print( void ) const ;

    #if defined( UNIT_TEST )
        static void         UnitTest( void ) ;
    #endif

    protected:
        void                InsertNode( LinkNode<ItemT> * pNode ) ;
        void                AppendNode( LinkNode<ItemT> * pNode ) ;
        LinkNode<ItemT> *   RemoveNode( const ItemT & rItem ) ;

        LinkNode<ItemT> * FindPrevNode( const ItemT & rItem ) const ;
        LinkNode<ItemT> * mHead ;
        LinkNode<ItemT> * mTail ;
        int               mNumNodes ;
} ;




//-----------------------------------------------------------------------------


/*  Linked list destructor
*/
template <class ItemT> SList<ItemT>::~SList()
{
    ASSERT( NULL == mHead ) ;   // ...otherwise memory leaks
    ASSERT( NULL == mTail ) ;   // ...otherwise memory leaks
    ASSERT( 0 == mNumNodes ) ;  // ...otherwise memory leaks
}




/*  Insert the given link node before the list head

    Caller provides the node itself, rather than having this class allocate a new node.

    See also SList::PushFront.
*/
template <class ItemT> void SList<ItemT>::InsertNode( LinkNode<ItemT> * pNode )
{
    ASSERT( NULL == pNode->GetLink() ) ;    // this method only supports inserting single nodes
    if( mHead != NULL )
    {   // List was not empty
        pNode->SetLink( mHead ) ;           // point new node to previous head
    }
    mHead = pNode ;                         // point head to new node
    if( NULL == mTail )
    {   // list /was/ empty
        mTail = mHead ;                     // point tail to new (only) node
    }
    ++ mNumNodes ;  // count number of nodes
}




/*  Insert the given item before the list head

    Allocate a new link node, associate it with the given item,
    and insert that into the list.  Caller shall use use Remove
    (not RemoveNode) for removing nodes inserted using this method.

*/
template <class ItemT> void SList<ItemT>::PushFront( const ItemT & rItem )
{
    LinkNode<ItemT> * pNewNode = new LinkNode<ItemT>( rItem ) ;
    ASSERT( pNewNode != NULL ) ;
    InsertNode( pNewNode ) ;
}




/*  Append the given link node after the list tail

    Caller provides the node itself, rather than having this class allocate a new node.

    See also SList::PushBack.
*/
template <class ItemT> void SList<ItemT>::AppendNode( LinkNode<ItemT> * pNode )
{
    ASSERT( NULL == pNode->GetLink() ) ;
    if( mTail != NULL )
    {   // Tail points to a node (i.e. list was not empty)
        ASSERT( mTail->GetLink() == NULL ) ;// tail should have no successor (ever!)
        mTail->SetLink( pNode ) ;           // point old tail link to new node
    }
    mTail = pNode ;                         // point tail to new node
    if( NULL == mHead )
    {   // List had no head (i.e. was empty) so new node is the only node.
        mHead = pNode ;                     // point head to new node
    }
    ++ mNumNodes ;  // count number of nodes
}




/*  Append the given item after the list tail

    Allocate a new link node, associate it with the given item,
    and append that into the list.  Caller shall use use Remove
    (not RemoveNode) for removing nodes inserted using this method.

*/
template <class ItemT> void SList<ItemT>::PushBack( const ItemT & rItem )
{
    LinkNode<ItemT> * pNewNode = new LinkNode<ItemT>( rItem ) ;
    ASSERT( pNewNode != NULL ) ;
    AppendNode( pNewNode ) ;
}




/*  Find the link node associated with the given item
*/
template <class ItemT> LinkNode<ItemT> * SList<ItemT>::Find( const ItemT & rItem ) const
{
    for( LinkNode<ItemT> * pNode = Head() ; pNode != NULL ; pNode = pNode->GetLink() )
    {
        if( pNode->GetItem() == rItem )
        {
            return pNode ;
        }
    }
    return NULL ;
}




/*  Find the node in the list that precedes the node associated with the given item

    This is useful for removing the node associated with the given item.
*/
template <class ItemT> LinkNode<ItemT> * SList<ItemT>::FindPrevNode( const ItemT & rItem ) const
{
    for( LinkNode<ItemT> * pNode = Head() ; pNode != NULL ; pNode = pNode->GetLink() )
    {
        if(     ( pNode->GetLink() != NULL )
            &&  ( pNode->GetLink()->GetItem() == rItem ) )
        {
            return pNode ;
        }
    }
    return NULL ;
}




/*  Remove link node associated with the given item.

    This does NOT delete the link node.
    See also SList<>::InsertNode.

*/
template <class ItemT> LinkNode<ItemT> * SList<ItemT>::RemoveNode( const ItemT & rItem )
{
    ASSERT( Head() != NULL ) ;
    ASSERT( mTail != NULL ) ;
    ASSERT( mNumNodes > 0 ) ;
    LinkNode<ItemT> * nodeToRemove = NULL ;
    LinkNode<ItemT> * prev = FindPrevNode( rItem ) ;
    if( prev != NULL )
    {   // Found predecessor
        nodeToRemove = prev->GetLink() ;
        ASSERT( nodeToRemove != NULL ) ;                // sought node must exist and follow prev
        ASSERT( nodeToRemove->GetItem() == rItem ) ;    // node data must match sought item
        LinkNode<ItemT> * next = nodeToRemove->GetLink() ;
        prev->SetLink( next ) ;                         // skip past sought node
    }
    else
    {   // Sought node has no predecessor.
        ASSERT( Head()->GetItem() == rItem ) ;           // sought node must be at head
        nodeToRemove = mHead ;
        ASSERT( nodeToRemove != NULL ) ;                // list must contain sought node
        mHead = nodeToRemove->GetLink() ;               // point head to node after sought
    }
    if( mTail == nodeToRemove )
    {   // Sought node was at tail.
        mTail = prev ;                                  // point tail to new last node
    }
    nodeToRemove->SetLink( NULL ) ;                     // detach list from node
    -- mNumNodes ;  // count number of nodes
    return nodeToRemove ;
}




/*  Remove link node associated with given item and delete link node.

    Use this on items inserted using PushFront or PushBack.
    See also SList::PushFront, SList::PushBack.

    Notes: STL slist allows multiple nodes with identical
    item values and STL slist::remove removes /all/ nodes that match the given item.
    STL slist::remove therefore takes O(N) time where N=slist::size().
    In contrast, this routine removes only the first node containing the given item.
    Whereas in general this implementation takes O(N) time for
    removing nodes with arbitrary placement in the list, it takes only
    constant time to remove the head node.  This feature gets exploited elsewhere.

*/
template <class ItemT> void SList<ItemT>::Remove( const ItemT & rItem )
{
    LinkNode<ItemT> * pNodeToDelete = RemoveNode( rItem ) ;
    ASSERT( pNodeToDelete != NULL ) ;
    delete pNodeToDelete ;
}




/*  Remove all elements
*/
template <class ItemT> void SList<ItemT>::Clear( void )
{
    for( LinkNode<ItemT> * pNode = Head() ; pNode != NULL ; pNode = Head() )
    {   // for each node in list (note special increment semantics because body edits list)
        Remove( pNode->GetItem() ) ;
        // Note: Removing one node at a time unnecessarily guarentees list integrity.
        // Since Clear removes /all/ nodes, this method could reduce processing by
        // simply deleting all nodes and then resetting mHead, mTail and mNumNodes.
        // Loop increment would be slightly more complicated.
    }
}




/*  Remove and delete all elements
    ItemT must be a pointer type for this method to mean anything.
*/
template <class ItemT> void SList<ItemT>::Obliterate( void )
{
    for( LinkNode<ItemT> * pNode = Head() ; pNode != NULL ; pNode = Head() )
    {   // for each node in list (note special increment semantics because body edits list)
        ItemT item = pNode->GetItem() ;
        Remove( item ) ;
        delete item ;   // this only works if ItemT is a pointer type
        // Note: Removing one node at a time unnecessarily guarentees list integrity.
        // Since Clear removes /all/ nodes, this method could reduce processing by
        // simply deleting all nodes and then resetting mHead, mTail and mNumNodes.
        // Loop increment would be slightly more complicated.
    }
}

#endif // LIST_H
